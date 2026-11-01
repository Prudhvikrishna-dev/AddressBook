/*
 * addressbook.h
 * Structures and function declarations for the address book program.
 */

#ifndef CONTACT_H
#define CONTACT_H
#include<stdio.h>


struct Contact{
    char name[50];
    char phone[15];
    char email[50];
};

struct AddressBook {
    // all saved contacts (maximum 100)
    struct Contact contacts[100];
    // number of contacts currently saved
    int contactCount;
    // positions in contacts[] of the contacts found by the last search
    int index_record[100];
    // number of contacts found by the last search
    int ir_size;
};

void add_contact(struct AddressBook *addressBook);
void search_contact(struct AddressBook *addressBook);
void edit_contact(struct AddressBook *addressBook);
void delete_contact(struct AddressBook *addressBook);
void list_contacts(struct AddressBook *addressBook);
// saves all contacts to database.csv
void save_contacts(struct AddressBook *addressBook);
// loads contacts from database.csv, if the file exists
void load_contacts(struct AddressBook *addressBook);
#endif
