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
DWORD Player_Thread(LPVOID lpParam)
{
	/* Check if lpParam is NULL */
	if (NULL == lpParam)
	{
		printf("error opening thread n data sent\n");
		return 1;
	}
	
	char* SendStr = malloc((MAX_NAME_SIZE + MAX_MESSEGE_SIZE) * sizeof(char));
	recived_data* parsed_str = malloc(sizeof(recived_data));
	bool quit = FALSE;// if true quit the game
	
	bool need_to_send = FALSE; // a boolean var that checks if we need to send data
	TransferResult_t RecvRes;
	char* AcceptedStr = NULL;
	char* user_name = (char*)lpParam;
	TransferResult_t SendRes;// the resulet we get from incoming data
	if (SendStr == NULL || parsed_str == NULL)
	{
		printf("error alcaiting memory in send thread\n");
		return 1;
	}
	
	
	fill_send_messege(SendStr, user_name, Client_Requset, MAX_NAME_SIZE);//fill the first messege
	need_to_send = TRUE; // tell the system we want to send

	/*
	SendRes = SendString(SendStr, m_socket);
	
	
	if (SendRes == TRNS_FAILED) // send has filed
	{
		printf("Socket error while trying to write data to socket\n");
		return 0x555;
	}
	printf(" sent to server %s\n", SendStr);
	*/
	while (TRUE)//(quit == FALSE)
	{
		if (need_to_send == TRUE) // check if we need to send data if
								// true send data
		{
			SendRes = SendString(SendStr, m_socket);


			if (SendRes == TRNS_FAILED) // send has filed
			{
				printf("Socket error while trying to write data to socket\n");
				return 0x555;
			}
			printf(" sent to server %s\n", SendStr);
			if (quit == TRUE)
			{
				break;
			}
			need_to_send = FALSE;
		}
		//#### now for the recived str #####
		RecvRes = ReceiveString(&AcceptedStr, m_socket);
		

		if (RecvRes == TRNS_FAILED)
		{
			printf("Socket error while trying to write data to socket\n");
			return 0x555;
		}
		parse_messege(AcceptedStr, parsed_str);
		printf(" recived from server %s{}\n", AcceptedStr);
		if (parsed_str->commaned_num == Servr_Approved)
		{
			printf("Servr_Approved\n");
		}
		else if (parsed_str->commaned_num == Servr_Denied)
		{
			printf("Servr_Denied\n");
		}

		else if(parsed_str->commaned_num == Server_main_menu)
		
		{
			quit = main_menu(SendStr);
			need_to_send = TRUE;
			

			
		}
		else if (parsed_str->commaned_num == Game_started)
		{
			printf("Game is on\n");
		}
		else if (parsed_str->commaned_num == Server_No_Opponents)
		{
			quit = main_menu(SendStr);
			need_to_send = TRUE;
		}
		else if (parsed_str->commaned_num == Turn_swich)
		{
			if (STRINGS_ARE_EQUAL(parsed_str->Parames, user_name))// meaning this is our turn
			{
				printf(" Your Turn!\n");
			}
			else
			{
				printf(" %s Turn!\n", parsed_str->Parames);
			}
		}
		else if (parsed_str->commaned_num == Server_Move_Request)
		{
			player_move(SendStr);
			need_to_send = TRUE;
		}
		else if (parsed_str->commaned_num == Server_Move_Request)
		{
			printf(" %s \n", parsed_str->Parames);
		}
		else if (parsed_str->commaned_num == Game_Ended)
		{
			printf(" %s WON! \n", parsed_str->Parames);
			quit = main_menu(SendStr);
			need_to_send = TRUE;
		}
		else if (parsed_str->commaned_num == Server_Oponent_Quit)
		{
			printf(" Opponemt quit!\n");
			quit = main_menu(SendStr);
			need_to_send = TRUE;
		}


		


		
		

		AcceptedStr = NULL;// reset the accept str

	}
	printf("closed thread.\n");
	free(SendStr);
	free(parsed_str); //free resoreses
	return 0;
}





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
	fill_send_messege(SendStr, user_name, Client_Requset, MAX_NAME_SIZE);
	SendRes = SendString(SendStr, m_socket);
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
the following func shows te main menu the player
it recives as input a send str buffer from user
 and feel it with the apropraite user nput
 if the user wants to quit return true
 else reten false

*/
bool main_menu(char* str_buf)
{
	bool valid_input = FALSE;// keep getig uaser input until valid
	while (valid_input == FALSE)
	{
		printf("choose what to do next:\n 1.play against another client\n 2. Quit\n");
		gets_s(str_buf, sizeof(str_buf)); //Reading a string from the keyboard
		if (*str_buf == 49) // equivilnt to "1"
		{
			fill_send_messege(str_buf, NULL, Client_Verses, MAX_NAME_SIZE);
			
			valid_input = TRUE;
			return FALSE;
		}
		else if (*str_buf == 50)// equivilnt to "2"
		{
			fill_send_messege(str_buf, NULL, Clienrt_Disconnected, MAX_NAME_SIZE);
			//need_to_send = TRUE;
			valid_input = TRUE;
			return TRUE;
		}
		else
		{
			printf("wrong input\n");
		}
	}
}

/*
the following func rcives the user input and checks if it is
a valid input for the game
the following fun recives a buffer to fill
the messege to be sent
*/
void player_move(char* str_buf)
{
	bool valid_input = FALSE;// keep getig uaser input until valid
	char* temp_char = NULL;
	while (valid_input == FALSE)
	{
		printf("Enter next number or boom\n");
		gets_s(temp_char, sizeof(temp_char)); //Reading a string from the keyboard
		if (STRINGS_ARE_EQUAL(temp_char, "boom")) // meaning the player choosed boom
		{
			fill_send_messege(str_buf, "boom", Client_Player_Move, MAX_NAME_SIZE);
			break;
		}
		else if (atoi(temp_char) >= 0 && atoi(temp_char) <= SIZE_OF_INT) // meaning we got a valid number
		{
			fill_send_messege(str_buf, "boom", Client_Player_Move, MAX_NAME_SIZE);
		}
		else
		{
			printf("wrong input\n");
		}


	}
}
