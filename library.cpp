#include "library.h"
#include<string>
#include<iostream>
#include<algorithm>
#include <stdexcept>


using namespace std;


void Library::loadData() {
    books.clear();
    members.clear();

    try {
        vector<int> bookIds = fileManager.loadBookIds();
        for (int i = 0; i < bookIds.size(); i++) {
            try {
                books.push_back(fileManager.loadBook(bookIds[i]));
            } catch (const exception& e) {
                cout << "Skipping book " << bookIds[i] << ": " << e.what() << endl;
            } catch (const char* e) {
                cout << "Skipping book " << bookIds[i] << ": " << e << endl;
            }
        }

        vector<int> memberIds = fileManager.loadMemberIds();
        for (int i = 0; i < memberIds.size(); i++) {
            try {
                members.push_back(fileManager.loadMember(memberIds[i]));
            } catch (const exception& e) {
                cout << "Skipping member " << memberIds[i] << ": " << e.what() << endl;
            } catch (const char* e) {
                cout << "Skipping member " << memberIds[i] << ": " << e << endl;
            }
        }

        cout << "Data loading completed. Books: " << books.size()
             << ", Members: " << members.size() << endl;
    } catch (const exception& e) {
        cout << "Unable to read data registry: " << e.what() << endl;
    }
}

