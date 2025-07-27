#include "LibraryManager.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <regex>
#include <limits>

// Constructor
LibraryManager::LibraryManager(const std::string& filename) : dataFile(filename), nextId(1) {
    loadBooksFromFile();
}

// Destructor
LibraryManager::~LibraryManager() {
    saveBooksToFile();
}

// Generate next available ID
int LibraryManager::generateNextId() {
    if (books.empty()) {
        return 1;
    }
    
    int maxId = 0;
    for (const auto& book : books) {
        if (book.getId() > maxId) {
            maxId = book.getId();
        }
    }
    return maxId + 1;
}

// Validate year
bool LibraryManager::isValidYear(int year) const {
    return year >= 1000 && year <= 2030;
}

// Validate ISBN (simplified validation)
bool LibraryManager::isValidISBN(const std::string& isbn) const {
    std::regex isbnPattern(R"(^(?:\d{10}|\d{13}|[\d-]{13,17})$)");
    return std::regex_match(isbn, isbnPattern);
}

// Load books from binary file
void LibraryManager::loadBooksFromFile() {
    std::ifstream file(dataFile, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "No existing data file found. Starting with empty library.\n";
        return;
    }
    
    books.clear();
    while (file.peek() != EOF) {
        Book book;
        book.readFromFile(file);
        if (file.good()) {
            books.push_back(book);
        }
    }
    
    file.close();
    std::cout << "Loaded " << books.size() << " books from file.\n";
}

// Save books to binary file
void LibraryManager::saveBooksToFile() {
    std::ofstream file(dataFile, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot save data to file " << dataFile << std::endl;
        return;
    }
    
    for (const auto& book : books) {
        book.writeToFile(file);
    }
    
    file.close();
}

// Add a new book record
bool LibraryManager::addRecord(const std::string& title, const std::string& author, 
                              int year, const std::string& isbn, const std::string& category) {
    // Validate input
    if (title.empty() || author.empty()) {
        std::cerr << "Error: Title and author cannot be empty.\n";
        return false;
    }
    
    if (!isValidYear(year)) {
        std::cerr << "Error: Invalid year. Must be between 1000 and 2030.\n";
        return false;
    }
    
    if (!isValidISBN(isbn)) {
        std::cerr << "Error: Invalid ISBN format.\n";
        return false;
    }
    
    // Check for duplicate ISBN
    for (const auto& book : books) {
        if (book.getIsbn() == isbn) {
            std::cerr << "Error: A book with this ISBN already exists.\n";
            return false;
        }
    }
    
    int newId = generateNextId();
    Book newBook(newId, title, author, year, isbn, category);
    books.push_back(newBook);
    
    std::cout << "Book added successfully with ID: " << newId << std::endl;
    return true;
}

// Display all records
void LibraryManager::displayAllRecords() const {
    if (books.empty()) {
        std::cout << "No books in the library.\n";
        return;
    }
    
    std::cout << "\n" << std::string(105, '=') << std::endl;
    std::cout << "LIBRARY BOOK RECORDS" << std::endl;
    std::cout << std::string(105, '=') << std::endl;
    std::cout << std::left << std::setw(5) << "ID" 
              << std::setw(25) << "Title" 
              << std::setw(20) << "Author" 
              << std::setw(6) << "Year" 
              << std::setw(15) << "ISBN" 
              << std::setw(15) << "Category" 
              << std::setw(10) << "Status" 
              << std::endl;
    std::cout << std::string(105, '-') << std::endl;
    
    for (const auto& book : books) {
        book.displayBook();
    }
    std::cout << std::string(105, '=') << std::endl;
    std::cout << "Total books: " << books.size() << std::endl;
}

// Search record by ID
Book* LibraryManager::searchRecordByID(int id) {
    auto it = std::find_if(books.begin(), books.end(), 
                          [id](const Book& book) { return book.getId() == id; });
    
    if (it != books.end()) {
        return &(*it);
    }
    return nullptr;
}

