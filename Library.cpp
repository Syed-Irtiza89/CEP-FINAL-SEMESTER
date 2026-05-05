// Standard library headers needed for this project
#include <iostream>   // For input/output (cin, cout)
#include <cstring>    // For string operations
#include <vector>     // For using dynamic lists (vector)
#include <cctype>     // For character checks (isalpha, tolower)
#include <iomanip>    // For formatting output (setprecision)
#include <exception>  // For exception handling
#include <fstream>    // For reading/writing files
#include <sstream>    // For string stream operations
#include <ctime>      // For getting today's date and time
#include <string>     // For using string type
#include <windows.h>  // For Windows console color control
using namespace std;

// GUI Helper Functions for Beginner Level
void clearScreen() {
    system("cls");
}
void setWhiteColor() {
    system("color 07"); // Black background, White text
}

void setNormalColor() {
    system("color 07"); // Black background, White text
}

void setRedColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // Light Red text
}

void printHeader(string title) {
    cout << "***************************************************" << endl;
    cout << "          " << title << endl;
    cout << "***************************************************" << endl;
}

// Custom exception classes for handling library errors
class LibraryException : public exception
{
protected:
    string message;

public:
    LibraryException(string msg) : message(msg) {}

    virtual const char *what() const throw()
    {
        return message.c_str();
    }
};

class ResourceNotAvailableException : public LibraryException
{
public:
    ResourceNotAvailableException() : LibraryException("Error: Resource is currently not available for borrowing!") {}
};

class InsufficientBalanceException : public LibraryException
{
public:
    InsufficientBalanceException() : LibraryException("Error: Insufficient account balance to pay the fine!") {}
};

class LimitExceededException : public LibraryException
{
public:
    LimitExceededException() : LibraryException("Error: You have reached the maximum borrow limit (Max 2)!") {}
};

class RecordNotFoundException : public LibraryException
{
public:
    RecordNotFoundException() : LibraryException("Error: No active record found for this resource ID!") {}
};

class InvalidDateException : public LibraryException
{
public:
    InvalidDateException() : LibraryException("Error: Invalid date entered!") {}
};

//=============================Helper Function==============================
bool isValidInput(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] != ' ') // if any character is NOT space
        {
            return true; // valid input
        }
    }
    return false; // only spaces OR empty
}
// Converts a string to all lowercase letters (used for case-insensitive comparisons)
string toLower(string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        s[i] = tolower(s[i]);
    }
    return s;
}

// Returns true if 'word' is found anywhere inside 'text'
bool contains(string text, string word)
{
    if (text.find(word) != string::npos)
    {
        return true;
    }
    return false;
}
// Returns true if the string contains only letters and spaces (no numbers or symbols)
bool isAlphaString(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (!isalpha(s[i]) && s[i] != ' ')
        {
            return false;
        }
    }
    return true;
}
// Converts a date stored as an integer (e.g. 20260501) to a readable format (e.g. 01-May-2026)
string dateToString(int date)
{
    if (date == 0)
        return "N/A";
    int year = date / 10000;
    int month = (date / 100) % 100;
    int day = date % 100;
    string months[] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                       "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    string result = "";
    if (day < 10)
        result += "0";
    result += to_string(day) + "-";
    if (month >= 1 && month <= 12)
        result += months[month];
    else
        result += "???";
    result += "-" + to_string(year);
    return result;
}
// Calculates how many days are between two dates (date2 - date1)
// Returns a positive number if date2 is later, negative if earlier
int daysBetween(int date1, int date2)
{
    if (date1 == 0 || date2 == 0)
        return 0;
    struct tm t1 = {0};
    t1.tm_year = (date1 / 10000) - 1900;
    t1.tm_mon = ((date1 / 100) % 100) - 1;
    t1.tm_mday = date1 % 100;
    struct tm t2 = {0};
    t2.tm_year = (date2 / 10000) - 1900;
    t2.tm_mon = ((date2 / 100) % 100) - 1;
    t2.tm_mday = date2 % 100;
    time_t time1 = mktime(&t1);
    time_t time2 = mktime(&t2);
    double diff = difftime(time2, time1);
    return (int)(diff / (60 * 60 * 24));
}
// Returns today's date as an integer in YYYYMMDD format (e.g. 20260501)
int getTodayDate()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;
    return year * 10000 + month * 100 + day;
}
// Adds a given number of days to a date and returns the new date
// Handles month/year rollovers automatically using mktime
int addDays(int date, int days)
{
    struct tm t = {0};
    t.tm_year = (date / 10000) - 1900;
    t.tm_mon = ((date / 100) % 100) - 1;
    t.tm_mday = date % 100 + days;

    mktime(&t); // auto normalizes date

    return (t.tm_year + 1900) * 10000 +
           (t.tm_mon + 1) * 100 +
           t.tm_mday;
}
// Represents a library resource (book/item)
class Resource
{
private:
    int id;           // Unique ID for each resource
    string title;     // Title of the book/magazine
    string author;    // Author name
    string category;  // Category (e.g. Fiction, Science)
    bool available;   // true = on shelf, false = borrowed

public:
    // Default constructor – sets all fields to empty/default values
    Resource() : id(0), title(""), author(""), category(""), available(true) {}

