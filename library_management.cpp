#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;

// Class representing a Book in the library
class Book {
public:
    int id;            // Unique identifier for the book
    string name;       // Name/title of the book
    string author;     // Author of the book
    string type;       //Type/genre of the book
    int pageCount;    // Number of pages in the book
    bool isBorrowed; // Indicates whether the book is currently borrowed
    time_t dueDate; // Due date for the book if it is borrowed
    int borrowedBy; // Member ID of the member who borrowed the book, -1 if not borrowed

    //Constructor for initializing a Book object
    Book(int _id, string _name, string _author, string _type, int _pageCount)
        : id(_id), name(_name), author(_author), type(_type), pageCount(_pageCount),
          isBorrowed(false), dueDate(0), borrowedBy(-1) {}
};

// Class representing a Member
class Member {
public:
    int id;
    string name;

    // Constructor for initializing a Member object
    Member(int _id, string _name) : id(_id), name(_name) {}
};

// Class representing a Library
class Library {
public:
    vector<Book> books;
    vector<Member> members;

public:
    // Method to add a new member to the library 
    void addMember() {
        int memberId = members.size() + 1;
        string memberName;
        cout << "Enter member name: ";
        cin.ignore(); // Ignore newline character in the buffer
        getline(cin, memberName);

        members.push_back(Member(memberId, memberName));

        cout << "\nMember created successfully!\n";
        displayMemberDetails(memberId);
    }

    // Method to issue a book to a member
    void issueBook(int memberId) {

        // Check if the provided member ID is valid
        if (memberId <= 0 || memberId > members.size()) {
            cout << "Invalid member ID. Please enter a valid member ID.\n";
            return;
        }

        int bookId;

        // Prompt the user to enter the book ID they want to issue
        cout << "Enter book ID to issue: ";
        cin >> bookId;

        // Check if the provided book ID is valid and the book is available for borrowing
        if (bookId <= 0 || bookId > books.size() || books[bookId - 1].isBorrowed) {
            cout << "Invalid book ID or book is already borrowed. Please enter a valid book ID.\n";
            return;
        }

        // Updates book information to mark it as borrowed by the specified member
        books[bookId - 1].isBorrowed = true;
        books[bookId - 1].dueDate = time(0) + 3 * 24 * 60 * 60; // 3 days from the current time
        books[bookId - 1].borrowedBy = memberId;

        // Display a success message and show the books currently borrow by the member
        cout << "\nBook issued successfully!\n";
        displayMemberBooks(memberId);
    }

    // Method to return a book from a member
    void returnBook(int memberId) {

        // Check if the provided member ID is valid 
        if (memberId <= 0 || memberId > members.size()) {
            cout << "Invalid member ID. Please enter a valid member ID.\n";
            return;
        }

        int bookId;

        // Prompt the user to enter the book ID they want to return
        cout << "Enter book ID to return: ";
        cin >> bookId;

        // Check if the provided book ID is valid and the book is currently borrowed
        if (bookId <= 0 || bookId > books.size() || !books[bookId - 1].isBorrowed) {
            cout << "Invalid book ID or book is not borrowed. Please enter a valid book ID.\n";
            return;
        }

        books[bookId - 1].isBorrowed = false;
        books[bookId - 1].borrowedBy = -1;

        // Get the current time
        time_t currentTime = time(0);

        // Check if the book is returned after the due date
        if (currentTime > books[bookId - 1].dueDate) {

            // Calculate the fine for overdue days (£1 per day overdue)
            int daysOverdue = static_cast<int>((currentTime - books[bookId - 1].dueDate) / (24 * 60 * 60));
            int fine = daysOverdue * 1; // £1 per day overdue
            cout << "\nBook returned successfully. Fine for overdue: £" << fine << "\n";
        } else {

            // If the book is returned on time, no fine is charged
            cout << "\nBook returned successfully. No fine.\n";
        }

        displayMemberBooks(memberId);
    }

    // Method to display books borrowed by a member
    void displayMemberBooks(int memberId) {
        if (memberId <= 0 || memberId > members.size()) {
            cout << "Invalid member ID. Please enter a valid member ID.\n";
            return;
        }

        cout << "\nBooks borrowed by Member ID " << memberId << ":\n";
        for (const Book& book : books) {
            if (book.isBorrowed && book.borrowedBy == memberId) {
                cout << "Book ID: " << book.id << " | Name: " << book.name << " | Due Date: "
                     << ctime(&book.dueDate);
            }
        }
    }

