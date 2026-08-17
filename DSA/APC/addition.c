#include "apc.h"
#include <stdio.h>
#include <stdlib.h>

/* Helper to insert a digit at the front of the result list */
int insert_first(Dlist **head, Dlist **tail, int data) 
{
	Dlist *new_node = malloc(sizeof(Dlist));
	if (!new_node) return FAILURE;
	
	new_node->data = data;
	new_node->prev = NULL;
	new_node->next = *head;
	
	if (*head != NULL) 
	{
		(*head)->prev = new_node;
	} 
	else 
	{
		*tail = new_node;
	}
	*head = new_node;
	return SUCCESS;
}

/* Helper to insert a digit at the back of string parsing lists */
int insert_last(Dlist **head, Dlist **tail, int data) 
{
	Dlist *new_node = malloc(sizeof(Dlist));
	if (!new_node) return FAILURE;

	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = *tail;

	if (*tail != NULL) 
	{
		(*tail)->next = new_node;
	} 
	else 
	{
		*head = new_node;
	}
	*tail = new_node;
	return SUCCESS;
}

/* Helper to display output nodes sequential chain layout */
void print_list(Dlist *head) 
{
	while (head != NULL && head->data == 0 && head->next != NULL) 
	{
		head = head->next;
	}
	while (head != NULL) 
	{
		printf("%d", head->data);
		head = head->next;
	}
	printf("\n");
}

/* Working logic to pull CLI strings into valid node datasets */
void digit_to_list(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, char *argv[]) 
{
	char *op1 = argv[1];
	char *op2 = argv[3];

	for (int i = 0; op1[i] != '\0'; i++) 
	{
		if (op1[i] >= '0' && op1[i] <= '9') 
		{
			insert_last(head1, tail1, op1[i] - '0');
		}
	}
	for (int i = 0; op2[i] != '\0'; i++) 
	{
		if (op2[i] >= '0' && op2[i] <= '9') 
		{
			insert_last(head2, tail2, op2[i] - '0');
		}
	}
}

/* Working Addition operation engine */
int addition(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR) 
{
	Dlist *t1 = *tail1;
	Dlist *t2 = *tail2;
	int sum = 0, carry = 0;

	while (t1 != NULL || t2 != NULL || carry != 0) 
	{
		sum = carry;
		if (t1 != NULL) 
		{
			sum += t1->data;
			t1 = t1->prev;
		}
		if (t2 != NULL) 
		{
			sum += t2->data;
			t2 = t2->prev;
		}

		carry = sum / 10;
		insert_first(headR, tailR, sum % 10);
	}
	return SUCCESS;
}
