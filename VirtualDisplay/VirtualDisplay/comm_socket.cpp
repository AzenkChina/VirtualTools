#include "comm_socket.h"

#if ( defined ( _WIN32 ) || defined ( _WIN64 ) ) && defined ( _MSC_VER )
#pragma comment(lib,"ws2_32.lib")
#endif

//服务端使用UDP广播接收
CommReceiver::CommReceiver(int Port)
{
#if defined ( _WIN32 ) || defined ( _WIN64 )
	WSADATA wsaData;
#endif

    this->Receiver = INVALID_SOCKET;

#if defined ( _WIN32 ) || defined ( _WIN64 )
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
        return;
    }
#endif

    this->Receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#if defined ( _WIN32 ) || defined ( _WIN64 )
    if(this->Receiver == INVALID_SOCKET) {
		WSACleanup();
        return;
    }

    int Mode = 1;
    ioctlsocket(this->Receiver, FIONBIO, (u_long FAR*) &Mode);//设置非阻塞
#elif defined ( __linux )
    if(this->Receiver <= 0) {
        return;
    }
	
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;
	setsockopt(this->Receiver, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));//设置非阻塞
#endif

    SOCKADDR_IN sin;
    sin.sin_family = AF_INET;
#if defined ( _WIN32 ) || defined ( _WIN64 )
	sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
#elif defined ( __linux )
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
    sin.sin_port = htons(Port);
    if(bind(this->Receiver, (SOCKADDR*)&sin, sizeof(sin)) != 0) {
#if defined ( _WIN32 ) || defined ( _WIN64 )
        closesocket(this->Receiver);
        this->Receiver = INVALID_SOCKET;
        WSACleanup();
#elif defined ( __linux )
        close(this->Receiver);
        this->Receiver = INVALID_SOCKET;
#endif
        return;
    }
}

CommReceiver::~CommReceiver()
{
    if(this->Receiver != INVALID_SOCKET) {
#if defined ( _WIN32 ) || defined ( _WIN64 )
        closesocket(this->Receiver);
        WSACleanup();
#elif defined ( __linux )
        close(this->Receiver);
#endif
    }
}

int32_t CommReceiver::Receive(uint8_t *Buffer, uint16_t BufferLength)
{
    if(this->Receiver == INVALID_SOCKET) {
        return -1;
    }

    SOCKADDR_IN Client;
#if defined ( _WIN32 ) || defined ( _WIN64 )
	int ClientLength = sizeof(Client);
#elif defined ( __linux )
	socklen_t ClientLength = sizeof(Client);
#endif
	int result = recvfrom(this->Receiver, (char *)Buffer, BufferLength, 0, (SOCKADDR*)&Client, &ClientLength);
#if defined ( _WIN32 ) || defined ( _WIN64 )
	if(result < 0)
	{
		if(WSAGetLastError() == 10035)
		{
			result = 0;
		}
	}
#endif
	return result;
}






//客户端使用UDP广播发送
CommEmitter::CommEmitter(int Port)
{
#if defined ( _WIN32 ) || defined ( _WIN64 )
	WSADATA wsaData;
#endif

    this->Emitter = INVALID_SOCKET;

#if defined ( _WIN32 ) || defined ( _WIN64 )
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
        return;
    }
#endif

    this->Emitter = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(this->Emitter == INVALID_SOCKET) {
#if defined ( _WIN32 ) || defined ( _WIN64 )
		WSACleanup();
#endif
        return;
    }

#if defined ( _WIN32 ) || defined ( _WIN64 )
    bool bOpt = true;
    setsockopt(this->Emitter, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));//打开广播选项
#elif defined ( __linux )
    int bOpt = 1;
    setsockopt(this->Emitter, SOL_SOCKET, SO_BROADCAST, &bOpt, sizeof(int));//打开广播选项
	setsockopt(this->Emitter, SOL_SOCKET, SO_REUSEADDR, &bOpt, sizeof(int));
#endif


#if defined ( _WIN32 ) || defined ( _WIN64 )
	int Mode = 1;
	ioctlsocket(this->Emitter, FIONBIO, (u_long FAR*) &Mode);//设置非阻塞
#elif defined ( __linux )
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;
	setsockopt(this->Emitter, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));//设置非阻塞
#endif

    //设置发往的地址
    memset(&this->sin, 0, sizeof(this->sin));
    this->sin.sin_family = AF_INET;
#if defined ( _WIN32 ) || defined ( _WIN64 )
	this->sin.sin_addr.S_un.S_addr = htonl(INADDR_BROADCAST);
#elif defined ( __linux )
	this->sin.sin_addr.s_addr = htonl(INADDR_BROADCAST);
#endif
    this->sin.sin_port = htons(Port);
}

//客户端使用UDP单播发送
CommEmitter::CommEmitter(char *IPaddr, int Port)
{
#if defined ( _WIN32 ) || defined ( _WIN64 )
	WSADATA wsaData;
#endif

    this->Emitter = INVALID_SOCKET;

#if defined ( _WIN32 ) || defined ( _WIN64 )
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
        return;
    }
#endif

    this->Emitter = socket(AF_INET, SOCK_DGRAM, 0);
    if(this->Emitter == INVALID_SOCKET) {
#if defined ( _WIN32 ) || defined ( _WIN64 )
		WSACleanup();
#endif
        return;
    }

#if defined ( _WIN32 ) || defined ( _WIN64 )
	int Mode = 1;
	ioctlsocket(this->Emitter, FIONBIO, (u_long FAR*) &Mode);//设置非阻塞
#elif defined ( __linux )
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;
	setsockopt(this->Emitter, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));//设置非阻塞
#endif

    //设置发往的地址
    memset(&this->sin, 0, sizeof(this->sin));
    this->sin.sin_family = AF_INET;
#if defined ( _WIN32 ) || defined ( _WIN64 )
	this->sin.sin_addr.S_un.S_addr = htonl(inet_addr(IPaddr));
#elif defined ( __linux )
	this->sin.sin_addr.s_addr = htonl(inet_addr(IPaddr));
#endif
    this->sin.sin_port = htons(Port);
}

CommEmitter::~CommEmitter()
{
    if(this->Emitter != INVALID_SOCKET) {
#if defined ( _WIN32 ) || defined ( _WIN64 )
        WSACleanup();
        closesocket(this->Emitter);
#elif defined ( __linux )
        close(this->Emitter);
#endif
    }
}

int32_t CommEmitter::Send(uint8_t *Buffer, uint16_t Length)
{
    if(this->Emitter == INVALID_SOCKET) {
        return -1;
    }

    return sendto(this->Emitter, (char *)Buffer, Length, 0, (SOCKADDR*)&this->sin, sizeof(SOCKADDR_IN));
}

