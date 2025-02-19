#include "precompiled.h"
#include "SocketClient.h"
#include <WS2tcpip.h>

#pragma comment (lib, "wsock32.lib")

#define SERVER_ADDR "54.174.136.215"
#define SERVER_PORT 3001
#define BUFLEN 512

int sendtoServer_Windows(std::string str) {
	// Winsock data
	WSADATA wsa_data;
	int res;

	// Initializing Winsock
	res = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (res != 0) {
		//debug_warn(L"Error initializing Winsock");
		WSACleanup();
		return -1;
	}

	// Creating Socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock == INVALID_SOCKET) {
		//debug_warn(L"Error in creating socket\n");
		WSACleanup();
		return -1;
	}

	// Resolve the hostname to an IP address, stored in the hostent structure
	char *hostname = SERVER_ADDR;
	struct hostent *server = gethostbyname(hostname);
	if (server == NULL) {
		//debug_warn(L"Error in resolving hostname to an IP address\n");
		WSACleanup();
		return -1;
	}

	SOCKADDR_IN server_info;
	server_info.sin_port = htons(SERVER_PORT);
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = *((unsigned long *)server->h_addr);
	
	// Connect to server
	res = connect(sock, (SOCKADDR*)(&server_info), sizeof(server_info));
	if (res != 0) {
		//debug_warn(L"Error connecting to server\n");
		WSACleanup();
	}

	// Send data to server
	str.append("\n");
	long strSize = str.length() + 1;

	res = send(sock, str.c_str(), strSize, 0);
	if (res == SOCKET_ERROR) {
		//debug_warn(L"Error in sending data");
		WSACleanup();
	}

	
	// Getting data from server
	char *wbuffer;
	wbuffer = new char[BUFLEN];
	res = recv(sock, wbuffer, BUFLEN, 0);
	if (res != 0) {
		//debug_warn(L"Error in receiving data");
		WSACleanup();
	}

	shutdown(sock, SD_SEND);
	closesocket(sock);
	WSACleanup();
	return 0;
}
