#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
SOCKET buildSocket()
{
    WSADATA wsaData;
    SOCKET serSocket;
    while (1) {
        WSAStartup(MAKEWORD(2, 2), &wsaData); //目前建议使用最新2.2版本
        serSocket = socket(AF_INET, SOCK_STREAM, 0);//创建了可识别套接字
        if(serSocket != -1)
        {
            // printf("服务端成功创建套接字！%llu\n", serSocket);
            printf("服务端成功创建套接字\n");
            break;
        }
    }
    return serSocket;
}
SOCKADDR_IN waitConnect(SOCKET *serSocket, SOCKET *serConn)
{
    printf("等待客户端套接字连接\n");
    SOCKADDR_IN clientsocket;
    int len = sizeof(SOCKADDR);
    //第二次握手，通过accept来接受对方的套接字的信息
    *serConn = accept(*serSocket, (SOCKADDR*)&clientsocket, &len);
    //如果这里不是accept而是conection的话。。就会不断的监听
    if(*serConn)
    {
        printf("监听到新客户端套接字连接\n");
    }
    return clientsocket;
}
int getText(SOCKET *serConn, SOCKADDR_IN *clientsocket)
{
    while (1)
    {
        char sendBuf[100];
        sprintf(sendBuf, "welcome %s to here", inet_ntoa(clientsocket->sin_addr));//找对对应的IP并且将这行字打印到那里
        //发送信息
        send(*serConn, sendBuf, strlen(sendBuf) + 1, 0);
        char receiveBuf[100];//接收
        int RecvLen;
        RecvLen = recv(*serConn, receiveBuf, 100, 0);
        if(RecvLen != -1) {
            //printf("%d %s\n", RecvLen, receiveBuf);
            printf("客户端：%s\n", receiveBuf);
        }
        else
            break;
    }
    return 0;
}
void close(SOCKET *serConn)
{
    closesocket(*serConn);//关闭
    WSACleanup();//释放资源的操作
}

int main()
{
    SOCKET serSocket = buildSocket();
    //需要绑定的参数，主要是本地的socket的一些信息。
    SOCKADDR_IN addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//ip地址
    addr.sin_port=htons(12345);//绑定端口

    bind(serSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR));//绑定完成
    listen(serSocket, 5);//其中第二个参数代表能够接收的最多的连接数
    SOCKET serConn;
    while (1)
    {
        SOCKADDR_IN clientsocket = waitConnect(&serSocket, &serConn);
        getText(&serConn, &clientsocket);
        printf("客户端套接字失效\n");
    }
    close(&serConn);
    return 0;
}
