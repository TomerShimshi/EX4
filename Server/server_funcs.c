
#pragma warning(disable : 4996)
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
#include "server_funcs.h"

extern HANDLE ThreadHandles[NUM_OF_WORKER_THREADS];// thread handels array
extern  SOCKET ThreadInputs[NUM_OF_WORKER_THREADS]; // saves the socket o each thread
extern int* current_step; // saves the current step in order to check if a boom is nescry
extern HANDLE log_file_mutex; // a lock for the log file
extern HANDLE shared_memory_mutex; // this mutex will be used to check the current step num
extern Player Players[NUM_OF_PLAYRES];// saves the shered data about the players


/// <summary>: this ethod goes over 
/// all the thread in the thread array and finds the first free space
/// </summary>
/// <returns></returns> the func then returns the place of the free space
 int FindFirstUnusedThreadSlot()
{
	int Ind;

	for (Ind = 0; Ind < NUM_OF_WORKER_THREADS; Ind++)
	{
		if (ThreadHandles[Ind] == NULL)
			break;
		else
		{
			// poll to check if thread finished running:
			DWORD Res = WaitForSingleObject(ThreadHandles[Ind], 0);

			if (Res == WAIT_OBJECT_0) // this thread finished running
			{
				CloseHandle(ThreadHandles[Ind]);
				ThreadHandles[Ind] = NULL;
				break;
			}
		}
	}

	return Ind;
}

 //Service thread is the thread that opens for each successful client connection and "talks" to the client.
DWORD ServiceThread(SOCKET* t_socket)
 {
	char* clinetname = malloc(MAX_NAME_SIZE * sizeof(char));

	 BOOL Done = FALSE;
	 TransferResult_t SendRes;
	 TransferResult_t RecvRes;
	
	 char* SendStr = malloc(MAX_NAME_SIZE * sizeof(char));
	 recived_data* parsed_str = malloc( sizeof(recived_data));
	 int wait_res;

	 
	 if (parsed_str == NULL|| SendStr == NULL || clinetname ==NULL)
	 {
		 printf("couled allocate memory in servie thread, closing thread.\n");
		 closesocket(*t_socket);
		 return 1;
	 }
	 while(!Done)
	 {
		 char* AcceptedStr = NULL;
		RecvRes = ReceiveString(&AcceptedStr, *t_socket);
	 if (RecvRes == TRNS_FAILED)
	 {
		 printf("Service socket error while reading, closing thread.\n");
		 closesocket(*t_socket);
		 return 1;
	 }
	 printf("The recived STR is %s.\n", AcceptedStr);
	 
	 parse_messege(AcceptedStr, parsed_str);
	 printf("The parsed STR is %d.\n", parsed_str->commaned_num);
	 printf("The parsed STR is %s.\n", parsed_str->Parames);
	 if (parsed_str->commaned_num == Client_Requset) // mening we recived the client name
	 {
		
		 printf("The parsed user_name is %s.\n", parsed_str->Parames);
		 strcpy_s(clinetname, MAX_NAME_SIZE, parsed_str->Parames);// save the client name
		 
		 // now we want to update the players DB
		 
		 wait_res = WaitForSingleObject(shared_memory_mutex, INFINITE);
		 if (wait_res != WAIT_OBJECT_0)
		 {
			 const int error = GetLastError();
			 printf("Error when waiting for the DB Semaphore error code: %d\n", error);
			 exit(1);
		 }

		 //  are in the sage we can update the players data
		 for (int i = 0; i < NUM_OF_PLAYRES; i++)
		 {
			 if (Players[i].player_name == NULL)
			 {
				 Players[i].player_name = clinetname;
				 //strcpy_s(Players[i].player_name, MAX_NAME_SIZE, clinetname);// save the player name in global array
				 break;
			 }
		 }
		 //we have updated the player array, we can leave the safe zone
		 if (ReleaseMutex(shared_memory_mutex) == false) // release the DB mutex
		 {
			 const int error = GetLastError();
			 printf("Error when realisng real pages  mutex error num: %d\n", error);
			 exit(1);
		 }

		 sprintf_s(SendStr, 10, "%d", Servr_Approved);// 
		 SendRes = SendString(SendStr, *t_socket);
		 sprintf_s(SendStr, 10, "%d", Server_main_menu);// 
		 SendRes = SendString(SendStr, *t_socket);
		 free(AcceptedStr);


	 }

	 
	
	 
	 }
	 free(clinetname);
	 free(SendStr);
	 free(parsed_str);
 }



 /*
 the following function initilize the players array

 */
 void initialize_play_array()
 {
	 int wait_res;
	 wait_res = WaitForSingleObject(shared_memory_mutex, INFINITE);
	 if (wait_res != WAIT_OBJECT_0)
	 {
		 const int error = GetLastError();
		 printf("Error when waiting for the DB Semaphore error code: %d\n", error);
		 exit(1);
	 }

	 //  are in the sage we can update the players data
	 for (int i = 0; i < NUM_OF_PLAYRES; i++)
	 {
		 Players[i].player_name = NULL;
		 Players[i].Play_online = FALSE;
	 }
	 //we have initialize the player array, we can leave the safe zone
	 if (ReleaseMutex(shared_memory_mutex) == false) // release the DB mutex
	 {
		 const int error = GetLastError();
		 printf("Error when realisng real pages  mutex error num: %d\n", error);
		 exit(1);
	 }



 }