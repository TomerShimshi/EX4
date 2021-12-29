/*
Authors:

Amit Tzah 316062959
Tomer Shimshi 203200480


Project: Ex4
*/

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


// #######gobal vars#######
HANDLE ThreadHandles[NUM_OF_WORKER_THREADS];// thread handels array
SOCKET ThreadInputs[NUM_OF_WORKER_THREADS]; // saves the socket o each thread
int* current_step; // saves the current step in order to check if a boom is nescry
HANDLE log_file_mutex; // a lock for the log file
HANDLE shared_memory_mutex; // this mutex will be used to check the current step num



int main(int argc, char* argv[]) {

	SOCKET* MainSocket = malloc(sizeof(SOCKET));//INVALID_SOCKET;
	unsigned long* Address = malloc(sizeof(unsigned long));
	SOCKADDR_IN* service = malloc(sizeof(SOCKADDR_IN));
	current_step = malloc(sizeof(int));
	int port_num = atoi(argv[1]); // from the imstruction we get the port num in the commaned prompet




	shared_memory_mutex = CreateMutex(
		NULL,   /* default security attributes */
		FALSE,	/* don't lock mutex immediately */
		NULL);  /* un-named */

	log_file_mutex = CreateMutex(
		NULL,   /* default security attributes */
		FALSE,	/* don't lock mutex immediately */
		NULL);  /* un-named */
	if (MainSocket == NULL || Address == NULL || current_step == NULL || log_file_mutex == NULL || shared_memory_mutex == NULL)
	{
		printf("Memory allocation failed in main!");
		goto Main_Cleanup;
	}

	// Initialize Winsock.
	WSADATA wsaData;
	int StartupRes = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (StartupRes != NO_ERROR)
	{
		printf("error %ld at WSAStartup( ), ending program.\n", StartupRes);// WSAGetLastError());
		// Tell the user that we could not find a usable WinSock DLL.                                  
		return;
	}

	// Create a socket.    
	MainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (MainSocket == INVALID_SOCKET)
	{
		printf("Error at socket( ): %ld\n", WSAGetLastError());
		goto Main_Cleanup;
	}

	/* The WinSock DLL is acceptable. Proceed. */

	// Create a socket.    
	MainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (MainSocket == INVALID_SOCKET)
	{
		printf("Error at socket( ): %ld\n", WSAGetLastError());
		goto Main_Cleanup;
	}

	// Create a sockaddr_in object and set its values.
	// Declare variables
	int return_val;
	return_val = inet_pton(2,SERVER_ADDRESS_STR, Address);
	if (return_val == INADDR_NONE)
	{
		printf("The string \"%s\" cannot be converted into an ip address. ending program.\n",
			SERVER_ADDRESS_STR);
		goto Main_Cleanup;
	}

	service->sin_family = AF_INET;
	service->sin_addr.s_addr = Address;
	service->sin_port = htons(SERVER_PORT); //The htons function converts a u_short from host to TCP/IP network byte order 
									   //( which is big-endian ).

	// Call the bind function, passing the created socket and the sockaddr_in structure as parameters. 
	// Check for general errors.
	return_val = bind(MainSocket, (SOCKADDR*)&service, sizeof(service));
	if (return_val == SOCKET_ERROR)
	{
		printf("bind( ) failed with error %ld. Ending program\n", WSAGetLastError());
		goto Main_Cleanup;
	}

	// Listen on the Socket.
	return_val = listen(MainSocket, SOMAXCONN);
	if (return_val == SOCKET_ERROR)
	{
		printf("Failed listening on socket, error %ld.\n", WSAGetLastError());
		goto Main_Cleanup;
	}


	// Initialize all thread handles to NULL, to mark that they have not been initialized
	for (int Ind = 0; Ind < NUM_OF_WORKER_THREADS; Ind++)
		ThreadHandles[Ind] = NULL;

	printf("Waiting for a client to connect...\n");



	// ### FINISHED THE BOILER PLATE CODE AND THE INIT OF THE SERVER APP#### 

	Main_Cleanup:


	if (log_file_mutex != NULL) CloseHandle(log_file_mutex);
	if (shared_memory_mutex != NULL) CloseHandle(shared_memory_mutex);
	free(MainSocket);
	free(Address);
	free(current_step);
	return 0;
}
