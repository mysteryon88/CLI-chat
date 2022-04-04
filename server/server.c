#include "server.h"

#define DATABASE "cli_chat.db"

int main(int argc, char *argv[])
{
	if (atexit(end))  
	{
		puts("Function registration error");	
		return 0;
	}
	
	// termination signal
	signal(SIGINT, signal_INT);

	struct sockaddr_in ServerIp;
	pthread_t recvt;
	int Client_sock = 0;

  	// create list of clients
	clients_list = create(-1);	
	
	// create a socket
	ServerIp.sin_family = AF_INET;
	ServerIp.sin_port = htons(PORT);
	ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1"); // our ip
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

	if (connectDatabase(DATABASE)) return 0;
	createTableClients();
	
	while(TRUE) 
	{
		// waiting for connection
		if((Client_sock = accept(sock, (struct sockaddr *)NULL, NULL)) < 0) 
			printf("Accept failed\n");
		else printf("Accept successfully\n");
		
		add_element_end(Client_sock, clients_list);
		
		// create a thread for each connection
		pthread_create(&recvt, NULL,(void *)recvmg, &Client_sock);
	}
	return 0; 
}

// if the server shuts down, then information about this is sent to all clients
void sendtoall(const char *msg)
{
	list *tmp = clients_list;
	tmp = tmp->next; 
	while (tmp != NULL) 
	{
		if(send(tmp->value, msg, strlen(msg), 0) < 0) continue;
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
		printf("%s\n", msg);
		sscanf(msg, "%s", command);
		
		fflush(stdout);
		if (!strcmp(command, "/dis")) break;
		// send friends list
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
		// registration
		else if(!strcmp(command, "up"))
		{
			char nickname[LEN_20];
			char pass[LEN_20];
			char *ans;
			sscanf(msg, "%s%s%s", nickname, nickname, pass);
			// checking for the same names
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
				
				send(sock, ans, strlen(ans), 0);				
				free(ans);
				continue;
			}
			else ans = "There is already a user with the same name";
							
			send(sock, ans, strlen(ans), 0);
		}	
		else if(!strcmp(command, "in"))
		{
			char nickname[LEN_20];
			char pass[LEN_20];
			char *ans;
			sscanf(msg, "%s%s%s", nickname, nickname, pass);
			
			// checking if the client exists in the database
			if(isExistInClients(nickname))
			{
				// login and password verification
				if (registrationCheck(nickname, pass))
				{
					// the connection is named
					nickname_change(sock, clients_list, nickname);
					char id[CODE_LEN];
					sprintf(id, "%d", getID(nickname)); 
					ans = (char*)malloc((strlen("You have successfully signed in, your unique code: ") + strlen(id)) * sizeof(char));
					strcpy(ans, "You have successfully signed in, your unique code: ");
					strcat(ans, id);
					send(sock, ans, strlen(ans), 0);	
					free(ans);
					continue;
				}
				else ans = "Wrong login or password";
			}
			else ans = "Your nickname is not registered";
					
			send(sock, ans, strlen(ans), 0);	

		}
		else if(!strcmp(command, "friends"))
		{
			char *ans;
			char *name = find_nickname(sock, clients_list);	
			char nickname[LEN_20];
			char code[CODE_LEN];
			
			// authorization check
			if(strcmp(name, ""))
			{
				sscanf(msg, "%s%s%s", nickname, nickname, code);
				// match your nickname and the one you entered
				if(strcmp(name, nickname))
					// check if the client exists
					if(isExistInClients(nickname))
					{
						pthread_mutex_lock(&database);
						// checking if a friend has already been added
						int exist = isExistFriend(name, nickname);
						pthread_mutex_unlock(&database);
						if(!exist)
							// verification of a unique code for adding to friends
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
				else ans = "You can't add yourself as a friend";
			}	
			else ans = "You are not sign in!";

			send(sock, ans, strlen(ans), 0);
		}
		// chat initiation
		else if(!strcmp(command, "chat")) 
		{
			char *ans;
			char nickname[LEN_20];
			char *name = find_nickname(sock, clients_list);	
			// authorization check
			if(strcmp(name, ""))
			{
				sscanf(msg, "%s%s", nickname, nickname);
				// online check
				if(!find_sock(nickname, clients_list))
					ans = "Your friend is not online or you got the name mixed up";
				else ans = "online";
			}	
			else ans = "You are not sign in!";
			
			send(sock, ans, strlen(ans), 0);
		}
		// creating a chat room
		else if(!strcmp(command, "start"))
		{
			char nickname_one[LEN_20], nickname_two[LEN_20];
			char *ans = (char*)malloc((strlen(" wants to start a chat with you, enter \"/start\"") + strlen(nickname_one) + strlen(nickname_one)) * sizeof(char));
			sscanf(msg, "%s%s%s", nickname_one, nickname_one, nickname_two);
			
			strcpy(ans, nickname_one);
			strcat(ans, " wants to start a chat with you, enter \"/start\"");
			
			char *name = (char*)malloc((strlen(nickname_one) + strlen(nickname_two) + strlen(":")) * sizeof(char));
			strcpy(name, nickname_one);
			strcat(name, ":");
			strcat(name, nickname_two);
			
			send(find_sock(nickname_two, clients_list), ans, strlen(ans), 0);
			nickname_change(sock, clients_list, name);
			free(ans);
			free(name);
		}
		// chat connection
		else if(!strcmp(command, "proof"))
		{	
			char nickname_one[LEN_20], nickname_two[LEN_20];
			sscanf(msg, "%s%s%s", nickname_one, nickname_one, nickname_two);

			char *name = (char*)malloc((strlen(nickname_one) + strlen(nickname_two) + strlen(":")) * sizeof(char));
			strcpy(name, nickname_one);
			strcat(name, ":");
			strcat(name, nickname_two);
			nickname_change(sock, clients_list, name);
		}
		// message forwarding
		else if(!strcmp(command, "mes"))
		{
			char name[41], leave[10];
			char *mes;
			
			sscanf(msg, "%s%s%s%s", name, name, leave, leave);
			int sock_ = find_sock(name, clients_list);
			
			// chat disconnect command
			if (!strcmp(leave, "/leave")) 
			{
				mes = "Your friend leave the chat enter /dis or ^C";
				
				send(sock_, mes, strlen(mes), 0);
				send(sock_, "Goodbye", strlen("Goodbye"), 0);
				clients_list = remove_element(sock_, clients_list);
				close(sock_);
				break;
			}
			else
			{
				// change message to normal
				mes = (char*)malloc((strlen(msg) - strlen("mes  ") - strlen(name)) * sizeof(char));
				strncpy(mes, msg + strlen("mes  ") + strlen(name), strlen(msg) - strlen("mes  ") - strlen(name));
				printf("%ld\n", send(sock_, mes, strlen(mes), 0));	
			}
		}
		memset(msg, '\0', MSG_LEN);
	}
	// if the client is disconnected, then remove it from the list of online clients
	clients_list = remove_element(sock, clients_list);
	close(sock);
	return 0;
}

void end(void)
{
	sendtoall("Server shutdown!");
	close(sock);
	remove_all(clients_list);
	puts("\nServer shutdown");
	closeDatabase();
}

void signal_INT(int signum) 
{
	exit(signum);
}