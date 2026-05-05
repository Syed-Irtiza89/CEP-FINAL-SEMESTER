================================================================================
          LIBRARY AND RESOURCE MANAGEMENT SYSTEM - CEP SPRING 2025
================================================================================

------------------------------------------------------------------------
1. GROUP MEMBERS
------------------------------------------------------------------------

Student 1:
  Full Name   : Syed Muhammad Irtiza Abbas Rizvi
  Roll Number : CS-25089
  Section     : B

Student 2:
  Full Name   : Muhib Ur Rub Qasmi
  Roll Number : CS-25086
  Section     : B

Student 3:
  Full Name   : Syed Zayan Mehmood
  Roll Number : CS-25090
  Section     : B

Student 4:
  Full Name   : Muhammad Hammad Ul Saboor
  Roll Number : CS-25080
  Section     : B

------------------------------------------------------------------------
2. HOW TO RUN THE PROJECT
------------------------------------------------------------------------

Step 1: Open your C++ compiler ( VS Code)

Step 2: Open the folder where you saved "Library.cpp".

Step 3: Compile the code ( use the terminal).
If using terminal, type: g++ Library.cpp -o Library.exe

Step 4: Run the file. If using terminal, type: .\Library.exe

Step 5: Follow the menu on the screen. Type a number and press Enter.

 Admin Login (Default):
 Username: admin
 Password: admin123

Note: If you are a new user, register first (Option 1).

IMPORTANT: Keep the "users.txt", "resources.txt", and "records.txt"
files in the same folder as the code. This is where the program saves all your data.


------------------------------------------------------------------------
3. OOP CONCEPTS USED - WHERE AND HOW
------------------------------------------------------------------------

1. CLASS AND OBJECTS
   We made 5 classes: Resource, BorrowRecord, Person, User, Admin,
   and library. Each class has its own private data and public functions.
   For example, the Resource class stores the book's id, title, author,
   category and availability. Objects of these classes are created inside
   the library class to manage everything together.

2. INHERITANCE
   We used inheritance between Person (base class) and User and Admin
   (derived classes). Person holds the common data like username,
   firstName, lastName, password. User adds address and balance.
   Admin does not add extra data but overrides the display function.
   This shows "is-a" relationship: a User IS A Person.

3. ENCAPSULATION
   All data members in every class are private. For example, in the
   User class, balance is private and can only be accessed through
   getBalance(), setBalance(), addBalance(), deductBalance() functions.
   This protects the data from being changed accidentally from outside.

4. ABSTRACTION (Abstract Class)
   The Person class has a pure virtual function:
       virtual void display() = 0;
   This makes Person an abstract class. You cannot create a Person
   object directly. Only User and Admin can be created because they
   override and implement the display() function. This hides internal
   details and forces proper usage.

5. POLYMORPHISM (Function Overriding)
   The display() function is defined differently in User and Admin.
   When we call display() on a User object it shows user details
   (username, balance, address). When we call it on Admin it shows
   admin details. This is runtime polymorphism through virtual functions.

6. OPERATOR OVERLOADING
   In the Resource class, we overloaded two operators:
   - "==" operator: compares two resources by their ID. Used in
     removeResource() to find the matching resource in the vector.
   - "<<" operator: prints resource details using cout directly.
   This makes the code cleaner and easier to read.

7. EXCEPTION HANDLING
   We made custom exception classes like:
   - ResourceNotAvailableException
   - InsufficientBalanceException
   - LimitExceededException
   - RecordNotFoundException
   - InvalidDateException
   All inherit from LibraryException which inherits from std::exception.
   In borrowResource() and returnResource() we use throw to raise errors
   and catch them in the menu functions using try-catch blocks. This
   stops the program from crashing on invalid input.

8. FILE HANDLING
   We used ifstream and ofstream to save and load all data.
   - resources.txt stores all books/resources.
   - users.txt stores all registered users and their balance.
   - records.txt stores all borrow and return transactions.
   The saveAll() and loadAll() functions in the library class handle
   this. Data is loaded when the program starts and saved after every
   change (register, borrow, return, update).

------------------------------------------------------------------------
4. WHAT WORKS AND WHAT DOES NOT WORK
------------------------------------------------------------------------

WHAT WORKS:
- User registration with full input validation (no empty fields,
  names must be alphabets only, minimum balance Rs50).
- User login and Admin login.
- Borrowing a resource with all constraints:
    * Max 2 active borrows at a time.
    * Cannot borrow the same book twice at once.
    * Cannot borrow if balance is negative.
- Returning a resource with automatic late fine calculation.
  Fine is Rs10 per day after the 14-day due date.
  Balance is deducted automatically.
- Searching resources by title, author, or category.
- Admin can add, remove, and update resources.
- Saving and loading data from files works correctly.
- Custom exceptions prevent crashes from bad input.

WHAT DOES NOT WORK / LIMITATIONS:
- Passwords are visible on screen when you type them (no masking).
- Admin credentials are hardcoded (admin / admin123). Cannot be
  changed from the menu.
- All classes are in one file (Library.cpp) instead of separate
  .h and .cpp files. We did this to make compilation easier.
- No GUI. Everything is text-based in the console only.
- Data files are plain text and not encrypted. Anyone can open
  users.txt and manually edit balances.
- No "reserve book" feature. You can only borrow if it is available.
- The system does not support multiple users at the same time.

------------------------------------------------------------------------
5. RESOURCES WE USED
------------------------------------------------------------------------

- cplusplus.com - for understanding vectors, file streams (fstream),
  and operator overloading syntax.
- GeeksforGeeks - for learning about abstract classes, virtual
  functions, and custom exception classes.
- Stack Overflow - for fixing specific bugs like getline() not
  reading properly after cin >>.
- Our course slides and lecture notes from the instructor.