#include <stdio.h>
#include <Winsock2.h>

#define PORT_NUM    5010 // ����� ��Ʈ ��ȣ ���� ----
#define MAX_MSG_LEN 256 //  �޽��� ũ�� ����
#define SERVER_IP   "127.0.0.1" //���� IP �ּ�--------

void ErrorHandling(char* message); // ���� ���� ��� �Լ� ȣ��

int main()
{
    WSADATA wsaData;
    SOCKET clnt_sock;
    int con_error = 0; 
    char msg[MAX_MSG_LEN] = ""; 
    int true = 1; 
    struct sockaddr_in serv_addr = { 0 }; 

    //������ ���� �ʱ�ȭ�� ���� ���� ��� ���� ǥ�� �� ���α׷� ����
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //�غ�������� window�� �ʿ��� �� ������ error -----
    {
        ErrorHandling("WSAStartup() error!");
    }

    clnt_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 

    if (clnt_sock == SOCKET_ERROR)  // ���� ������ ������ ��� ���� ǥ�� �� ���α׷� ���� 
    {
        ErrorHandling("hSocketet() error!");
    }

    serv_addr.sin_family = AF_INET; // IPv4 ��� -- server�� �ּҸ� ä������ ���̴�. 

    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);   // ������ �����ϱ� ���� ���� IP ���� -- htonl()�� �����Ǿ��ִ� ���� �����ϱ�---------

    serv_addr.sin_port = htons(PORT_NUM);   // ���� ��Ʈ ���� -- 5010���� ���� ���־��� �ִٰ� shark���� �� �� Ȯ���Ѵ�.----------------

    con_error = connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));    // ���� ��û ���� ���� -- �����ͷ� �ּ����� �Ѱ��ش�-------

    if (con_error == SOCKET_ERROR)  //���ῡ ������ ��� ���� ǥ�� �� ���α׷� ����
    {
        ErrorHandling("connect() error!");
    }

    while (true)    // ������ ������ ���������� �� ��� while�� ����
    {
        fputs("Enter a message to send. (Please enter 'stop' to exit.) : ", stdout);
        gets_s(msg, MAX_MSG_LEN); // �Է¹��� ������ ���� msg�� ����
        send(clnt_sock, msg, sizeof(msg), 0); // ���� msg�� ������ ������ �۽�

        // stop �� �ԷµǸ� while�� ����
        if (strcmp(msg, "stop") == 0)
        {
            fputs("Shut down the client.", stdout);
            break;
        }

        // �����κ��� ���ŵ� ���� Ȯ��
        recv(clnt_sock, msg, sizeof(msg), 0); //server�� connect�� �� ����.---------------
        printf("Sent message : %s \n\n", msg);
    }

    closesocket(clnt_sock); //���� �ݱ�
    WSACleanup(); //������ ���� ����ȭ
    return 0;
}

// ���� ���� ��� �Լ�
void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}