/*
 * addressbook.c
 * Contact operations of the address book program:
 * add, search, edit, delete and list contacts.
 *
 * Validation:
 *   Phone - exactly 10 digits, first digit 6 to 9, must be unique.
 *   Email - format name@domain.com, stored in lowercase, must be unique.
 *
 * Search results are stored in index_record (ir_size = number of matches),
 * so edit and delete can find the real position of the selected contact.
 * The main menu is not part of this file.
 */
#include "addressbook.h"
#include<string.h>
#include<ctype.h>
// clears leftover characters in the input buffer up to the newline
void clear_buffer()
{
   int c;
   while((c=getchar())!='\n' && c!=EOF)
   {
   }
}
// str is the mode: "ADD"/"EDIT" also check duplicates, "search" does not
// index is the contact being edited (skipped in duplicate check), -1 if none
void phone_validation(struct AddressBook *addressBook,char str[],char phone[],int index)
{
   again:
   printf("Enter phone number : ");
   scanf("%14s",phone);
   clear_buffer();
   // phone number must be exactly 10 digits
   if(strlen(phone)==10)
   {
      // first digit must be between 6 and 9
      if(!((*phone)>='6' && (*phone)<='9'))
      {
         printf("Phone number must start with a digit from 6 to 9.\n");
         goto again;
      }
      else
      {
         // every character must be a digit
         char *ptr=phone;
         while((*ptr)!='\0')
         {
            if(!((*ptr)>='0' && (*ptr)<='9'))
            {
               printf("Phone number must contain only digits.\n");
               goto again;
            }
            ptr++;
         }
         // in ADD/EDIT mode the phone number must not already exist
         if(strcmp(str,"ADD")==0 || strcmp(str,"EDIT")==0)
         {
            for(int i=0;i<addressBook->contactCount;i++)
            {
               if(i!=index)
               {
                  if(strcmp(phone,addressBook->contacts[i].phone)==0)
                  {
                     printf("Phone number already exists.\n");
                     goto again;
                  }
               }
            }
         }
      }
   }
   else
   {
      printf("Phone number must have exactly 10 digits.\n");
      goto again;
   }
}
// same str/index meaning as phone_validation
void email_validation(struct AddressBook *addressBook,char str[],char mail[],int index)
{
   again:
   printf("Enter email : ");
   scanf("%49s",mail);
   clear_buffer();
   // convert to lowercase so emails are case-insensitive
   for(char *p=mail;*p;p++)
   {
      *p=tolower((unsigned char)*p);
   }
   // email must contain @
   if(strstr(mail,"@"))
   {
      if(*mail=='@')
      {
         printf("Email must not start with @.\n");
         goto again;
      }
      else
      {
         // x points to the last @, len is the length from @ to the end
         char *x=strrchr(mail,'@');
         int len=strlen(x);
         // only one @ is allowed
         if(x!=strchr(mail,'@'))
         {
            printf("Email must contain only one @.\n");
            goto again;
         }
         // allow only letters, digits, . _ - and @, and no two dots together
         char *ptr=mail;
         while((*ptr)!='\0')
         {
            if(!(isalnum((unsigned char)*ptr) || *ptr=='.' || *ptr=='_' || *ptr=='-' || *ptr=='@'))
            {
               printf("Email can contain only letters, digits and the symbols . _ - @\n");
               goto again;
            }
            if(*ptr=='.' && *(ptr+1)=='.')
            {
               printf("Email must not have two dots together.\n");
               goto again;
            }
            ptr++;
         }
         // there must be something between @ and .com, no dot right after @, and the mail must end with .com
         if(len<6 || x[1]=='.' || x[len-4]!='.' || tolower((unsigned char)x[len-3])!='c' || tolower((unsigned char)x[len-2])!='o' || tolower((unsigned char)x[len-1])!='m')
         {
            printf("Email must be in the format name@domain.com.\n");
            goto again;
         }
      }
   }
   else
   {
      printf("Email must contain @.\n");
      goto again;
   }
   // in ADD/EDIT mode the email must not already exist
   if(strcmp(str,"ADD")==0 || strcmp(str,"EDIT")==0)
   {
      for(int i=0;i<addressBook->contactCount;i++)
      {
         if(i!=index)
         {
            if(strcmp(mail,addressBook->contacts[i].email)==0)
            {
               printf("Email already exists.\n");
               goto again;
            }
         }
      }
   }
}
void add_contact(struct AddressBook *addressBook)
{
   // stop if the contact array is full
   if(addressBook->contactCount>=(int)(sizeof(addressBook->contacts)/sizeof(addressBook->contacts[0])))
   {
      printf("Address book is full, cannot add more contacts.\n");
      return;
   }
   printf("Enter name : ");
   scanf(" %49[^\n]",addressBook->contacts[addressBook->contactCount].name);
   clear_buffer();
   // phone and email are validated before the contact is counted
   phone_validation(addressBook,"ADD",addressBook->contacts[addressBook->contactCount].phone,-1);
   email_validation(addressBook,"ADD",addressBook->contacts[addressBook->contactCount].email,-1);
   addressBook->contactCount++;
   printf("Contact added successfully\n");
}
// shows the search menu and stores the positions of matches in index_record (ir_size = number of matches)
void search_contact(struct AddressBook *addressBook)
{
   /* if( addressBook->ir_size == 0 )
   printf("Contact not found\n");
   else
   {
   // Print the searched contacts
   } */
   // clear the previous search results
   addressBook->ir_size=0;
   printf("\n1. Search by name\n");
   printf("2. Search by phone number\n");
   printf("3. Search by email\n");
   printf("Enter your choice : ");
   int choice;
   int serial_no=1;
   // invalid input (not a number) is treated as choice 0
   if(scanf("%d",&choice)!=1)
   {
      choice=0;
   }
   clear_buffer();
   int found=0;
   switch(choice)
   {
      case 1:
      {
         char name[50];
         printf("Enter name to search : ");
         scanf(" %49[^\n]",name);
         clear_buffer();
         // name search is exact (case-sensitive), all matches are shown
         for(int i=0;i<addressBook->contactCount;i++)
         {
            if(strcmp(name,addressBook->contacts[i].name)==0)
            {
               printf("%d.%-25s %-15s %-30s\n",serial_no++,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
               found=1;
               addressBook->index_record[addressBook->ir_size]=i;
               addressBook->ir_size++;
            }
         }
         break;
      }
      case 2:
      {
         // phone search is exact, phone numbers are unique so stop at the first match
         char phone[15];
         phone_validation(addressBook,"search",phone,-1);
         for(int i=0;i<addressBook->contactCount;i++)
         {
            if(strcmp(phone,addressBook->contacts[i].phone)==0)
            {
               printf("%d.%-25s %-15s %-30s\n",serial_no++,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
               found=1;
               addressBook->index_record[addressBook->ir_size]=i;
               addressBook->ir_size++;
               break;
            }
         }
         break;
      }
      case 3:
      {
         // email search is exact, emails are stored in lowercase and are unique so stop at the first match
         char mail[50];
         email_validation(addressBook,"search",mail,-1);
         for(int i=0;i<addressBook->contactCount;i++)
         {
            if(strcmp(mail,addressBook->contacts[i].email)==0)
            {
               printf("%d.%-25s %-15s %-30s\n",serial_no++,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
               found=1;
               addressBook->index_record[addressBook->ir_size]=i;
               addressBook->ir_size++;
               break;
            }
         }
         break;
      }
      default:
      {
         printf("Invalid choice, enter 1, 2 or 3.\n");
         return;
      }
   }
   if(found==0)
   {
      printf("\nContact not found\n");
   }
}
// search first, then select which of the found contacts to edit
void edit_contact(struct AddressBook *addressBook)
{
   search_contact(addressBook);
   if(addressBook->ir_size==0)
   {
      return;
   }
   int contact_index;
   printf("Select contact number to edit : ");
   if(scanf("%d",&contact_index)!=1)
   {
      contact_index=0;
   }
   clear_buffer();
   if(contact_index>=1 && contact_index<=addressBook->ir_size)
   {
      // convert the selected number to the real position in the contacts array
      int index=addressBook->index_record[contact_index-1];
      printf("What do you want to edit?\n");
      int choice;
      // keep showing the edit menu until the user chooses save
      do
      {
         printf("\n1. Edit name\n2. Edit phone number\n3. Edit email\n4. Save and exit\n");
         printf("Enter your choice : ");
         if(scanf("%d",&choice)!=1)
         {
            choice=0;
         }
         clear_buffer();
         switch(choice)
         {
            case 1:
            {
               printf("Enter new name : ");
               scanf(" %49[^\n]",addressBook->contacts[index].name);
               clear_buffer();
               break;
            }
            case 2:
            {
               phone_validation(addressBook,"EDIT",addressBook->contacts[index].phone,index);
               break;
            }
            case 3:
            {
               email_validation(addressBook,"EDIT",addressBook->contacts[index].email,index);
               break;
            }
            case 4:
            {
               printf("Contact edited and saved successfully\n");
               break;
            }
            default:
            {
               printf("Invalid choice, enter 1 to 4.\n");
               break;
            }
         }
      }while(choice!=4);
   }
   else
   {
      printf("Invalid contact number.\n");
   }
}
// search, select a contact and confirm before deleting
void delete_contact(struct AddressBook *addressBook)
{
   int contact_index;
   int choice;
   do
   {
      search_contact(addressBook);
      if(addressBook->ir_size==0)
      {
         printf("No contact to delete.\n");
         return;
      }
      printf("Select contact number to delete : ");
      if(scanf("%d",&contact_index)!=1)
      {
         contact_index=0;
      }
      clear_buffer();
      printf("\nWhat do you want to do?\n1. Delete this contact\n2. Cancel\nEnter your choice : ");
      if(scanf("%d",&choice)!=1)
      {
         choice=0;
      }
      clear_buffer();
      switch(choice)
      {
         case 1:
         {
            if(contact_index>=1 && contact_index<=addressBook->ir_size)
            {
               int index=addressBook->index_record[contact_index-1];
               // shift all later contacts one place up to fill the gap
               for(int i=index;i<addressBook->contactCount-1;i++)
               {
                  addressBook->contacts[i]=addressBook->contacts[i+1];
               }
               addressBook->contactCount--;
               printf("Contact deleted successfully\n");
               printf("Delete another contact?\n1. Yes\n2. No\nEnter your choice : ");
               if(scanf("%d",&choice)!=1)
               {
                  choice=0;
               }
               clear_buffer();
               if(choice!=1)
               {
                  choice=2;
               }
            }
            else
            {
               printf("Invalid contact number.\n");
            }
            break;
         }
         case 2:
         {
            printf("Deletion cancelled\n");
            break;
         }
         default:
         {
            printf("Invalid choice, enter 1 or 2.\n");
            break;
         }
      }
   }while(choice!=2);
}
// prints all contacts in a table
void list_contacts(struct AddressBook *addressBook)
{
   printf("\n--------------------------------------------------------------------------\n");
   printf("%-25s %-15s %-30s\n", "Name", "Phone Number", "Email");
   printf("--------------------------------------------------------------------------\n");
   for(int i=0;i<addressBook->contactCount;i++)
   {
      printf("%-25s %-15s %-30s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
      printf("--------------------------------------------------------------------------\n");
   }
}
// saves all contacts to database.csv, one contact per line as name,phone,email
// note: if a name contains a comma, the line will not load back correctly
void save_contacts(struct AddressBook *addressBook)
{
   FILE *file=fopen("database.csv","w");
   if(file==NULL)
   {
      printf("Could not save contacts to database.csv\n");
      return;
   }
   for(int i=0;i<addressBook->contactCount;i++)
   {
      fprintf(file,"%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
   }
   fclose(file);
}
// loads contacts from database.csv into addressBook, if the file exists
void load_contacts(struct AddressBook *addressBook)
{
   FILE *file=fopen("database.csv","r");
   // no file yet: nothing to load, start with an empty address book
   if(file==NULL)
   {
      return;
   }
   int max_contacts=(int)(sizeof(addressBook->contacts)/sizeof(addressBook->contacts[0]));
   while(addressBook->contactCount<max_contacts)
   {
      struct Contact c;
      // matches the format written by save_contacts: name,phone,email
      int fields=fscanf(file," %49[^,],%14[^,],%49[^\n]",c.name,c.phone,c.email);
      if(fields!=3)
      {
         break;
      }
      addressBook->contacts[addressBook->contactCount]=c;
      addressBook->contactCount++;
   }
   fclose(file);
}
