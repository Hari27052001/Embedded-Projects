#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "Contact.h"

int validatePhone(char phone[])
{
    int i;

    if(strlen(phone)!=10)
        return 0;

    for(i=0;phone[i]!='\0';i++)
    {
        if(!isdigit(phone[i]))
            return 0;
    }

    return 1;
}

int validateEmail(char email[])
{
    int at=0,dot=0,i;

    for(i=0;email[i]!='\0';i++)
    {
        if(email[i]=='@')
            at++;

        if(email[i]=='.')
            dot=1;
    }

    return (at==1 && dot);
}

int phoneExists(AddressBook *addressBook,char phone[])
{
    int i;

    for(i=0;i<addressBook->contactCount;i++)
    {
        if(strcmp(phone,addressBook->contacts[i].phone)==0)
            return 1;
    }

    return 0;
}

int emailExists(AddressBook *addressBook,char email[])
{
    int i;

    for(i=0;i<addressBook->contactCount;i++)
    {
        if(strcmp(email,addressBook->contacts[i].email)==0)
            return 1;
    }

    return 0;
}

void createContact(AddressBook *addressBook)
{
    Contact *newContact;

    if(addressBook->contactCount>=MAX_CONTACTS)
    {
        printf("Address Book Full\n");
        return;
    }

    newContact=&addressBook->contacts[addressBook->contactCount];

    printf("Enter Name : ");
    scanf(" %[^\n]",newContact->name);

    do
    {
        printf("Enter Phone : ");
        scanf("%s",newContact->phone);

        if(!validatePhone(newContact->phone))
            printf("Invalid Phone Number\n");

    }while(!validatePhone(newContact->phone));

    if(phoneExists(addressBook,newContact->phone))
    {
        printf("Phone Number Already Exists\n");
        return;
    }

    do
    {
        printf("Enter Email : ");
        scanf("%s",newContact->email);

        if(!validateEmail(newContact->email))
            printf("Invalid Email\n");

    }while(!validateEmail(newContact->email));

    if(emailExists(addressBook,newContact->email))
    {
        printf("Email Already Exists\n");
        return;
    }

    addressBook->contactCount++;

    printf("Contact Added Successfully\n");
}

void searchContact(AddressBook *addressBook)
{
    char name[50];
    int i,found=0;

    printf("Enter Name : ");
    scanf(" %[^\n]",name);

    for(i=0;i<addressBook->contactCount;i++)
    {
        if(strcasecmp(name,addressBook->contacts[i].name)==0)
        {
            printf("\n%s\t%s\t%s\n",
                    addressBook->contacts[i].name,
                    addressBook->contacts[i].phone,
                    addressBook->contacts[i].email);

            found=1;
        }
    }

    if(!found)
        printf("Contact Not Found\n");
}

void searchByPhone(AddressBook *addressBook)
{
    char phone[20];
    int i;

    printf("Enter Phone : ");
    scanf("%s",phone);

    for(i=0;i<addressBook->contactCount;i++)
    {
        if(strcmp(phone,addressBook->contacts[i].phone)==0)
        {
            printf("\nName  : %s\n",
                   addressBook->contacts[i].name);

            printf("Phone : %s\n",
                   addressBook->contacts[i].phone);

            printf("Email : %s\n",
                   addressBook->contacts[i].email);

            return;
        }
    }

    printf("Contact Not Found\n");
}

void editContact(AddressBook *addressBook)
{
    char name[50];
    int i;

    printf("Enter Name : ");
    scanf(" %[^\n]",name);

    for(i=0;i<addressBook->contactCount;i++)
    {
        if(strcasecmp(name,addressBook->contacts[i].name)==0)
        {
            printf("Enter New Phone : ");
            scanf("%s",addressBook->contacts[i].phone);

            printf("Enter New Email : ");
            scanf("%s",addressBook->contacts[i].email);

            printf("Contact Updated\n");
            return;
        }
    }

    printf("Contact Not Found\n");
}

void deleteContact(AddressBook *addressBook)
{
    char name[50];
    char choice;
    int i,j;

    printf("Enter Name : ");
    scanf(" %[^\n]",name);

    for(i=0;i<addressBook->contactCount;i++)
    {
        if(strcasecmp(name,addressBook->contacts[i].name)==0)
        {
            printf("Confirm Delete (Y/N): ");
            scanf(" %c",&choice);

            if(choice!='Y' && choice!='y')
                return;

            for(j=i;j<addressBook->contactCount-1;j++)
            {
                addressBook->contacts[j]=
                addressBook->contacts[j+1];
            }

            addressBook->contactCount--;

            printf("Deleted Successfully\n");
            return;
        }
    }

    printf("Contact Not Found\n");
}

void listContacts(AddressBook *addressBook)
{
    int i;

    printf("\n---------------------------------------------------------------\n");
    printf("%-5s %-20s %-15s %-25s\n",
            "S.No","Name","Phone","Email");
    printf("---------------------------------------------------------------\n");

    for(i=0;i<addressBook->contactCount;i++)
    {
        printf("%-5d %-20s %-15s %-25s\n",
                i+1,
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }
}

void sortContacts(AddressBook *addressBook)
{
    int i,j;
    Contact temp;

    for(i=0;i<addressBook->contactCount-1;i++)
    {
        for(j=i+1;j<addressBook->contactCount;j++)
        {
            if(strcasecmp(addressBook->contacts[i].name,
                          addressBook->contacts[j].name)>0)
            {
                temp=addressBook->contacts[i];
                addressBook->contacts[i]=addressBook->contacts[j];
                addressBook->contacts[j]=temp;
            }
        }
    }

    printf("Contacts Sorted\n");
}

void saveContacts(AddressBook *addressBook)
{
    FILE *fp=fopen("contacts.txt","w");
    int i;

    if(fp==NULL)
    {
        printf("File Open Error\n");
        return;
    }

    for(i=0;i<addressBook->contactCount;i++)
    {
        fprintf(fp,"%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fp);

    printf("Contacts Saved Successfully\n");
}

void loadContacts(AddressBook *addressBook)
{
    FILE *fp=fopen("contacts.txt","r");

    if(fp==NULL)
    {
        initialize(addressBook);
        return;
    }

    addressBook->contactCount=0;

    while(fscanf(fp,"%49[^,],%19[^,],%49[^\n]\n",
          addressBook->contacts[addressBook->contactCount].name,
          addressBook->contacts[addressBook->contactCount].phone,
          addressBook->contacts[addressBook->contactCount].email)==3)
    {
        addressBook->contactCount++;
    }

    fclose(fp);
}