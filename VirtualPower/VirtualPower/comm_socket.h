#ifndef _COMM_SOCKET_H_
#define _COMM_SOCKET_H_

#include <stdint.h>

#if defined ( _WIN32 ) || defined ( _WIN64 )

#include <Ws2tcpip.h>

#elif defined ( __linux )

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#define SOCKET			int
#define SOCKADDR		struct sockaddr
#define SOCKADDR_IN		struct sockaddr_in
#define INVALID_SOCKET	0

#else

#error platform unsupported

#endif

class CommReceiver
{
	public:
        CommReceiver(int Port);
		int32_t Receive(uint8_t *Buffer, uint16_t BufferLength);
        virtual ~CommReceiver();
	private:
        SOCKET Receiver;
};

class CommEmitter
{
	public:
		CommEmitter(int port);
		CommEmitter(char *IPaddr, int Port);
		int32_t Send(uint8_t *Buffer, uint16_t Length);
		virtual ~CommEmitter();
	private:
		SOCKET Emitter;
		SOCKADDR_IN sin;
};

#endif

