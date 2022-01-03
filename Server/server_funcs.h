#ifndef SERVER_FUNCS_H
#define SERVER_FUNCS_H
#include <stdbool.h>

typedef struct
{
	char* player_name;
	bool Play_online;

} Player;



int FindFirstUnusedThreadSlot();
DWORD ServiceThread(SOCKET* t_socket);
void initialize_play_array();

#endif