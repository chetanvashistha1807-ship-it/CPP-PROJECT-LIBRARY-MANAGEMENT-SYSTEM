#include"book.h"

#pragma once

using namespace std;

class FileManager{



    public:
    bool saveBook(const Book& book);
    Book loadBook(int id);
    bool saveBookIds(const vector<int> &bookIds);
    vector<int> loadBookIds();
    bool deleteBookFile(int id);
};