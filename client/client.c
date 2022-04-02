#include "client.h"

int main(int argc, char *argv[])
{
	if (atexit(end)) 
	{
		puts("Function registration error");	
		return 0;
	}
	
	signal(SIGINT, signal_INT);		

	pthread_t recvt;
    	int len;
    	char send_msg[MSG_LEN], command[COM_LEN], password[LEN_20], buf[MSG_LEN];
	char friend[LEN_20];
	
    	struct sockaddr_in ServerIp;

    	sock = socket(AF_INET, SOCK_STREAM, 0);
   	ServerIp.sin_port = htons(PORT);
  	ServerIp.sin_family= AF_INET;
	ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if((connect(sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp))) == -1) 
	{
		printf("Connection to socket failed. Try later...\n");
		return 0;
	}	

	printf("\nConnection is successful!\n");
	printf("Welcome to CLI-Chat!\n/help\n");
	printComands();
	
	//stream for receiving messages
	pthread_create(&recvt, NULL, (void *)recvmg, &sock);
	
	//fgets(msg, MSG_LEN, stdin) > 0

	if (argc < 2)
	{
		while(TRUE) 
		{	
			fflush(stdout);
			printf(">> ");
			scanf("%s", command);
			if(!strcmp(command, "/help")) 
				printComands();
			else if(!strcmp(command, "/about"))
				printAbout(); 
			else if(!strcmp(command, "/myfriends")) 
			{
				len = send(sock, command, strlen(command), 0);
				sleep(1);
			}
			else if(!strcmp(command, "/clear")) 
			{
				system("clear");
				printComands();
			}
			else if (!strcmp(command, "/dis")) 
			{
				len = send(sock, command, strlen(command), 0);
				break;
			}
			else if(!strcmp(command, "/up"))
			{
				printf("Enter nickname: ");
				scanf("%s", nickname);
	 			printf("Enter password: ");
				scanf("%s", password);
				strcpy(buf, "up ");

		        	strcat(buf, nickname);
				strcat(buf, " ");
				strcat(buf, password);
				len = send(sock, buf, 100, 0);
				sleep(1);
			}
			else if(!strcmp(command, "/in"))
			{
				printf("Enter nickname: ");
				scanf("%s", nickname);
	 			printf("Enter password: ");
				scanf("%s", password);
				strcpy(buf, "in ");
				strcat(buf, nickname);
				strcat(buf, " ");
				strcat(buf, password);
				len = send(sock, buf, 100, 0);
				sleep(1);
			}
			else if(!strcmp(command, "/friends"))
			{
				char code[CODE_LEN];
				printf("Enter nickname: ");
				scanf("%s", friend);
				printf("Enter its unique code: ");
				scanf("%s", code);
				strcpy(buf, "friends ");

				strcat(buf, friend);
				strcat(buf, " ");
				strcat(buf, code);
				len = send(sock, buf, 100, 0);
				sleep(1);
			}
			else if(!strcmp(command, "/chat")) 
			{
				printf("Enter nickname: ");
				scanf("%s", friend);
				strcpy(buf, "chat ");

		        	strcat(buf, friend);
				len = send(sock, buf, 100, 0);
				if(strcmp(buf, "Your friend is not online or you got the name mixed up"))
				{
					//open new terminal
				}
				
				sleep(1);
			}

			if(len < 0) printf("\nMessage wasn't sent \n");
		}
	}
	/*
	else if () 
	{
		
	}
	*/
    return 0;
}

void *recvmg(void *my_sock) 
{
	int sock = *((int *)my_sock), len;

	while((len = recv(sock, msg, MSG_LEN, 0)) > 0) 
	{
		printf("%s\n", msg);
		fflush(stdout);
		memset(msg, '\0', MSG_LEN);
	}
	raise(SIGINT);
}

void end(void)
{
	char *com = "/dis";
	send(sock, com, strlen(com), 0);
	close(sock);
	puts("\nGoodbye!");
}

void signal_INT(int signum) 
{
	exit(signum);
}

void printComands()
{
	printf("         Hepl\n");
	printf("/up - sign up\n");
	printf("/in - sign in\n");
	printf("/clear - clear\n");
	printf("/friends - add new friends\n");
	printf("/myfriends - withdrawal of friends\n");
	printf("/chat - send a request to start a chat\n");
	printf("/dis or ^C - disconnect from the server\n");
	printf("/about\n\n");
}

void printAbout()
{
	printf("        CLI-chat\n");
	printf("      Sobolev Sergey\n");
	printf("Git: https://github.com/mysteryon88\n\n");
}