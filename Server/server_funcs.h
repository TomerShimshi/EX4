#ifndef SERVER_FUNCS_H
#define SERVER_FUNCS_H
#include <stdbool.h>
int FindFirstUnusedThreadSlot();
DWORD ServiceThread(SOCKET* t_socket);
void initialize_play_array();
void parse_messege(char input_str[], char* arr);
typedef struct
{
	char* player_name;
	bool Play_online;

} Player;

#endif