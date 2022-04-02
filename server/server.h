#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#include "database.h"
#include "list.h"

#define PORT      2325
#define MSG_LEN   100
#define COM_LEN	  9
#define CODE_LEN  5
#define TRUE      1

pthread_mutex_t database;

list *clients_list; //list of clients

int sock = 0;

void end(void);
void sendtoall(char *msg);
void signal_INT(int signum);
void *recvmg(void *client_sock);

#endif // SERVER_H