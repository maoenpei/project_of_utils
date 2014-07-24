#ifndef __NARAN_SYNC_SOCKET_H__
#define __NARAN_SYNC_SOCKET_H__

#include "CommonDef.h"
#include "autoref/AutoReference.h"

NS_DEF_NARAN{

	class CLS_EXPORT Socket
	{
	public:
		void send(arr(byte) data);
		arr(byte) receive();

		grab(Socket) create(const char *ip, int port);

	private:
		void *mSockData;

		CLS_HIDE(Socket);
	};

	class CLS_EXPORT SocketServer
	{
	public:
		grab(Socket) accept();

		grab(SocketServer) create(const char *ip, int port);

	private:
		void *mSockData;

		CLS_HIDE(SocketServer);
	};

}

#endif

