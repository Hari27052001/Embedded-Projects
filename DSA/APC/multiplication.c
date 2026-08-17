#include "apc.h"
#include <stdio.h>
#include <stdlib.h>

/* Helper to clean up temporary list memory allocations */
static void free_temp_list(Dlist **head, Dlist **tail) 
{
    Dlist *current = *head;
    while (current != NULL) 
    {
        Dlist *next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
    *tail = NULL;
}

/* Multiplication Engine Algorithm */
int multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR) 
{
    Dlist *t2 = *tail2;
    int shift = 0;

    // Initialize Result list with a single zero entry node
    if (insert_first(headR, tailR, 0) == FAILURE) return FAILURE;

    // Loop through each digit of multiplier (List 2) from right to left
    while (t2 != NULL) 
    {
        Dlist *t1 = *tail1;
        Dlist *p_head = NULL, *p_tail = NULL; 
        int carry = 0, prod = 0;

        // Append trailing zeros matching the current decimal shift position
        for (int i = 0; i < shift; i++) 
        {
            if (insert_first(&p_head, &p_tail, 0) == FAILURE) return FAILURE;
        }

        // Multiply multiplicand (List 1) by the single digit from List 2
        while (t1 != NULL || carry != 0) 
        {
            prod = carry;
            if (t1 != NULL) 
            {
                prod += (t1->data * t2->data);
                t1 = t1->prev;
            }
            carry = prod / 10;
            if (insert_first(&p_head, &p_tail, prod % 10) == FAILURE) return FAILURE;
        }

        // Add the running product total to the partial sum
        Dlist *temp_head = NULL, *temp_tail = NULL;
        addition(headR, tailR, &p_head, &p_tail, &temp_head, &temp_tail);
        
        // Clean up heap space for old running totals and partial chains
        free_temp_list(headR, tailR);
        free_temp_list(&p_head, &p_tail);

        // Assign the newly merged sum back to the response output structure
        *headR = temp_head;
        *tailR = temp_tail;

        shift++;
        t2 = t2->prev;
    }
    return SUCCESS;
}
