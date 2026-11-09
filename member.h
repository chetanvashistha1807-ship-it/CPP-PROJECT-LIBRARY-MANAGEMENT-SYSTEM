#pragma once
#include<iostream>
using namespace std;

class Member{
    private:
    int id;
    string name;
    string email;
    string phone;

    public:
    Member(int Id, string Name, string Email, string Phone);
    
    //Getters
    int getMemberId() const;            
    string getName() const;
    string getEmail() const;
    string getPhone() const;
    //Setters
    void updateName(string s);
    void updateEmail(string s);
    void updatePhone(string n);
    
    //Display Function
    void displayMemberInfo();




    
};