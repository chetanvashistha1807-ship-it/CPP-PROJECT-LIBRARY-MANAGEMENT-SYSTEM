# Library Management System (C++)

A console-based Library Management System built incrementally in C++ to practise Object-Oriented Programming, modular programming, STL containers, pointers, references, and data-structure-driven workflows.

The project currently models books, library members, borrowing records, and per-book FIFO reservation queues. The emphasis is on understanding why each component exists and how the operations work together.

## Current implementation status

Implemented:
- Book management: add, display, search, update, delete, and sort.
- Member management: add, display, search, update, delete, and sort.
- Book issuing and returning.
- Per-book reservation queues with automatic allocation when a copy is returned.
- Duplicate ID checks and duplicate-borrow / duplicate-reservation prevention.
- Test harness covering the borrowing and reservation workflows.

Not implemented yet:
- Undo / operation history.
- File persistence (records currently exist only in memory).
- A final interactive application menu and comprehensive input validation.
- Production-grade exception handling.

## Project structure

```text
CPP-PROJECT-LIBRARY-MANAGEMENT-SYSTEM/
├── main.cpp       # Current test harness
├── book.h         # Book class declaration
├── book.cpp       # Book class implementation
├── member.h       # Member class declaration
├── member.cpp     # Member class implementation
├── library.h      # Library class declaration
├── library.cpp    # Library operations and workflows
├── .gitignore
└── README.md
```

The project uses lowercase filenames. Include names in source files should match the actual filenames on disk.

## Architecture: how the classes work together

### 1. `Book`

A `Book` object represents one book title/record in the library, including the number of physical copies.

**Data members**
- `book_id`: unique identifier for the book record.
- `title`: book title.
- `author`: author name.
- `total_copies`: total physical copies owned by the library.
- `available_copies`: copies currently available to issue.
- `reservation`: `queue<int>` containing waiting member IDs for this particular book.

**Functions**
| Function | Purpose |
|---|---|
| `Book(int id, string Title, string Author, int Total_copies, int Available_copies)` | Constructs a book. Requires a positive ID, non-negative total copies, and available copies between zero and total copies. |
| `issueBook()` | Decreases `available_copies` by one if a copy is available; returns `true` on success and `false` otherwise. |
| `returnBook()` | Increases `available_copies` if it is below `total_copies`; returns whether the operation succeeded. |
| `displayBookInfo()` | Prints the book's ID, title, author, total copies, and available copies. |
| `getBookId() const` | Returns the book ID without modifying the object. |
| `getTitle() const` | Returns the title; used by title sorting. |
| `updateTitle(string s)` | Replaces the title. |
| `updateAuthor(string s)` | Replaces the author. |
| `reserveMember(int id)` | Adds a member ID to this book's reservation queue unless that ID is already queued. |
| `getNextReservation()` | Removes and returns the queue's front member ID; returns `-1` when the queue is empty. |

**Why the reservation queue belongs to Book:** reservations are for a particular book, not for the library as a whole. Keeping the queue inside each `Book` means every title maintains an independent waiting line.

### 2. `Member`

A `Member` object represents a registered library member.

**Data members**
- `id`: unique member identifier.
- `name`, `email`, `phone`: member details. Phone is stored as a `string` so leading zeroes and country-code characters can be preserved.
- `borrowedBookIds`: `vector<int>` containing IDs of books currently borrowed by this member.

**Functions**
| Function | Purpose |
|---|---|
| `Member(int Id, string Name, string Email, string Phone)` | Constructs a member and validates that the ID is positive. |
| `getMemberId() const` | Returns the member ID. |
| `getName() const`, `getEmail() const`, `getPhone() const` | Return the corresponding member details. |
| `updateName(string s)` | Updates the member's name. |
| `updateEmail(string s)` | Updates the member's email. |
| `updatePhone(string n)` | Updates the phone number. |
| `displayMemberInfo()` | Prints the member's details. |
| `addBorrowedBook(int id)` | Adds a book ID to the borrowing record. Rejects the ID if it is already present and returns a success flag. |
| `removeBook(int id)` | Removes a book ID from the borrowing record. Returns `false` if the member does not have that book recorded. |

**Borrowing policy:** one member may hold only one copy of a particular book ID at a time. This is enforced by `addBorrowedBook()`, even when the library owns multiple physical copies of that title.

### 3. `Library`

The `Library` class coordinates the system. It owns:
- `vector<Book> books`
- `vector<Member> members`

