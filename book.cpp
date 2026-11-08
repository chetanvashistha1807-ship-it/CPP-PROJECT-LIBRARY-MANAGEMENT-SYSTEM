#include "book.h"
#include<iostream>


Book::Book(int id, string Title, string Author, int Total_copies, int Available_copies){
    if(id>0){
        book_id = id;
    } else{
        throw "Invalid book id";
    }
    if(Total_copies>=0)total_copies = Total_copies;
    else throw "Invalid total copies";

    if(Available_copies>=0 && Available_copies<=Total_copies)available_copies =Available_copies;
    else throw "Invalid available copies";

    title = Title;
    author = Author;
    
}

 
bool Book::issueBook(){
    if(available_copies>0){
        available_copies--;
        cout<<"issued successfully";
        return true;
    }
    else return false;
}

bool Book::returnBook(){
    if(available_copies<total_copies){
        available_copies++;
        cout<<"returned successfully";
        return true;
    } else return false;
}

void Book::displayBookInfo(){
    cout<<"ID: "<<book_id<<endl;
    cout<<"Title: "<<title<<endl;
    cout<<"Author: "<<author<<endl;
    cout<<"Total copies : "<<total_copies<<endl;
    cout<<"Available copies: "<<available_copies<<endl;
}

int Book::getBookId() const{
    return book_id;
}

void Book::updateTitle(string s){
    title = s;
}

void Book::updateAuthor(string s){
    author = s;
}