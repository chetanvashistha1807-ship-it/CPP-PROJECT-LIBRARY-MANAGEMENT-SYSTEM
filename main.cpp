
#include <iostream>
#include <sstream>
#include <string>
#include "Library.h"
#include "Book.h"
#include "Member.h"

using namespace std;

// Runs a function while capturing its console input and output.
string runAction(Library& library, string input, bool issue) {

    istringstream testInput(input);
    ostringstream testOutput;

    streambuf* originalCin = cin.rdbuf();
    streambuf* originalCout = cout.rdbuf();

    cin.rdbuf(testInput.rdbuf());
    cout.rdbuf(testOutput.rdbuf());

    if (issue)
        library.issuebook();
    else
        library.returnbook();

    cin.rdbuf(originalCin);
    cout.rdbuf(originalCout);

    return testOutput.str();
}

void printResult(string testName, bool passed, int& count) {

    if (passed) {
        cout << "[PASS] " << testName << endl;
        count++;
    }
    else {
        cout << "[FAIL] " << testName << endl;
    }
}

int main() {

    int passed = 0;
    int total = 6;

    cout << "\n===== AUTOMATED RETURN BOOK TESTS =====\n\n";

    // TEST 1: Successful return
    {
        Library library;

        Book b(103, "The Alchemist", "Paulo Coelho", 2, 2);
        Member m(10, "Rahul", "rahul@gmail.com", "9876543210");

        library.addBook(b);
        library.addMember(m);

        runAction(library, "10\n103\n", true);

        string output = runAction(library, "10\n103\n", false);

        bool passedTest =
            output.find("Return Failed") == string::npos &&
            output.find("No member found") == string::npos &&
            output.find("No Book with this book id found.") == string::npos &&
            output.find("No book with this book id found.") == string::npos;

        printResult("Successful return", passedTest, passed);
    }

    // TEST 2: Member tries to return a book they never borrowed
    {
        Library library;

        Book b(103, "The Alchemist", "Paulo Coelho", 2, 2);
        Member m(10, "Rahul", "rahul@gmail.com", "9876543210");

        library.addBook(b);
        library.addMember(m);

        string output = runAction(library, "10\n103\n", false);

        bool passedTest =
            output.find("No book with this book id found.") != string::npos;

        printResult("Returning a book not borrowed", passedTest, passed);
    }

    // TEST 3: Nonexistent book
    {
        Library library;

        Book b(103, "The Alchemist", "Paulo Coelho", 2, 2);
        Member m(10, "Rahul", "rahul@gmail.com", "9876543210");

        library.addBook(b);
        library.addMember(m);

        string output = runAction(library, "10\n999\n", false);

        bool passedTest =
            output.find("No Book with this book id found.") != string::npos;

        printResult("Invalid book ID", passedTest, passed);
    }

    // TEST 4: Nonexistent member
    {
        Library library;

        Book b(103, "The Alchemist", "Paulo Coelho", 2, 2);
        library.addBook(b);

        string output = runAction(library, "99\n", false);

        bool passedTest =
            output.find("No member found") != string::npos;

        printResult("Invalid member ID", passedTest, passed);
    }

    // TEST 5: Returning the same book twice
    {
        Library library;

        Book b(103, "The Alchemist", "Paulo Coelho", 2, 2);
        Member m(10, "Rahul", "rahul@gmail.com", "9876543210");

        library.addBook(b);
        library.addMember(m);

        runAction(library, "10\n103\n", true);
        runAction(library, "10\n103\n", false);

        string output = runAction(library, "10\n103\n", false);

        bool passedTest =
            output.find("No book with this book id found.") != string::npos;

        printResult("Duplicate return prevention", passedTest, passed);
    }

    // TEST 6: Rollback when Book::returnBook() fails
    {
        Library library;

        // Book is already at maximum available copies.
        Book b(103, "The Alchemist", "Paulo Coelho", 2, 2);
        Member m(10, "Rahul", "rahul@gmail.com", "9876543210");

        // Create a borrowing record to simulate an inconsistent state.
        m.addBorrowedBook(103);

        library.addBook(b);
        library.addMember(m);

        // First return: removal succeeds, book return fails,
        // so the borrowing record should be restored.
        runAction(library, "10\n103\n", false);

        // Second attempt: if rollback worked, the borrowing record
        // should still exist and removal should succeed again.
        string output = runAction(library, "10\n103\n", false);

        bool passedTest =
            output.find("No book with this book id found.") == string::npos;

        printResult("Rollback restores borrowing record", passedTest, passed);
    }

    cout << "\n=======================================\n";
    cout << "RESULT: " << passed << "/" << total << " tests passed.\n";
    cout << "=======================================\n";

    return 0;
}
