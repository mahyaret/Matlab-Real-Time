// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

using namespace std;

int main(int argc, char **argv)
{
	WSADATA            wsaData;
	SOCKET             SRSocket;
	SOCKADDR_IN        ReceiverAddr;
	int                recPort = 5150, sndPort = 5151;
	char			   ReceiveBuf[1] = { 0 }, SendBuf[1] = { 0 };
	int                BufLength = 1;
	SOCKADDR_IN        SenderAddr;
	int                SenderAddrSize = sizeof(SenderAddr), ReceiverAddrSize = sizeof(ReceiverAddr);
	int                ByteReceived = 5, ByteSent=5;

	// Initialize Winsock version 2.2
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Server: WSAStartup failed with error %ld\n", WSAGetLastError());
		return -1;
	}
	else
		printf("Server: The Winsock DLL status is %s.\n", wsaData.szSystemStatus);



	// Create a new socket to receive datagrams on.
	SRSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SRSocket == INVALID_SOCKET)
	{
		printf("Server: Error at socket(): %ld\n", WSAGetLastError());
		// Clean up
		WSACleanup();
		// Exit with error
		return -1;
	}
	else
		printf("Server: socket() is OK!\n");

	// Set up a SOCKADDR_IN structure that will tell bind that we
	// want to receive datagrams from all interfaces using port 5150.
	// The IPv4 family
	ReceiverAddr.sin_family = AF_INET;
	// Port no. 5150
	ReceiverAddr.sin_port = htons(recPort);
	// From all interface (0.0.0.0)
	ReceiverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	SenderAddr.sin_family = AF_INET;
	SenderAddr.sin_port = htons(sndPort);
	SenderAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// Associate the address information with the socket using bind.
	// At this point you can receive datagrams on your bound socket.
	if (bind(SRSocket, (SOCKADDR *)&ReceiverAddr, sizeof(ReceiverAddr)) == SOCKET_ERROR)
	{
		printf("Server: bind() failed! Error: %ld.\n", WSAGetLastError());
		// Close the socket
		closesocket(SRSocket);
		// Do the clean up
		WSACleanup();
		// and exit with error
		return -1;
	}
	else
		printf("Server: bind() is OK!\n");

	// Some info on the receiver side...
	//getsockname(SRSocket, (SOCKADDR *)&ReceiverAddr, (int *)sizeof(ReceiverAddr));
	printf("Server: Receiving IP(s) used: %s\n", inet_ntoa(ReceiverAddr.sin_addr));
	printf("Server: Receiving port used: %d\n", htons(ReceiverAddr.sin_port));
	printf("Server: Sending IP used: %s\n", inet_ntoa(SenderAddr.sin_addr));
	printf("Server: Sending port used: %d\n", htons(SenderAddr.sin_port));
	printf("Server: I\'m ready to receive a datagram...\n");

	// At this point you can receive datagrams on your bound socket.
	while (ByteReceived > 0 && ByteSent > 0){
		ByteReceived = recvfrom(SRSocket, ReceiveBuf, BufLength, 0,
			(SOCKADDR *)&ReceiverAddr, &ReceiverAddrSize);

		if (ByteReceived > 0)
		{
			printf("Server: Total Bytes received: %d\n", ByteReceived);
			printf("Server: Total Bytes sent: %d\n", ByteSent);
			printf("Server: The data is \"%d\"\n", ReceiveBuf[0]); //<< 56) | (ReceiveBuf)[6] << 48) | (ReceiveBuf[5] << 40) | (ReceiveBuf[4] << 32) | (ReceiveBuf[3] << 24) | (ReceiveBuf[2] << 16) | (ReceiveBuf[1] << 8) | ReceiveBuf[0]));
		}
		else if (ByteReceived <= 0)
			printf("Server: Connection closed with error code: %ld\n", WSAGetLastError());
		else
			printf("Server: recvfrom() failed with error code: %d\n", WSAGetLastError());

		ByteSent = sendto(SRSocket, ReceiveBuf, BufLength, 0,
			(SOCKADDR *)&SenderAddr, SenderAddrSize);

		if (ByteSent > 0)
		{
			printf("Server: Total Bytes sent: %d\n", ByteSent);
		}
		else if (ByteSent <= 0)
			printf("Server: Connection closed with error code: %ld\n", WSAGetLastError());
		else
			printf("Server: recvfrom() failed with error code: %d\n", WSAGetLastError());
	}

	// When your application is finished receiving datagrams close the socket.
	printf("Server: Finished receiving. Closing the listening socket...\n");
	if (closesocket(SRSocket) != 0)
		printf("Server: closesocket() failed! Error code: %ld\n", WSAGetLastError());
	else
		printf("Server: closesocket() is OK\n");

	// When your application is finished call WSACleanup.
	printf("Server: Cleaning up...\n");
	if (WSACleanup() != 0)
		printf("Server: WSACleanup() failed! Error code: %ld\n", WSAGetLastError());
	else
		printf("Server: WSACleanup() is OK\n");

	// Back to the system

	return 0;
}


