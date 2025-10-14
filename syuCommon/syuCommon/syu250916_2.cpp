#include <C:\Users\dlwjd\Downloads\Common.h>

int main(int argc, char* argv[])
{
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    /*------------------*/
    /* IPv4 변환 연습 */
    /*------------------*/
    // 원래의 IPv4 주소 출력
    const char* ipv4test = "147.46.114.70";
    printf("IPv4 주소(변환 전) = %s\n", ipv4test);

    // inet_pton() 함수 연습
    struct in_addr ipv4num;
    inet_pton(AF_INET, ipv4test, &ipv4num);
    printf("IPv4 주소(변환 후) = %#x\n", ipv4num.s_addr);

    // inet_ntop() 함수 연습
    char ipv4str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ipv4num, ipv4str, sizeof(ipv4str));
    printf("IPv4 주소(다시 변환 후) = %s\n", ipv4str);
    printf("\n");

    // 윈속 종료
    WSACleanup();
    return 0;
}