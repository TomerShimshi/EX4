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



HANDLE ThreadHandles[NUM_OF_WORKER_THREADS];// thread handels array
SOCKET ThreadInputs[NUM_OF_WORKER_THREADS]; // saves the socket o each thread
int* current_step; // saves the current step in order to check if a boom is nescry
HANDLE log_file_mutex; // a lock for the log file
HANDLE shared_memory_mutex; // this mutex will be used to check the current step num
