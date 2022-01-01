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

#include "create_and_handle_processes.h"
#include "HardCodedData.h"
#include "file_IO.h"
#include "SocketExampleShared.h"
#include "SocketSendRecvTools.h"
#include "client_funcs.h"

extern SOCKET m_socket;

//Reading data coming from the server
DWORD RecvDataThread(void)
{
	TransferResult_t RecvRes;

	while (1)
	{
		char* AcceptedStr = NULL;
		RecvRes = ReceiveString(&AcceptedStr, m_socket);
		
		if (RecvRes == TRNS_FAILED)
		{
			printf("Socket error while trying to write data to socket\n");
			return 0x555;
		}
		
		else if (RecvRes == TRNS_DISCONNECTED)
		{
			printf("Server closed connection. Bye!\n");
			return 0x555;
		}
		else 
		{
			printf(" recived from server %s{}\n", AcceptedStr);
			if (atoi(AcceptedStr) == Server_main_menu)
				printf("choose what to do next:\n 1.play against another client\n 2. Quit");
		}

		free(AcceptedStr);
	}

	//return 0;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

//Sending data to the server
DWORD SendDataThread(LPVOID lpParam)
{
	/* Check if lpParam is NULL */
	if (NULL == lpParam)
	{
		printf("error opening thread n data sent\n");
		return 1;
	}

	char* SendStr = malloc((MAX_NAME_SIZE+ MAX_MESSEGE_SIZE)* sizeof(char));
	if (SendStr == NULL)
	{
		printf("error alcaiting memory in send thread\n");
		return 1;
	}
	char* user_name = (char*)lpParam;
	TransferResult_t SendRes;
	fill_send_messege(SendStr, user_name, Client_Requset);
	SendRes = SendString(user_name, m_socket);
	BOOL quit = FALSE;

	
	
	if (SendRes == TRNS_FAILED)
	{
		printf("Socket error while trying to write data to socket\n");
		return 0x555;
	}
	printf(" sent to server %s{}\n", user_name);

	while (quit ==FALSE)
	{
		gets_s(SendStr, sizeof(SendStr)); //Reading a string from the keyboard

		

		SendRes = SendString(SendStr, m_socket);

		if (SendRes == TRNS_FAILED)
		{
			printf("Socket error while trying to write data to socket\n");
			return 0x555;
		}
		printf(" sent to server %s{}\n", SendStr);

	}
}

/*
* the following method fills out the messege to be trancferd 
* as specified in the HW 
* it recvies as input, a str buffer, the messge type, parames( if no parames nedded send null)
* and build the send str
*/
void fill_send_messege(char* str_buffer, char* parames, int messege_num)
{
	
	sprintf_s(str_buffer, 3, "%d", messege_num);// set the messege type
	strcat_s(str_buffer,1, ":"); // add the : to ithinghish between messge and params
	strcat_s(str_buffer,sizeof(*parames), parames); // add th client name
	strcat_s(str_buffer,2, "\n"); // the terminate of the send messege
}