
#include <iostream>
#include <sstream>
#include <string>
#include <functional>
#include "Library.h"

using namespace std;

int passed = 0;
int failed = 0;

string runTest(function<void()> operation, string input) {
    istringstream in(input);
    ostringstream out;

    streambuf* oldCin = cin.rdbuf(in.rdbuf());
    streambuf* oldCout = cout.rdbuf(out.rdbuf());

    operation();

    cin.rdbuf(oldCin);
    cout.rdbuf(oldCout);

    return out.str();
}

void check(string testName, bool condition) {
    if(condition) {
        cout << "[PASS] " << testName << endl;
        passed++;
    }
    else {
        cout << "[FAIL] " << testName << endl;
        failed++;
    }
}

int main() {

    // TEST 1: Successful reservation
    {
        Library lib;
        lib.addBook(Book(10, "C++", "Author", 1, 0));
        lib.addMember(Member(101, "A", "a@test.com", "111"));

        string output = runTest([&]() {
            lib.issuebook();
        }, "101\n10\n1\n");

        check("Successful reservation",
              output.find("reserved") != string::npos);
    }

    // TEST 2: Duplicate reservation
    {
        Library lib;
        lib.addBook(Book(10, "C++", "Author", 1, 0));
        lib.addMember(Member(101, "A", "a@test.com", "111"));

        runTest([&]() {
            lib.issuebook();
        }, "101\n10\n1\n");

        string output = runTest([&]() {
            lib.issuebook();
        }, "101\n10\n1\n");

        check("Duplicate reservation rejected",
              output.find("already reserved") != string::npos);
    }

    // TEST 3: First reservation receives returned book
    {
        Library lib;
        lib.addBook(Book(10, "C++", "Author", 1, 1));

        lib.addMember(Member(101, "A", "a@test.com", "111"));
        lib.addMember(Member(102, "B", "b@test.com", "222"));
        lib.addMember(Member(103, "C", "c@test.com", "333"));

        runTest([&]() {
            lib.issuebook();
        }, "101\n10\n");

        runTest([&]() {
            lib.issuebook();
        }, "102\n10\n1\n");

        runTest([&]() {
            lib.issuebook();
        }, "103\n10\n1\n");

        string output = runTest([&]() {
            lib.returnbook();
        }, "101\n10\n");

        check("First reservation receives returned book",
              output.find("102") != string::npos);
    }

    // TEST 4: Second reservation receives next returned book
    {
        Library lib;
        lib.addBook(Book(10, "C++", "Author", 1, 1));

        lib.addMember(Member(101, "A", "a@test.com", "111"));
        lib.addMember(Member(102, "B", "b@test.com", "222"));
        lib.addMember(Member(103, "C", "c@test.com", "333"));

        runTest([&]() {
            lib.issuebook();
        }, "101\n10\n");

        runTest([&]() {
            lib.issuebook();
        }, "102\n10\n1\n");

        runTest([&]() {
            lib.issuebook();
        }, "103\n10\n1\n");

        runTest([&]() {
            lib.returnbook();
        }, "101\n10\n");

        string output = runTest([&]() {
            lib.returnbook();
        }, "102\n10\n");

        check("Second reservation receives next returned book",
              output.find("103") != string::npos);
    }

    // TEST 5: Deleted reserved member is skipped
    {
        Library lib;
        lib.addBook(Book(10, "C++", "Author", 1, 1));

        lib.addMember(Member(101, "A", "a@test.com", "111"));
        lib.addMember(Member(102, "B", "b@test.com", "222"));
        lib.addMember(Member(103, "C", "c@test.com", "333"));

        runTest([&]() {
            lib.issuebook();
        }, "101\n10\n");

        runTest([&]() {
            lib.issuebook();
        }, "102\n10\n1\n");

        runTest([&]() {
            lib.issuebook();
        }, "103\n10\n1\n");

        runTest([&]() {
            lib.deleteMember();
        }, "102\n");

        string output = runTest([&]() {
            lib.returnbook();
        }, "101\n10\n");

        check("Deleted reservation member skipped",
              output.find("103") != string::npos);
    }

    // TEST 6: Return book when nobody is waiting
{
    Library lib;
    lib.addBook(Book(10, "C++", "Author", 1, 1));

    lib.addMember(Member(101, "A", "a@test.com", "111"));
    lib.addMember(Member(102, "B", "b@test.com", "222"));

    runTest([&]() {
        lib.issuebook();
    }, "101\n10\n");

    string returnOutput = runTest([&]() {
        lib.returnbook();
    }, "101\n10\n");

    string issueOutput = runTest([&]() {
        lib.issuebook();
    }, "102\n10\n");

    check("Return with empty queue keeps book available",
          returnOutput.find("No pending reservations") != string::npos &&
          issueOutput.find("Book issued successfully") != string::npos);
}

// TEST 7: Skip member who already has the book
{
    Library lib;
    lib.addBook(Book(10, "C++", "Author", 1, 1));

    lib.addMember(Member(101, "A", "a@test.com", "111"));
    lib.addMember(Member(102, "B", "b@test.com", "222"));
    lib.addMember(Member(103, "C", "c@test.com", "333"));

    runTest([&]() {
        lib.issuebook();
    }, "101\n10\n");

    runTest([&]() {
        lib.issuebook();
    }, "102\n10\n1\n");

    runTest([&]() {
        lib.issuebook();
    }, "103\n10\n1\n");

    // Simulate Member 102 already having this book.
    lib.searchMemberById(102)->addBorrowedBook(10);

    string output = runTest([&]() {
        lib.returnbook();
    }, "101\n10\n");

    check("Member already holding book is skipped",
          output.find("103") != string::npos);
}

// TEST 8: All reserved members deleted
{
    Library lib;
    lib.addBook(Book(10, "C++", "Author", 1, 1));

    lib.addMember(Member(101, "A", "a@test.com", "111"));
    lib.addMember(Member(102, "B", "b@test.com", "222"));
    lib.addMember(Member(103, "C", "c@test.com", "333"));
    lib.addMember(Member(104, "D", "d@test.com", "444"));

    runTest([&]() {
        lib.issuebook();
    }, "101\n10\n");

    runTest([&]() {
        lib.issuebook();
    }, "102\n10\n1\n");

    runTest([&]() {
        lib.issuebook();
    }, "103\n10\n1\n");

    runTest([&]() {
        lib.deleteMember();
    }, "102\n");

    runTest([&]() {
        lib.deleteMember();
    }, "103\n");

    string returnOutput = runTest([&]() {
        lib.returnbook();
    }, "101\n10\n");

    string issueOutput = runTest([&]() {
        lib.issuebook();
    }, "104\n10\n");

    check("All deleted reservations skipped; book remains available",
          returnOutput.find("No pending reservations") != string::npos &&
          issueOutput.find("Book issued successfully") != string::npos);
}

// TEST 9: Verify allocated member can return the book
{
    Library lib;
    lib.addBook(Book(10, "C++", "Author", 1, 1));

    lib.addMember(Member(101, "A", "a@test.com", "111"));
    lib.addMember(Member(102, "B", "b@test.com", "222"));
    lib.addMember(Member(103, "C", "c@test.com", "333"));

    runTest([&]() {
        lib.issuebook();
    }, "101\n10\n");

    runTest([&]() {
        lib.issuebook();
    }, "102\n10\n1\n");

    string firstReturn = runTest([&]() {
        lib.returnbook();
    }, "101\n10\n");

    // Member 102 should now have the book in their records.
    string secondReturn = runTest([&]() {
        lib.returnbook();
    }, "102\n10\n");

    // Member 103 should be able to borrow the returned copy.
    string nextIssue = runTest([&]() {
        lib.issuebook();
    }, "103\n10\n");

    check("Automatic allocation updates borrowing records and availability",
          firstReturn.find("102") != string::npos &&
          secondReturn.find("No pending reservations") != string::npos &&
          nextIssue.find("Book issued successfully") != string::npos);
}



    cout << "\n--------------------------\n";
    cout << "Passed: " << passed << endl;
    cout << "Failed: " << failed << endl;
    cout << "--------------------------\n";

    return 0;
}
