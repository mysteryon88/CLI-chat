#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#define PORT      2343
#define MSG_LEN   100
#define LEN_20    20
#define COM_LEN	  9
#define CODE_LEN  5
#define TRUE	  1

char nickname[LEN_20];
int sock = 0;

void *recvmg(void *my_sock);
void signal_INT(int signum);
void printComands();
void printAbout();
void end(void);

#endif // CLIENT_H