// Search records by title
std::vector<Book*> LibraryManager::searchRecordsByTitle(const std::string& title) {
    std::vector<Book*> results;
    std::string lowerTitle = title;
    std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
    
    for (auto& book : books) {
        std::string bookTitle = book.getTitle();
        std::transform(bookTitle.begin(), bookTitle.end(), bookTitle.begin(), ::tolower);
        
        if (bookTitle.find(lowerTitle) != std::string::npos) {
            results.push_back(&book);
        }
    }
    
    return results;
}

// Search records by author
std::vector<Book*> LibraryManager::searchRecordsByAuthor(const std::string& author) {
    std::vector<Book*> results;
    std::string lowerAuthor = author;
    std::transform(lowerAuthor.begin(), lowerAuthor.end(), lowerAuthor.begin(), ::tolower);
    
    for (auto& book : books) {
        std::string bookAuthor = book.getAuthor();
        std::transform(bookAuthor.begin(), bookAuthor.end(), bookAuthor.begin(), ::tolower);
        
        if (bookAuthor.find(lowerAuthor) != std::string::npos) {
            results.push_back(&book);
        }
    }
    
    return results;
}

// Delete record by ID
bool LibraryManager::deleteRecord(int id) {
    auto it = std::find_if(books.begin(), books.end(), 
                          [id](const Book& book) { return book.getId() == id; });
    
    if (it != books.end()) {
        std::cout << "Deleting book: " << it->getTitle() << " by " << it->getAuthor() << std::endl;
        books.erase(it);
        std::cout << "Book deleted successfully.\n";
        return true;
    }
    
    std::cout << "Book with ID " << id << " not found.\n";
    return false;
}

// Update record
bool LibraryManager::updateRecord(int id) {
    Book* book = searchRecordByID(id);
    if (!book) {
        std::cout << "Book with ID " << id << " not found.\n";
        return false;
    }
    
    std::cout << "Current book details:\n";
    book->displayBook();
    std::cout << "\nEnter new details (press Enter to keep current value):\n";
    
    std::cout << "Current title: " << book->getTitle() << "\nNew title: ";
    std::string newTitle = getValidatedStringInput("", true);
    if (!newTitle.empty()) {
        book->setTitle(newTitle);
    }
    
    std::cout << "Current author: " << book->getAuthor() << "\nNew author: ";
    std::string newAuthor = getValidatedStringInput("", true);
    if (!newAuthor.empty()) {
        book->setAuthor(newAuthor);
    }
    
    std::cout << "Current year: " << book->getYear() << "\nNew year (0 to keep current): ";
    int newYear = getValidatedIntInput("", 0, 2030);
    if (newYear > 0 && isValidYear(newYear)) {
        book->setYear(newYear);
    }
    
    std::cout << "Current ISBN: " << book->getIsbn() << "\nNew ISBN: ";
    std::string newIsbn = getValidatedStringInput("", true);
    if (!newIsbn.empty() && isValidISBN(newIsbn)) {
        book->setIsbn(newIsbn);
    }
    
    std::cout << "Current category: " << book->getCategory() << "\nNew category: ";
    std::string newCategory = getValidatedStringInput("", true);
    if (!newCategory.empty()) {
        book->setCategory(newCategory);
    }
    
    std::cout << "Book updated successfully.\n";
    return true;
}

// Sort books by title
void LibraryManager::sortByTitle() {
    std::sort(books.begin(), books.end(), 
              [](const Book& a, const Book& b) { return a.getTitle() < b.getTitle(); });
    std::cout << "Books sorted by title.\n";
}

// Sort books by author
void LibraryManager::sortByAuthor() {
    std::sort(books.begin(), books.end(), 
              [](const Book& a, const Book& b) { return a.getAuthor() < b.getAuthor(); });
    std::cout << "Books sorted by author.\n";
}

