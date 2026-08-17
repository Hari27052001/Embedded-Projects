#include "apc.h"
#include <stdio.h>
#include <stdlib.h>

/* Structural check utility: Returns 1 if List1 >= List2, otherwise 0 */
int is_greater_or_equal(Dlist *h1, Dlist *h2) 
{
    // Skip leading zeros for accurate evaluation
    while (h1 && h1->data == 0 && h1->next) h1 = h1->next;
    while (h2 && h2->data == 0 && h2->next) h2 = h2->next;

    int len1 = 0, len2 = 0;
    for (Dlist *t = h1; t; t = t->next) len1++;
    for (Dlist *t = h2; t; t = t->next) len2++;

    if (len1 > len2) return 1;
    if (len1 < len2) return 0;

    // Lengths match: compare values digit-by-digit from head to tail
    while (h1 && h2) 
    {
        if (h1->data > h2->data) return 1;
        if (h1->data < h2->data) return 0;
        h1 = h1->next;
        h2 = h2->next;
    }
    return 1; // Both values are identical
}

/* Subtraction Engine Algorithm */
int subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR) 
{
    Dlist *t1 = *tail1;
    Dlist *t2 = *tail2;
    int diff = 0, borrow = 0;

    // Loop through the primary list from right to left (Tails to Heads)
    while (t1 != NULL) 
    {
        diff = t1->data - borrow;
        
        if (t2 != NULL) 
        {
            diff -= t2->data;
            t2 = t2->prev;
        }

        // Handle arithmetic borrowing logic
        if (diff < 0) 
        {
            diff += 10;
            borrow = 1;
        } 
        else 
        {
            borrow = 0;
        }

        // Insert calculation node at the front of the result list
        insert_first(headR, tailR, diff);
        t1 = t1->prev;
    }
    return SUCCESS;
}
