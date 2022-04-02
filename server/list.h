#ifndef LIST_H
#define LIST_H

#include <malloc.h>
#include <string.h>

typedef struct Node 
{
	int value;
	char *nickname;
	struct Node *next;
} list;

list *create(int data);
list *remove_all(list *head);
list *remove_element(int data, list *head);

void add_element_end(int data, list *head);
void nickname_change(int data, list *head, const char *nik);

void print_list(list *head);
char *find_nickname(int data, list *head);
int find_sock(const char *nik, list *head);

#endif // LIST_H