// Sort books by year
void LibraryManager::sortByYear() {
    std::sort(books.begin(), books.end(), 
              [](const Book& a, const Book& b) { return a.getYear() < b.getYear(); });
    std::cout << "Books sorted by year.\n";
}

// Generic sort function
void LibraryManager::sortBy(std::function<bool(const Book&, const Book&)> comparator) {
    std::sort(books.begin(), books.end(), comparator);
}

// Export to CSV
bool LibraryManager::exportToCSV(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot create CSV file " << filename << std::endl;
        return false;
    }
    
    // Write header
    file << "ID,Title,Author,Year,ISBN,Category,Status\n";
    
    // Write book data
    for (const auto& book : books) {
        file << book.toCSV() << "\n";
    }
    
    file.close();
    std::cout << "Data exported to " << filename << " successfully.\n";
    return true;
}

// Get number of available books
int LibraryManager::getAvailableBooks() const {
    return std::count_if(books.begin(), books.end(), 
                        [](const Book& book) { return book.getAvailability(); });
}

// Get number of borrowed books
int LibraryManager::getBorrowedBooks() const {
    return books.size() - getAvailableBooks();
}

// Display library statistics
void LibraryManager::displayStatistics() const {
    std::cout << "\n=== LIBRARY STATISTICS ===\n";
    std::cout << "Total books: " << getTotalBooks() << std::endl;
    std::cout << "Available books: " << getAvailableBooks() << std::endl;
    std::cout << "Borrowed books: " << getBorrowedBooks() << std::endl;
    std::cout << "===========================\n";
}

// Borrow a book
bool LibraryManager::borrowBook(int id) {
    Book* book = searchRecordByID(id);
    if (!book) {
        std::cout << "Book with ID " << id << " not found.\n";
        return false;
    }
    
    if (!book->getAvailability()) {
        std::cout << "Book is already borrowed.\n";
        return false;
    }
    
    book->setAvailability(false);
    std::cout << "Book '" << book->getTitle() << "' borrowed successfully.\n";
    return true;
}

// Return a book
bool LibraryManager::returnBook(int id) {
    Book* book = searchRecordByID(id);
    if (!book) {
        std::cout << "Book with ID " << id << " not found.\n";
        return false;
    }
    
    if (book->getAvailability()) {
        std::cout << "Book is already available.\n";
        return false;
    }
    
    book->setAvailability(true);
    std::cout << "Book '" << book->getTitle() << "' returned successfully.\n";
    return true;
}

// Validate input based on type
bool LibraryManager::validateInput(const std::string& input, const std::string& type) {
    if (type == "number") {
        return !input.empty() && std::all_of(input.begin(), input.end(), ::isdigit);
    } else if (type == "string") {
        return !input.empty();
    }
    return true;
}

// Get validated integer input
int LibraryManager::getValidatedIntInput(const std::string& prompt, int min, int max) {
    int value;
    std::string input;
    
    while (true) {
        if (!prompt.empty()) {
            std::cout << prompt;
        }
        
        std::getline(std::cin, input);
        
        if (input.empty() && min == 0) {
            return 0; // Allow empty input if min is 0
        }
        
        try {
            value = std::stoi(input);
            if (value >= min && value <= max) {
                return value;
            } else {
                std::cout << "Please enter a number between " << min << " and " << max << ": ";
            }
        } catch (const std::exception& e) {
            std::cout << "Invalid input. Please enter a valid number: ";
        }
    }
}

// Get validated string input
std::string LibraryManager::getValidatedStringInput(const std::string& prompt, bool allowEmpty) {
    std::string input;
    
    while (true) {
        if (!prompt.empty()) {
            std::cout << prompt;
        }
        
        std::getline(std::cin, input);
        
        if (!input.empty() || allowEmpty) {
            return input;
        } else {
            std::cout << "Input cannot be empty. Please try again: ";
        }
    }
}
