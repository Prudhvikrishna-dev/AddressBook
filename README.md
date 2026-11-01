# Address Book in C

A simple menu-driven address book that runs in the terminal. You can add, search, edit, delete and list contacts. Phone numbers and emails are validated and must be unique.

**Author:** Damaraju Prudhvikrishna

## Features

- Add a contact with a name, phone number and email
- Search by name, phone number or email
- Edit the name, phone number or email of a contact
- Delete a contact, with a choice to delete another
- List all contacts in a table
- Save contacts to `database.csv` on exit and load them the next time the program runs
- Reject invalid or non-numeric menu input in every menu

## Project Files

| File | Purpose |
|---|---|
| `main.c` | Entry point. Shows the main menu and calls the matching function |
| `addressbook.c` | Add, search, edit, delete and list contacts, plus phone and email validation |
| `addressbook.h` | Structures (`Contact`, `AddressBook`) and function declarations |
| `database.csv` | Stores contact information |

## How to Compile and Run

You need a C compiler such as GCC.

```bash
gcc main.c addressbook.c -o addressbook
./addressbook
```

On Windows, run `addressbook.exe` instead of `./addressbook`.

## Menu

```text
Address Book Menu:
1. Add/Create contact
2. Search contact
3. Edit contact
4. Delete contact
5. List all contacts
6. Exit
```

## Validation Rules

### Phone Number
- Exactly 10 digits
- The first digit must be from 6 to 9
- Digits only
- Must not already exist

### Email
- Must contain exactly one `@` and must not start with it
- Only letters, digits and the symbols `. _ - @` are allowed
- No two dots together, and no dot right after `@`
- Must be in the format `name@domain.com`
- Stored in lowercase, so `A@B.COM` and `a@b.com` are treated as the same email
- Must not already exist

### Name
- Can be anything, up to 49 characters

## Search Behaviour

- **Name:** Exact match, case-sensitive. All contacts with that name are shown.
- **Phone number and email:** Exact match. Since both are unique, at most one contact is found.

## Limit
