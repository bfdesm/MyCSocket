#include <winsock2.h>
#include <stdio.h>
#include <stdbool.h>

#pragma comment(lib,"ws2_32.lib")
int buildSocket(SOCKET *clientSocket)
{
    WORD versionRequired;
    WSADATA wsaData;
    versionRequired = MAKEWORD(2,2);
    int err = WSAStartup(versionRequired, &wsaData);//协议库的版本信息
    //通过WSACleanup的返回值来确定socket协议是否启动
    if (!err)
    {
        printf("客户端成功创建套接字\n");
        *clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        return 1;
    }
    else
    {
        printf("客户端创建套接字失败\n");
        return 0;//结束
    }

}
int checkConnect(SOCKET *clientSocket)
{
    HANDLE closeEvent = WSACreateEvent();
    WSAEventSelect(*clientSocket, closeEvent, FD_CLOSE);
    bool ret = false;
    DWORD dwRet = WaitForSingleObject(closeEvent, 0);
    if(dwRet == WSA_WAIT_EVENT_0)
        ret = true;
    else if(dwRet == WSA_WAIT_TIMEOUT)
        ret = false;
    return ret;
}
void sendText(SOCKET *clientSocket){
    char SendBuf[100];
    while (1)
    {
        gets(SendBuf);
        if(strcmp(SendBuf, "quit")==0)
        {
            break;
        }
        send(*clientSocket, SendBuf, strlen(SendBuf) + 1, 0);
        int ret = checkConnect(&*clientSocket);
        if (ret == 1)
        {
            break;
        }
    }
}
void close(SOCKET *clientSocket)
{
    closesocket(*clientSocket);
    //关闭服务
    WSACleanup();
}
int main()
{
    //socket编程中，它定义了一个结构体SOCKADDR_IN来存计算机的一些信息，像socket的系统，
    //端口号，ip地址等信息，这里存储的是服务器端的计算机的信息
    SOCKADDR_IN clientsock_in;
    clientsock_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    clientsock_in.sin_family = AF_INET;
    clientsock_in.sin_port = htons(12345);
    //注意socket这个函数，他三个参数定义了socket的所处的系统，socket的类型，以及一些其他信息
    int result = 0;
    //前期定义了套接字，定义了服务器端的计算机的一些信息存储在clientsock_in中，
    //准备工作完成后，然后开始将这个套接字链接到远程的计算机
    //也就是第一次握手
    while (1)
    {
        SOCKET clientSocket;
        while(result == 0) {
            result = buildSocket(&clientSocket);
        }
        result = -1;
        while(result == -1)
        {
            result = connect(clientSocket,(SOCKADDR*)&clientsock_in,sizeof(SOCKADDR));//开始连接
        }
        result = 0;
        printf("连接服务端套接字成功\n");
        sendText(&clientSocket);
        closesocket(clientSocket);
        printf("服务端套接字失效\n");
    }
    WSACleanup();
    return 0;
}
