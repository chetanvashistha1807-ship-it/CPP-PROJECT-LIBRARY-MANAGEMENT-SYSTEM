#pragma once
#include "book.h"
#include "member.h"
#include <vector>
using namespace std;

class FileManager {
public:
    bool saveBook(const Book& book);
    Book loadBook(int id);
    bool saveBookIds(const vector<int>& bookIds);
    vector<int> loadBookIds();
    bool deleteBookFile(int id);

    bool saveMember(const Member& member);
    Member loadMember(int id);
    bool saveMemberIds(const vector<int>& memberIds);
    vector<int> loadMemberIds();
    bool deleteMemberFile(int id);
};