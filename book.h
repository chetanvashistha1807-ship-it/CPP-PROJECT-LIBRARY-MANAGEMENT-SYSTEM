#pragma once
#include <string>

using namespace std;


class Book{
    private:
    int book_id;
    string title;
    string author;
    int total_copies;
    int available_copies;

    public:
    Book(int id, string Title, string Author, int Total_copies, int Available_copies);

    bool issueBook();
    bool returnBook();
    void displayBookInfo();

    int getBookId() const;

    void updateTitle(string s);
    void updateAuthor(string s);
};