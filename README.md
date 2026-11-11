# Library Management System (C++)

A modular, console-based Library Management System built in C++ as a project-based learning exercise.

The project focuses on Object-Oriented Programming, modular programming, STL containers, pointers, references, data structures, searching, sorting, and file handling.

The system manages books and library members, supports borrowing and returning, maintains FIFO reservation queues, and stores records persistently so that data survives program restarts.

## Current Implementation Status

### Implemented

- Book management: add, display, search, update, delete, and sort.
- Member management: add, display, search, update, delete, and sort.
- Duplicate ID prevention.
- Book issuing and returning.
- Borrowing records maintained for each member.
- Per-book FIFO reservation queues.
- Automatic allocation of returned copies to eligible reserved members.
- Rollback logic to maintain consistency when operations fail.
- File persistence for books, members, borrowing records, and reservations.
- Loading records when the application starts.
- Separate files for individual book and member records.
- ID registry files for tracking stored records.
- Automated test harnesses for core workflows and file persistence.

### Remaining

- Final interactive application menu.
- Stronger input validation and stream failure recovery.
- Final integration testing and refinement.

## Project Structure

```text
CPP-PROJECT-LIBRARY-MANAGEMENT-SYSTEM/
│
├── main.cpp
│
├── book.h
├── book.cpp
│
├── member.h
├── member.cpp
│
├── library.h
├── library.cpp
│
├── filemanager.h
├── filemanager.cpp
│
├── Books/
│   └── .gitkeep
│
├── Members/
│   └── .gitkeep
│
├── .gitignore
└── README.md
```

### Responsibilities of each file

| File | Responsibility |
|---|---|
| `main.cpp` | Application entry point and current test harness |
| `book.h/.cpp` | Book class and book-related operations |
| `member.h/.cpp` | Member class and borrowing records |
| `library.h/.cpp` | Coordinates library operations and workflows |
| `filemanager.h/.cpp` | Handles saving, loading, and deleting persistent records |

The project uses separate header and implementation files to maintain modularity.

## Architecture

### 1. Book

The `Book` class represents a book record in the library.

**Data members**

- `book_id`: Unique identifier for the book.
- `title`: Book title.
- `author`: Author name.
- `total_copies`: Total physical copies owned.
- `available_copies`: Copies currently available for issuing.
- `reservation`: A `queue<int>` containing the IDs of members waiting for this book.

**Important operations**

| Function | Responsibility |
|---|---|
| `issueBook()` | Decreases available copies when a copy is available |
| `returnBook()` | Increases available copies when a copy is returned |
| `reserveMember(int id)` | Adds a member to the reservation queue |
| `getNextReservation()` | Retrieves and removes the next reservation |
| `getBookId() const` | Returns the book ID |
| `getTitle() const` | Returns the book title |
| `getReservationIds()` | Provides reservation IDs for persistence |
| `restoreReservation(int memberId)` | Restores a reservation during loading |

The reservation queue belongs to `Book` because each book has its own independent waiting list.

### 2. Member

The `Member` class represents a registered library member.

**Data members**

- `id`: Unique member identifier.
- `name`: Member name.
- `email`: Email address.
- `phone`: Phone number stored as a string.
- `borrowedBookIds`: A `vector<int>` containing IDs of currently borrowed books.

**Important operations**

| Function | Responsibility |
|---|---|
| `addBorrowedBook(int id)` | Adds a book to the member's borrowing records |
| `removeBook(int id)` | Removes a borrowed book from the member's records |
| `getMemberId() const` | Returns the member ID |
| `getBorrowedBookIds()` | Provides borrowed book IDs for persistence |
| `restoreBorrowedBook(int bookId)` | Restores a borrowing record during loading |

Phone numbers are stored as strings to preserve leading zeroes and country-code characters.

### 3. Library

The `Library` class coordinates the system and owns the main collections.

```cpp
vector<Book> books;
vector<Member> members;
FileManager fileManager;
```

**Book operations**

- Add, display, search, update, and delete books.
- Sort books by ID.
- Sort books alphabetically by title.

**Member operations**

- Add, display, search, update, and delete members.
- Sort members by ID.

**Borrowing operations**

- Issue books to members.
- Process book returns.
- Maintain borrowing records.
- Handle reservation requests.
- Allocate returned copies to the next eligible member.

**Persistence operations**

- Save updated book and member records.
- Delete corresponding files when records are deleted.
- Load saved records when the library is initialized.

The `Library` class acts as the coordinator: `Book` manages book-specific state, `Member` manages member-specific state, and `Library` coordinates operations involving both.

### 4. FileManager

The `FileManager` class is responsible for persistent storage.

It separates file operations from the business logic implemented in `Library`.

**Main operations**

| Function | Responsibility |
|---|---|
| `saveBook(const Book&)` | Saves a book record |
| `loadBook(int id)` | Loads a book using its ID |
| `saveBookIds(const vector<int>&)` | Saves the book ID registry |
| `loadBookIds()` | Reads the book ID registry |
| `deleteBookFile(int id)` | Deletes a book's data file |
| `saveMember(const Member&)` | Saves a member record |
| `loadMember(int id)` | Loads a member using their ID |
| `saveMemberIds(const vector<int>&)` | Saves the member ID registry |
| `loadMemberIds()` | Reads the member ID registry |
| `deleteMemberFile(int id)` | Deletes a member's data file |

