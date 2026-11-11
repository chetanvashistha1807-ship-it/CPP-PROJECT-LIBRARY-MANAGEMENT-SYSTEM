#pragma once
#include <string>
#include <queue>
#include <vector>
using namespace std;

class Book {
private:
    int book_id;
    string title;
    string author;
    int total_copies;
    int available_copies;
    queue<int> reservation;

public:
    Book(int id, string Title, string Author, int Total_copies, int Available_copies);
    bool issueBook();
    bool returnBook();
    void displayBookInfo();

    int getBookId() const;
    string getTitle() const;
    string getAuthor() const;
    int getTotalCopies() const;
    int getAvailableCopies() const;
    vector<int> getReservationIds() const;
    void restoreReservation(int memberId);

    void updateTitle(string s);
    void updateAuthor(string s);
    bool reserveMember(int id);
    int getNextReservation();
};