    // Parameterized constructor – creates a resource with given details
    Resource(int i, string t, string a, string c, bool avail) : id(i), title(t), author(a), category(c), available(avail) {}

    int getId()
    {
        return id;
    }

    string getTitle()
    {
        return title;
    }

    string getAuthor()
    {
        return author;
    }

    string getCategory()
    {
        return category;
    }

    bool getAvailable()
    {
        return available;
    }

    void display()
    {
        cout << "ID: " << id << endl;
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "Category: " << category << endl;
        cout << "Available: " << (available ? "Yes" : "No") << endl;
    }

    bool isAvailable()
    {
        return available;
    }

    void setAvailable(bool avail)
    {
        available = avail;
    }

    // Overload == to compare resource IDs
    bool operator==(const Resource &other)
    {
        return id == other.id; // Two resources are "equal" if they have the same ID
    }

    // Overload << to print resource details
    friend ostream &operator<<(ostream &os, const Resource &r)
    {
        os << "ID: " << r.id << " | Title: " << r.title
           << " | Author: " << r.author
           << " | Category: " << r.category
           << " | Available: " << (r.available ? "Yes" : "No");
        return os;
    }
};

// Tracks borrowing transactions and fines
class BorrowRecord
{
private:
    int recordId;          // Unique ID for each borrow record
    int resourceId;        // ID of the borrowed resource
    string username;       // Username of the borrower
    string resourceTitle;  // Title of the borrowed resource (stored for display)
    int issueDate;         // Date the resource was borrowed
    int returnDate;        // Date it was returned (0 if not yet returned)
    int dueDate;           // Deadline to return without a fine
    double fineAmount;     // Fine charged if returned late

public:
    BorrowRecord() : recordId(0), resourceId(0), username(""), resourceTitle(""), issueDate(0), returnDate(0), dueDate(0), fineAmount(0.0) {}

    BorrowRecord(int rId, int resId, string user, string resTitle, int issue, int ret, int due, double fine)
        : recordId(rId), resourceId(resId), username(user), resourceTitle(resTitle),
          issueDate(issue), returnDate(ret), dueDate(due), fineAmount(fine) {}
    // getter
    int getRecordId()
    {
        return recordId;
    }

    int getResourceId()
    {
        return resourceId;
    }

    string getUsername()
    {
        return username;
    }

    string getResourceTitle()
    {
        return resourceTitle;
    }

    int getIssueDate()
    {
        return issueDate;
    }

    int getReturnDate()
    {
        return returnDate;
    }

    int getDueDate()
    {
        return dueDate;
    }

    double getFineAmount()
    {
        return fineAmount;
    }

    void display() const
    {
        cout << "  Record #" << recordId << " | Resource: " << resourceTitle << " (ID: " << resourceId << ")"
             << " | Issued: " << dateToString(issueDate) << " | Due: " << dateToString(dueDate);
        if (returnDate != 0)
        {
            cout << " | Returned: " << dateToString(returnDate);
            if (fineAmount > 0)
                cout << " | Fine: Rs." << fixed << setprecision(2) << fineAmount;
        }
        else
        {
            cout << " | Status: " << (isOverdue() ? "OVERDUE" : "Active");
        }
        cout << endl;
    }

    // inside BorrowRecord class
    double returnResource(int retDate, double finePerDay)
    {
        returnDate = retDate;
        fineAmount = 0.0;
        int daysLate = daysBetween(dueDate, returnDate);
        if (daysLate > 0)
            fineAmount = daysLate * finePerDay;
        return fineAmount;
    }
    // Returns true if the resource has NOT been returned yet
    bool isActive() const { return returnDate == 0; }

    // Returns true if the resource is still borrowed AND past its due date
    bool isOverdue() const
    {
        if (!isActive())
            return false;

        return daysBetween(dueDate, getTodayDate()) > 0;
    }
};

// Base abstract class for any person in the system (User or Admin)
// Uses inheritance: both User and Admin extend this class
class Person
{
protected:
    // Common info shared by both User and Admin
    string username;   // Login name
    string firstName;  // First name
    string lastName;   // Last name
    string password;   // Login password

public:
    Person() : username(""), firstName(""), lastName(""), password("") {}

    Person(string user, string first, string last, string pass) : username(user), firstName(first), lastName(last), password(pass) {}

    // getter
    string getUsername()
    {
        return username;
    }

    string getFirstName()
    {
        return firstName;
    }

    string getLastName()
    {
        return lastName;
    }

    string getPassword()
    {
        return password;
    }