It is responsible for finding records, preventing duplicate IDs, managing CRUD operations, and coordinating the book/member state changes involved in issuing and returning.

**Book operations**
| Function | Purpose |
|---|---|
| `addBook(const Book& book)` | Adds a book unless another book has the same ID. A const reference avoids copying the argument just to pass it in and prevents this function from modifying the supplied object. |
| `displayBooks()` | Displays every stored book. |
| `searchBookById(int id)` | Searches by ID, displays the matching book, and returns a pointer to the stored object; returns `nullptr` if not found. |
| `updateBookInfo(int bookId)` | Finds a book and presents options to update its title or author. Uses `getline()` for multiword text. |
| `deleteBook()` | Reads a book ID, finds the matching vector element, and erases it. |
| `sortBooksById()` | Sorts books by ascending ID using `std::sort` and a lambda comparator. |
| `sortBooksByTitle()` | Sorts books alphabetically by title using `std::sort` and a lambda comparator. |

**Member operations**
| Function | Purpose |
|---|---|
| `addMember(const Member& member)` | Adds a member unless the member ID already exists. |
| `displayMembers()` | Displays all stored members. |
| `searchMemberById(int id)` | Returns a pointer to the matching stored member, or `nullptr` if no match exists. |
| `updateMemberInfo(int id)` | Presents options to update name, email, or phone number. |
| `deleteMember()` | Reads a member ID and erases the matching member. |
| `sortMembersById()` | Sorts members by ascending ID using `std::sort`. |

**Borrowing operations**
| Function | Purpose |
|---|---|
| `issuebook()` | Reads member and book IDs, validates both records, records the borrowing, and decreases the book's available-copy count. If no copy is available, it rolls back the temporary borrowing record and offers a reservation. |
| `returnbook()` | Validates the member and book, removes the borrowing record, increases availability, and then attempts to allocate the returned copy to the next eligible reservation. |

## Important workflows

### Add, search, update, and delete

1. A `Book` or `Member` object is constructed with its data.
2. `Library::addBook()` or `Library::addMember()` checks for a duplicate ID before storing it in the relevant vector.
3. Display functions iterate through the vector and call the object's display function.
4. Search functions compare the requested ID with each object's ID.
5. Update functions locate the object and call its update methods.
6. Delete functions locate the matching vector element and erase it.

IDs are used to identify records. A member ID is not the same thing as a member's position (index) in the `members` vector. The vector can be sorted or elements can be erased, so code must search by ID rather than assume that an ID is a vector index.

### Issue a book

The current `Library::issuebook()` workflow is:

1. Read the member ID and search the members vector.
2. If the member does not exist, stop.
3. Read the book ID and search the books vector.
4. If the book does not exist, stop.
5. Call `Member::addBorrowedBook(bookId)`. If the member already has this book recorded, stop.
6. Call `Book::issueBook()`.
7. If a copy is available, its count decreases and the issue succeeds.
8. If no copy is available, remove the temporary borrowing record with `Member::removeBook()`. This rollback is important: a failed issue must not leave the member marked as borrowing a book they did not receive.
9. Ask whether the member wants to reserve the book. If yes, call `Book::reserveMember(memberId)`.

### Reserve a book: FIFO queue

Each book has its own `queue<int>` of waiting member IDs.

- A queue follows **FIFO (First In, First Out)**: the earliest reservation is considered first.
- `reserveMember()` copies the queue into a temporary queue to check whether the member ID is already present. It searches the temporary copy so the real queue remains unchanged. If no duplicate exists, the ID is pushed onto the real queue.
- `getNextReservation()` reads and removes the front ID. It returns `-1` when no reservation exists.

Example: if members 102, 103, and 104 reserve the same unavailable book in that order, the queue is:

```text
Front -> 102, 103, 104 <- Back
```

When a copy is returned, member 102 is considered first. If eligible, they receive it; otherwise the system moves on to 103, then 104.

### Return a book and allocate it to the next eligible member

The current `Library::returnbook()` workflow is:

1. Read the member ID and find that member.
2. Read the book ID and find that book.
3. Call `Member::removeBook(bookId)`. If the member does not have this book recorded, reject the return.
4. Call `Book::returnBook()` to increase the available-copy count.
5. If the book-level return fails, restore the member's borrowing record with `addBorrowedBook()` and stop. This is another rollback to keep the two records consistent.
6. Repeatedly call `getNextReservation()` while checking the queue:
   - If it returns `-1`, no reservations remain. The returned copy stays available.
   - Search the members vector for the reserved member ID. If that member has been deleted, skip the stale reservation.
   - Try `addBorrowedBook(bookId)`. If it fails because that member already has the book recorded, skip that reservation too.
   - For the first eligible member, call `Book::issueBook()` to allocate the returned copy (availability decreases again), report the allocation, and stop.
