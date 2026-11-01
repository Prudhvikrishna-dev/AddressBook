/*
 * main.c
 * Author: Damaraju Prudhvikrishna
 * Entry point of the address book program.
 * Shows the main menu and calls the matching function from addressbook.c
 * (add, search, edit, delete and list contacts).
 * The menu is shown again and again until the user chooses 6 (Exit).
 */
#include "addressbook.h"

int main()
{
    // menu option entered by the user
    int choice;
    int c;//stores character from getchar()
    // the address book starts empty: no contacts and no search results
    struct AddressBook addressBook;
    addressBook.contactCount = 0;
    addressBook.ir_size = 0;
    // load any contacts saved from a previous run
    load_contacts(&addressBook);

    // keep showing the menu until the user chooses Exit
    do
    {
        printf("\nAddress Book Menu:\n");
        printf("1. Add/Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        if((scanf("%d", &choice))!=1)
        choice=0;
        while((c=getchar())!='\n' && c!=EOF);// to clear the input buffer;
        // each option calls the matching function in addressbook.c
        switch (choice)
        {
            case 1:
            add_contact(&addressBook);
            break;
            case 2:
            search_contact(&addressBook);
            break;
            case 3:
            edit_contact(&addressBook);
            break;
            case 4:
            delete_contact(&addressBook);
            break;
            case 5:
            list_contacts(&addressBook);
            break;
            case 6:
            // Exit: save all contacts before the loop ends because choice is 6
            save_contacts(&addressBook);
            printf("Saving and Exiting...\n");
            break;
            default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}