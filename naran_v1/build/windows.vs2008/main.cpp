
#include "structure/KLongLong.h"
#include "framework/Utils.h"

#include <windows.h>
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")

using namespace Naran;

int g_k = 0;

int main(int argc, char **argv)
{
	char command[] = "GET http://update.jyhxz.com:8080/NewUpdateServer/CheckUpdate?uid=NOUNID*b4:52:7e:e1:9d:f8*357577058918158&event=0&step=0\r\n";
	char buf[1024] = {0};
	WSADATA wsaData;
	WORD version = MAKEWORD(2 , 0);
	WSAStartup(version , &wsaData);

	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("101.251.201.84");
	addr.sin_port = htons(8080);
	
	SOCKET tsock = socket(AF_INET , SOCK_STREAM , 0);
	int ret = connect(tsock , (struct sockaddr *)&addr , sizeof(addr));
	if (ret == 0){
		send(tsock, command, sizeof(command), 0);
		recv(tsock, buf, sizeof(buf), 0);
		closesocket(tsock);
	}

	WSACleanup();

	return 0;
}