    virtual void display() = 0; // pure virtual function to make Person an abstract class

    virtual ~Person() {} // virtual destructor
};

// Represents a library customer (inherits from Person)
// Has extra fields for address and wallet balance
class User : public Person
{
private:
    string address;  // Home address of the user
    double balance;  // Wallet balance (used to pay fines)

public:
    User() : Person(), address(""), balance(0.0) {}

    User(string user, string first, string last, string pass, string addr, double bal) : Person(user, first, last, pass), address(addr), balance(bal) {}

    // Getters
    string getAddress()
    {
        return address;
    }

    double getBalance()
    {
        return balance;
    }

    // Setters
    void setAddress(string addr)
    {
        address = addr;
    }

    void setBalance(double bal)
    {
        balance = bal;
    }

    // Adds money to the user's balance (e.g. when they recharge)
    void addBalance(double amount)
    {
        balance += amount;
    }

    // Deducts fine from the user's balance
    // Balance can go negative if the fine is more than what they have
    void deductBalance(double amount)
    {
        balance -= amount;
    }

    void display() override
    {
        cout << "Username: " << username << endl;
        cout << "First Name: " << firstName << endl;
        cout << "Last Name: " << lastName << endl;
        cout << "Address: " << address << endl;
        cout << "Balance: Rs" << fixed << setprecision(2) << balance << endl;
    }
};

// Represents an Administrator (inherits from Person)

class Admin : public Person
{
public:
    Admin() : Person() {}

    Admin(string user, string first, string last, string pass) : Person(user, first, last, pass) {}

    void display() override
    {
        cout << "Admin Details: " << endl;
        cout << "Username: " << username << endl;
        cout << "First Name: " << firstName << endl;
        cout << "Last Name: " << lastName << endl;
    }
};

// Core Library Class: Manages all collections and business logic
// This class holds all the data and handles all operations in the system
class library
{
private:
    vector<Resource> resources;        // List of all books/resources in the library
    vector<BorrowRecord> borrowRecords; // List of all borrowing transactions
    vector<User> users;                // List of all registered users
    Admin admin;                       // Single admin account
    int nextResourceId;                // Auto-increment ID for new resources
    int nextRecordId;                  // Auto-increment ID for new borrow records
    double finePerDay;                 // Fine charged per day for late returns (Rs10)
    int maxBorrowDays;                 // Maximum days a user can keep a resource (14 days)

public:
    library() : nextResourceId(1), nextRecordId(1), finePerDay(10.0), maxBorrowDays(14)
    {
        admin = Admin("admin", "Library", "Administrator", "admin123");
        loadAll(); // Load data from files when the system starts
    }

    // File Handling Methods

    void saveAll()
    {
        // 1. Save Resources
        ofstream resFile("resources.txt");
        for (int i = 0; i < resources.size(); i++)
        {
            resFile << resources[i].getId() << "\n"
                    << resources[i].getTitle() << "\n"
                    << resources[i].getAuthor() << "\n"
                    << resources[i].getCategory() << "\n"
                    << resources[i].getAvailable() << "\n";
        }
        resFile.close();

        // 2. Save Users
        ofstream userFile("users.txt");
        for (int i = 0; i < users.size(); i++)
        {
            userFile << users[i].getUsername() << "\n"
                     << users[i].getPassword() << "\n"
                     << users[i].getFirstName() << "\n"
                     << users[i].getLastName() << "\n"
                     << users[i].getAddress() << "\n"
                     << users[i].getBalance() << "\n";
        }
        userFile.close();

        // 3. Save Borrow Records
        ofstream recFile("records.txt");
        for (int i = 0; i < borrowRecords.size(); i++)
        {
            recFile << borrowRecords[i].getRecordId() << "\n"
                    << borrowRecords[i].getResourceId() << "\n"
                    << borrowRecords[i].getUsername() << "\n"
                    << borrowRecords[i].getResourceTitle() << "\n"
                    << borrowRecords[i].getIssueDate() << "\n"
                    << borrowRecords[i].getReturnDate() << "\n"
                    << borrowRecords[i].getDueDate() << "\n"
                    << borrowRecords[i].getFineAmount() << "\n";
        }
        recFile.close();
    }
    void loadAll()
    {
        resources.clear();
        users.clear();
        borrowRecords.clear();

        // ==================== 1. LOAD RESOURCES ====================
        ifstream resFile("resources.txt");

        if (!resFile)
        {
            cout << "resources.txt not found. Starting empty.\n";
        }
        else
        {
            int id;
            string title, author, cat;
            bool avail;

            while (resFile >> id)
            {
                resFile.ignore();
                getline(resFile, title);
                getline(resFile, author);
                getline(resFile, cat);
                resFile >> avail;
                resFile.ignore();

                resources.push_back(Resource(id, title, author, cat, avail));

                if (id >= nextResourceId)
                    nextResourceId = id + 1;
            }
            resFile.close();
        }

        // ==================== 2. LOAD USERS ====================
        ifstream userFile("users.txt");

        if (!userFile)
        {
            cout << "users.txt not found.\n";
        }
        else
        {
            string user, pass, first, last, addr;
            double bal;

            while (getline(userFile, user))
            {
                if (user.empty())
                    continue;

                getline(userFile, pass);
                getline(userFile, first);
                getline(userFile, last);
                getline(userFile, addr);
                userFile >> bal;
                userFile.ignore();

                users.push_back(User(user, first, last, pass, addr, bal));
            }
            resFile.close();
        }

        // ==================== 3. LOAD RECORDS ====================
        ifstream recFile("records.txt");

        if (!recFile)
        {
            cout << "records.txt not found.\n";
        }
        else
        {
            int rId, resId, issue, ret, due;
            double fine;
            string rUser, rTitle;

            while (recFile >> rId)
            {
                recFile >> resId;
                recFile.ignore();
                getline(recFile, rUser);
                getline(recFile, rTitle);
                recFile >> issue >> ret >> due >> fine;
                recFile.ignore();

                borrowRecords.push_back(
                    BorrowRecord(rId, resId, rUser, rTitle, issue, ret, due, fine));

                if (rId >= nextRecordId)
                    nextRecordId = rId + 1;
            }
            recFile.close();
        }
    }
    // =======================================================================

