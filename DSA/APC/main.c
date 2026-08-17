#include "apc.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	/* 1. Prevent unexpected program crashes if arguments are missing */
	if (argc < 4)
	{
		printf("Error: Missing execution arguments.\n");
		printf("Usage: ./apc <number1> <operator> <number2>\n");
		return FAILURE;
	}

	/* Declare the pointers */
	Dlist *head1 = NULL, *tail1 = NULL;
	Dlist *head2 = NULL, *tail2 = NULL; 
	Dlist *headR = NULL, *tailR = NULL;

	char operator = argv[2][0];

	/* Convert string arguments to linked list data arrays */
	digit_to_list(&head1, &tail1, &head2, &tail2, argv);

	switch (operator)
	{
		case '+':
			/* call the function to perform the addition operation */
			addition(&head1, &tail1, &head2, &tail2, &headR, &tailR);
			printf("Result: ");
			print_list(headR);
			break;

		case '-':	
			/* call the function to perform the subtraction operation */
			/* Check if Number1 >= Number2 to cleanly process negative signs */
			if (!is_greater_or_equal(head1, head2))
			{
				printf("Result: -");
				subtraction(&head2, &tail2, &head1, &tail1, &headR, &tailR);
			}
			else
			{
				printf("Result: ");
				subtraction(&head1, &tail1, &head2, &tail2, &headR, &tailR);
			}
			print_list(headR);
			break;

		case '*':	
			/* call the function to perform the multiplication operation */
			multiplication(&head1, &tail1, &head2, &tail2, &headR, &tailR);
			printf("Result: ");
			print_list(headR);
			break;

		case '/':	
			/* call the function to perform the division operation */
			if (division(&head1, &tail1, &head2, &tail2, &headR, &tailR) == SUCCESS)
			{
				printf("Result: ");
				print_list(headR);
			}
			break;

		default:
			printf("Invalid Input:-( Try again...\n");
	}

	return 0;
}
