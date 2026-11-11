#include "filemanager.h"
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstdio>

using namespace std;

bool FileManager::saveBook(const Book& book) {
    string fileName = "Books/" + to_string(book.getBookId()) + ".txt";
    ofstream file(fileName.c_str());
    if (!file.is_open()) return false;

    file << book.getBookId() << endl;
    file << book.getTitle() << endl;
    file << book.getAuthor() << endl;
    file << book.getTotalCopies() << endl;
    file << book.getAvailableCopies() << endl;

    vector<int> reservations = book.getReservationIds();
    for (int i = 0; i < reservations.size(); i++) file << reservations[i] << endl;

    file.close();
    return !file.fail();
}

Book FileManager::loadBook(int id) {
    string fileName = "Books/" + to_string(id) + ".txt";
    ifstream file(fileName.c_str());
    if (!file.is_open()) throw runtime_error("Unable to open book file");

    int storedId, totalCopies, availableCopies;
    string title, author;
    if (!(file >> storedId)) throw runtime_error("Invalid book ID in file");
    file.ignore(10000, '\n');
    if (!getline(file, title) || !getline(file, author) ||
        !(file >> totalCopies) || !(file >> availableCopies))
        throw runtime_error("Invalid or incomplete book file");
    if (storedId != id) throw runtime_error("Book ID mismatch");

    Book book(storedId, title, author, totalCopies, availableCopies);
    int memberId;
    while (file >> memberId) {
        if (memberId <= 0) throw runtime_error("Invalid reservation ID in book file");
        book.restoreReservation(memberId);
    }
    if (!file.eof()) throw runtime_error("Invalid reservation data in book file");
    return book;
}

bool FileManager::saveBookIds(const vector<int>& bookIds) {
    ofstream file("Books/BookIds.txt");
    if (!file.is_open()) return false;
    for (int i = 0; i < bookIds.size(); i++) file << bookIds[i] << endl;
    file.close();
    return !file.fail();
}

vector<int> FileManager::loadBookIds() {
    vector<int> ids;
    ifstream file("Books/BookIds.txt");
    if (!file.is_open()) return ids;
    int id;
    while (file >> id) {
        if (id <= 0) throw runtime_error("Invalid book ID in registry");
        ids.push_back(id);
    }
    if (!file.eof()) throw runtime_error("Invalid book registry");
    return ids;
}

bool FileManager::deleteBookFile(int id) {
    string fileName = "Books/" + to_string(id) + ".txt";
    return remove(fileName.c_str()) == 0;
}

bool FileManager::saveMember(const Member& member) {
    string fileName = "Members/" + to_string(member.getMemberId()) + ".txt";
    ofstream file(fileName.c_str());
    if (!file.is_open()) return false;

    file << member.getMemberId() << endl;
    file << member.getName() << endl;
    file << member.getEmail() << endl;
    file << member.getPhone() << endl;
    const vector<int>& borrowed = member.getBorrowedBookIds();
    for (int i = 0; i < borrowed.size(); i++) file << borrowed[i] << endl;

    file.close();
    return !file.fail();
}

Member FileManager::loadMember(int id) {
    string fileName = "Members/" + to_string(id) + ".txt";
    ifstream file(fileName.c_str());
    if (!file.is_open()) throw runtime_error("Unable to open member file");

    int storedId;
    string name, email, phone;
    if (!(file >> storedId)) throw runtime_error("Invalid member ID in file");
    file.ignore(10000, '\n');
    if (!getline(file, name) || !getline(file, email) || !getline(file, phone))
        throw runtime_error("Invalid or incomplete member file");
    if (storedId != id) throw runtime_error("Member ID mismatch");

    Member member(storedId, name, email, phone);
    int bookId;
    while (file >> bookId) {
        if (bookId <= 0) throw runtime_error("Invalid borrowed book ID in member file");
        member.restoreBorrowedBook(bookId);
    }
    if (!file.eof()) throw runtime_error("Invalid borrowed-book data in member file");
    return member;
}

bool FileManager::saveMemberIds(const vector<int>& memberIds) {
    ofstream file("Members/MemberIds.txt");
    if (!file.is_open()) return false;
    for (int i = 0; i < memberIds.size(); i++) file << memberIds[i] << endl;
    file.close();
    return !file.fail();
}

vector<int> FileManager::loadMemberIds() {
    vector<int> ids;
    ifstream file("Members/MemberIds.txt");
    if (!file.is_open()) return ids;
    int id;
    while (file >> id) {
        if (id <= 0) throw runtime_error("Invalid member ID in registry");
        ids.push_back(id);
    }
    if (!file.eof()) throw runtime_error("Invalid member registry");
    return ids;
}

bool FileManager::deleteMemberFile(int id) {
    string fileName = "Members/" + to_string(id) + ".txt";
    return remove(fileName.c_str()) == 0;
}