    bool registerUser(const string &username, const string &password, const string &firstName,
                      const string &lastName, const string &address, double balance)
    {
        // FIX #1: Convert username to lowercase for case-insensitive comparison
        string lowerUsername = toLower(username);

        if (!isValidInput(lowerUsername) || !isValidInput(password) ||
            !isValidInput(firstName) || !isValidInput(lastName) ||
            !isValidInput(address))
        {
            cout << "All fields must be valid (no empty or spaces only)!\n";
            return false;
        }
        if (!isAlphaString(firstName) || !isAlphaString(lastName))
        {
            cout << "Name must contain only alphabets!\n";
            return false;
        }
        if (balance < 50)
        {
            cout << "Error: Minimum balance is Rs50!\n";
            return false;
        }
        if (lowerUsername.empty() || password.empty() || firstName.empty() || lastName.empty() || balance < 0)
        {
            return false;
        }

        // FIX #2: Check for duplicate username using case-insensitive comparison
        for (int i = 0; i < users.size(); i++)
        {
            if (toLower(users[i].getUsername()) == lowerUsername)
            {
                cout << "Username already exists!\n";
                return false;
            }
        }

        users.push_back(User(lowerUsername, firstName, lastName, password, address, balance));
        saveAll(); // Save after registration
        return true;
    }

    int loginUser(const string &username, const string &password)
    {
        // FIX #3: Convert input username to lowercase for case-insensitive login
        string lowerUsername = toLower(username);

        for (int i = 0; i < users.size(); i++)
        {
            if (toLower(users[i].getUsername()) == lowerUsername &&
                users[i].getPassword() == password)
            {
                return i;
            }
        }
        return -1;
    }

    bool loginAdmin(const string &username, const string &password)
    {
        return (admin.getUsername() == username && admin.getPassword() == password); // Admin login check
    }

    // Core Logic: Handles the borrowing process with 10-step validation.
    // EXCEPTION HANDLING: Using throw instead of return false.
    // If any check fails, the function "throws" an error and stops immediately.
    void borrowResource(int userIndex, int resourceId, int currentDate)
    {
        // Step 1: Validate if the user exists in our session
        if (userIndex < 0 || userIndex >= users.size())
        {
            throw LibraryException("Invalid User Index Mapping!");
        }

        // Step 2: Ensure the date is a positive number
        if (currentDate <= 0)
        {
            throw InvalidDateException();
        }

        // Step 2.5: Block borrowing if the user owes fines (negative balance)
        if (users[userIndex].getBalance() < 0)
        {
            throw InsufficientBalanceException();
        }

        string username = users[userIndex].getUsername();

        // Step 3: Check if the user is trying to borrow the SAME resource twice
        for (int i = 0; i < borrowRecords.size(); i++)
        {
            if (borrowRecords[i].getUsername() == username &&
                borrowRecords[i].getResourceId() == resourceId &&
                borrowRecords[i].isActive())
            {
                throw LibraryException("Error: You have already borrowed this resource!");
            }
        }

        // Step 4: Check Total Active Borrows (Constraint: Max 2 at a time)
        int activeCount = 0;
        for (int i = 0; i < borrowRecords.size(); i++)
        {
            if (borrowRecords[i].getUsername() == username &&
                borrowRecords[i].isActive())
            {
                activeCount++;
            }
        }

        if (activeCount >= 2)
        {
            throw LimitExceededException();
        }

        // Step 5: Check Daily Limit (Constraint: Max 2 on the same day)
        int todayCount = 0;
        for (int i = 0; i < borrowRecords.size(); i++)
        {
            if (borrowRecords[i].getUsername() == username &&
                borrowRecords[i].getIssueDate() == currentDate)
            {
                todayCount++;
            }
        }

        if (todayCount >= 2)
        {
            throw LibraryException("Error: You cannot borrow more than 2 resources in one single day!");
        }

        // Step 6: Search for the resource in the library database
        bool found = false;

        for (int i = 0; i < resources.size(); i++)
        {
            if (resources[i].getId() == resourceId)
            {
                found = true;

                // Step 7: Check if the book is actually on the shelf (Available)
                if (!resources[i].isAvailable())
                {
                    throw ResourceNotAvailableException();
                }

                // Step 8: Calculate Due Date (Issue Date + 14 Days)
                int dueDate = addDays(currentDate, maxBorrowDays);

                // Step 9: Create and store the Borrow Record
                borrowRecords.push_back(
                    BorrowRecord(nextRecordId++,
                                 resourceId,
                                 username,
                                 resources[i].getTitle(),
                                 currentDate,
                                 0,
                                 dueDate,
                                 0.0)); // fine initially 0

                // Step 10: Update the shelf (Mark as Unavailable)
                resources[i].setAvailable(false);

                saveAll(); // Save after borrowing
                cout << "Borrowed successfully! Due date: " << dueDate << endl;
                return;
            }
        }

        // Error: Resource not found in the system
        if (!found)
        {
            throw LibraryException("Error: Invalid Resource ID provided!");
        }
    }

