#include "C:\Users\dlwjd\Downloads\Common.h" 

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define CLIENTPORT 50000 // ������ Ŭ���̾�Ʈ ��Ʈ

int main(int argc, char* argv[])
{
    int retval;

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // ���� ����
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // bind() ���� ��Ʈ ��ȣ�� 50000���� ����
    struct sockaddr_in clientaddr;
    memset(&clientaddr, 0, sizeof(clientaddr));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = htonl(INADDR_ANY); // ��� IP �ּҿ��� ���� ���
    clientaddr.sin_port = htons(CLIENTPORT);
    retval = bind(sock, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    if (retval == SOCKET_ERROR) {
        err_quit("bind()");
    }
    printf("Ŭ���̾�Ʈ ��Ʈ�� %d�� �����߽��ϴ�.\n", CLIENTPORT);


    // connect()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");

    printf("������ ���������� ����Ǿ����ϴ�.\n");
    printf("�ƹ� Ű�� ������ ���α׷��� �����մϴ�...");
    getchar(); // ���α׷��� �ٷ� ������� �ʵ��� ��� ���

    // ���� �ݱ�
    closesocket(sock);

    // ���� ����
    WSACleanup();
    return 0;
}