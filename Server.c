#include <stdio.h>
#include <winsock2.h> //ws2_32.lib 확인

#define PORT_NUM      5010 // 사용할 포트번호 지정-----------------------
#define BLOG_SIZE     5     // 백 로그 큐 크기 지정
#define MAX_MSG_LEN   256  // 메시지 크기 지정---------------------------

SOCKET SetTCPServer(short pnum, int blog); 
void ErrorHandling(char* message); 
void AcceptLoop(SOCKET sock);
void ReSend(SOCKET dosock); 

int main()
{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //준비과정으로 window와 맞는게 없으면 error---------------------
    {
        ErrorHandling("WSAStartup() error!"); 
    }

    SOCKET serv_sock = SetTCPServer(PORT_NUM, BLOG_SIZE); //대기 소켓 설정

    if (serv_sock == SOCKET_ERROR)  // 소켓 생성에 실패한 경우 에러 표시 및 프로그램 종료
    {
        ErrorHandling("hSocketet() error!");
    }

    AcceptLoop(serv_sock);
    WSACleanup(); //윈도우 소켓 해제
    return 0;
}

SOCKET SetTCPServer(short pnum, int blog)
{
    SOCKET serv_sock; 
    SOCKADDR_IN serv_addr = { 0 }; 
    int serv_error = 0; 

    // 소켓 생성 변수 지정
    serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  

    if (serv_sock == SOCKET_ERROR)   // 소켓 생성에 실패한 경우 에러 표시 및 프로그램 종료
    {
        ErrorHandling("hSocketet() error!");
    }

    serv_addr.sin_family = AF_INET; // IPv4 사용 -- server의 주소를 채워놓은 것이다. ------------------------------------
    
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // -- system 내 IP주소를  s_addr로 저장한다. ------------------------
    // 서버 IP 지정

    serv_addr.sin_port = htons(PORT_NUM);   // 연결 포트 지정 -------------------------

    serv_error = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));  // 소켓 주소 할당 변수 지정----------------

    if (serv_error == SOCKET_ERROR) // 소켓 주소 할당에 실패할 경우 에러표시 및 프로그램 종료   //바인드 성공시 listen으로 넘어간다.----------------
    {
        ErrorHandling("bind() error!");
    }

    serv_error = listen(serv_sock, blog);   // 소켓 연결 요청 대기 변수 지정

    if (serv_error == SOCKET_ERROR) // 연결 요청 대기중 문제 발생시 에러표시 및 프로그램 종료
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

    clnt_addr_size = sizeof(clnt_addr); // 클라이언트 주소 크기를 변수에 대입

    while (true)    // 연결 대기중 반복
    {
        // 클라이언트와의 연결에 실패한 경우 에러표시 및 프로그램 종료
        dosock = accept(sock, (SOCKADDR*)&clnt_addr, &clnt_addr_size); //accept가 child socket 서버는 이것 계속 만들어 사용, 클라이언트는 쓰던 것사용 이유는 계속 통신하고 받아야하기에
        if (dosock == SOCKET_ERROR)
        {
            ErrorHandling("accept() error");
        }
        printf("Accept connection %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));  // 클라이언트와 연결 성공시 해당 IP 주소와 포트번호 기록

        ReSend(dosock); // while문 종료 및 수신된 내용 기록 및 송신 함수 호출
    }
    closesocket(sock); 
}

// 클라이언트로부터 수신된 내용 기록 및 송신 함수
void ReSend(SOCKET dosock)
{
    char msg[MAX_MSG_LEN] = ""; // 문자열 변수 초기화

    while (recv(dosock, msg, sizeof(msg), 0) > 0)   // 클라이언트로부터 입력된 내용이 수신될 때까지 반복
    {
        printf("Client message : %s \n", msg);

        send(dosock, msg, sizeof(msg), 0);  // 수신이 완료되면 다시 해당 내용을 송신
    }
    closesocket(dosock); // 소켓 닫기
}

// 에러 내용 출력 함수
void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}