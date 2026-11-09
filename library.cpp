#include "library.h"
#include<iostream>
#include<algorithm> //used for using inbuilt sort function of cpp.


using namespace std;


//Every function related to books.
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

bool Library::updateBookInfo(int bookId){
    
    for(int i=0; i<books.size(); i++){
        if(books[i].getBookId()==bookId){
           int found = i;
           int choice;
           cout<<"1). Change Title"<<endl<<"2). Change Author"<<endl<<"3). Exit"<<endl;
           cin>>choice;
           cin.ignore();
           if(choice==1){
            string s;
            cout<<"Enter new title: ";
            getline(cin,s);      //cin only takes one word at a time, so a sentence will not be eligible for it and it will tak only the first word.That is why we have used the getline function.
 
            books[found].updateTitle(s);
            cout<<"successfully updated";
            return true;
            } 
            else if(choice==2){
                string s;
                cout<<"Enter new Author: ";
                getline(cin,s);
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

bool Library::deleteBook(){
    int id;
    cout<<"Enter book id which you want to delete: ";
    cin>>id;
    for(int i = 0; i<books.size(); i++){
        if(books[i].getBookId()==id){
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



//Every function related to members.

void Library::addMember(const Member& member){
    int x = member.getMemberId();
    for(int i = 0; i<members.size(); i++){
        if(x==members[i].getMemberId()){
            cout<<"Member already exist with this id.";
            return;
        }
    }
    members.push_back(member);
}

void Library::displayMembers(){
    for(int i = 0; i<members.size(); i++){
        members[i].displayMemberInfo();
        cout<<endl<<endl;
    }
}

Member* Library::searchMemberById(int id){
    for(int i = 0; i<members.size(); i++){
        if(members[i].getMemberId()==id){
            return &members[i];
        }
    }
    cout<<"No member with this member id found";
    return nullptr;
}

bool Library::updateMemberInfo(int id){

    
    for(int i = 0; i<members.size(); i++){
        if(members[i].getMemberId()==id){
            string s;
            int choice;
            cout<<"1). Change Name"<<endl<<"2). Change Email"<<endl<<"3). Change Phone Number"<<endl<<"4). Exit"<<endl;
            cin>>choice;
            cin.ignore();
            if(choice==1){
                
            cout<<"Enter New Name: "<<endl;
            getline(cin,s);
            members[i].updateName(s);
            cout<<endl<<"Name updated successfully.";
            return true;
        }  else if(choice==2){
            cout<<"Enter New Email: "<<endl;
            getline(cin,s);
            members[i].updateEmail(s);
            cout<<endl<<"Email updated successfully.";
            return true;
        }  else if(choice==3){
            cout<<"Enter new phone number: "<<endl;
            cin>>s;
            members[i].updatePhone(s);
            cout<<endl<<"Phone updated successfully.";
            return true;
        }   else if(choice==4){
            cout<<"Operation Terminated";
            return false;
        }   else {cout<<"Invalid Choice";
            return false;
        }
        }
    }
    cout<<"Didn't found the member or invalid choice.";
    return false;
}

bool Library::deleteMember(){
    int id;
    cout<<"Enter member id which you want to be deleted: ";
    cin>>id;
    for(int i = 0 ; i<members.size(); i++){
        if(members[i].getMemberId()==id){
            members.erase(members.begin()+i);
            cout<<"Member deleted successfully.";
            return true;
        }
    }
       cout<<"Didn't found the member.";
       return false;
}

void Library::sortMembersById(){
       sort(members.begin(),members.end(),[](const Member&a, const Member&b){
        return a.getMemberId() < b.getMemberId();
       });
}