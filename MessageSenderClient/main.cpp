#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<string>

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
int main() {
	//winsock setup
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//define buffer
	const int MAXLINE = 20;
	char recvline[MAXLINE + 1];
	int n;
	//define socket
	SOCKET sock;
	sock = Socket(AF_INET, SOCK_STREAM, 0);
	//initialize servaddr 
	sockaddr_in servaddr = {};
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9999);
	//change IP to int and write result to servaddr.sin_addr;
	ChangeIPtoInt(AF_INET, "127.0.0.1", servaddr.sin_addr);
	//by now servaddr has information of port,protocol,and server's IP address
	//so that we can connect to server
	Connect(sock, (sockaddr*)&servaddr);
	//receive message and output
	while ((n = recv(sock, recvline, MAXLINE,0)) > 0) {
		recvline[n] = 0;
		cout << recvline << endl;
		if (cout.eof())
			raiseError("fputs error");
	}
	if (n < 0)
		raiseError("read error");
	
	closesocket(sock);
	WSACleanup();

}