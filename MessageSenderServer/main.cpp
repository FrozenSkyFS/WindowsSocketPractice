#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<string>
#include<time.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

void raiseError(char* s) {
	cout << s << endl;
	exit(0);
}
SOCKET Socket(int family, int type, int protocol) {
	int n;
	n = socket(family, type, protocol);
	if (n < 0) raiseError("socket error");
	return n;

}
int ChangeIPtoInt(int family, char* ip, in_addr& result) {
	int n = inet_pton(family, ip, &result);
	if (n <= 0) raiseError("change ip to int error");
	return n;
}
int Connect(SOCKET sock, sockaddr* sa) {
	int n = connect(sock, sa, sizeof(*sa));
	if (n < 0) raiseError("connect error");
	return n;
}
int Bind(SOCKET sock, sockaddr* sa) {
	int n = bind(sock, sa, sizeof(*sa));
	if (n < 0)raiseError("bind error");
	return n;
}
int main() {
	cout << "Server" << endl;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	const int MAXLINE = 20;
	SOCKET listenfd, connfd;
	int n;
	char recvline[MAXLINE + 1];
	sockaddr_in servaddr = {};
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9999);
	servaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	Bind(listenfd, (sockaddr*)&servaddr);
	listen(listenfd, 2);
	while (true) {
		connfd = accept(listenfd, nullptr, nullptr);
		cout << "connection is made " << endl;
		while (cin >> recvline) 
			send(connfd, recvline, strlen(recvline), 0);
		closesocket(connfd);
	}
	WSACleanup();

}