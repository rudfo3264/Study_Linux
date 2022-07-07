#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>

void error_handling(char * message);		// 에러 발생 시 문구 출력
char webpage[] = "HTTP/1.1 200 OK\r\n"		// 접속할 웹페이지 양식(이미지 포함)
		"Server:Linux Web Server\r\n"
		"Content-Type: text/html; charset=UTF-8\r\n\r\n"
		"<!DOCTYPE html>\r\n"
		"<html><head><title> My Web Page </title>\r\n"
		"<style>body {background-color: #FFFF00 }</style></head>\r\n"
		"<body><center><h1>Feels Good!!</h1><br>\r\n"
		"<img src=\"snoopy_spoon.png\"></center></body></html>\r\n";

int main(int argc, char * argv[])		// argc가 변수 갯수, argv[0] 프로그램명 argv[1] 포트번호
{
	int serv_sock;		//	서버에서 클라이언트측의 통신을 받을 소켓
	int clnt_sock;		// 서버에서 클라이언트측으로 통신할 소켓
	struct sockaddr_in serv_addr;		// serv_socket 주소 구조체  sin_family가 AF_INET일때 사용
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;		// clnt_addr 크기값을 담을 변수
	char buf[1024];					// 웹 요청을 담을 공간
	char img_buf[100000];			// 이미지 파일을 담을 공간
	clnt_addr_size = sizeof(clnt_addr);
	int img;

	serv_sock = socket(PF_INET, SOCK_STREAM,0); // 소켓 생성 (IPv4,
	if(serv_sock == -1)		//소켓 생성 실패 시 -1 반환 / 성공시 파일 디스크립터 반환
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));			// memoryset
	serv_addr.sin_family = AF_INET;						// IPv4 사용 시 famliy 설정은 AF_INET
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// 어느 클라이언트가 통신을 요청하든 연결하겠다
	serv_addr.sin_port = htons(atoi(argv[1]));		// argv[1]은 파일 실행 시 사용자가 입력하는 포트번호

	if(bind(serv_sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr)) ==-1)	//serv_sock에 설정 bind
		error_handling("bind() error!");

	if(listen(serv_sock,5) == -1)		// serv_socket에서 클라이언트 통신 대기 최대 5개 클라이언트까지
		error_handling("listen() error!");

	while(1)
	{
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		// 클라이언트와 통신할 clnt_socket 생성
		if(clnt_sock == -1)
			error_handling("accept() error");

		read(clnt_sock, buf, sizeof(buf));		// clnt_sock이 클라이언트로부터 받은 값을 buf에 저장
		printf("%s\n",buf);							// bud에 저장된 값 확인용

		if(!strncmp(buf, "GET / HTTP/1.1", 14))	// buf에 저장된 값에 맨 앞글자부터 비교하여 같다면
		// strcnmp 글자 수만큼 비교. 모든 글자가 같으면 0, 사전적으로 앞 문자열이 빠르면 양수 느리면 음수 반환
			write(clnt_sock, webpage, sizeof(webpage));	// clnt_socket을 통해 클라이언트측으로 webpage 문자열 반환

		if(!strncmp(buf, "GET /snoopy_spoon.png",21))
		{
			img = open("snoopy_spoon.png", O_RDONLY);		// 이미지 파일을 읽기전용으로 open. 반환된 파일스크립터 저장
			read(img, img_buf, sizeof(img_buf));
			write(clnt_sock, img_buf, sizeof(img_buf));
			close(img);
			close(clnt_sock);	// clnt_sock 연결 종료
			break; // 반복문 탈출
		}
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
