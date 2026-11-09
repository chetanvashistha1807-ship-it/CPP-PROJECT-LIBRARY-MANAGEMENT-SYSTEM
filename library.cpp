#include "library.h"
#include<iostream>
#include<algorithm> //used for using inbuilt sort function of cpp.


using namespace std;

void Library::addBook(const Book& book){
    int x = book.getBookId();
    for(int i = 0; i<books.size(); i++){
        if(x==books[i].getBookId()){
            cout<<"Book Id already exists ";
            return;
        }
    }
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
            books[i].displayBookInfo();
            return &books[i];
         } 
    }
    cout<<"Book Not found";
    return  nullptr;


}

bool Library::updateInfo(int bookId, int choice){
    
    for(int i=0; i<books.size(); i++){
        if(books[i].getBookId()==bookId){
           int found = i;
           if(choice==1){
            string s;
            cout<<"Enter new title: ";
            cin>>s;
 
            books[found].updateTitle(s);
            cout<<"successfully updated";
            return true;
            } 
            else if(choice==2){
                string s;
                cout<<"Enter new Author: ";
                cin>>s;
                books[found].updateAuthor(s);
                cout<<"successfully updated";
                return true;
            } else if(choice==3){
                cout<<"Updation cancelled";
                return false;
            } else {cout<<"invalid choice";
                return false;}
            
           
        }
    }

    cout<<"Book not found";
    return false;
    
}

bool Library::deleteBook(int bookId){
    for(int i = 0; i<books.size(); i++){
        if(books[i].getBookId()==bookId){
            books.erase(books.begin()+i);
            cout<<"Book deleted successfully";
            return true;
        }
    }
    cout<<"Book is not found";
    return false;
}

void Library::sortBooksById() {
    std::sort(books.begin(), books.end(),
        [](const Book& a, const Book& b) {                //It is a comparator, provides functionality of ssearching by parameters.
            return a.getBookId() < b.getBookId();
        }
    );
}

void Library::sortBooksByTitle(){
    std::sort(books.begin(), books.end(),
               [](const Book& a, const Book &b){
                return a.getTitle() < b.getTitle();
               });
}