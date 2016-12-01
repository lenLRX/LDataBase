#ifndef __SOCKET_HEADER_H__
#define __SOCKET_HEADER_H__
#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>

namespace socket_namespace{
	#include <arpa/inet.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <unistd.h>
}
#endif//__SOCKET_HEADER_H__