    // Handles the return of a borrowed resource
    // Calculates any fine if returned late and deducts it from the user's balance
    void returnResource(int userIndex, int resourceId, int currentDate)
    {
        // Make sure the user index is valid
        if (userIndex < 0 || userIndex >= users.size())
        {
            throw LibraryException("Invalid User Index Mapping!");
        }

        // Make sure the date provided is valid
        if (currentDate <= 0)
        {
            throw InvalidDateException();
        }

        bool foundRecord = false;

        for (int i = 0; i < borrowRecords.size(); i++)
        {
            if (borrowRecords[i].getResourceId() == resourceId &&
                borrowRecords[i].getUsername() == users[userIndex].getUsername() &&
                borrowRecords[i].isActive())
            {
                if (currentDate < borrowRecords[i].getIssueDate())
                {
                    throw LibraryException("Error: Return date cannot be before issue date!");
                }

                double fine = 0;

                int daysLate = daysBetween(borrowRecords[i].getDueDate(), currentDate);

                if (daysLate > 0)
                {
                    fine = daysLate * finePerDay;
                }

                if (fine > 0)
                {
                    cout << "Overdue Fine Detected: Rs" << fixed << setprecision(2) << fine << endl;
                    users[userIndex].deductBalance(fine);
                    
                    if (users[userIndex].getBalance() < 0)
                    {
                        setRedColor();
                        cout << "Warning: Account balance is now negative. You cannot borrow new books until you recharge!" << endl;
                        setWhiteColor();
                    }
                }

                borrowRecords[i].returnResource(currentDate, finePerDay);
                foundRecord = true;

                for (int j = 0; j < resources.size(); j++)
                {
                    if (resources[j].getId() == resourceId)
                    {
                        resources[j].setAvailable(true);
                        break;
                    }
                }

                saveAll(); // Save after returning
                cout << "Resource returned successfully!" << endl;
                return;
            }
        }

        if (!foundRecord)
        {
            throw RecordNotFoundException();
        }
    }

    // Searches all resources by title, author, or category
    // The search is case-insensitive (e.g. "FICTION" matches "fiction")
    void searchResources(const string &keyword)
    {
        bool found = false;
        string key = toLower(keyword); // Convert search word to lowercase for comparison

        for (int i = 0; i < resources.size(); i++)
        {
            string title = toLower(resources[i].getTitle());
            string author = toLower(resources[i].getAuthor());
            string category = toLower(resources[i].getCategory());

            if (contains(title, key) || contains(author, key) || contains(category, key))
            {
                resources[i].display();
                cout << "-------------------------" << endl;
                found = true;
            }
        }

        if (!found)
        {
            cout << "No resource found" << endl;
        }
    }

    // Displays full borrowing history for a specific user
    void displayUserHistory(int userIndex)
    {
        if (userIndex < 0 || userIndex >= users.size())
        {
            return;
        }

        cout << "--- Full Borrowing History for " << users[userIndex].getUsername() << " ---" << endl;

        bool hasHistory = false;

        for (int i = 0; i < borrowRecords.size(); i++)
        {
            if (borrowRecords[i].getUsername() == users[userIndex].getUsername())
            {
                borrowRecords[i].display();
                cout << "-------------------------" << endl;
                hasHistory = true;
            }
        }

        if (!hasHistory)
        {
            cout << "No history found." << endl;
        }
    }

