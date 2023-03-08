#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
SOCKET buildSocket()
{
    WSADATA wsaData;
    SOCKET serSocket;
    while (1) {
        WSAStartup(MAKEWORD(2, 2), &wsaData); //Ŀǰ����ʹ������2.2�汾
        serSocket = socket(AF_INET, SOCK_STREAM, 0);//�����˿�ʶ���׽���
        if(serSocket != -1)
        {
            // printf("����˳ɹ������׽��֣�%llu\n", serSocket);
            printf("����˳ɹ������׽���\n");
            break;
        }
    }
    return serSocket;
}
SOCKADDR_IN waitConnect(SOCKET *serSocket, SOCKET *serConn)
{
    printf("�ȴ��ͻ����׽�������\n");
    SOCKADDR_IN clientsocket;
    int len = sizeof(SOCKADDR);
    //�ڶ������֣�ͨ��accept�����ܶԷ����׽��ֵ���Ϣ
    *serConn = accept(*serSocket, (SOCKADDR*)&clientsocket, &len);
    //������ﲻ��accept����conection�Ļ������ͻ᲻�ϵļ���
    if(*serConn)
    {
        printf("�������¿ͻ����׽�������\n");
    }
    return clientsocket;
}
int getText(SOCKET *serConn, SOCKADDR_IN *clientsocket)
{
    while (1)
    {
        char sendBuf[100];
        sprintf(sendBuf, "welcome %s to here", inet_ntoa(clientsocket->sin_addr));//�ҶԶ�Ӧ��IP���ҽ������ִ�ӡ������
        //������Ϣ
        send(*serConn, sendBuf, strlen(sendBuf) + 1, 0);
        char receiveBuf[100];//����
        int RecvLen;
        RecvLen = recv(*serConn, receiveBuf, 100, 0);
        if(RecvLen != -1) {
            //printf("%d %s\n", RecvLen, receiveBuf);
            printf("�ͻ��ˣ�%s\n", receiveBuf);
        }
        else
            break;
    }
    return 0;
}
void close(SOCKET *serConn)
{
    closesocket(*serConn);//�ر�
    WSACleanup();//�ͷ���Դ�Ĳ���
}

int main()
{
    SOCKET serSocket = buildSocket();
    //��Ҫ�󶨵Ĳ�������Ҫ�Ǳ��ص�socket��һЩ��Ϣ��
    SOCKADDR_IN addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//ip��ַ
    addr.sin_port=htons(12345);//�󶨶˿�

    bind(serSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR));//�����
    listen(serSocket, 5);//���еڶ������������ܹ����յ�����������
    SOCKET serConn;
    while (1)
    {
        SOCKADDR_IN clientsocket = waitConnect(&serSocket, &serConn);
        getText(&serConn, &clientsocket);
        printf("�ͻ����׽���ʧЧ\n");
    }
    close(&serConn);
    return 0;
}
