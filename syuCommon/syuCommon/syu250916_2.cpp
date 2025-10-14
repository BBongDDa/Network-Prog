#include <C:\Users\dlwjd\Downloads\Common.h>

int main(int argc, char* argv[])
{
    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    /*------------------*/
    /* IPv4 ��ȯ ���� */
    /*------------------*/
    // ������ IPv4 �ּ� ���
    const char* ipv4test = "147.46.114.70";
    printf("IPv4 �ּ�(��ȯ ��) = %s\n", ipv4test);

    // inet_pton() �Լ� ����
    struct in_addr ipv4num;
    inet_pton(AF_INET, ipv4test, &ipv4num);
    printf("IPv4 �ּ�(��ȯ ��) = %#x\n", ipv4num.s_addr);

    // inet_ntop() �Լ� ����
    char ipv4str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ipv4num, ipv4str, sizeof(ipv4str));
    printf("IPv4 �ּ�(�ٽ� ��ȯ ��) = %s\n", ipv4str);
    printf("\n");

    // ���� ����
    WSACleanup();
    return 0;
}