#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
using namespace std;
// Struct to store book information
struct Book {
    int id;
    string title;
    string author;
    bool isIssued;
};

// Struct to store issued book details
struct IssuedBook {
    int bookId;
    string borrower;
    time_t issueDate;
    time_t dueDate;
};

// Global variables
vector<Book> books;
vector<IssuedBook> issuedBooks;

// File paths
const string BOOKS_FILE = "books.txt";
const string ISSUED_BOOKS_FILE = "issued_books.txt";

// Function to load books from file
void loadBooks() {
    ifstream file(BOOKS_FILE);
    if (!file.is_open()) return;

    Book book;
    while (file >> book.id >> book.isIssued) {
        file.ignore();
        getline(file, book.title);
        getline(file, book.author);
        books.push_back(book);
    }
    file.close();
}

// Function to save books to file
void saveBooks() {
    ofstream file(BOOKS_FILE);
    if (!file.is_open()) {
        cerr << "Error: Unable to save books to file.\n";
        return;
    }
    for (const auto& book : books) {
        file << book.id << " " << book.isIssued << "\n"
             << book.title << "\n"
             << book.author << "\n";
    }
    file.close();
}

// Function to load issued books from file
void loadIssuedBooks() {
    ifstream file(ISSUED_BOOKS_FILE);
    if (!file.is_open()) return;

    IssuedBook issuedBook;
    while (file >> issuedBook.bookId >> issuedBook.issueDate >> issuedBook.dueDate) {
        file.ignore();
        getline(file, issuedBook.borrower);
        issuedBooks.push_back(issuedBook);
    }
    file.close();
}

// Function to save issued books to file
void saveIssuedBooks() {
    ofstream file(ISSUED_BOOKS_FILE);
    if (!file.is_open()) {
        cerr << "Error: Unable to save issued books to file.\n";
        return;
    }
    for (const auto& issuedBook : issuedBooks) {
        file << issuedBook.bookId << " " << issuedBook.issueDate << " " << issuedBook.dueDate << "\n"
             << issuedBook.borrower << "\n";
    }
    file.close();
}

// Function to add a new book
void addBook() {
    Book book;
    cout << "Enter book ID: ";
    cin >> book.id;
    cin.ignore();
    cout << "Enter book title: ";
    getline(cin, book.title);
    cout << "Enter book author: ";
    getline(cin, book.author);
    book.isIssued = false;

    books.push_back(book);
    cout << "Book added successfully!\n";
    saveBooks();
}

// Function to delete a book
void deleteBook() {
    int id;
    cout << "Enter book ID to delete: ";
    cin >> id;

    auto it = remove_if(books.begin(), books.end(), [id](const Book& book) { return book.id == id; });
    if (it != books.end()) {
        books.erase(it, books.end());
        cout << "Book deleted successfully!\n";
        saveBooks();
    } else {
        cout << "Book not found!\n";
    }
}

// Function to search for a book
void searchBook() {
    string query;
    cout << "Enter book title or author to search: ";
    cin.ignore();
    getline(cin, query);

    for (const auto& book : books) {
        if (book.title.find(query) != string::npos || book.author.find(query) != string::npos) {
            cout << "Book ID: " << book.id << "\n"
                 << "Title: " << book.title << "\n"
                 << "Author: " << book.author << "\n"
                 << "Issued: " << (book.isIssued ? "Yes" : "No") << "\n\n";
        }
    }
}

// Function to issue a book
void issueBook() {
    int id;
    cout << "Enter book ID to issue: ";
    cin >> id;

    for (auto& book : books) {
        if (book.id == id && !book.isIssued) {
            IssuedBook issuedBook;
            issuedBook.bookId = id;
            cout << "Enter borrower name: ";
            cin.ignore();
            getline(cin, issuedBook.borrower);

            time_t now = time(nullptr);
            issuedBook.issueDate = now;
            issuedBook.dueDate = now + (7 * 24 * 60 * 60); // 7 days later

            book.isIssued = true;
            issuedBooks.push_back(issuedBook);
            cout << "Book issued successfully!\n";
            saveBooks();
            saveIssuedBooks();
            return;
        }
    }
    cout << "Book not found or already issued!\n";
}

// Function to return a book
void returnBook() {
    int id;
    cout << "Enter book ID to return: ";
    cin >> id;

    for (auto it = issuedBooks.begin(); it != issuedBooks.end(); ++it) {
        if (it->bookId == id) {
            for (auto& book : books) {
                if (book.id == id) {
                    book.isIssued = false;
                    break;
                }
            }

            time_t now = time(nullptr);
            if (now > it->dueDate) {
                int overdueDays = (now - it->dueDate) / (24 * 60 * 60);
                cout << "Book is overdue by " << overdueDays << " days.\n";
                cout << "Fine: " << overdueDays * 10 << " units.\n";
            }

            issuedBooks.erase(it);
            cout << "Book returned successfully!\n";
            saveBooks();
            saveIssuedBooks();
            return;
        }
    }
    cout << "Issued book not found!\n";
}

// Function to display all books
void displayBooks() {
    cout << "\nAll Books:\n";
    for (const auto& book : books) {
        cout << "Book ID: " << book.id << "\n"
             << "Title: " << book.title << "\n"
             << "Author: " << book.author << "\n"
             << "Issued: " << (book.isIssued ? "Yes" : "No") << "\n\n";
    }
}

// Main menu
void menu() {
    int choice;
    do {
        cout << "\nLibrary Management System\n";
        cout << "1. Add Book\n";
        cout << "2. Delet`e Book\n";
        cout << "3. Search Book\n";
        cout << "4. Issue Book\n";
        cout << "5. Return Book\n";
        cout << "6. Display All Books\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addBook(); break;
            case 2: deleteBook(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: displayBooks(); break;
            case 7: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 7);
}

int main() {
    loadBooks();
    loadIssuedBooks();
    menu();
    return 0;
}
