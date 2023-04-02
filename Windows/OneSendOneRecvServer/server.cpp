#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>

enum CMD {
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR
};

struct DataHeader {
	short dataLength;
	short cmd;
};

struct Login : public DataHeader {
	Login() {
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char userName[32];
	char passWord[32];
};

struct LoginResult : public DataHeader {
	LoginResult() {
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
};

struct LoginOut : public DataHeader {
	LoginOut() {
		dataLength = sizeof(LoginOut);
		cmd = CMD_LOGOUT;
	}
	char userName[32];
};

struct LoginOutResult : public DataHeader {
	LoginOutResult() {
		dataLength = sizeof(LoginOutResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;

};

int main() {
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	int af = AF_INET;
	int type = SOCK_STREAM;
	int protocol = IPPROTO_TCP;
	SOCKET sock = socket(af, type, protocol);

	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.s_addr = INADDR_ANY;

	int bindRes = bind(sock, (sockaddr*)&_sin, sizeof(_sin));
	if (bindRes == SOCKET_ERROR) {
		printf("[server]绑定端口失败, bindRes=%d\n", bindRes);
	}

	int listenRes = listen(sock, 5);
	if (listenRes == SOCKET_ERROR) {
		printf("[server]监听失败,listenRes=%d\n", listenRes);
	}
	printf("[server]开始等待客户端连接...\n");

	sockaddr_in clientAddr;
	int nCliAddrLen = sizeof(clientAddr);

	int cliFd = accept(sock, (sockaddr*)&clientAddr, &nCliAddrLen);
	if (cliFd == INVALID_SOCKET) {
		printf("[server]accept 失败\n");
	}
	printf("[server]客户端连接地址=%s\n", inet_ntoa(clientAddr.sin_addr));

	DataHeader header;
	LoginResult loginResult;
	LoginOutResult loginOutResult;
	int nSend = 0;
	int nRecv = 0;
	char szRecv[1024] = {};
	while (1) {
		nRecv = recv(cliFd, (char*)&szRecv, sizeof(DataHeader), 0);
		if (nRecv < 0) {
			printf("[server]客户端已退出\n");
			break;
		}
		DataHeader* header = (DataHeader*)szRecv;
		if (nRecv > 0) {
			switch (header->cmd) {
			case CMD_LOGIN:
			{
				nRecv = recv(cliFd, (char*)&szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
				Login* login = (Login*)szRecv;
				printf("[server]收到的登录包的内容:header: cmd=%d, headlen=%d, user=%s, password=%s\n", header->cmd, header->dataLength, login->userName, login->passWord);
				nSend = send(cliFd, (char*)&loginResult, sizeof(loginResult), 0);
				break;
			}

			case CMD_LOGOUT:
			{
				nRecv = recv(cliFd, (char*)&szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
				LoginOut* loginOut = (LoginOut*)szRecv;
				printf("[server]收到的登录包的内容:header: cmd=%d, headlen=%d, user=%s,\n", header->cmd, header->dataLength, loginOut->userName);
				nSend = send(cliFd, (char*)&loginOutResult, sizeof(loginOutResult), 0);
				break;
			}
			}
		}

	}
	closesocket(sock);

	WSACleanup();
	getchar();
	return 0;
}
