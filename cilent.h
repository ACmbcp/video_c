#ifndef _CLIENT__H
#define _CLIENT__H
#include<winsock2.h>
#include<WS2tcpip.h>
#pragma comment (lib,"ws2_32.lib")

#include <iostream>
#include <fstream>
#include <cstring>


#define SERVER_PORT 8888
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 512

int FileSend();
#endif