#include "Contact.h"

static Contact dummyContacts[] =
{
    {"John Doe","1234567890","john@example.com"},
    {"Alice Smith","9876543210","alice@example.com"},
    {"Bob Johnson","1112223333","bob@company.com"},
    {"Carol White","4445556666","carol@company.com"},
    {"David Brown","7778889999","david@example.com"}
};

void initialize(AddressBook *addressBook)
{
    int i;

    addressBook->contactCount = 0;

    for(i=0;i<5;i++)
    {
        addressBook->contacts[i] = dummyContacts[i];
        addressBook->contactCount++;
    }
}