7. If every queued reservation is stale or ineligible, the copy remains available for a normal issue.

The queue stores member IDs rather than vector indexes. This matters because vector positions can change after sorting or deletion, whereas the member ID is the record's identifier.

## Design decisions and the reasoning behind them

- **Vectors for books and members:** they provide straightforward storage, iteration, searching, sorting, and erasing for this project's current scale.
- **Unique IDs:** IDs provide stable record identity and prevent duplicate book/member records.
- **Pointers from search functions:** returning a pointer lets a caller access the stored object without making a separate copy. A returned pointer is only safe to use while the corresponding vector element remains valid; operations that erase or rearrange vector elements can invalidate pointers.
- **IDs in borrowing records:** a member stores the IDs of books they hold, rather than duplicate Book objects.
- **One-copy-per-member rule:** `addBorrowedBook()` centralizes the duplicate-borrow check.
- **One queue per book:** each title has an independent waiting line.
- **FIFO reservation policy:** members are considered in the order they reserved.
- **Rollback on failed operations:** if one side of an operation changes but the corresponding book/member operation fails, the code attempts to restore the earlier state.
- **Boolean results:** methods such as `issueBook()`, `returnBook()`, `addBorrowedBook()`, and `removeBook()` communicate whether an operation succeeded.
- **Library as coordinator:** Book manages its own copy count and reservation queue; Member manages its own borrowing records; Library coordinates operations involving both.

## Testing

The current `main.cpp` is a test harness rather than the final user-facing menu. It uses:
- `istringstream` to provide simulated keyboard input.
- `ostringstream` to capture console output.
- `runTest()` to temporarily redirect `cin` and `cout` while an operation runs.
- `check()` to report pass/fail results.

The reservation test suite has been run with **9/9 tests passing**. It covers:
1. Successful reservation.
2. Duplicate reservation rejection.
3. First reservation receiving the returned copy.
4. Second reservation receiving the next returned copy.
5. Deleted reserved member being skipped.
6. Return when nobody is waiting, followed by a successful normal issue.
7. A reserved member who already has the book being skipped.
8. All deleted/ineligible reservations being skipped while the returned copy remains available.
9. Automatic allocation updating borrowing records and availability so the recipient can return the book.

The issue/return workflow was also tested separately with **12/12 reported tests passing**, covering successful operations, invalid IDs, duplicate borrowing/return, unavailable copies, and rollback behaviour.

These are manual test-harness results from the development process; they are not a claim of a continuously running CI test pipeline.

## Build and run

The repository currently targets the GCC/MinGW toolchain used during development.

Compile all implementation files together:

```bash
g++ main.cpp book.cpp library.cpp member.cpp -o library
```

Run on Windows PowerShell:

```powershell
.\library.exe
```

The current `main.cpp` runs the test suite. As the project develops, this entry point is expected to evolve into the final application menu.

## Current limitations and next steps

- All books, members, borrowing records, and reservations are stored in memory. They are lost when the program exits.
- Input handling is currently basic; invalid non-numeric input and stream failure recovery still need work.
- Constructor validation currently throws string literals. Standard exception types and consistent exception handling have not yet been introduced.
- Reservation entries are removed when processed. If a member has been deleted, their queued ID is skipped when the book is returned.
- The current implementation is a learning project, not a production-ready library system.

Planned development:
1. Undo / operation history.
2. File persistence so data survives program restarts.
3. A final interactive menu and stronger input validation.
4. Integration testing and refinement.

## Concepts practised so far

- Classes, objects, constructors, encapsulation, getters, and update methods.
- Header/source separation and multi-file compilation.
- `std::vector`, `std::queue`, iteration, searching, and erasing.
- Pointers, references, `nullptr`, and object access through returned pointers.
- `std::sort` and lambda comparators.
- FIFO queue behaviour and state coordination across objects.
- Console input with `cin`, `getline()`, and `cin.ignore()`.
- Test input/output redirection with string streams.
- Rollback logic to preserve consistency after failed operations.

---

**Author:** Chetan Vashistha

**Repository:** [CPP Project – Library Management System](https://github.com/chetanvashistha1807-ship-it/CPP-PROJECT-LIBRARY-MANAGEMENT-SYSTEM)
