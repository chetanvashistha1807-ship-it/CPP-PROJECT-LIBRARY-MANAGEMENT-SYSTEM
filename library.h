#pragma once
#include<vector>
#include "book.h"
using namespace std;

class Library{
    private:
    vector<Book> books;

    public:
    void addBook(const Book& book); //Here we used const so that the library class can't modify the book object and we passed it by refrence rather than value
    void displayBooks();
    
    Book* searchBookById(int id);

    


};