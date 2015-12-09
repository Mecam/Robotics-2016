#include <thread>
#include <ws2tcpip.h>
#include <cstdio>
#pragma comment(lib, "ws2_32.lib")
WSADATA wsadat;
SOCKET connection, listens;
struct addrinfo *listena = NULL, server;
struct sockaddr client;
char rbuf[120], sbuf[120], recs[3], snds[3];
char* port = "666";
bool close = false;
char usr[3], *username;
void rec()
{
	while (!close)
	{
		ZeroMemory(rbuf, 120);
		ZeroMemory(recs, 3);
		if (recv(connection, rbuf, 5, 0) == SOCKET_ERROR)
			close = true;

		if (rbuf[0])
			printf("Data::%s\n", rbuf);
	}
}
void sd()
{
	while (!close)
	{
		ZeroMemory(sbuf, 120);
		ZeroMemory(snds, 3);
		scanf("%s", sbuf);
		_itoa(strlen(sbuf), snds, 10);
		send(connection, snds, 3, 0);
		send(connection, sbuf, strlen(sbuf), 0);
	}
}
int main()
{
	WSAStartup(MAKEWORD(2, 2), &wsadat);

	ZeroMemory(&server, sizeof(server));
	server.ai_family = AF_INET;
	server.ai_socktype = SOCK_STREAM;
	server.ai_protocol = IPPROTO_TCP;
	server.ai_flags = AI_PASSIVE;


	if (getaddrinfo(NULL, port, &server, &listena) != 0) {
		printf("Error!\n");
		WSACleanup();
		return 1;
	}

	listens = socket(listena->ai_family, listena->ai_socktype, listena->ai_protocol);
	if (listens == INVALID_SOCKET)
	{
		printf("Error, invalid socket!\n");
		WSACleanup();
		return 1;
	}

	if (bind(listens, listena->ai_addr, (int)listena->ai_addrlen) == SOCKET_ERROR);
	freeaddrinfo(listena);
	listen(listens, SOMAXCONN);
	int x = WSAGetLastError();
	while (true)
	{
		connection = accept(listens, NULL, NULL);
		printf("A client has connected!\n");
		std::thread recieve(rec), send(sd);

		recieve.join();
		send.join();
	}
	closesocket(connection);
	WSACleanup();
	return 0;
}