#include <stdio.h>
#include <winsock2.h> //ws2_32.lib Ȯ��

#define PORT_NUM      5010 // ����� ��Ʈ��ȣ ����-----------------------
#define BLOG_SIZE     5     // �� �α� ť ũ�� ����
#define MAX_MSG_LEN   256  // �޽��� ũ�� ����---------------------------

SOCKET SetTCPServer(short pnum, int blog); 
void ErrorHandling(char* message); 
void AcceptLoop(SOCKET sock);
void ReSend(SOCKET dosock); 

int main()
{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //�غ�������� window�� �´°� ������ error---------------------
    {
        ErrorHandling("WSAStartup() error!"); 
    }

    SOCKET serv_sock = SetTCPServer(PORT_NUM, BLOG_SIZE); //��� ���� ����

    if (serv_sock == SOCKET_ERROR)  // ���� ������ ������ ��� ���� ǥ�� �� ���α׷� ����
    {
        ErrorHandling("hSocketet() error!");
    }

    AcceptLoop(serv_sock);
    WSACleanup(); //������ ���� ����
    return 0;
}

SOCKET SetTCPServer(short pnum, int blog)
{
    SOCKET serv_sock; 
    SOCKADDR_IN serv_addr = { 0 }; 
    int serv_error = 0; 

    // ���� ���� ���� ����
    serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  

    if (serv_sock == SOCKET_ERROR)   // ���� ������ ������ ��� ���� ǥ�� �� ���α׷� ����
    {
        ErrorHandling("hSocketet() error!");
    }

    serv_addr.sin_family = AF_INET; // IPv4 ��� -- server�� �ּҸ� ä������ ���̴�. ------------------------------------
    
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // -- system �� IP�ּҸ�  s_addr�� �����Ѵ�. ------------------------
    // ���� IP ����

    serv_addr.sin_port = htons(PORT_NUM);   // ���� ��Ʈ ���� -------------------------

    serv_error = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));  // ���� �ּ� �Ҵ� ���� ����----------------

    if (serv_error == SOCKET_ERROR) // ���� �ּ� �Ҵ翡 ������ ��� ����ǥ�� �� ���α׷� ����   //���ε� ������ listen���� �Ѿ��.----------------
    {
        ErrorHandling("bind() error!");
    }

    serv_error = listen(serv_sock, blog);   // ���� ���� ��û ��� ���� ����

    if (serv_error == SOCKET_ERROR) // ���� ��û ����� ���� �߻��� ����ǥ�� �� ���α׷� ����
    {
        ErrorHandling("listen() error");
    }
    return serv_sock;
}

void AcceptLoop(SOCKET sock)    
{
    SOCKET dosock;
    SOCKADDR_IN clnt_addr = { 0 };
    int true = 1;
    int clnt_addr_size; 

    clnt_addr_size = sizeof(clnt_addr); // Ŭ���̾�Ʈ �ּ� ũ�⸦ ������ ����

    while (true)    // ���� ����� �ݺ�
    {
        // Ŭ���̾�Ʈ���� ���ῡ ������ ��� ����ǥ�� �� ���α׷� ����
        dosock = accept(sock, (SOCKADDR*)&clnt_addr, &clnt_addr_size); //accept�� child socket ������ �̰� ��� ����� ���, Ŭ���̾�Ʈ�� ���� �ͻ�� ������ ��� ����ϰ� �޾ƾ��ϱ⿡
        if (dosock == SOCKET_ERROR)
        {
            ErrorHandling("accept() error");
        }
        printf("Accept connection %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));  // Ŭ���̾�Ʈ�� ���� ������ �ش� IP �ּҿ� ��Ʈ��ȣ ���

        ReSend(dosock); // while�� ���� �� ���ŵ� ���� ��� �� �۽� �Լ� ȣ��
    }
    closesocket(sock); 
}

// Ŭ���̾�Ʈ�κ��� ���ŵ� ���� ��� �� �۽� �Լ�
void ReSend(SOCKET dosock)
{
    char msg[MAX_MSG_LEN] = ""; // ���ڿ� ���� �ʱ�ȭ

    while (recv(dosock, msg, sizeof(msg), 0) > 0)   // Ŭ���̾�Ʈ�κ��� �Էµ� ������ ���ŵ� ������ �ݺ�
    {
        printf("Client message : %s \n", msg);

        send(dosock, msg, sizeof(msg), 0);  // ������ �Ϸ�Ǹ� �ٽ� �ش� ������ �۽�
    }
    closesocket(dosock); // ���� �ݱ�
}

// ���� ���� ��� �Լ�
void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}