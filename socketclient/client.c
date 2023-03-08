#include <winsock2.h>
#include <stdio.h>
#include <stdbool.h>

#pragma comment(lib,"ws2_32.lib")
int buildSocket(SOCKET *clientSocket)
{
    WORD versionRequired;
    WSADATA wsaData;
    versionRequired = MAKEWORD(2,2);
    int err = WSAStartup(versionRequired, &wsaData);//Э���İ汾��Ϣ
    //ͨ��WSACleanup�ķ���ֵ��ȷ��socketЭ���Ƿ�����
    if (!err)
    {
        printf("�ͻ��˳ɹ������׽���\n");
        *clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        return 1;
    }
    else
    {
        printf("�ͻ��˴����׽���ʧ��\n");
        return 0;//����
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
    //�رշ���
    WSACleanup();
}
int main()
{
    //socket����У���������һ���ṹ��SOCKADDR_IN����������һЩ��Ϣ����socket��ϵͳ��
    //�˿ںţ�ip��ַ����Ϣ������洢���Ƿ������˵ļ��������Ϣ
    SOCKADDR_IN clientsock_in;
    clientsock_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    clientsock_in.sin_family = AF_INET;
    clientsock_in.sin_port = htons(12345);
    //ע��socket�������������������������socket��������ϵͳ��socket�����ͣ��Լ�һЩ������Ϣ
    int result = 0;
    //ǰ�ڶ������׽��֣������˷������˵ļ������һЩ��Ϣ�洢��clientsock_in�У�
    //׼��������ɺ�Ȼ��ʼ������׽������ӵ�Զ�̵ļ����
    //Ҳ���ǵ�һ������
    while (1)
    {
        SOCKET clientSocket;
        while(result == 0) {
            result = buildSocket(&clientSocket);
        }
        result = -1;
        while(result == -1)
        {
            result = connect(clientSocket,(SOCKADDR*)&clientsock_in,sizeof(SOCKADDR));//��ʼ����
        }
        result = 0;
        printf("���ӷ�����׽��ֳɹ�\n");
        sendText(&clientSocket);
        closesocket(clientSocket);
        printf("������׽���ʧЧ\n");
    }
    WSACleanup();
    return 0;
}
