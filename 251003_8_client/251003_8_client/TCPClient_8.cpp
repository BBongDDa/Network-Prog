#include "C:\Users\dlwjd\Downloads\Common.h" 

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define CLIENTPORT 50000 // 고정할 클라이언트 포트

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

    // bind() 로컬 포트 번호를 50000으로 고정
    struct sockaddr_in clientaddr;
    memset(&clientaddr, 0, sizeof(clientaddr));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 IP 주소에서 접속 허용
    clientaddr.sin_port = htons(CLIENTPORT);
    retval = bind(sock, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    if (retval == SOCKET_ERROR) {
        err_quit("bind()");
    }
    printf("클라이언트 포트를 %d로 고정했습니다.\n", CLIENTPORT);


    // connect()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");

    printf("서버에 성공적으로 연결되었습니다.\n");
    printf("아무 키나 누르면 프로그램을 종료합니다...");
    getchar(); // 프로그램이 바로 종료되지 않도록 잠시 대기

    // 소켓 닫기
    closesocket(sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}