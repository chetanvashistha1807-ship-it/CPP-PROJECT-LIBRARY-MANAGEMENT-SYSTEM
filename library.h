#pragma once
#include<vector>
#include "book.h"
#include "member.h"
#include"filemanager.h"
using namespace std;

class Library{
    private:
    vector<Book> books;
    vector<Member> members;
    FileManager fileManager;

    public:
    void addBook(const Book& book); //Here we used const so that the library class can't modify the book object and we passed it by refrence rather than value
    void displayBooks();
    Book* searchBookById(int id);
    bool updateBookInfo(int bookId);
    bool deleteBook();
    void sortBooksById();
    void sortBooksByTitle();


    void addMember(const Member& member);
    void displayMembers();
    Member* searchMemberById(int id);
    bool updateMemberInfo(int id);
    bool deleteMember();
    void sortMembersById();
    void issuebook();
    void returnbook();
    
};