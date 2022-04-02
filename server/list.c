#include "list.h" 

list *create(int data) 
{	
	list *tmp = (list*)malloc(sizeof(list));
	tmp->value = data;
	tmp->nickname = "root";
	tmp->next = NULL;

	return(tmp);
}

void add_element_end(int data, list *head) 
{
	list *tmp = (list*)malloc(sizeof(list));
	tmp->value = data;
	tmp->nickname = (char*)malloc(20 * sizeof(char)); //name_len=20
	strcpy(tmp->nickname, "");
	tmp->next = NULL;
	list *p = head;
	while (p->next != NULL) p = p->next;
	p->next = tmp;
}

list *remove_all(list *head) 
{	
	while (head != NULL) 
	{
		list *p = head;
		head = head->next;
		free(p);
	}
	return NULL;
}

list *remove_element(int data, list *head) 
{

	list *tmp = head, *p = NULL;

	if (head == NULL) return NULL;

	while (tmp && tmp -> value != data) 
	{
		p = tmp;
		tmp = tmp -> next;
	}
	// If the item to be removed is the first
	if (tmp == head) 
	{
		free(head);
		return tmp->next;
	}

	if (!tmp) return head;
	p->next = tmp->next;	
	free(tmp->nickname);
	free(tmp);
	return head;
}

void print_list(list *head)
{
	list *tmp = head;
	while (tmp != NULL) 
	{
		printf("%d %s\n", tmp->value, tmp->nickname);
		tmp = tmp->next;
	}
}

char *find_nickname(int data, list *head)
{	
	list *tmp = head, *p = NULL;

	if (head == NULL) return NULL;

	while (tmp && tmp->value != data) 
	{
		p = tmp;
		tmp = tmp->next;
	}
	
	return tmp->nickname;
}

void nickname_change(int data, list *head, const char *nik)
{

	list *tmp = head, *p = NULL;

	if (head == NULL) return;

	while (tmp && tmp->value != data) 
	{
		p = tmp;
		tmp = tmp->next;
	}
	strcpy(tmp->nickname, nik);
}

int find_sock(const char *nik, list *head)
{
	list *tmp = head, *p = NULL;

	if (head == NULL) return 0;

	while (tmp && strcmp(tmp->nickname, nik)) 
	{
		p = tmp;
		tmp = tmp->next;
	}
	if(tmp == NULL) return 0;
	return tmp->value;
}