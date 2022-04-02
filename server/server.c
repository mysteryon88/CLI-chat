#include "server.h"

#define DATABASE "cli_chat.db"

int main(int argc, char *argv[])
{
	if (atexit(end))  
	{
		puts("Function registration error");	
		return 0;
	}
	
	signal(SIGINT, signal_INT);

	struct sockaddr_in ServerIp;
	pthread_t recvt;
	int Client_sock = 0;

  	//create list of clients
	clients_list = create(-1);	

	ServerIp.sin_family = AF_INET;
	ServerIp.sin_port = htons(PORT);
	ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1");
	sock = socket( AF_INET , SOCK_STREAM, 0 );
	
	if(bind(sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp)) == -1)
	{
		printf("Cannot bind, error!! \n");
		return 0;
	}
	else printf("Server started working!\n");

	if(listen(sock, SOMAXCONN) == -1) 
	{
		printf("Listening failed!\n");
		return 0;	
	}
	
	connectDatabase(DATABASE);
	createTableClients();
	
	while(TRUE) 
	{
		if((Client_sock = accept(sock, (struct sockaddr *)NULL, NULL)) < 0) 
			printf("Accept failed\n");
		else printf("Accept successfully\n");
		
		add_element_end(Client_sock, clients_list);

		pthread_create(&recvt, NULL,(void *)recvmg, &Client_sock);
	}
	return 0; 
}

void sendtoall(char *msg)
{
	list *tmp = clients_list;
	tmp = tmp->next; 
	while (tmp != NULL) 
	{
		if(send(tmp->value, msg, strlen(msg), 0) < 0) 
		{
			printf("Sending failure\n");
			continue;
		}
		tmp = tmp->next;
	}
}

void *recvmg(void *client_sock)
{
	int sock = *((int *)client_sock);
	char msg[MSG_LEN], command[COM_LEN];
	int len = 0;
	while((len = recv(sock, msg, MSG_LEN, 0)) > 0) 
	{
		//len = recv(sock, msg, MSG_LEN, 0);
		printf("%s\n", msg);
		sscanf(msg, "%s", command);
		
		fflush(stdout);
		if (!strcmp(command, "/dis")) break;
		else if(!strcmp(command, "/myfriends"))
		{
			char *ans;
			if(strcmp(find_nickname(sock, clients_list), ""))
				selectAllFriends(&sock, find_nickname(sock, clients_list));
			else 
			{
				ans = "You are not sign in";
				send(sock, ans, strlen(ans), 0);
			}
		}
		else if(!strcmp(command, "up"))
		{
			char nickname[LEN_20];
			char pass[LEN_20];
			char *ans;
			sscanf(msg, "%s%s%s", nickname, nickname, pass);
			//printf("%s\n", nikname);
			//printf("%s\n", pass);
			if(!isExistInClients(nickname))
			{
				
				pthread_mutex_lock(&database);
				
				insertIntoClients(nickname, pass);

				pthread_mutex_unlock(&database);	
				char id[CODE_LEN];
				sprintf(id, "%d", getID(nickname)); 
				ans = (char*)malloc((strlen("You have been registered, your code: ") + strlen(id)) * sizeof(char));
				strcpy(ans, "You have been registered, your code: ");
				strcat(ans, id);
				
			}
			else ans = "There is already a user with the same name";
							
			send(sock, ans, strlen(ans), 0);
			//selectAllClients();
		}	
		else if(!strcmp(command, "in"))
		{
			char nickname[LEN_20];
			char pass[LEN_20];
			char *ans;
			sscanf(msg, "%s%s%s", nickname, nickname, pass);
			//printf("%s\n", nikname);
			//printf("%s\n", pass);
			if(isExistInClients(nickname))
			{
				if (registrationCheck(nickname, pass))
				{
					nickname_change(sock, clients_list, nickname);
					char id[CODE_LEN];
					sprintf(id, "%d", getID(nickname)); 
					ans = (char*)malloc((strlen("You have successfully signed in, your code: ") + strlen(id)) * sizeof(char));
					strcpy(ans, "You have successfully signed in, your code: ");
					strcat(ans, id);
				}
				else ans = "Wrong login or password";
			}
			else ans = "Your nickname is not registered";
					
			send(sock, ans, strlen(ans), 0);	
			//selectAllClients();
		}
		else if(!strcmp(command, "friends"))
		{
			char *ans;
			char *name = find_nickname(sock, clients_list);	
			char nickname[LEN_20];
			char code[CODE_LEN];
			if(strcmp(name, ""))
			{
				sscanf(msg, "%s%s%s", nickname, nickname, code);
				if(isExistInClients(nickname))
				{
					pthread_mutex_lock(&database);
					int exist = isExistFriend(name, nickname);
					pthread_mutex_unlock(&database);
					if(!exist)
						if(atoi(code) == getID(nickname))
						{
							pthread_mutex_lock(&database);
							insertIntoTable(nickname, name);
							insertIntoTable(name, nickname);
							pthread_mutex_unlock(&database);
							ans = "Now you are friends!";
						}
						else ans = "Wrong unique code!";
					else ans = "You are already friends!";
				}
				else ans = "Such user doesn't exist!";
			}	
			else ans = "You are not sign in!";

			send(sock, ans, strlen(ans), 0);
		}
		else if(!strcmp(command, "chat")) 
		{
			char *ans;
			char nickname[LEN_20];
			if(strcmp(find_nickname(sock, clients_list), ""))
			{
				sscanf(msg, "%s%s", nickname, nickname);
				if(!find_sock(nickname, clients_list))
				{
					ans = "Your friend is not online or you got the name mixed up";
					send(sock, ans, strlen(ans), 0);
				}
			}	
			else 
			{
				ans = "You are not sign in!";
				send(sock, ans, strlen(ans), 0);
			}
			
		}
		
		memset(msg, '\0', MSG_LEN);
	}
	clients_list = remove_element(sock, clients_list);
	close(sock);
	return 0;
}

void end(void)
{
	char *end = "Server shutdown!";
	sendtoall(end);
	close(sock);
	remove_all(clients_list);
	puts("\nServer shutdown");
	closeDatabase();
}

void signal_INT(int signum) 
{
	exit(signum);
}