//Every function related to books.
void Library::addBook(const Book& book) {
    int x = book.getBookId();

    // Check for duplicate ID
    for (int i = 0; i < books.size(); i++) {
        if (x == books[i].getBookId()) {
            cout << "Book ID already exists";
            return;
        }
    }

    // Save individual book file
    if (!fileManager.saveBook(book)) {
        cout << "Failed to save book!";
        return;
    }

    // Collect existing book IDs
    vector<int> bookIds;

    for (int i = 0; i < books.size(); i++) {
        bookIds.push_back(books[i].getBookId());
    }

    // Add new book ID
    bookIds.push_back(x);

    // Rewrite the registry
    if (!fileManager.saveBookIds(bookIds)) {
        fileManager.deleteBookFile(x);
        cout << "Failed to update book registry!";
        return;
    }

    // Add book to memory only after persistence succeeds
    books.push_back(book);

    cout << "Book added successfully!";
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
            if (!fileManager.saveBook(books[found])) { cout << "Saving updated book failed."; return false; }
            cout<<"successfully updated";
            return true;
            } 
            else if(choice==2){
                string s;
                cout<<"Enter new Author: ";
                getline(cin,s);
                books[found].updateAuthor(s);
                if (!fileManager.saveBook(books[found])) { cout << "Saving updated book failed."; return false; }
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

bool Library::deleteBook() {
    int id;
    cout << "Enter book id which you want to delete: ";
    cin >> id;

    for (int i = 0; i < books.size(); i++) {
        if (books[i].getBookId() == id) {

            // Collect IDs excluding the book being deleted
            vector<int> bookIds;

            for (int j = 0; j < books.size(); j++) {
                if (j != i) {
                    bookIds.push_back(books[j].getBookId());
                }
            }

            // Update the registry
            if (!fileManager.saveBookIds(bookIds)) {
                cout << "Failed to update book registry!";
                return false;
            }

            if (!fileManager.deleteBookFile(id)) {
            cout << "Warning: Book removed from registry, but file deletion failed.";
            }

            // Delete from memory only after registry update succeeds
            books.erase(books.begin() + i);

            cout << "Book deleted successfully";
            return true;
        }
    }

    cout << "Book is not found";
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
    if (!fileManager.saveMember(member)) { cout << "Failed to save member!"; return; }
    vector<int> ids;
    for (int i = 0; i < members.size(); i++) ids.push_back(members[i].getMemberId());
    ids.push_back(x);
    if (!fileManager.saveMemberIds(ids)) {
        fileManager.deleteMemberFile(x);
        cout << "Failed to update member registry!";
        return;
    }
    members.push_back(member);
    cout << "Member added successfully.";
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
            if (!fileManager.saveMember(members[i])) { cout << "Saving updated member failed."; return false; }
            cout<<endl<<"Name updated successfully.";
            return true;
        }  else if(choice==2){
            cout<<"Enter New Email: "<<endl;
            getline(cin,s);
            members[i].updateEmail(s);
            if (!fileManager.saveMember(members[i])) { cout << "Saving updated member failed."; return false; }
            cout<<endl<<"Email updated successfully.";
            return true;
        }  else if(choice==3){
            cout<<"Enter new phone number: "<<endl;
            cin>>s;
            members[i].updatePhone(s);
            if (!fileManager.saveMember(members[i])) { cout << "Saving updated member failed."; return false; }
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
            vector<int> ids;
            for (int j = 0; j < members.size(); j++) if (j != i) ids.push_back(members[j].getMemberId());
            if (!fileManager.saveMemberIds(ids)) { cout << "Failed to update member registry!"; return false; }
            if (!fileManager.deleteMemberFile(id)) cout << "Warning: member file deletion failed.";
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

void Library::issuebook(){
    int bookId, MemberId, x,y;
    //Check for MEMBER.
    cout<<"Enter Member Id: ";
    cin>>MemberId;
    bool foundMember = false;
    for(int i = 0; i<members.size(); i++){
        if(members[i].getMemberId()==MemberId){
            foundMember=true;
            x = i;
            break;
        }
    } if(foundMember == false){
        cout<<"No member found";
        return;
    }
    //Check For BOOK.
    cout<<endl<<"Enter Book Id: ";
    cin>>bookId;
    bool foundBook = false;
    for(int i = 0; i<books.size(); i++){
        if(books[i].getBookId()==bookId){
            foundBook=true;
            y = i;
            break;
        }
    } if(foundBook==false){
        cout<<"No Book found";
        return;
    }
    
    //Issuing book 
    if(!members[x].addBorrowedBook(bookId)){
    return;
        }

   if(!books[y].issueBook()){
    members[x].removeBook(bookId);
    cout<<"No copies available. ";
    cout<<"Would you like to reserve this book? "<<endl;
    cout<<"1)Yes.    2).No";
    int x;
    cin>>x;
    if(x==1){
        if (books[y].reserveMember(MemberId) && !fileManager.saveBook(books[y]))
            cout << "Warning: reservation was made but saving failed.";
        return;
    } else if(x==2) return;
    else {cout<<"Invalid selection";
    return;}
    
  }

   bool bookSaved = fileManager.saveBook(books[y]);
   bool memberSaved = fileManager.saveMember(members[x]);
   if (!bookSaved || !memberSaved)
       cout << "Warning: issue succeeded in memory, but saving failed.";
   cout<<"Book issued successfully";
    
    
}

void Library::returnbook(){
    int bookId, MemberId, x, y;

    // Find the member returning the book
    cout << "Enter Member Id: ";
    cin >> MemberId;

    bool foundMember = false;

    for(int i = 0; i < members.size(); i++){
        if(members[i].getMemberId() == MemberId){
            foundMember = true;
            x = i;
            break;
        }
    }

    if(!foundMember){
        cout << "No member found";
        return;
    }

    // Find the book
    cout << "Enter Book Id to return: ";
    cin >> bookId;

    bool bookExists = false;

    for(int i = 0; i < books.size(); i++){
        if(books[i].getBookId() == bookId){
            bookExists = true;
            y = i;
            break;
        }
    }

    if(!bookExists){
        cout << "No Book with this book id found.";
        return;
    }

    // Remove book from member's borrowed records
    if(!members[x].removeBook(bookId)){
        cout << "Return Failed";
        return;
    }

    // Increase available copies
    if(!books[y].returnBook()){
        members[x].addBorrowedBook(bookId);
        return;
    }

    // Allocate the returned copy to the next eligible reservation
    while(true){

        int newMember = books[y].getNextReservation();

        if(newMember == -1){
            bool bookSaved = fileManager.saveBook(books[y]);
            bool memberSaved = fileManager.saveMember(members[x]);
            if (!bookSaved || !memberSaved)
                cout << " Warning: return succeeded in memory, but saving failed.";
            cout << "Book returned successfully. No pending reservations.";
            return;
        }

        bool foundReservedMember = false;
        int z = -1;

        for(int i = 0; i < members.size(); i++){
            if(members[i].getMemberId() == newMember){
                foundReservedMember = true;
                z = i;
                break;
            }
        }

        // Skip members who no longer exist
        if(!foundReservedMember){
            continue;
        }

        // Skip members who already have this book
        if(!members[z].addBorrowedBook(bookId)){
            continue;
        }

        // Allocate the returned copy
        books[y].issueBook();

        bool bookSaved = fileManager.saveBook(books[y]);
        bool returningMemberSaved = fileManager.saveMember(members[x]);
        bool receivingMemberSaved = fileManager.saveMember(members[z]);
        if (!bookSaved || !returningMemberSaved || !receivingMemberSaved)
            cout << "Warning: allocation succeeded in memory, but saving failed.";
        cout << "Book issued to the reserving member with ID: " << newMember;
        return;
    }
}

