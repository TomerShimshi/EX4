#ifndef CLIENT_FUNCS_H
#define CLIENT_FUNCS_H


DWORD RecvDataThread(void);
DWORD SendDataThread(LPVOID lpParam);
DWORD Player_Thread(LPVOID lpParam);
bool main_menu(char* str_buf);
void player_move(char* str_buf);



#endif