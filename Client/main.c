

/*
Authors:

Amit Tzah 316062959
Tomer Shimshi 203200480


Project: Ex4
*/
#pragma warning(suppress : 4996).


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")


#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "HardCodedData.h"
#include "client_funcs.h"


// #########GLOBALS#############
SOCKET m_socket;
char* user_name = NULL;



int main(int argc, char* argv[]) {

	char* ip = malloc(32 * sizeof(char)); 
	char* port = argv[2];
	 user_name = malloc(MAX_NAME_SIZE * sizeof(char));
	 unsigned long* Address = malloc(sizeof(unsigned long));
	if (ip == NULL || user_name == NULL || Address == NULL)
	{
		printf("Memory allocation failed in server main!");
		goto Main_Cleanup;
	}
	

	ip = argv[1];
	
	user_name = argv[3];
	SOCKADDR_IN clientService;
	HANDLE hThread[2];

	// Initialize Winsock.
	WSADATA wsaData; //Create a WSADATA object called wsaData.
	//The WSADATA structure contains information about the Windows Sockets implementation.

	//Call WSAStartup and check for errors.
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
		printf("Error at WSAStartup()\n");

	//Call the socket function and return its value to the m_socket variable. 
	// For this application, use the Internet address family, streaming sockets, and the TCP/IP protocol.

	// Create a socket.
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	// Check for errors to ensure that the socket is a valid socket.
	if (m_socket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	//For a client to communicate on a network, it must connect to a server.
	// Connect to a server.
	
	int return_val;
	return_val = inet_pton(2, ip, Address);
	if (return_val == INADDR_NONE)
	{
		printf("The string \"%s\" cannot be converted into an ip address. ending program.\n",
			SERVER_ADDRESS_STR);
		goto Main_Cleanup;
	}
	int temp = atoi(port);

	//Create a sockaddr_in object clientService and set  values.
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = *Address; //Setting the IP address to connect to
	clientService.sin_port = htons(temp); //Setting the port to connect to.

	

	// Call the connect function, passing the created socket and the sockaddr_in structure as parameters. 
	// Check for general errors.
	if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		int error = WSAGetLastError();
		printf("connect( ) failed with error %ld. Ending program\n", error);
		WSACleanup();
		goto Main_Cleanup;
	}
	printf("Connected to server on %s :%s\n",ip,port);
	/*
	hThread[0] = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)SendDataThread,
		user_name,
		0,
		NULL
	);
	*/
	hThread[1] = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)Player_Thread,
		user_name,
		0,
		NULL
	);
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	while (TRUE)
	{

	}
	
	TerminateThread(hThread[0], 0x555);
	TerminateThread(hThread[1], 0x555);
	Main_Cleanup:
	free(ip);
	//free(port);
	//free(user_name);
	//free(Address);
	return 0;
}
