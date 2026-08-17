#include "apc.h"
#include <stdio.h>
#include <stdlib.h>

/* Helper to safely clear dynamic memory lists without impacting main contexts */
static void free_temp_div_list(Dlist **head, Dlist **tail) 
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

/* Division Engine Algorithm via Repeated Subtraction */
int division(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR) 
{
    // Prevent and catch dividing calculations targeting system zero elements
    Dlist *check2 = *head2;
    while (check2 && check2->data == 0 && check2->next) check2 = check2->next;
    if (check2 && check2->data == 0 && check2->next == NULL) 
    {
        printf("Error: Division by zero\n");
        return FAILURE;
    }

    // Initialize quotient counter list structure with a value of 0
    Dlist *count_head = NULL, *count_tail = NULL;
    if (insert_first(&count_head, &count_tail, 0) == FAILURE) return FAILURE;

    // Build static structural numeric step tracking node equal to 1
    Dlist *one_head = NULL, *one_tail = NULL;
    if (insert_first(&one_head, &one_tail, 1) == FAILURE) 
    {
        free_temp_div_list(&count_head, &count_tail);
        return FAILURE;
    }

    // Duplicate input list 1 data stack to prevent breaking data strings in main contexts
    Dlist *temp_h1 = NULL, *temp_t1 = NULL;
    for (Dlist *curr = *head1; curr; curr = curr->next) 
    {
        if (insert_last(&temp_h1, &temp_t1, curr->data) == FAILURE) 
        {
            free_temp_div_list(&count_head, &count_tail);
            free_temp_div_list(&one_head, &one_tail);
            return FAILURE;
        }
    }

    // Loop evaluation steps while Dividend (temp_h1) >= Divisor (head2)
    while (is_greater_or_equal(temp_h1, *head2)) 
    {
        Dlist *next_head = NULL, *next_tail = NULL;
        subtraction(&temp_h1, &temp_t1, head2, tail2, &next_head, &next_tail);
        
        // Wipe old intermediate calculations to completely prevent memory leaks
        free_temp_div_list(&temp_h1, &temp_t1);
        temp_h1 = next_head;
        temp_t1 = next_tail;

        // Increment structural counter variable list up by 1 step
        Dlist *new_count_head = NULL, *new_count_tail = NULL;
        addition(&count_head, &count_tail, &one_head, &one_tail, &new_count_head, &new_count_tail);
        
        free_temp_div_list(&count_head, &count_tail);
        count_head = new_count_head;
        count_tail = new_count_tail;
    }

    // Clean structural variables from system heaps
    free_temp_div_list(&temp_h1, &temp_t1);
    free_temp_div_list(&one_head, &one_tail);

    // Bind working tracking heads out to global returns
    *headR = count_head;
    *tailR = count_tail;
    return SUCCESS;
}