## Important Workflows

### Book Issuing

1. Find the member using their ID.
2. Find the requested book using its ID.
3. Check whether the member already has that book.
4. Add the book ID to the member's borrowing records.
5. Decrease the book's available-copy count.
6. If no copy is available, roll back the temporary borrowing record.
7. Offer the member an opportunity to reserve the book.
8. Persist the affected records.

### Book Returning

1. Find the member and book.
2. Remove the book from the member's borrowing records.
3. Increase the book's available-copy count.
4. Process the reservation queue in FIFO order.
5. Skip deleted members and members who already have the book.
6. Allocate the returned copy to the first eligible member.
7. Persist the updated book and member records.

### Reservation Queue

Each book maintains its own queue of waiting member IDs.

The queue follows **FIFO (First In, First Out)**.

For example:

```text
Front -> 102 -> 103 -> 104 <- Back
```

When a copy is returned, member 102 is considered first. If that member is no longer eligible, the system checks the next reservation.

The queue stores member IDs rather than vector indexes because vector positions can change after sorting or deletion.

## File Persistence Design

The system uses individual text files for book and member records, along with separate ID registry files.

### Directory Structure

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

### Book Record Format

A book file stores:

1. Book ID
2. Title
3. Author
4. Total copies
5. Available copies
6. Reservation member IDs, one per line

### Member Record Format

A member file stores:

1. Member ID
2. Name
3. Email
4. Phone number
5. Borrowed book IDs, one per line

### Why ID Registry Files?

Instead of relying on directory iteration, the system maintains registry files containing the IDs of stored books and members.

During loading:

1. Read the relevant ID registry.
2. Retrieve each record using its ID.
3. Reconstruct the corresponding objects.
4. Restore borrowing records and reservation queues.
5. Skip missing or malformed records with appropriate messages.

This approach also avoids using `std::filesystem`, which is not supported by the older MinGW GCC 6.3.0 toolchain used during development.

## Design Decisions

- **Vectors:** Used for storing books and members because they provide straightforward iteration, searching, sorting, and deletion.
- **Queues:** Used for per-book reservations to maintain FIFO order.
- **Pointers:** Search functions can return pointers to stored objects, avoiding unnecessary object copies.
- **IDs:** Used as stable identifiers instead of relying on vector indexes.
- **Separate classes:** Each class has a defined responsibility.
- **FileManager:** Separates persistence logic from library operations.
- **Rollback logic:** Helps maintain consistency when a multi-step operation fails.
- **Boolean return values:** Used by operations to communicate success or failure.

## Testing

The project has used separate test harnesses to verify different parts of the system.

### Reservation and Borrowing Workflows

Earlier test harnesses covered:

- Successful borrowing and returning.
- Invalid book and member IDs.
- Duplicate borrowing and returning.
- Reservation requests.
- Duplicate reservation prevention.
- FIFO reservation allocation.
- Skipping deleted or ineligible members.
- Maintaining consistent borrowing records and available-copy counts.

### File Persistence

The persistence test harness completed with **11/11 tests passing**.

The tests covered:

- Saving books and members.
- Persisting borrowing records.
- Persisting reservation queues.
- Loading records into a new `Library` instance.
- Restoring borrowing and reservation data after restarting.
- Allocating a returned copy after loading.
- Verifying that updated records survive another restart.

These are development test-harness results, not a claim of a continuously running CI pipeline.

## Build and Run

The project was developed using the GCC/MinGW toolchain.

Compile all implementation files together:

```bash
g++ main.cpp book.cpp member.cpp library.cpp filemanager.cpp -o library
```

Run on Windows PowerShell:

```powershell
.\library.exe
```

The current `main.cpp` is still being used as a test harness. The final interactive menu and stronger input validation are remaining development tasks.

## Current Limitations

- Input validation and recovery from invalid stream input still need refinement.
- Constructor validation currently uses string literals for exceptions.
- The project is designed as a learning-oriented console application, not a production-grade library management system.
- Advanced features such as undo/redo, borrowing history, fines, due dates, and overdue tracking are intentionally outside the current base-project scope.

## Concepts Practised

- Classes, objects, constructors, and encapsulation.
- Header and source file separation.
- Multi-file compilation and linking.
- `std::vector` and `std::queue`.
- Pointers, references, and `nullptr`.
- Searching and sorting with `std::sort` and lambda comparators.
- FIFO queue operations.
- Object relationships and coordination.
- Console input using `cin`, `getline()`, and `cin.ignore()`.
- File handling using input and output file streams.
- Serialization and reconstruction of objects.
- Persistent storage and ID registries.
- Rollback logic and consistency across multiple objects.
- Test input/output redirection using string streams.

---

**Author:** Chetan Vashistha

**Repository:** [CPP Project – Library Management System](https://github.com/chetanvashistha1807-ship-it/CPP-PROJECT-LIBRARY-MANAGEMENT-SYSTEM)
