#ifndef CLIENT_FUNCS_H
#define CLIENT_FUNCS_H


DWORD RecvDataThread(void);
DWORD SendDataThread(LPVOID lpParam);
void fill_send_messege(char* str_buffer, char* parames, int messege_num);



#endif