    // Shows the profile details (name, balance, address) of the logged-in user
    void displayUserDetails(int userIndex)
    {
        if (userIndex < 0 || userIndex >= users.size())
        {
            return;
        }
        users[userIndex].display();
    }

    // Shows only the resources that are currently available (not borrowed)
    void displayResources()
    {
        cout << "--- Available Resources ---" << endl;

        for (int i = 0; i < resources.size(); i++)
        {
            if (resources[i].isAvailable())
            {
                resources[i].display();
                cout << "-------------------------" << endl;
            }
        }
    }

    // Shows ALL resources (available + currently borrowed) — used by admin
    void displayAllResources()
    {
        cout << "--- All Resources in Library ---" << endl;

        for (int i = 0; i < resources.size(); i++)
        {
            resources[i].display();
            cout << "-------------------------" << endl;
        }
    }

    // Shows the full list of every borrow transaction in the system
    void displayBorrowRecords()
    {
        cout << "--- All Borrow Records ---" << endl;

        for (int i = 0; i < borrowRecords.size(); i++)
        {
            borrowRecords[i].display();
            cout << "-------------------------" << endl;
        }
    }

    // Displays a report of all currently issued and overdue resources
    void printIssuedOverdueReport(int currentDate)
    {
        cout << "--- Current Issued & Overdue Report ---" << endl;

        bool found = false;

        for (int i = 0; i < borrowRecords.size(); i++)
        {
            if (borrowRecords[i].isActive())
            {
                borrowRecords[i].display();

                if (borrowRecords[i].isOverdue())
                {
                    cout << ">>> STATUS: OVERDUE <<<" << endl;
                }
                else
                {
                    cout << ">>> STATUS: ISSUED <<<" << endl;
                }

                cout << "-------------------------" << endl;
                found = true;
            }
        }

        if (!found)
        {
            cout << "No resources are currently issued." << endl;
        }
    }
    // Adds a new book/resource to the library
    // Validates that all fields are filled and contain only letters
    bool addResource(const string &title, const string &author, const string &category)
    {
        if (title == "" || author == "" || category == "")
        {
            cout << "Error: All fields are required!\n";
            return false;
        }

        if (!isAlphaString(author) || !isAlphaString(category))
        {
            cout << "Error: Author and Category must contain only letters!\n";
            return false;
        }

        resources.push_back(Resource(nextResourceId++, title, author, category, true));
        saveAll();

        return true;
    }
    bool removeResource(int resourceId)
    {
        // Check if resource is currently borrowed
        for (int i = 0; i < borrowRecords.size(); i++)
        {
            if (borrowRecords[i].getResourceId() == resourceId &&
                borrowRecords[i].isActive())
            {
                cout << "Cannot delete borrowed resource!\n";
                return false;
            }
        }

        // Now actually remove resource
        // OPERATOR OVERLOADING IN USE: We create a temporary Resource with the target ID
        // and use our custom "==" operator to find the matching resource in the list.
        Resource target(resourceId, "", "", "", true);

        for (int i = 0; i < resources.size(); i++)
        {
            // Using our overloaded == operator to compare by ID
            if (resources[i] == target)
            {
                resources.erase(resources.begin() + i);
                saveAll(); // Save after removal
                return true;
            }
        }

        cout << "Resource not found!\n";
        return false;
    }

    // Updates the title, author, and category of an existing resource
    // The resource must exist and all fields must be valid
    bool updateResource(int resourceId, const string &title,
                        const string &author, const string &category)
    {
        if (title == "" || author == "" || category == "")
        {
            cout << "Error: All fields are required!\n";
            return false;
        }

        if (!isAlphaString(author) || !isAlphaString(category))
        {
            cout << "Error: Author and Category must contain only letters!\n";
            return false;
        }

        for (int i = 0; i < resources.size(); i++)
        {
            if (resources[i].getId() == resourceId)
            {
                resources[i] = Resource(resourceId, title, author, category,
                                        resources[i].getAvailable());

                saveAll();
                return true;
            }
        }

        cout << "Resource not found!\n";
        return false;
    }

    // Prints a full report of all users and their borrowing history
    void printAllCustomersReports()
    {
        cout << "--- All Customers Report ---" << endl;

        for (int i = 0; i < users.size(); i++)
        {
            cout << "Customer: "
                 << users[i].getFirstName() << " "
                 << users[i].getLastName()
                 << " (" << users[i].getUsername() << ")" << endl;
            cout << "Balance: Rs" << fixed << setprecision(2) << users[i].getBalance() << endl;

            cout << "Borrow History:\n";

            for (int j = 0; j < borrowRecords.size(); j++)
            {
                if (borrowRecords[j].getUsername() == users[i].getUsername())
                {
                    borrowRecords[j].display();
                    cout << "-------------------\n";
                }
            }

            cout << "=============================\n";
        }
    }

