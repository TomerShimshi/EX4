//This header file holds different macros and structures used in the project.

#ifndef HARD_CODED_DATA_H
#define HARD_CODED_DATA_H

//general constants
#define TIMEOUT_IN_MILLISECONDS 5000
#define BRUTAL_TERMINATION_CODE 0x55
#define ERROR_CODE ((int)(-1))
#define SUCCESS_CODE ((int)(0))
#define MAX_LENGTH_OF_PATH_TO_A_FILE 300




//project specific constants


#define SERVER_ADDRESS_STR "127.0.0.1"
#define SERVER_PORT 2345
#define NUM_OF_WORKER_THREADS 2
#define NUM_OF_PLAYRES 2
#define MAX_NAME_SIZE 20
#define MAX_MESSEGE_SIZE 20
#define THREAD_TIMEOUT_IN_MS 80
#define MAX_LENGTH_OF_ROW 200
#define MAX_SIZE_OF_PORT 5
#define SIZE_OF_INT 32766

enum E_Server_messege { Servr_Approved, Servr_Denied, Server_main_menu, Game_started,Turn_swich,Game_Ended, Server_No_Opponents,Gam_Veiw,Server_Oponent_Quit, Server_Move_Request};
enum E_Client_messege { Client_Requset, Client_Verses,Client_Player_Move,Clienrt_Disconnected };


// creat an array forl all comands
typedef struct
{
	char messeg[MAX_MESSEGE_SIZE];

} messeges;
#endif