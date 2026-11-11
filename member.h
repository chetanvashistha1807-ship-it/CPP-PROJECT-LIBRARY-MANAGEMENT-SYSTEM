#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Member {
private:
    int id;
    string name;
    string email;
    string phone;
    vector<int> borrowedBookIds;

public:
    Member(int Id, string Name, string Email, string Phone);
    int getMemberId() const;
    string getName() const;
    string getEmail() const;
    string getPhone() const;
    const vector<int>& getBorrowedBookIds() const;
    void restoreBorrowedBook(int bookId);

    void updateName(string s);
    void updateEmail(string s);
    void updatePhone(string n);
    void displayMemberInfo();
    bool addBorrowedBook(int id);
    bool removeBook(int id);
};