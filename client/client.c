#include "client.h"

static int online = 0;

int main(int argc, char *argv[])
{
	if (atexit(end)) 
	{
		puts("Function registration error");	
		return 0;
	}
	
	// termination signal
	signal(SIGINT, signal_INT);		

	pthread_t recvt;
    int len;
    char command[COM_LEN], password[LEN_20], buf[MSG_LEN];
	char friend[LEN_20];
	
    struct sockaddr_in ServerIp;
		
	// create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
   	ServerIp.sin_port = htons(PORT);
  	ServerIp.sin_family= AF_INET;
	ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1"); // server address
	
	if((connect(sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp))) == -1) 
	{
		printf("Connection to socket failed. Try later...\n");
		return 0;
	}	
	
	// created a thread for receiving messages
	pthread_create(&recvt, NULL, (void *)recvmg, &sock);
	
	// menu
	if (argc < 2)
	{
		printf("\nConnection is successful!\n");
		printf("Welcome to CLI-Chat!\n/help\n");
		printComands();
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
				// get list of friends
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
				// disconnection
				len = send(sock, command, strlen(command), 0);
				break;
			}
			// registration
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
				
				len = send(sock, buf, MSG_LEN, 0);
				sleep(1);
			}
			// authorization
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
				
				len = send(sock, buf, MSG_LEN, 0);
				sleep(1);
			}
			// adding to friends
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
				
				len = send(sock, buf, MSG_LEN, 0);
				sleep(1);
			}
			// chat initiation
			else if(!strcmp(command, "/chat")) 
			{
				printf("Enter nickname: ");
				scanf("%s", friend);
				if(!strcmp(friend, nickname)) 
				{
					printf("\nYou can't chat with yourself!\n");
					continue;
				}
				// online check
				strcpy(buf, "chat ");
		        strcat(buf, friend);
				
				len = send(sock, buf, MSG_LEN, 0);
				sleep(1);
				if (online == 1)
				{
					// open new terminal
					char *terminal = (char*)malloc((strlen("gnome-terminal --command='./") +
							strlen(argv[0]) + strlen(friend) + strlen(nickname) + strlen("  1' &")) * sizeof(char));
					strcpy(terminal, "gnome-terminal --command='./");
					strcat(terminal, argv[0]);
					strcat(terminal, " ");
					strcat(terminal, nickname);
					strcat(terminal, " ");
					strcat(terminal, friend);
					strcat(terminal, " 1' &");
					system(terminal);
					free(terminal);
				}				
			}
			// connection to the created chat
			else if(!strcmp(command, "/start"))
			{
				printf("Enter nickname: ");
				scanf("%s", friend);
				char *terminal = (char*)malloc((strlen("gnome-terminal --command='./") +
						strlen(argv[0]) + strlen(friend) + strlen(nickname) + strlen("  2' &")) * sizeof(char));
				strcpy(terminal, "gnome-terminal --command='./");
				strcat(terminal, argv[0]);
				strcat(terminal, " ");
				strcat(terminal, nickname);
				strcat(terminal, " ");
				strcat(terminal, friend);
				strcat(terminal, " 2' &");
				system(terminal);
				free(terminal);
				
			} 
			if(len < 0) printf("\nMessage wasn't sent\n");
		}
	}
	// chat window
	else if (argc == 4) 
	{
		char msg[MSG_LEN];
		printf("            Chat with %s \n", argv[2]);
		printf("/leave - leave the chat\n");

		// chat initiator
		if (atoi(argv[3]) == 1)
		{
			printf("Waiting for connection...\n");
			strcpy(buf, "start ");
			strcat(buf, argv[1]);
			strcat(buf, " ");
			strcat(buf, argv[2]);
			send(sock, buf, MSG_LEN, 0);
		}
		// connection to the created chat
		else if (atoi(argv[3]) == 2) 
		{
			strcpy(buf, "proof ");
			strcat(buf, argv[1]);
			strcat(buf, " ");
			strcat(buf, argv[2]);
			send(sock, buf, MSG_LEN, 0);

			strcpy(buf, "mes ");
			strcat(buf, argv[2]);
			strcat(buf, ":");
			strcat(buf, argv[1]);
			strcat(buf, " ");
			strcat(buf, argv[1]);
			strcat(buf, " joined the chat");
			send(sock, buf, MSG_LEN, 0);	
			
		}
		// enter message
		while(fgets(msg, MSG_LEN, stdin) > 0) 
		{
			strcpy(buf, "mes ");
			strcat(buf, argv[2]);
			strcat(buf, ":");
			strcat(buf, argv[1]);
			strcat(buf, " ");
			strcat(buf, argv[1]);
			strcat(buf, ": ");			
			strcat(buf, msg);
			send(sock, buf, MSG_LEN, 0);

			if (!strcmp(msg, "/leave"))
				len = send(sock, "/dis", strlen("/dis"), 0);
			
		}
	}
	
    return 0;
}

void *recvmg(void *my_sock) 
{
	int sock = *((int *)my_sock), len;
	char msg[MSG_LEN];
	
	while((len = recv(sock, msg, MSG_LEN, 0)) > 0) 
	{	
		msg[len] = '\0';
		if (!strcmp(msg, "online")) 	
		{
			online = 1;
			continue;
		}
		printf("%s\n", msg);
		if (!strcmp(msg, "Your friend is not online or you got the name mixed up")) online = 0;
		
		fflush(stdout);
		memset(msg, '\0', MSG_LEN);
	}
	raise(SIGINT);
}

void end(void)
{
	send(sock, "/dis", strlen("/dis"), 0);
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