    // Adds money to a user's balance and saves the change to file
    void updateUserBalance(int userIndex, double amount)
    {
        if (userIndex < 0 || userIndex >= users.size())
        {
            return;
        }
        users[userIndex].addBalance(amount);
        saveAll(); // Save after balance update
    }

    void mainMenu();
    void userMenu(int userIndex);
    void adminMenu();
};

// Main menu shown when the program starts
// Lets the user choose to register, login, or exit
void library::mainMenu()
{
    int choice;

    while (true)
    {
        setWhiteColor();
        clearScreen();
        printHeader("MAIN MENU - LIBRARY SYSTEM");
        
        cout << " 1. Register New User" << endl;
        cout << " 2. User Login" << endl;
        cout << " 3. Admin Login" << endl;
        cout << " 4. Exit System" << endl;
        cout << "\nEnter your choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            choice = -1;
        }

        if (choice == 1)
        {
            string username, password, firstName, lastName, address;
            double balance;
            cin.ignore(10000, '\n');

            cout << "Enter username: ";
            getline(cin, username);

            if (!isValidInput(username))
            {
                cout << "Invalid! Cannot be empty or spaces only.\n";
                continue;
            }
            cout << "Enter password: ";
            getline(cin, password);
            if (!isValidInput(password))
            {
                cout << "Invalid! Cannot be empty or spaces only.\n";
                continue;
            }
            cout << "Enter first name: ";
            getline(cin, firstName);

            if (!isAlphaString(firstName))
            {
                cout << "Invalid first name!\n";
                continue;
            }
            if (!isValidInput(firstName))
            {
                cout << "Invalid! Cannot be empty or spaces only.\n";
                continue;
            }
            cout << "Enter last name: ";
            getline(cin, lastName);
            if (!isValidInput(lastName))
            {
                cout << "Invalid! Cannot be empty or spaces only.\n";
                continue;
            }
            if (!isAlphaString(lastName))
            {
                cout << "Invalid last name!\n";
                continue;
            }
            cout << "Enter address: ";
            getline(cin, address);

            cout << "Enter initial balance: Rs" << endl;
            cout << "Minimum balance must be Rs50!\n";
            cin >> balance;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid balance! Must be a number.\n";
                continue;
            }

            cin.ignore(10000, '\n');

            if (balance < 50)
            {
                cout << "Error:Minimum balance must be Rs50!\n";
                continue;
            }
            if (registerUser(username, password, firstName, lastName, address, balance))
            {
                cout << "\nUser registered successfully!" << endl;
            }
            else
            {
                setRedColor();
                cout << "\nRegistration failed! Check input or username already exists.\n";
                setWhiteColor(); // Back to Main Menu color
            }
            system("pause");
        }
        else if (choice == 2)
        {
            string username, password;
            cout << "Username: ";
            cin.ignore(10000, '\n');
            getline(cin, username);
            cout << "Password: ";
            getline(cin, password);
            int userIdx = loginUser(username, password);
            if (userIdx != -1)
            {
                cout << "\nLogin successful! Welcome " << username << endl;
                system("pause");
                userMenu(userIdx);
            }
            else
            {
                setRedColor();
                cout << "\nInvalid username or password!" << endl;
                setWhiteColor();
                system("pause");
            }
        }
        else if (choice == 3)
        {
            string username, password;
            cout << "Admin Username: ";
            cin.ignore(10000, '\n');
            getline(cin, username);
            cout << "Admin Password: ";
            getline(cin, password);

            if (loginAdmin(username, password))
            {
                cout << "\nLogin successful! Welcome Admin." << endl;
                system("pause");
                adminMenu();
            }
            else
            {
                setRedColor();
                cout << "\nAccess Denied!" << endl;
                setWhiteColor();
                system("pause");
            }
        }
        else if (choice == 4)
        {
            cout << "\nExiting the system. Goodbye!" << endl;
            break;
        }
        else
        {
            cout << "\nInvalid choice! Please try again." << endl;
        }
    }
}

