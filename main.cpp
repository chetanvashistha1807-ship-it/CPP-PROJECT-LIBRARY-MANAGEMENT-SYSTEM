#include<iostream>
#include"Book.h"
#include"library.h"

using namespace std;

int main(){
    Library lib;
    Book b1(101, "The Hobbit", "J.R.R. Tolkien", 5, 5);
    Book b2(102, "The Song of Ice And Fire", "J.R.R. Martin", 10, 10); 
    lib.addBook(b1);
    lib.addBook(b2);

    lib.displayBooks();
    
    Book* x = lib.searchBookById(102);
    if(x!=nullptr){
        
        x->displayBookInfo();
    } else cout<<"Book Not found";



}
