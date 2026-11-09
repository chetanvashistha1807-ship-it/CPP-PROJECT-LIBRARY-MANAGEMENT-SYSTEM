#include<iostream>
#include"Book.h"
#include"library.h"
#include"member.h"

using namespace std;

int main(){

    Member m(12, "Chetan", "chetanhero18@gmail.com", "+919660801333");
    m.displayMemberInfo();
   /* Library lib;
    Book b1(100, "The Xavier", "J.R.R. Tolkien", 9, 9);
    Book b2(101, "The Song of Ice And Fire", "J.R.R. Martin", 10, 10); 
    Book b3(102,"The witcher 3","Me",10,10);
    lib.addBook(b3);
    lib.addBook(b2);
    lib.addBook(b1);*/

    

    /*lib.displayBooks();
    
    

    /*lib.updateInfo(102,1);
    lib.updateInfo(102,2);
    lib.updateInfo(102,3);
    lib.updateInfo(102,4);
    lib.updateInfo(104,1);

    lib.deleteBook(101);
    lib.displayBooks();*/

    /*lib.sortBooksById();
    cout<<"after sorting";

    lib.displayBooks();

    lib.sortBooksByTitle();
    cout<<"after soring with title"<<endl;

    lib.displayBooks();*/

}
