#include "C:\Users\dlwjd\Downloads\Common.h"
#include <cstdint>

#define SERVERPORT 9000

int main(int argc, char* argv[])
{
    int retval;

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // ���� ����
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    // ������ ��ſ� ����� ����
    SOCKET client_sock;
    struct sockaddr_in clientaddr;
    int addrlen;

    printf("TCP ������ ���۵Ǿ����ϴ�. Ŭ���̾�Ʈ�� ������ ��ٸ��ϴ�...\n");

    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        // ������ Ŭ���̾�Ʈ ���� ���
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
            addr, ntohs(clientaddr.sin_port));

        // Ŭ���̾�Ʈ�� ������ ���
        while (1) {
            int32_t received_nums[2];

            // ������ �ޱ� (32��Ʈ ���� 2�� = 8����Ʈ)
            retval = recv(client_sock, (char*)received_nums, sizeof(received_nums), MSG_WAITALL);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0) {
                break;
            }

            // ���� ������ ó�� (Network to Host Byte Order)
            int32_t num1 = ntohl(received_nums[0]);
            int32_t num2 = ntohl(received_nums[1]);
            int32_t sum = num1 + num2;

            printf("[TCP/%s:%d] ���� ����: %d, %d | ���� ��: %d\n", addr, ntohs(clientaddr.sin_port), num1, num2, sum);

            // ������ ������ (Host to Network Byte Order)
            int32_t sum_to_send = htonl(sum);
            retval = send(client_sock, (char*)&sum_to_send, sizeof(sum_to_send), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
        }

        // ���� �ݱ�
        closesocket(client_sock);
        printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
            addr, ntohs(clientaddr.sin_port));
    }

    // ���� ���� �ݱ� �� ���� ����
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}