#include<iostream>
#include"Book.h"
#include"library.h"

using namespace std;

int main(){
    Library lib;
    Book b1(1, "The Hobbit", "J.R.R. Tolkien", 9, 10);
    Book b2(102, "The Song of Ice And Fire", "J.R.R. Martin", 10, 10); 
    Book b3(102,"The witcher 3","Me",10,10);
    lib.addBook(b1);
    lib.addBook(b2);
    lib.addBook(b3);

    lib.displayBooks();
    
    

    /*lib.updateInfo(102,1);
    lib.updateInfo(102,2);
    lib.updateInfo(102,3);
    lib.updateInfo(102,4);
    lib.updateInfo(104,1);

    lib.deleteBook(101);
    lib.displayBooks();*/



}
