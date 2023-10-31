#include<winsock2.h>
#include<WS2tcpip.h>
#pragma comment (lib,"ws2_32.lib")

#include <iostream>
#include <fstream>
#include <cstring>

#define SERVER_PORT 8888
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 512
int FileSend() {

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        return 1;
    }


    int sockfd;
    sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    //serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        std::cerr << "inet_pton() failed" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }


    std::ifstream inFile("output.265", std::ios::binary);

    while (!inFile.eof()) {
        inFile.read(buffer, BUFFER_SIZE);
        sendto(sockfd, buffer, inFile.gcount(), 0, (const sockaddr*)&serverAddr, sizeof(serverAddr));
    }



    // 在文件传输完成后，发送结束信号
    const char* endSignal = "__END_OF_FILE__";
    sendto(sockfd, endSignal, strlen(endSignal), 0, (const sockaddr*)&serverAddr, sizeof(serverAddr));

    inFile.close();
    closesocket(sockfd);
    WSACleanup();
    return 0;
}