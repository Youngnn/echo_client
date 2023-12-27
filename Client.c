#include <stdio.h>
#include <Winsock2.h>

#define PORT_NUM    5010 // 사용할 포트 번호 지정 ----
#define MAX_MSG_LEN 256 //  메시지 크기 지정
#define SERVER_IP   "127.0.0.1" //서버 IP 주소--------

void ErrorHandling(char* message); // 에러 내용 출력 함수 호출

int main()
{
    WSADATA wsaData;
    SOCKET clnt_sock;
    int con_error = 0; 
    char msg[MAX_MSG_LEN] = ""; 
    int true = 1; 
    struct sockaddr_in serv_addr = { 0 }; 

    //윈도우 소켓 초기화가 되지 않은 경우 에러 표시 및 프로그램 종료
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //준비과정으로 window에 필요한 것 없으면 error -----
    {
        ErrorHandling("WSAStartup() error!");
    }

    clnt_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 

    if (clnt_sock == SOCKET_ERROR)  // 소켓 생성에 실패한 경우 에러 표시 및 프로그램 종료 
    {
        ErrorHandling("hSocketet() error!");
    }

    serv_addr.sin_family = AF_INET; // IPv4 사용 -- server의 주소를 채워놓은 것이다. 

    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);   // 서버에 접속하기 위해 서버 IP 지정 -- htonl()이 생략되어있는 이유 설명하기---------

    serv_addr.sin_port = htons(PORT_NUM);   // 연결 포트 지정 -- 5010으로 지정 해주었다 있다가 shark에서 볼 때 확인한다.----------------

    con_error = connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));    // 연결 요청 변수 지정 -- 포인터로 주소정보 넘겨준다-------

    if (con_error == SOCKET_ERROR)  //연결에 실패한 경우 에러 표시 및 프로그램 종료
    {
        ErrorHandling("connect() error!");
    }

    while (true)    // 서버와 연결이 정상적으로 된 경우 while문 실행
    {
        fputs("Enter a message to send. (Please enter 'stop' to exit.) : ", stdout);
        gets_s(msg, MAX_MSG_LEN); // 입력받은 내용을 변수 msg에 삽입
        send(clnt_sock, msg, sizeof(msg), 0); // 변수 msg의 내용을 서버로 송신

        // stop 이 입력되면 while문 종료
        if (strcmp(msg, "stop") == 0)
        {
            fputs("Shut down the client.", stdout);
            break;
        }

        // 서버로부터 수신된 내용 확인
        recv(clnt_sock, msg, sizeof(msg), 0); //server가 connect한 것 받음.---------------
        printf("Sent message : %s \n\n", msg);
    }

    closesocket(clnt_sock); //소켓 닫기
    WSACleanup(); //윈도우 소켓 해제화
    return 0;
}

// 에러 내용 출력 함수
void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}