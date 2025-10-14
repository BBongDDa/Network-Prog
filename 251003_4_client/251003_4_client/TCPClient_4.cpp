#include "C:\Users\dlwjd\Downloads\Common.h" 
#include <cstdint> 

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000

int main(int argc, char* argv[])
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // 소켓 생성
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

    printf("TCP 클라이언트가 서버에 연결되었습니다.\n");

    // 서버와 데이터 통신
    while (1) {
        int32_t num1, num2;

        // 사용자로부터 숫자 입력
        printf("\n첫 번째 숫자를 입력하세요: ");
        scanf("%d", &num1);
        printf("첫 번째 숫자: %d\n", num1);

        printf("두 번째 숫자를 입력하세요: ");
        scanf("%d", &num2);
        printf("두 번째 숫자: %d\n", num2);

        // 보낼 데이터 준비 (Host to Network Byte Order)
        int32_t nums_to_send[2];
        nums_to_send[0] = htonl(num1);
        nums_to_send[1] = htonl(num2);

        // 데이터 보내기
        retval = send(sock, (char*)nums_to_send, sizeof(nums_to_send), 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }
        printf("서버로 '%d'와(과) '%d'를 전송했습니다.\n", num1, num2);

        // 데이터 받기
        int32_t sum_received;
        retval = recv(sock, (char*)&sum_received, sizeof(sum_received), MSG_WAITALL);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0) {
            printf("\n서버와의 연결이 끊겼습니다.\n");
            break;
        }

        // 받은 데이터 처리 (Network to Host Byte Order) 및 출력
        int32_t sum = ntohl(sum_received);
        printf("서버로부터 받은 합계: %d\n", sum);
    }

    // 소켓 닫기
    closesocket(sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}