#include<stdio.h>
#include<stdlib.h>
#include "Contact.h"

int main()
{
    AddressBook addressBook;
    int choice;

    loadContacts(&addressBook);

    while(1)
    {
        printf("\n===== ADDRESS BOOK =====\n");
        printf("1.Create Contact\n");
        printf("2.Search Contact\n");
        printf("3.Edit Contact\n");
        printf("4.Delete Contact\n");
        printf("5.List Contacts\n");
        printf("6.Save Contacts\n");
        printf("7.Search By Phone\n");
        printf("8.Sort Contacts\n");
        printf("9.Exit\n");

        printf("Enter Choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                createContact(&addressBook);
                break;

            case 2:
                searchContact(&addressBook);
                break;

            case 3:
                editContact(&addressBook);
                break;

            case 4:
                deleteContact(&addressBook);
                break;

            case 5:
                listContacts(&addressBook);
                break;

            case 6:
                saveContacts(&addressBook);
                break;

            case 7:
                searchByPhone(&addressBook);
                break;

            case 8:
                sortContacts(&addressBook);
                break;

            case 9:
                saveContacts(&addressBook);
                printf("Thank You\n");
                exit(0);

            default:
                printf("Invalid Choice\n");
        }
    }

    return 0;
}