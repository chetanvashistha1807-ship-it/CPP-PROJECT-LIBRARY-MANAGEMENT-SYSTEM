#include "member.h"


Member::Member(int Id, string Name, string Email, string Phone){
    if(Id>0)id = Id;
    else throw "Invalid Id";

    name = Name;
    email = Email;

    phone = Phone;
    
}

string Member::getName() const{
    return name;
}

string Member::getEmail() const{
    return email;
}

int Member::getMemberId() const{
    return id;
}

string Member::getPhone() const{
    return phone;
}

void Member::updateEmail(string s){
       email = s;
}

void Member::updateName(string s){
    name = s;
}

void Member::updatePhone(string n){
    phone = n;
}



void Member::displayMemberInfo(){
    cout<<"Email: "<<email<<endl;
    cout<<"Name: "<<name<<endl;
    cout<<"Member Id: "<<id<<endl;
    cout<<"Phone Number: "<<phone<<endl;
   
    
}

bool Member::addBorrowedBook(int id){
    for(int i = 0; i<borrowedBookIds.size(); i++){
        if(borrowedBookIds[i]==id){
            cout<<"Book with same book id has already been borrowed by member.";
            return false;
        }
    }

    borrowedBookIds.push_back(id);
    return true;

}

bool Member::removeBook(int id){
    for(int i = 0; i<borrowedBookIds.size(); i++){
        if(borrowedBookIds[i]==id){
            borrowedBookIds.erase(borrowedBookIds.begin()+i);
            return true;
        }
    }

    cout<<"No book with this book id found.";
    return false;


}