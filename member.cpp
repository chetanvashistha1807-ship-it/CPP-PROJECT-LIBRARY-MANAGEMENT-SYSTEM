#include "member.h"

Member::Member(int Id, string Name, string Email, string Phone) {
    if (Id > 0) id = Id;
    else throw "Invalid Id";
    name = Name;
    email = Email;
    phone = Phone;
}

string Member::getName() const { return name; }
string Member::getEmail() const { return email; }
int Member::getMemberId() const { return id; }
string Member::getPhone() const { return phone; }
const vector<int>& Member::getBorrowedBookIds() const { return borrowedBookIds; }

void Member::restoreBorrowedBook(int bookId) {
    borrowedBookIds.push_back(bookId);
}

void Member::updateEmail(string s) { email = s; }
void Member::updateName(string s) { name = s; }
void Member::updatePhone(string n) { phone = n; }

void Member::displayMemberInfo() {
    cout << "Email: " << email << endl;
    cout << "Name: " << name << endl;
    cout << "Member Id: " << id << endl;
    cout << "Phone Number: " << phone << endl;
}

bool Member::addBorrowedBook(int bookId) {
    for (int i = 0; i < borrowedBookIds.size(); i++) {
        if (borrowedBookIds[i] == bookId) {
            cout << "Book with same book id has already been borrowed by member.";
            return false;
        }
    }
    borrowedBookIds.push_back(bookId);
    return true;
}

bool Member::removeBook(int bookId) {
    for (int i = 0; i < borrowedBookIds.size(); i++) {
        if (borrowedBookIds[i] == bookId) {
            borrowedBookIds.erase(borrowedBookIds.begin() + i);
            return true;
        }
    }
    cout << "No book with this book id found.";
    return false;
}