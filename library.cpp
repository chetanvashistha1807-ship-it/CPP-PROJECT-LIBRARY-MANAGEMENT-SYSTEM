#include "library.h"
#include<iostream>


using namespace std;

void Library::addBook(const Book& book){
    books.push_back(book);
}

void Library::displayBooks(){
    for(int i = 0; i<books.size(); i++){
        books[i].displayBookInfo();
        cout<<endl<<endl;
    }
}

Book* Library::searchBookById(int id){
    for(int i = 0; i<books.size(); i++){
         if(books[i].getBookId()==id){
            return &books[i];
         }
    }
    return  nullptr;


}