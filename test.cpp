#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include "library.h"

using namespace std;

int passed = 0;
int failed = 0;

string capture(void (*operation)(), const string& input); // Not used; operations below use the templated helper.

template <typename Function>
string runWithInput(Function operation, const string& input) {
    istringstream in(input);
    ostringstream out;
    streambuf* oldIn = cin.rdbuf(in.rdbuf());
    streambuf* oldOut = cout.rdbuf(out.rdbuf());

    operation();

    cin.rdbuf(oldIn);
    cout.rdbuf(oldOut);
    return out.str();
}

void check(const string& name, bool condition) {
    if (condition) {
        cout << "[PASS] " << name << '\n';
        ++passed;
    } else {
        cout << "[FAIL] " << name << '\n';
        ++failed;
    }
}

bool contains(const string& text, const string& part) {
    return text.find(part) != string::npos;
}

void section(const string& name) {
    cout << "\n========== " << name << " ==========\n";
}

int main() {
    cout << "LIBRARY MANAGEMENT SYSTEM - AUTOMATED TEST SUITE\n";
    cout << "IMPORTANT: Run this executable from a CLEAN TEST COPY of the project.\n";
    cout << "It writes to Books/ and Members/ and replaces their ID registries.\n\n";

    // The project uses relative paths and the C++ standard library available in GCC 6.3.
    // Books/ and Members/ must already exist in the test-copy working directory.
    {
        ofstreamTestPlaceholder:
        ;
    }

    section("BOOK CLASS");

    {
        Book book(9001, "The C++ Primer", "Stanley Lippman", 3, 3);
        check("Book constructor and getters",
              book.getBookId() == 9001 &&
              book.getTitle() == "The C++ Primer" &&
              book.getAuthor() == "Stanley Lippman" &&
              book.getTotalCopies() == 3 &&
              book.getAvailableCopies() == 3);

        check("Issue book decrements available copies",
              book.issueBook() && book.getAvailableCopies() == 2);

        check("Issue until no copies remain",
              book.issueBook() && book.issueBook() &&
              book.getAvailableCopies() == 0);

        check("Issue fails when no copies are available",
              !book.issueBook() && book.getAvailableCopies() == 0);

        check("Return increments available copies",
              book.returnBook() && book.getAvailableCopies() == 1);

        check("Return cannot exceed total copies",
              book.returnBook() && book.returnBook() &&
              !book.returnBook() && book.getAvailableCopies() == 3);

        book.updateTitle("Modern C++");
        book.updateAuthor("New Author");
        check("Book title and author updates",
              book.getTitle() == "Modern C++" &&
              book.getAuthor() == "New Author");

        check("First reservation is accepted",
              book.reserveMember(201));

        check("Duplicate reservation is rejected",
              !book.reserveMember(201));

        check("Different member can reserve",
              book.reserveMember(202));

        vector<int> reservations = book.getReservationIds();
        check("Reservation IDs preserve FIFO order",
              reservations.size() == 2 &&
              reservations[0] == 201 && reservations[1] == 202);

        check("getNextReservation follows FIFO order",
              book.getNextReservation() == 201 &&
              book.getNextReservation() == 202 &&
              book.getNextReservation() == -1);

        book.restoreReservation(203);
        check("restoreReservation restores queue data",
              book.getNextReservation() == 203);

        bool invalidIdThrown = false;
        try { Book invalid(0, "X", "Y", 1, 1); }
        catch (const char*) { invalidIdThrown = true; }
        check("Book rejects non-positive ID", invalidIdThrown);

        bool invalidTotalThrown = false;
        try { Book invalid(1, "X", "Y", -1, 0); }
        catch (const char*) { invalidTotalThrown = true; }
        check("Book rejects negative total copies", invalidTotalThrown);

        bool invalidAvailableThrown = false;
        try { Book invalid(1, "X", "Y", 2, 3); }
        catch (const char*) { invalidAvailableThrown = true; }
        check("Book rejects available copies above total", invalidAvailableThrown);

        string display = runWithInput([&]() { book.displayBookInfo(); }, "");
        check("Book display includes identifying information",
              contains(display, "9001") &&
              contains(display, "Modern C++") &&
              contains(display, "New Author"));
    }

    section("MEMBER CLASS");

    {
        Member member(9101, "Aarav Sharma", "aarav@example.com", "0987654321");
        check("Member constructor and getters",
              member.getMemberId() == 9101 &&
              member.getName() == "Aarav Sharma" &&
              member.getEmail() == "aarav@example.com" &&
              member.getPhone() == "0987654321");

        check("Borrowed-book list initially empty",
              member.getBorrowedBookIds().empty());

        check("Member can borrow a book",
              member.addBorrowedBook(9001) &&
              member.getBorrowedBookIds().size() == 1);

        check("Member cannot add the same book twice",
              !member.addBorrowedBook(9001) &&
              member.getBorrowedBookIds().size() == 1);

        check("Member can borrow another book",
              member.addBorrowedBook(9002) &&
              member.getBorrowedBookIds().size() == 2);

        check("Member can remove a borrowed book",
              member.removeBook(9001) &&
              member.getBorrowedBookIds().size() == 1 &&
              member.getBorrowedBookIds()[0] == 9002);

        check("Removing a book not borrowed fails",
              !member.removeBook(9999));

        member.updateName("Aarav S.");
        member.updateEmail("new@example.com");
        member.updatePhone("0012345678");
        check("Member information updates",
              member.getName() == "Aarav S." &&
              member.getEmail() == "new@example.com" &&
              member.getPhone() == "0012345678");

        member.restoreBorrowedBook(9003);
        check("restoreBorrowedBook restores saved borrowing data",
              member.getBorrowedBookIds().size() == 2 &&
              member.getBorrowedBookIds()[1] == 9003);

        bool invalidIdThrown = false;
        try { Member invalid(0, "X", "x@y.com", "123"); }
        catch (const char*) { invalidIdThrown = true; }
        check("Member rejects non-positive ID", invalidIdThrown);

        string display = runWithInput([&]() { member.displayMemberInfo(); }, "");
        check("Member display includes identifying information",
              contains(display, "Aarav S.") &&
              contains(display, "new@example.com") &&
              contains(display, "0012345678"));
    }

    section("FILE MANAGER");

    {
        FileManager files;
        Book book(9201, "File Handling in C++", "B. Author", 4, 2);
        book.restoreReservation(9202);
        book.restoreReservation(9203);

        Member member(9202, "File Member", "file@example.com", "000123");
        member.restoreBorrowedBook(9201);
        member.restoreBorrowedBook(9204);

        check("saveBook writes a book record", files.saveBook(book));
        check("loadBook restores book fields and reservations", [&]() {
            Book loaded = files.loadBook(9201);
            vector<int> q = loaded.getReservationIds();
            return loaded.getBookId() == 9201 &&
                   loaded.getTitle() == "File Handling in C++" &&
                   loaded.getAuthor() == "B. Author" &&
                   loaded.getTotalCopies() == 4 &&
                   loaded.getAvailableCopies() == 2 &&
                   q.size() == 2 && q[0] == 9202 && q[1] == 9203;
        }());

        check("saveMember writes a member record", files.saveMember(member));
        check("loadMember restores member fields and borrowed IDs", [&]() {
            Member loaded = files.loadMember(9202);
            const vector<int>& ids = loaded.getBorrowedBookIds();
            return loaded.getMemberId() == 9202 &&
                   loaded.getName() == "File Member" &&
                   loaded.getEmail() == "file@example.com" &&
                   loaded.getPhone() == "000123" &&
                   ids.size() == 2 && ids[0] == 9201 && ids[1] == 9204;
        }());

        vector<int> bookIds;
        bookIds.push_back(9201);
        bookIds.push_back(9205);
        check("saveBookIds and loadBookIds", 
              files.saveBookIds(bookIds) &&
              files.loadBookIds() == bookIds);

        vector<int> memberIds;
        memberIds.push_back(9202);
        memberIds.push_back(9206);
        check("saveMemberIds and loadMemberIds",
              files.saveMemberIds(memberIds) &&
              files.loadMemberIds() == memberIds);

        bool missingBookThrows = false;
        try { files.loadBook(999999); }
        catch (const exception&) { missingBookThrows = true; }
        check("Loading a missing book reports an error", missingBookThrows);

        bool missingMemberThrows = false;
        try { files.loadMember(999999); }
        catch (const exception&) { missingMemberThrows = true; }
        check("Loading a missing member reports an error", missingMemberThrows);

        check("deleteBookFile removes a saved book",
              files.deleteBookFile(9201));

        check("deleteMemberFile removes a saved member",
              files.deleteMemberFile(9202));

        // Restore empty registries before the Library integration tests.
        check("Reset book registry for integration tests",
              files.saveBookIds(vector<int>()));

        check("Reset member registry for integration tests",
              files.saveMemberIds(vector<int>()));
    }

    section("LIBRARY - BOOK AND MEMBER CRUD");

    {
        Library library;
        runWithInput([&]() { library.loadData(); }, "");

        runWithInput([&]() {
            library.addBook(Book(9302, "Zebra", "Author Z", 2, 2));
            library.addBook(Book(9301, "Alpha", "Author A", 3, 3));
        }, "");

        check("Add books and find by ID",
              library.searchBookById(9301) != nullptr &&
              library.searchBookById(9302) != nullptr);

        string duplicateBookOutput = runWithInput([&]() {
            library.addBook(Book(9301, "Duplicate", "Nobody", 1, 1));
        }, "");
        check("Duplicate book ID is rejected",
              contains(duplicateBookOutput, "already exists") &&
              library.searchBookById(9301)->getTitle() == "Alpha");

        check("Search missing book returns null",
              library.searchBookById(999999) == nullptr);

        string bookDisplay = runWithInput([&]() { library.displayBooks(); }, "");
        check("Display books prints stored records",
              contains(bookDisplay, "Alpha") && contains(bookDisplay, "Zebra"));

        check("Update book title through Library",
              runWithInput([&]() { library.updateBookInfo(9301); },
                           "1\nAlpha Updated\n").size() > 0 &&
              library.searchBookById(9301)->getTitle() == "Alpha Updated");

        check("Update book author through Library",
              runWithInput([&]() { library.updateBookInfo(9301); },
                           "2\nUpdated Author\n").size() > 0 &&
              library.searchBookById(9301)->getAuthor() == "Updated Author");

        check("Update missing book fails",
              !runWithInput([&]() { library.updateBookInfo(999999); }, "").empty());

        library.sortBooksById();
        string sortedById = runWithInput([&]() { library.displayBooks(); }, "");
        check("Sort books by ID",
              sortedById.find("9301") < sortedById.find("9302"));

        library.sortBooksByTitle();
        string sortedByTitle = runWithInput([&]() { library.displayBooks(); }, "");
        check("Sort books by title",
              sortedByTitle.find("Alpha Updated") < sortedByTitle.find("Zebra"));

        runWithInput([&]() {
            library.addMember(Member(9402, "Zoya Khan", "zoya@example.com", "222"));
            library.addMember(Member(9401, "Aman Das", "aman@example.com", "111"));
        }, "");

        check("Add members and find by ID",
              library.searchMemberById(9401) != nullptr &&
              library.searchMemberById(9402) != nullptr);

        string duplicateMemberOutput = runWithInput([&]() {
            library.addMember(Member(9401, "Duplicate", "d@example.com", "333"));
        }, "");
        check("Duplicate member ID is rejected",
              contains(duplicateMemberOutput, "already exist") &&
              library.searchMemberById(9401)->getName() == "Aman Das");

        check("Search missing member returns null",
              library.searchMemberById(999999) == nullptr);

        string memberDisplay = runWithInput([&]() { library.displayMembers(); }, "");
        check("Display members prints stored records",
              contains(memberDisplay, "Aman Das") && contains(memberDisplay, "Zoya Khan"));

        check("Update member name through Library",
              !runWithInput([&]() { library.updateMemberInfo(9401); },
                            "1\nAman Updated\n").empty() &&
              library.searchMemberById(9401)->getName() == "Aman Updated");

        check("Update member email through Library",
              !runWithInput([&]() { library.updateMemberInfo(9401); },
                            "2\nupdated@example.com\n").empty() &&
              library.searchMemberById(9401)->getEmail() == "updated@example.com");

        check("Update member phone through Library",
              !runWithInput([&]() { library.updateMemberInfo(9401); },
                            "3\n000999\n").empty() &&
              library.searchMemberById(9401)->getPhone() == "000999");

        check("Update missing member fails",
              !runWithInput([&]() { library.updateMemberInfo(999999); }, "").empty());

        library.sortMembersById();
        string sortedMembers = runWithInput([&]() { library.displayMembers(); }, "");
        check("Sort members by ID",
              sortedMembers.find("9401") < sortedMembers.find("9402"));

        check("Delete book through Library",
              !runWithInput([&]() { library.deleteBook(); }, "9302\n").empty() &&
              library.searchBookById(9302) == nullptr);

        check("Delete member through Library",
              !runWithInput([&]() { library.deleteMember(); }, "9402\n").empty() &&
              library.searchMemberById(9402) == nullptr);

        check("Deleting missing book reports failure",
              !runWithInput([&]() { library.deleteBook(); }, "999999\n").empty());

        check("Deleting missing member reports failure",
              !runWithInput([&]() { library.deleteMember(); }, "999999\n").empty());

        // Persistence after CRUD and update operations.
        Library reloaded;
        runWithInput([&]() { reloaded.loadData(); }, "");
        check("Library loadData restores books and members",
              reloaded.searchBookById(9301) != nullptr &&
              reloaded.searchBookById(9302) == nullptr &&
              reloaded.searchMemberById(9401) != nullptr &&
              reloaded.searchMemberById(9402) == nullptr);

        check("Updated book and member fields persist",
              reloaded.searchBookById(9301)->getTitle() == "Alpha Updated" &&
              reloaded.searchMemberById(9401)->getName() == "Aman Updated" &&
              reloaded.searchMemberById(9401)->getPhone() == "000999");
    }

    section("LIBRARY - ISSUE, RETURN AND RESERVATIONS");

    {
        Library library;
        runWithInput([&]() { library.loadData(); }, "");

        runWithInput([&]() {
            library.addBook(Book(9501, "One Copy Book", "Author", 1, 1));
            library.addMember(Member(9511, "Borrower One", "one@example.com", "1"));
            library.addMember(Member(9512, "Borrower Two", "two@example.com", "2"));
            library.addMember(Member(9513, "Borrower Three", "three@example.com", "3"));
        }, "");

        runWithInput([&]() { library.issuebook(); }, "999999\n");
        check("Issue rejects unknown member",
              library.searchBookById(9501)->getAvailableCopies() == 1);

        runWithInput([&]() { library.issuebook(); }, "9511\n999999\n");
        check("Issue rejects unknown book",
              library.searchBookById(9501)->getAvailableCopies() == 1);

        runWithInput([&]() { library.issuebook(); }, "9511\n9501\n");
        check("Successful issue updates book and member",
              library.searchBookById(9501)->getAvailableCopies() == 0 &&
              library.searchMemberById(9511)->getBorrowedBookIds().size() == 1);

        string duplicateIssue = runWithInput([&]() {
            library.issuebook();
        }, "9511\n9501\n");
        check("Member cannot borrow the same book twice",
              contains(duplicateIssue, "already been borrowed") &&
              library.searchMemberById(9511)->getBorrowedBookIds().size() == 1);

        string reserveOne = runWithInput([&]() {
            library.issuebook();
        }, "9512\n9501\n1\n");
        check("Unavailable book can be reserved",
              contains(reserveOne, "reserved") &&
              library.searchBookById(9501)->getReservationIds().size() == 1);

        string reserveDuplicate = runWithInput([&]() {
            library.issuebook();
        }, "9512\n9501\n1\n");
        check("Duplicate reservation is rejected",
              contains(reserveDuplicate, "already reserved") &&
              library.searchBookById(9501)->getReservationIds().size() == 1);

        runWithInput([&]() { library.issuebook(); }, "9513\n9501\n1\n");
        check("Second reservation joins FIFO queue",
              library.searchBookById(9501)->getReservationIds().size() == 2);

        string returnToFirst = runWithInput([&]() {
            library.returnbook();
        }, "9511\n9501\n");
        check("Return allocates copy to first reservation",
              contains(returnToFirst, "9512") &&
              library.searchMemberById(9511)->getBorrowedBookIds().empty() &&
              library.searchMemberById(9512)->getBorrowedBookIds().size() == 1 &&
              library.searchBookById(9501)->getAvailableCopies() == 0);

        string returnToSecond = runWithInput([&]() {
            library.returnbook();
        }, "9512\n9501\n");
        check("Next return allocates to second reservation",
              contains(returnToSecond, "9513") &&
              library.searchMemberById(9513)->getBorrowedBookIds().size() == 1);

        string finalReturn = runWithInput([&]() {
            library.returnbook();
        }, "9513\n9501\n");
        check("Return with empty queue makes copy available",
              contains(finalReturn, "No pending reservations") &&
              library.searchBookById(9501)->getAvailableCopies() == 1);

        string invalidReturnMember = runWithInput([&]() {
            library.returnbook();
        }, "999999\n");
        check("Return rejects unknown member",
              contains(invalidReturnMember, "No member found"));

        runWithInput([&]() { library.issuebook(); }, "9511\n9501\n");
        string returnNotBorrowed = runWithInput([&]() {
            library.returnbook();
        }, "9512\n9501\n");
        check("Return rejects a book not borrowed by that member",
              contains(returnNotBorrowed, "Return Failed") &&
              library.searchBookById(9501)->getAvailableCopies() == 0);

        // Verify transaction state survives a fresh Library load.
        Library reloaded;
        runWithInput([&]() { reloaded.loadData(); }, "");
        check("Issue/return state persists after reload",
              reloaded.searchBookById(9501)->getAvailableCopies() == 0 &&
              reloaded.searchMemberById(9511)->getBorrowedBookIds().size() == 1 &&
              reloaded.searchMemberById(9513)->getBorrowedBookIds().empty());
    }

    section("FINAL RESULT");
    cout << "Passed: " << passed << '\n';
    cout << "Failed: " << failed << '\n';
    cout << "Total : " << passed + failed << '\n';

    if (failed == 0) {
        cout << "\nALL TESTS PASSED.\n";
        return 0;
    }

    cout << "\nSOME TESTS FAILED. Review the failed cases before marking the project complete.\n";
    return 1;
}
