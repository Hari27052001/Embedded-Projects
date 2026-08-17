#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100

typedef struct
{
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct
{
    Contact contacts[MAX_CONTACTS];
    int contactCount;
} AddressBook;

void initialize(AddressBook *addressBook);

void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);

void saveContacts(AddressBook *addressBook);
void loadContacts(AddressBook *addressBook);

void sortContacts(AddressBook *addressBook);
void searchByPhone(AddressBook *addressBook);

int validatePhone(char phone[]);
int validateEmail(char email[]);
int phoneExists(AddressBook *addressBook,char phone[]);
int emailExists(AddressBook *addressBook,char email[]);

#endif