#include "C:\Users\dlwjd\Downloads\Common.h" 
#include <cstdint> 

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000

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

    // connect()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");

    printf("TCP Ŭ���̾�Ʈ�� ������ ����Ǿ����ϴ�.\n");

    // ������ ������ ���
    while (1) {
        int32_t num1, num2;

        // ����ڷκ��� ���� �Է�
        printf("\nù ��° ���ڸ� �Է��ϼ���: ");
        scanf("%d", &num1);
        printf("ù ��° ����: %d\n", num1);

        printf("�� ��° ���ڸ� �Է��ϼ���: ");
        scanf("%d", &num2);
        printf("�� ��° ����: %d\n", num2);

        // ���� ������ �غ� (Host to Network Byte Order)
        int32_t nums_to_send[2];
        nums_to_send[0] = htonl(num1);
        nums_to_send[1] = htonl(num2);

        // ������ ������
        retval = send(sock, (char*)nums_to_send, sizeof(nums_to_send), 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }
        printf("������ '%d'��(��) '%d'�� �����߽��ϴ�.\n", num1, num2);

        // ������ �ޱ�
        int32_t sum_received;
        retval = recv(sock, (char*)&sum_received, sizeof(sum_received), MSG_WAITALL);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0) {
            printf("\n�������� ������ ������ϴ�.\n");
            break;
        }

        // ���� ������ ó�� (Network to Host Byte Order) �� ���
        int32_t sum = ntohl(sum_received);
        printf("�����κ��� ���� �հ�: %d\n", sum);
    }

    // ���� �ݱ�
    closesocket(sock);

    // ���� ����
    WSACleanup();
    return 0;
}