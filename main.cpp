#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include "library.h"

using namespace std;

// Read a complete line and accept it only if it contains one valid integer.
int readInt(const string& prompt) {
    string line;
    int value;
    while (true) {
        cout << prompt;
        if (!getline(cin, line)) {
            return -1;
        }

        stringstream input(line);
        char extra;
        if (input >> value && !(input >> extra)) {
            return value;
        }

        cout << "Invalid input. Please enter a whole number.\n";
    }
}

string readText(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

void showBookMenu() {
    cout << "\n========== BOOK MANAGEMENT ==========\n"
         << "1. Add book\n"
         << "2. Display all books\n"
         << "3. Search book by ID\n"
         << "4. Update book information\n"
         << "5. Delete book\n"
         << "6. Sort books by ID\n"
         << "7. Sort books by title\n"
         << "0. Back to main menu\n";
}

void showMemberMenu() {
    cout << "\n========= MEMBER MANAGEMENT =========\n"
         << "1. Add member\n"
         << "2. Display all members\n"
         << "3. Search member by ID\n"
         << "4. Update member information\n"
         << "5. Delete member\n"
         << "6. Sort members by ID\n"
         << "0. Back to main menu\n";
}

void bookManagement(Library& library) {
    while (true) {
        showBookMenu();
        int choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1: {
                int id = readInt("Enter book ID: ");
                if (id <= 0) {
                    cout << "Book ID must be positive.\n";
                    break;
                }

                string title = readText("Enter book title: ");
                string author = readText("Enter author name: ");
                int copies = readInt("Enter total copies: ");

                if (copies <= 0) {
                    cout << "Total copies must be greater than zero.\n";
                    break;
                }

                Book book(id, title, author, copies, copies);
                library.addBook(book);
                cout << endl;
                break;
            }

            case 2:
                cout << "\n--------------- BOOKS ---------------\n";
                library.displayBooks();
                break;

            case 3: {
                int id = readInt("Enter book ID to search: ");
                if (id > 0) {
                    library.searchBookById(id);
                } else {
                    cout << "Book ID must be positive.\n";
                }
                cout << endl;
                break;
            }

            case 4: {
                int id = readInt("Enter book ID to update: ");
                if (id > 0) {
                    library.updateBookInfo(id);
                } else {
                    cout << "Book ID must be positive.\n";
                }
                cout << endl;
                break;
            }

            case 5:
                library.deleteBook();
                cout << endl;
                break;

            case 6:
                library.sortBooksById();
                cout << "Books sorted by ID.\n";
                break;

            case 7:
                library.sortBooksByTitle();
                cout << "Books sorted by title.\n";
                break;

            case 0:
                return;

            default:
                cout << "Invalid choice. Select an option from the menu.\n";
        }
    }
}

void memberManagement(Library& library) {
    while (true) {
        showMemberMenu();
        int choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1: {
                int id = readInt("Enter member ID: ");
                if (id <= 0) {
                    cout << "Member ID must be positive.\n";
                    break;
                }

                string name = readText("Enter member name: ");
                string email = readText("Enter email address: ");
                string phone = readText("Enter phone number: ");

                Member member(id, name, email, phone);
                library.addMember(member);
                cout << endl;
                break;
            }

            case 2:
                cout << "\n-------------- MEMBERS --------------\n";
                library.displayMembers();
                break;

            case 3: {
                int id = readInt("Enter member ID to search: ");
                if (id > 0) {
                    Member* member = library.searchMemberById(id);
                    if (member != nullptr) {
                        member->displayMemberInfo();
                    }
                } else {
                    cout << "Member ID must be positive.\n";
                }
                cout << endl;
                break;
            }

            case 4: {
                int id = readInt("Enter member ID to update: ");
                if (id > 0) {
                    library.updateMemberInfo(id);
                } else {
                    cout << "Member ID must be positive.\n";
                }
                cout << endl;
                break;
            }

            case 5:
                library.deleteMember();
                cout << endl;
                break;

            case 6:
                library.sortMembersById();
                cout << "Members sorted by ID.\n";
                break;

            case 0:
                return;

            default:
                cout << "Invalid choice. Select an option from the menu.\n";
        }
    }
}

int main() {
    Library library;

    cout << "=====================================\n"
         << "       LIBRARY MANAGEMENT SYSTEM     \n"
         << "=====================================\n";

    // Restore previously saved books and members before showing the menu.
    library.loadData();

    while (true) {
        cout << "\n============== MAIN MENU ==============\n"
             << "1. Book management\n"
             << "2. Member management\n"
             << "3. Issue a book\n"
             << "4. Return a book\n"
             << "0. Save and exit\n";

        int choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1:
                bookManagement(library);
                break;

            case 2:
                memberManagement(library);
                break;

            case 3:
                library.issuebook();
                cout << endl;
                break;

            case 4:
                library.returnbook();
                cout << endl;
                break;

            case 0:
                cout << "Exiting Library Management System. Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice. Select an option from the menu.\n";
        }
    }
}
