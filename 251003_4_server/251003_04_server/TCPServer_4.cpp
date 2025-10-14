#include "C:\Users\dlwjd\Downloads\Common.h"
#include <cstdint>

#define SERVERPORT 9000

int main(int argc, char* argv[])
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // 소켓 생성
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

    // 데이터 통신에 사용할 변수
    SOCKET client_sock;
    struct sockaddr_in clientaddr;
    int addrlen;

    printf("TCP 서버가 시작되었습니다. 클라이언트의 접속을 기다립니다...\n");

    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        // 접속한 클라이언트 정보 출력
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
            addr, ntohs(clientaddr.sin_port));

        // 클라이언트와 데이터 통신
        while (1) {
            int32_t received_nums[2];

            // 데이터 받기 (32비트 정수 2개 = 8바이트)
            retval = recv(client_sock, (char*)received_nums, sizeof(received_nums), MSG_WAITALL);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0) {
                break;
            }

            // 받은 데이터 처리 (Network to Host Byte Order)
            int32_t num1 = ntohl(received_nums[0]);
            int32_t num2 = ntohl(received_nums[1]);
            int32_t sum = num1 + num2;

            printf("[TCP/%s:%d] 받은 숫자: %d, %d | 계산된 합: %d\n", addr, ntohs(clientaddr.sin_port), num1, num2, sum);

            // 데이터 보내기 (Host to Network Byte Order)
            int32_t sum_to_send = htonl(sum);
            retval = send(client_sock, (char*)&sum_to_send, sizeof(sum_to_send), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
        }

        // 소켓 닫기
        closesocket(client_sock);
        printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
            addr, ntohs(clientaddr.sin_port));
    }

    // 리슨 소켓 닫기 및 윈속 종료
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}