    // Method to calculate fine for a member 
    void calculateFine(int memberId) {

        // Check if the provided member ID is valid
        if (memberId <= 0 || memberId > members.size()) {
            cout << "Invalid member ID. Please enter a valid member ID.\n";
            return;
        }

        int fine = 0;
        time_t currentTime = time(0);

        // Iterate through each book in the library
        for (const Book& book : books) {

            //Check if the book is currently borrowed  by the specified member and is overdue
            if (book.isBorrowed && book.dueDate < currentTime && book.borrowedBy == memberId) {
                
                // Calculate the number of days the book is overdue
                int daysOverdue = static_cast<int>((currentTime - book.dueDate) / (24 * 60 * 60));

                // Calculate the fine based on the number of days overdue
                fine += daysOverdue * 1; // £1 per day overdue
            }
        }

        // Print the calculated fine for the member
        cout << "\nFine for Member ID " << memberId << ": £" << fine << "\n";
    }

    // Method to display details of a member
    void displayMemberDetails(int memberId) {
        if (memberId <= 0 || memberId > members.size()) {
            cout << "Invalid member ID. Please enter a valid member ID.\n";
            return;
        }

        cout << "\nMember details:\n";
        cout << "Member ID: " << members[memberId - 1].id << " | Name: " << members[memberId - 1].name << "\n";
    }

    // Mehtod to display the library management system menu
    void displayMenu() {
        cout << "\nLibrary Management System Menu:\n";
        cout << "1. Add a member\n";
        cout << "2. Issue a book to a member\n";
        cout << "3. Return a book\n";
        cout << "4. Display all books borrowed by a member\n";
        cout << "5. Calculate fine for a member\n";
        cout << "6. Exit\n";
    }

    // Metod to read books from a CSV file
    void readBooksFromCSV(const string& filename) {

    // Attempt to open the CSV file    
    ifstream file(filename);

    // Check if the file is successfully opened
    if (!file.is_open()) {
        cerr << "Error opening CSV file\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string token;
        vector<string> tokens;

        // Tokenize the line using a comma as the delimiter
        while (getline(iss, token, ',')) {
            tokens.push_back(token);
        }

        // Ensure the line has the expected number of fields
        if (tokens.size() != 6) {
            cerr << "Error reading book information from CSV: " << line << endl;
            continue;
        }

        int id, pageCount;
        string name, authorFirstName, authorLastName, type;

        // Convert relevant tokens to the appropriate types
        try {
            id = stoi(tokens[0]);
            name = tokens[1];
            pageCount = stoi(tokens[2]);
            authorFirstName = tokens[3];
            authorLastName = tokens[4];
            type = tokens[5];
        } catch (const exception& e) {
            cerr << "Error converting data types for book information: " << e.what() << endl;
            continue;
        }

        books.push_back(Book(id, name, authorFirstName + " " + authorLastName, type, pageCount));
    }

    file.close();
}

    // Method to run the library management system
    void run() {
        int choice;

        // Continue displaying the menu until the user chooses to exit (Option 6)
        do {
            displayMenu();

            // Prompt the user for their choice
            cout << "Enter your choice (1-6): ";
            cin >> choice;

            // Process the user's choice using a switch statement
            switch (choice) {
                case 1:
                    // Option to add a new member
                    addMember();
                    break;
                case 2: {

                    // Option to issue a book to a member
                    int memberId;
                    cout << "Enter member ID: ";
                    cin >> memberId;
                    issueBook(memberId);
                    break;
                }
                case 3: {

                    // Option to return a book from member
                    int memberId;
                    cout << "Enter member ID: ";
                    cin >> memberId;
                    returnBook(memberId);
                    break;
                }
                case 4: {

                    // Option to display books borrow by a member
                    int memberId;
                    cout << "Enter member ID: ";
                    cin >> memberId;
                    displayMemberBooks(memberId);
                    break;
                }
                case 5: {

                    // Option to calculate fine for a member
                    int memberId;
                    cout << "Enter member ID: ";
                    cin >> memberId;
                    calculateFine(memberId);
                    break;
                }
                case 6:
                    // Option to exit the library management system
                    cout << "Exiting the Library Management System. Goodbye!\n";
                    break;
                default:
                    // Handle invalid choice
                    cout << "Invalid choice. Please enter a number between 1 and 6.\n";
            }
        } while (choice != 6);  // Continue the loop until the user chooses to exit
    }
};

// Main function
int main() {
    Library library;

    // Reading books from CSV file
    library.readBooksFromCSV("library_books.csv");

    // Running the library management system 
    library.run();

    return 0;
}
