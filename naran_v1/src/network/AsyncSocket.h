#ifndef __NARAN_ASYNC_SOCKET_H__
#define __NARAN_ASYNC_SOCKET_H__

#include "CommonDef.h"
#include "autoref/AutoReference.h"
#include "autoref/AutoCounter.h"

NS_DEF_NARAN{

	class IOnSocketReceive
	{
	public:
		virtual void onReceive(arr(byte) data) = 0;
	};

	class ISocketConnection
	{
	public:
		virtual void send(arr(unsigned char) data) = 0;
		virtual void setReceiveListener(interf(IOnSocketReceive) recv) = 0;
	};

	class CLS_EXPORT SocketPool
	{
	public:
		interf(ISocketConnection) addConnection(const char *ip, int port);
		void removeConnection(interf(ISocketConnection) connect);

		void runService();

		grab(SocketPool) create();

	private:
		void * mPoolData;

		CLS_HIDE(SocketPool);
	};

}

#endif

