
#include "filemanager.h"
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstdio>


using namespace std;

bool FileManager::saveBook(const Book& book) {

    string fileName = "Books/" + to_string(book.getBookId()) + ".txt";
    ofstream file(fileName);

    if (!file.is_open()) {
        return false;
    }

    file << book.getBookId() << '\n';
    file << book.getTitle() << '\n';
    file << book.getAuthor() << '\n';
    file << book.getTotalCopies() << '\n';
    file << book.getAvailableCopies() << '\n';

    file.close();

    return !file.fail();
}


Book FileManager::loadBook(int id){
    string fileName = "Books/" + to_string(id) + ".txt";
    ifstream file (fileName);
    if(!file.is_open()){
        throw runtime_error("Unable to open book file");
    }
    int BookId;
    string title;
    string author;
    int available_copies;
    int total_copies;
    file>>BookId;
    file.ignore();
    getline(file,title);
    getline(file,author);
    file>>total_copies;
    file>>available_copies;

    if (!file) {    //a file might have corrupted or incomplete data, this function throws an error if it has.
    throw runtime_error("Invalid book file");
     }
    
    Book book(id, title, author, total_copies, available_copies);
    return book;
}

bool FileManager::saveBookIds(const vector<int> &bookIds){
    ofstream file("Books/BookIds.txt");
    if(!file.is_open()){
        return false;
    }
    for(int i = 0; i<bookIds.size(); i++){

    
    file<<bookIds[i]<<endl;
    
    }

   return file.good();
}

vector<int> FileManager::loadBookIds(){
    vector<int> bookIds;
    ifstream file("Books/BookIds.txt");
    int id;
    if(!file.is_open()){
        return bookIds;
    }
    while(file>>id){
        bookIds.push_back(id);
    }

    return bookIds;
}

bool FileManager::deleteBookFile(int id) {
    string fileName = "Books/" + to_string(id) + ".txt";

    if (remove(fileName.c_str()) == 0) {
        return true;
    }

    return false;
}