// Menu shown after a user logs in successfully
// Lets the user borrow, return, search, or view their profile
void library::userMenu(int userIndex)
{
    int choice;

    while (true)
    {
        setWhiteColor();
        clearScreen();
        printHeader("USER MENU - WELCOME " + users[userIndex].getUsername());

        cout << " 1. View Available Resources" << endl;
        cout << " 2. Borrow a Resource" << endl;
        cout << " 3. Return a Resource" << endl;
        cout << " 4. Search Library" << endl;
        cout << " 5. My Profile Details" << endl;
        cout << " 6. Full Borrowing History" << endl;
        cout << " 7. Add Balance to Account" << endl;
        cout << " 8. Logout" << endl;
        cout << "\nEnter your choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            choice = -1;
        }

        if (choice == 1)
        {
            displayResources();
            system("pause");
        }
        else if (choice == 2)
        {
            try
            {
                int resourceId;
                cout << "Enter Resource ID: ";
                cin >> resourceId;
                int currentDate = getTodayDate();

                borrowResource(userIndex, resourceId, currentDate);
                cout << "\nBook issued successfully!" << endl;
                system("pause");
            }
            catch (const LibraryException &e)
            {
                setRedColor();
                cout << "\n" << e.what() << endl;
                setWhiteColor();
                system("pause");
            }
        }
        else if (choice == 3)
        {
            try
            {
                int resourceId;
                cout << "Enter Resource ID: ";
                cin >> resourceId;
                if (cin.fail() || resourceId <= 0)
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid Resource ID!\n";
                    continue;
                }
                int currentDate = getTodayDate();

                returnResource(userIndex, resourceId, currentDate);
                cout << "\nResource returned successfully!" << endl;
                system("pause");
            }
            catch (const LibraryException &e)
            {
                setRedColor();
                cout << "\n" << e.what() << endl;
                setWhiteColor();
                system("pause");
            }
        }
        else if (choice == 4)
        {
            string keyword;
            cout << "Enter Title/Author/Category: ";
            cin.ignore(10000, '\n');
            getline(cin, keyword);
            searchResources(keyword);
            system("pause");
        }
        else if (choice == 5)
        {
            displayUserDetails(userIndex);
            system("pause");
        }
        else if (choice == 6)
        {
            displayUserHistory(userIndex);
            system("pause");
        }
        else if (choice == 7)
        {
            double amount;
            cout << "Amount to add: Rs  ";
            cin >> amount;

            if (cin.fail() || amount <= 0)
            {
                cin.clear();
                cin.ignore(10000, '\n');
                setRedColor();
                cout << "Invalid amount!" << endl;
                setWhiteColor();
            }
            else
            {
                updateUserBalance(userIndex, amount);
                cout << "Balance updated!" << endl;
            }
            system("pause");
        }
        else if (choice == 8)
        {
            break;
        }
    }
}

// Menu shown after admin logs in
// Lets the admin manage resources and view reports
void library::adminMenu()
{
    int choice;

    while (true)
    {
        system("color 07"); // White for Admin
        clearScreen();
        printHeader("ADMINISTRATOR PANEL");

        cout << " 1. View All Library Resources" << endl;
        cout << " 2. Add New Resource" << endl;
        cout << " 3. Remove Resource" << endl;
        cout << " 4. Update Resource Details" << endl;
        cout << " 5. All Customer History Report" << endl;
        cout << " 6. Issued & Overdue Report" << endl;
        cout << " 7. Logout" << endl;
        cout << "\nEnter admin choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            choice = -1;
        }

        if (choice == 1)
        {
            displayAllResources();
            system("pause");
        }
        else if (choice == 2)
        {
            string title, author, category;
            cout << "Title: ";
            cin.ignore(10000, '\n');
            getline(cin, title);
            cout << "Author: ";
            getline(cin, author);
            cout << "Category: ";
            getline(cin, category);

            if (!isValidInput(title) || !isValidInput(author) || !isValidInput(category))
            {
                setRedColor();
                cout << "Error: Inputs cannot be empty or spaces only!" << endl;
                system("color 07");
                system("pause");
                continue;
            }

            if (addResource(title, author, category))
            {
                cout << "\nResource Added!" << endl;
                system("pause");
            }
        }
        else if (choice == 3)
        {
            int resourceId;
            cout << "Enter Resource ID: ";
            cin >> resourceId;

            if (removeResource(resourceId))
            {
                cout << "Successfully removed." << endl;
            }
            else
            {
                setRedColor();
                cout << "Error: Could not remove resource. It might be borrowed." << endl;
                system("color 07"); // Back to Admin White
            }
            system("pause");
        }
        else if (choice == 4)
        {
            int resourceId;
            string title, author, category;
            cout << "Resource ID: ";
            cin >> resourceId;
            cout << "New Title: ";
            cin.ignore(10000, '\n');
            getline(cin, title);
            cout << "New Author: ";
            getline(cin, author);
            cout << "New Category: ";
            getline(cin, category);

            if (updateResource(resourceId, title, author, category))
            {
                cout << "Updated successfully!" << endl;
            }
            else
            {
                setRedColor();
                cout << "Error: Update failed! Resource ID not found." << endl;
                system("color 07");
            }
            system("pause");
        }
        else if (choice == 5)
        {
            printAllCustomersReports();
            system("pause");
        }
        else if (choice == 6)
        {
            int currentDate = getTodayDate();
            printIssuedOverdueReport(currentDate);
            system("pause");
        }
        else if (choice == 7)
        {
            break;
        }
    }
}

// Program entry point
// Creates the library object (which loads all saved data) and starts the main menu
int main()
{
    library lib; // Constructor automatically loads data from files
    lib.mainMenu();
    return 0;
}