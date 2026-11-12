# Library Management System (C++)

A modular, console-based Library Management System built in C++ as a project-based learning exercise.

The application manages books and library members, supports issuing and returning books, maintains per-book FIFO reservation queues, and persists records between program runs.

## Features

### Book management
- Add, display, search, update, and delete books.
- Prevent duplicate book IDs.
- Sort books by ID or title.
- Track total and available copies.

### Member management
- Add, display, search, update, and delete members.
- Prevent duplicate member IDs.
- Sort members by ID.
- Maintain each member's currently borrowed book IDs.

### Borrowing and reservations
- Issue books to registered members when copies are available.
- Return borrowed books and update availability.
- Prevent duplicate borrowing and duplicate reservations.
- Maintain a separate FIFO reservation queue for each book.
- When a copy is returned, allocate it to the next eligible member.
- Skip reservations belonging to deleted or otherwise ineligible members.
- Keep book availability and member borrowing records consistent across operations.

### Persistent storage
- Save individual book and member records as text files.
- Maintain ID registry files for loading records at startup.
- Persist borrowing records and reservation queues.
- Restore saved records when the application starts.

## Project structure

```text
CPP-PROJECT-LIBRARY-MANAGEMENT-SYSTEM/
│
├── main.cpp
├── test.cpp
│
├── book.h
├── book.cpp
├── member.h
├── member.cpp
├── library.h
├── library.cpp
├── filemanager.h
├── filemanager.cpp
│
├── Books/
│   └── .gitkeep
├── Members/
│   └── .gitkeep
│
├── .gitignore
└── README.md
```

| File | Responsibility |
|---|---|
| `main.cpp` | Application entry point, input helpers, and interactive menus |
| `book.h/.cpp` | Book data, copy availability, and reservation-queue operations |
| `member.h/.cpp` | Member data and borrowed-book records |
| `library.h/.cpp` | Coordinates book/member management, borrowing, returns, and reservations |
| `filemanager.h/.cpp` | Saves, loads, and deletes persistent book/member records |
| `test.cpp` | Automated test harness for classes, workflows, and persistence |

The project separates declarations into header files and implementations into source files. This keeps class responsibilities distinct and makes the application a multi-file C++ program.

## Design and data structures

### Book

A `Book` stores:
- `book_id`: unique book identifier.
- `title` and `author`: descriptive information.
- `total_copies` and `available_copies`: inventory counts.
- `queue<int> reservation`: member IDs waiting for this book.

Each book owns its own reservation queue, so reservations for different books remain independent.

### Member

A `Member` stores:
- `id`: unique member identifier.
- `name`, `email`, and `phone`.
- `vector<int> borrowedBookIds`: IDs of books currently borrowed by the member.

Phone numbers are stored as strings so leading zeroes and country-code characters are preserved.

### Library

The `Library` class coordinates operations across books and members. It owns:
```cpp
vector<Book> books;
vector<Member> members;
FileManager fileManager;
```

Search functions return pointers to stored objects when a matching ID is found, or `nullptr` otherwise. IDs are used as stable identifiers rather than vector positions, because sorting or deleting elements can change vector indexes.

### FileManager

`FileManager` handles persistence separately from the library's operational logic. It provides operations to save, load, and delete individual records, and to read/write the ID registries.

## Important workflows

### Issuing a book
1. Locate the member and requested book by ID.
2. Check that the member is not already borrowing that book.
3. Record the borrowing against the member.
4. Issue a copy if one is available.
5. If the issue cannot be completed, roll back the temporary borrowing change.
6. If no copy is available, the member can request a reservation.
7. Persist the affected records.

### Returning a book
1. Locate the member and book.
2. Confirm and remove the borrowing record.
3. Update the book's available-copy count.
4. Process reservations in FIFO order.
5. Skip members who no longer exist or are not eligible to receive the book.
6. Allocate the returned copy to the first eligible reservation, then persist the affected records.

### Reservation queue

A book's reservation queue stores member IDs in arrival order:

```text
Front -> 102 -> 103 -> 104 <- Back
```

Member 102 is considered first. If that reservation is no longer eligible, the system checks the next one. The queue stores IDs rather than vector indexes so that sorting the member collection does not invalidate reservations.

## File storage format

The application uses relative paths and expects the `Books/` and `Members/` directories to exist in its working directory.

Example layout:

```text
Books/
├── 101.txt
├── 102.txt
└── BookIds.txt

Members/
├── 201.txt
├── 202.txt
└── MemberIds.txt
```

Each book file stores its ID, title, author, total copies, available copies, and reservation member IDs. Each member file stores the member ID, name, email, phone number, and borrowed book IDs. The registry files contain the IDs used to locate individual records during startup.

The project uses ID registries instead of directory iteration. This also avoids depending on `std::filesystem`, which was not available in the older MinGW GCC 6.3.0 toolchain used during development.

## Build and run

Compile the application from the project directory:

```bash
g++ -std=c++11 main.cpp book.cpp member.cpp library.cpp filemanager.cpp -o library
```

Run on Windows PowerShell:

```powershell
.\\library.exe
```

At startup, the application loads saved records and displays the main menu. The menu provides book management, member management, issuing, returning, and exit options. Changes are persisted through the library/file-management operations.

## Running the automated tests

The test harness compiles separately from the application:

```bash
g++ -std=c++11 test.cpp book.cpp member.cpp library.cpp filemanager.cpp -o test
```

Run on Windows PowerShell:

```powershell
.\\test.exe
```

**Important:** Run the test executable from a separate, clean copy of the project with empty `Books/` and `Members/` directories. The tests write test records and replace the ID registry files; running them in a directory containing real library data can overwrite that data.

The local test run reported for this project completed with **77 tests passing**. This is a locally run test-harness result, not a claim of a configured continuous-integration pipeline.

## Concepts practised

- Classes, objects, constructors, and encapsulation.
- Header/source separation and multi-file compilation/linking.
- Object relationships and separation of responsibilities.
- `std::vector` for collections of books, members, and borrowed IDs.
- `std::queue` for FIFO reservations.
- Pointers, references, and `nullptr`.
- Searching and sorting with `std::sort` and lambda comparators.
- Console input with `cin`, `getline()`, and stream handling.
- File streams, text-file persistence, and object reconstruction.
- Maintaining consistency across related objects and rolling back failed operations.
- Automated testing with redirected input/output streams.

## Scope and limitations

This is a learning-oriented C++ console application, not a production library platform. It focuses on core catalog, member, borrowing, reservation, and persistence workflows. Features such as due dates, overdue fines, borrowing history, authentication, a graphical interface, and database-backed storage are outside the current project scope.

---

**Author:** Chetan Vashistha

**Repository:** [CPP Project – Library Management System](https://github.com/chetanvashistha1807-ship-it/CPP-PROJECT-LIBRARY-MANAGEMENT-SYSTEM)
