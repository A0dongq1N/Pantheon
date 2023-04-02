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
	_sin.sin_addr.s_addr = inet_addr("127.0.0.1");

	int conRes = connect(sock, (sockaddr*)&_sin, sizeof(_sin));
	if (conRes != 0) {
		printf("[client]连接服务器失败，conRes=%d\n", conRes);
		return 0;
	}
	printf("[client]连接服务器成功\n");

	int nRecv;
	int nSend;
	Login login;
	LoginOut loginOut;
	CHAR cmdBuf[32];
	while (1) {
		scanf("%s", cmdBuf);
		if (strcmp(cmdBuf, "exit") == 0) {
			printf("[client]客户端退出");
			break;
		}
		if (strcmp(cmdBuf, "login") == 0) {
			strcpy(login.userName, "jeremyqin");
			strcpy(login.passWord, "ok");
			nSend = send(sock, (char*)&login, sizeof(login), 0);

			LoginResult loginResult;
			nRecv = recv(sock, (char*)&loginResult, sizeof(loginResult), 0);
			printf("[client]login result=%d\n", loginResult.result);
		}

		if (strcmp(cmdBuf, "loginOut") == 0) {
			strcpy(loginOut.userName, "weifang");
			nSend = send(sock, (char*)&loginOut, sizeof(loginOut), 0);

			LoginOutResult loginOutResult;
			nRecv = recv(sock, (char*)&loginOutResult, sizeof(loginOutResult), 0);
			printf("[client]loginOutResult=%d\n", loginOutResult.result);
		}
	}

	closesocket(sock);

	WSACleanup();
	getchar();
	return 0;
}
