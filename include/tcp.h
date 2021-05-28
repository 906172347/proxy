#ifndef _TCP_H
#define _TCP_H
#include "head.h"
int INET_Connect(int port,char *address);
int UNIX_Connect(char *filename);
int INET_Listen(int Port);
int UNIX_Listen(char *filename);
int Server_GetCommSock(int iDomain,int iListenFd);
int UDP_Connect(int port,char *address);
int UDP_Listen(int Port);
#endif
