

#include "AsyncSocket.h"
#include "structure/KHashmap.h"
#include "structure/KArray.h"
#include "structure/KBuffer.h"
#include <pthread.h>

#ifdef WIN32
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")
#else
#define closesocket		close
#endif

NS_DEF_NARAN{

#define mData		((Data_SocketPool *)mPoolData)

	struct Data_SocketPool
	{
		InterfSet(ISocketConnection) mConnections;
		inline Data_SocketPool(){
			pthread_mutex_init(&mSendMutex, NULL);
			pthread_mutex_init(&mRecvMutex, NULL);
			pthread_mutex_init(&mSocketsMutex, NULL);
		}
		inline ~Data_SocketPool(){
			pthread_mutex_destroy(&mRecvMutex);
			pthread_mutex_destroy(&mSendMutex);
			pthread_mutex_destroy(&mSocketsMutex);
		}

		struct Data_SendEntry
		{
			SOCKET mSock;
			arr(byte) m_data;
			inline Data_SendEntry(SOCKET sock, arr(byte) data)
				: mSock(sock), m_data(data){}
		};
		Array<Data_SendEntry> mSendQueue;
		pthread_mutex_t mSendMutex;
		inline void addSendData(SOCKET sock, arr(byte) data){
			pthread_mutex_lock(&mSendMutex);
			mSendQueue.insert(Data_SendEntry(sock, data));
			pthread_mutex_unlock(&mSendMutex);
		}

		VarMap(SOCKET, interf(IOnSocketReceive)) mRecvHandlers;
		pthread_mutex_t mRecvMutex;
		inline void setReceiveHandler(SOCKET sock, interf(IOnSocketReceive) handler){
			pthread_mutex_lock(&mRecvMutex);
			mRecvHandlers.set(sock, handler);
			pthread_mutex_unlock(&mRecvMutex);
		}

		VarSet(SOCKET) mSockets;
		pthread_mutex_t mSocketsMutex;
		inline void addSocket(SOCKET sock){
			pthread_mutex_lock(&mSocketsMutex);
			mSockets.insert(sock);
			pthread_mutex_unlock(&mSocketsMutex);
		}
		inline void removeSocket(SOCKET sock){
			pthread_mutex_lock(&mSocketsMutex);
			mSockets.remove(sock);
			mRecvHandlers.remove(sock);
			pthread_mutex_unlock(&mSocketsMutex);
		}

		void sendList(int count, int timeout){
			pthread_mutex_lock(&mSocketsMutex);
			while(1){
				pthread_mutex_lock(&mSendMutex);
				if (count != 0 && mSendQueue.count() > 0){
					Data_SendEntry data = mSendQueue[0];
					mSendQueue.remove(0);
					pthread_mutex_unlock(&mSendMutex);
					
					count--;
					
					if (mSockets.has(data.mSock)){
						timeval tm;
						tm.tv_sec  = 0;
						tm.tv_usec = timeout * 1000;
						fd_set	fds;
						FD_ZERO(&fds);
						FD_SET(data.mSock, &fds);
						int result = select(data.mSock + 1, 0, &fds, 0, &tm);
						
						if (result > 0){
							arr(byte) stm = data.m_data;
							int start = 0;
							while(start < stm.count()){
								start += send(data.mSock, (char *)stm.get() + start, stm.count() - start, 0);
							}
						}else if (result == 0){
							pthread_mutex_lock(&mSendMutex);
							mSendQueue.insert(data, 0);
							pthread_mutex_unlock(&mSendMutex);
							break;
						}else{
							// exception
						}
					}
				}else{
					pthread_mutex_unlock(&mSendMutex);
					break;
				}
			}
			pthread_mutex_unlock(&mSocketsMutex);
		}
		void recvList(int count, int timeout){
			pthread_mutex_lock(&mSocketsMutex);
			while(count > 0){
				VarSet(SOCKET)::IterType it = mSockets.iterator();
				if (! it.hasNext)
					break;
				
				count--;
				
				timeval tm;
				tm.tv_sec  = 0;
				tm.tv_usec = timeout * 1000;
				SOCKET maxSock = 0;
				fd_set	fds;
				FD_ZERO(&fds);
				while(it.hasNext){
					FD_SET(it.value, &fds);
					maxSock = (it.value > maxSock ? it.value : maxSock);
					it.next();
				}
				
				int result = select(maxSock + 1, 0, &fds, 0, &tm);
				
				if (result > 0){
					SOCKET readable = 0;
					it = mSockets.iterator();
					while(it.hasNext){
						if (FD_ISSET(it.value, &fds)){
							readable = it.value;
						}
						it.next();
					}
					if (readable){
						int singleSize = 1024;
						Buffer<byte> buf(singleSize);
						while(1){
							byte *data = buf.want_tail(singleSize);
							int nr = recv(readable, (char *)data, singleSize, 0);
							if (nr < singleSize){
								buf.limit(data + nr);
								break;
							}
						}
						
						pthread_mutex_lock(&mRecvMutex);
						interf(IOnSocketReceive) interf = mRecvHandlers.get(readable);
						if (interf){
							interf->onReceive(buf.toArr());
						}
						pthread_mutex_unlock(&mRecvMutex);
					}
				}else if (result == 0){
					break;
				}else{
					// exception
					break;
				}
			}
			pthread_mutex_unlock(&mSocketsMutex);
		}
		
		/*
			pthread_mutex_lock(&mSocketsMutex);
			pthread_mutex_unlock(&mSocketsMutex);
		*/

		inline static SOCKET utilConnect(const char *ip, int port){
			
			SOCKET sock = socket(AF_INET , SOCK_STREAM , 0);
			if (sock == -1)
				return sock;
#ifdef WIN32
			int iMode = 1;
			ioctlsocket(sock, FIONBIO, (u_long FAR*) &iMode);
#else
			int flags = fcntl(sock, F_GETFL, 0);
			fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#endif

			struct sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = inet_addr(ip);
			addr.sin_port = htons(port);
			memset(&(addr.sin_zero) , 0 , sizeof(addr.sin_zero));

			connect(sock , (struct sockaddr *)&addr , sizeof(addr));
			return sock;
		}
	};

	class Impl_SocketConnection : public ISocketConnection
	{
	public:
		void * mPoolData;
		SOCKET mSock;
		Impl_SocketConnection(void *data, SOCKET sock) : mPoolData(data), mSock(sock){}
		virtual void send(arr(byte) data)
		{
			mData->addSendData(mSock, data);
		}
		virtual void setReceiveListener(interf(IOnSocketReceive) recv)
		{
			mData->setReceiveHandler(mSock, recv);
		}
	};

	interf(ISocketConnection) SocketPool::addConnection(const char *ip, int port)
	{
		SOCKET sock = Data_SocketPool::utilConnect(ip, port);
		if (sock == -1)
			return nullof(ISocketConnection);
		
		interf(ISocketConnection) inst = interf_make_grab(ISocketConnection)(new Impl_SocketConnection(mPoolData, sock));
		
		mData->addSocket(sock);

		mData->mConnections.insert(inst);
		return inst;
	}

	void SocketPool::removeConnection(interf(ISocketConnection) connect)
	{
		Impl_SocketConnection *inst = dynamic_cast<Impl_SocketConnection *>(connect.get());
		SOCKET sock = inst->mSock;
		
		mData->removeSocket(sock);

		closesocket(sock);
		mData->mConnections.remove(connect);
	}

	void SocketPool::runService()
	{
		mData->sendList(20, 1);
		mData->sendList(1, 5);
	}

	SocketPool::SocketPool()
	{
#ifdef WIN32
		WSADATA wsaData;
		WORD version = MAKEWORD(2 , 0);
		WSAStartup(version , &wsaData);
#endif
	}

	SocketPool::~SocketPool()
	{
#ifdef WIN32
		WSACleanup();
#endif
	}

}

