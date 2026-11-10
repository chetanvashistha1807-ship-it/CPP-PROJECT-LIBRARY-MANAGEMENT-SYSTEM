
#include <iostream>
#include <sstream>
#include <string>
#include "Library.h"
#include "Book.h"
#include "Member.h"

using namespace std;

bool runTest(Library& library, string testName,
             string input, string expectedOutput) {

    istringstream testInput(input);
    ostringstream testOutput;

    streambuf* originalCin = cin.rdbuf();
    streambuf* originalCout = cout.rdbuf();

    cin.rdbuf(testInput.rdbuf());
    cout.rdbuf(testOutput.rdbuf());

    library.issuebook();

    // Restore normal input and output
    cin.rdbuf(originalCin);
    cout.rdbuf(originalCout);

    string actualOutput = testOutput.str();

    bool passed = actualOutput.find(expectedOutput) != string::npos;

    cout << (passed ? "[PASS] " : "[FAIL] ")
         << testName << endl;

    if (!passed) {
        cout << "Expected: " << expectedOutput << endl;
        cout << "Actual output: " << actualOutput << endl;
    }

    return passed;
}

int main() {

    Library library;

    // Sample book: 2 available copies
    Book b1(103, "The Alchemist", "Paulo Coelho", 2, 2);
    library.addBook(b1);

    // Sample members
    Member m1(10, "Rahul", "rahul@gmail.com", "9876543210");
    Member m2(20, "Aman", "aman@gmail.com", "9876543211");
    Member m3(30, "Vikram", "vikram@gmail.com", "9876543212");

    library.addMember(m1);
    library.addMember(m2);
    library.addMember(m3);

    int passed = 0;
    int total = 6;

    cout << "\n===== AUTOMATED ISSUE BOOK TESTS =====\n\n";

    // Test 1: Successful issuance
    if (runTest(library, "Successful issuance",
                "10\n103\n", "Book issued successfully"))
        passed++;

    // Test 2: Duplicate borrowing
    if (runTest(library, "Duplicate borrowing",
                "10\n103\n", "already been borrowed"))
        passed++;

    // Test 3: Another member successfully borrows
    if (runTest(library, "Second member borrows book",
                "20\n103\n", "Book issued successfully"))
        passed++;

    // Test 4: No copies available
    if (runTest(library, "No copies available",
                "30\n103\n", "No copies available"))
        passed++;

    // Test 5: Invalid member ID
    if (runTest(library, "Invalid member ID",
                "99\n", "No member found"))
        passed++;

    // Test 6: Invalid book ID
    if (runTest(library, "Invalid book ID",
                "20\n999\n", "No Book found"))
        passed++;

    cout << "\n====================================\n";
    cout << "RESULT: " << passed << "/" << total << " tests passed.\n";
    cout << "====================================\n";

    return 0;
}
