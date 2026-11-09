
#include <iostream>
#include "Book.h"
#include "Library.h"
#include "member.h"

using namespace std;

int main() {

    Library lib;

    // Sample books
    Book b1(103, "The Hobbit", "J.R.R. Tolkien", 5, 5);
    Book b2(101, "Atomic Habits", "James Clear", 3, 3);
    Book b3(102, "The Alchemist", "Paulo Coelho", 4, 4);

    lib.addBook(b1);
    lib.addBook(b2);
    lib.addBook(b3);

    // Sample members
    Member m1(203, "Chetan", "chetan@gmail.com", "9876543210");
    Member m2(201, "Rahul", "rahul@gmail.com", "9876543211");
    Member m3(202, "Aman", "aman@gmail.com", "9876543212");

    lib.addMember(m1);
    lib.addMember(m2);
    lib.addMember(m3);

    int choice;

    do {
        cout << "\n\n========== LIBRARY TEST MENU ==========\n";
        cout << "1. Display Books\n";
        cout << "2. Search Book\n";
        cout << "3. Update Book\n";
        cout << "4. Delete Book\n";
        cout << "5. Sort Books by ID\n";
        cout << "6. Sort Books by Title\n";
        cout << "7. Display Members\n";
        cout << "8. Search Member\n";
        cout << "9. Update Member\n";
        cout << "10. Delete Member\n";
        cout << "11. Sort Members by ID\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;

        switch (choice) {

            case 1:
                lib.displayBooks();
                break;

            case 2: {
                int id;
                cout << "Enter Book ID: ";
                cin >> id;
                lib.searchBookById(id);
                break;
            }

            case 3: {
                int id;
                cout << "Enter Book ID: ";
                cin >> id;
                lib.updateBookInfo(id);
                break;
            }

            case 4:
                lib.deleteBook();
                break;

            case 5:
                lib.sortBooksById();
                cout << "Books sorted by ID.\n";
                break;

            case 6:
                lib.sortBooksByTitle();
                cout << "Books sorted by title.\n";
                break;

            case 7:
                lib.displayMembers();
                break;

            case 8: {
                int id;
                cout << "Enter Member ID: ";
                cin >> id;

                Member* member = lib.searchMemberById(id);

                if (member != nullptr) {
                    member->displayMemberInfo();
                }
                break;
            }

            case 9: {
                int id;
                cout << "Enter Member ID: ";
                cin >> id;
                lib.updateMemberInfo(id);
                break;
            }

            case 10:
                lib.deleteMember();
                break;

            case 11:
                lib.sortMembersById();
                cout << "Members sorted by ID.\n";
                break;

            case 0:
                cout << "Exiting test program.\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    return 0;
}
