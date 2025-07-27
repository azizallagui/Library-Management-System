/*
 * Library Management System - Single File Version
 * 
 * A comprehensive file-based library management system built in C++
 * with Object-Oriented Programming principles.
 * 
 * Features:
 * - Add/Edit/Delete/Search books
 * - Sort by title/author/year
 * - Borrow/Return system
 * - Export to CSV
 * - Binary file storage
 * - Input validation
 * 
 * Compile with: g++ -std=c++17 library_manager_single.cpp -o library_manager.exe
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <regex>
#include <limits>
#include <functional>
#include <iomanip>
#include <cstdlib>

//=============================================================================
// BOOK CLASS
//=============================================================================

class Book {
private:
    int id;
    std::string title;
    std::string author;
    int year;
    std::string isbn;
    std::string category;
    bool isAvailable;

public:
    // Constructors
    Book() : id(0), title(""), author(""), year(0), isbn(""), category(""), isAvailable(true) {}
    
    Book(int id, const std::string& title, const std::string& author, 
         int year, const std::string& isbn, const std::string& category, bool available = true)
        : id(id), title(title), author(author), year(year), isbn(isbn), category(category), isAvailable(available) {}
    
    // Getters
    int getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    int getYear() const { return year; }
    std::string getIsbn() const { return isbn; }
    std::string getCategory() const { return category; }
    bool getAvailability() const { return isAvailable; }
    
    // Setters
    void setId(int newId) { id = newId; }
    void setTitle(const std::string& newTitle) { title = newTitle; }
    void setAuthor(const std::string& newAuthor) { author = newAuthor; }
    void setYear(int newYear) { year = newYear; }
    void setIsbn(const std::string& newIsbn) { isbn = newIsbn; }
    void setCategory(const std::string& newCategory) { category = newCategory; }
    void setAvailability(bool available) { isAvailable = available; }
    
    // Display book information
    void displayBook() const {
        std::cout << std::left << std::setw(5) << id 
                  << std::setw(25) << title 
                  << std::setw(20) << author 
                  << std::setw(6) << year 
                  << std::setw(15) << isbn 
                  << std::setw(15) << category 
                  << std::setw(10) << (isAvailable ? "Available" : "Borrowed") 
                  << std::endl;
    }
    
    // Convert book to string representation
    std::string toString() const {
        std::ostringstream oss;
        oss << "ID: " << id << ", Title: " << title << ", Author: " << author 
            << ", Year: " << year << ", ISBN: " << isbn << ", Category: " << category 
            << ", Status: " << (isAvailable ? "Available" : "Borrowed");
        return oss.str();
    }
    
    // Convert book to CSV format
    std::string toCSV() const {
        std::ostringstream oss;
        oss << id << "," << title << "," << author << "," << year << "," 
            << isbn << "," << category << "," << (isAvailable ? "Available" : "Borrowed");
        return oss.str();
    }
    
    // Write book data to binary file
    void writeToFile(std::ofstream& out) const {
        out.write(reinterpret_cast<const char*>(&id), sizeof(id));
        
        size_t titleSize = title.size();
        out.write(reinterpret_cast<const char*>(&titleSize), sizeof(titleSize));
        out.write(title.c_str(), titleSize);
        
        size_t authorSize = author.size();
        out.write(reinterpret_cast<const char*>(&authorSize), sizeof(authorSize));
        out.write(author.c_str(), authorSize);
        
        out.write(reinterpret_cast<const char*>(&year), sizeof(year));
        
        size_t isbnSize = isbn.size();
        out.write(reinterpret_cast<const char*>(&isbnSize), sizeof(isbnSize));
        out.write(isbn.c_str(), isbnSize);
        
        size_t categorySize = category.size();
        out.write(reinterpret_cast<const char*>(&categorySize), sizeof(categorySize));
        out.write(category.c_str(), categorySize);
        
        out.write(reinterpret_cast<const char*>(&isAvailable), sizeof(isAvailable));
    }
    
    // Read book data from binary file
    void readFromFile(std::ifstream& in) {
        in.read(reinterpret_cast<char*>(&id), sizeof(id));
        
        size_t titleSize;
        in.read(reinterpret_cast<char*>(&titleSize), sizeof(titleSize));
        title.resize(titleSize);
        in.read(&title[0], titleSize);
        
        size_t authorSize;
        in.read(reinterpret_cast<char*>(&authorSize), sizeof(authorSize));
        author.resize(authorSize);
        in.read(&author[0], authorSize);
        
        in.read(reinterpret_cast<char*>(&year), sizeof(year));
        
        size_t isbnSize;
        in.read(reinterpret_cast<char*>(&isbnSize), sizeof(isbnSize));
        isbn.resize(isbnSize);
        in.read(&isbn[0], isbnSize);
        
        size_t categorySize;
        in.read(reinterpret_cast<char*>(&categorySize), sizeof(categorySize));
        category.resize(categorySize);
        in.read(&category[0], categorySize);
        
        in.read(reinterpret_cast<char*>(&isAvailable), sizeof(isAvailable));
    }
    
    // Equality operator
    bool operator==(const Book& other) const {
        return id == other.id;
    }
};

//=============================================================================
// LIBRARY MANAGER CLASS
//=============================================================================

class LibraryManager {
private:
    std::vector<Book> books;
    std::string dataFile;
    int nextId;
    
    // Generate next available ID
    int generateNextId() {
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
    bool isValidYear(int year) const {
        return year >= 1000 && year <= 2030;
    }
    
    // Validate ISBN (simplified validation)
    bool isValidISBN(const std::string& isbn) const {
        std::regex isbnPattern(R"(^(?:\d{10}|\d{13}|[\d-]{13,17})$)");
        return std::regex_match(isbn, isbnPattern);
    }
    
    // Load books from binary file
    void loadBooksFromFile() {
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
    void saveBooksToFile() {
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
    
public:
    // Constructor
    LibraryManager(const std::string& filename = "library_data.bin") : dataFile(filename), nextId(1) {
        loadBooksFromFile();
    }
    
    // Destructor
    ~LibraryManager() {
        saveBooksToFile();
    }
    
    // Add a new book record
    bool addRecord(const std::string& title, const std::string& author, 
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
    void displayAllRecords() const {
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
    Book* searchRecordByID(int id) {
        auto it = std::find_if(books.begin(), books.end(), 
                              [id](const Book& book) { return book.getId() == id; });
        
        if (it != books.end()) {
            return &(*it);
        }
        return nullptr;
    }
    
    // Search records by title
    std::vector<Book*> searchRecordsByTitle(const std::string& title) {
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
    std::vector<Book*> searchRecordsByAuthor(const std::string& author) {
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
    bool deleteRecord(int id) {
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
    
    // Sort books by title
    void sortByTitle() {
        std::sort(books.begin(), books.end(), 
                  [](const Book& a, const Book& b) { return a.getTitle() < b.getTitle(); });
        std::cout << "Books sorted by title.\n";
    }
    
    // Sort books by author
    void sortByAuthor() {
        std::sort(books.begin(), books.end(), 
                  [](const Book& a, const Book& b) { return a.getAuthor() < b.getAuthor(); });
        std::cout << "Books sorted by author.\n";
    }
    
    // Sort books by year
    void sortByYear() {
        std::sort(books.begin(), books.end(), 
                  [](const Book& a, const Book& b) { return a.getYear() < b.getYear(); });
        std::cout << "Books sorted by year.\n";
    }
    
    // Export to CSV
    bool exportToCSV(const std::string& filename) const {
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
    int getAvailableBooks() const {
        return std::count_if(books.begin(), books.end(), 
                            [](const Book& book) { return book.getAvailability(); });
    }
    
    // Get number of borrowed books
    int getBorrowedBooks() const {
        return books.size() - getAvailableBooks();
    }
    
    // Display library statistics
    void displayStatistics() const {
        std::cout << "\n=== LIBRARY STATISTICS ===\n";
        std::cout << "Total books: " << books.size() << std::endl;
        std::cout << "Available books: " << getAvailableBooks() << std::endl;
        std::cout << "Borrowed books: " << getBorrowedBooks() << std::endl;
        std::cout << "===========================\n";
    }
    
    // Borrow a book
    bool borrowBook(int id) {
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
    bool returnBook(int id) {
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
    
    // Get validated integer input
    static int getValidatedIntInput(const std::string& prompt, int min = 0, int max = 9999) {
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
    static std::string getValidatedStringInput(const std::string& prompt, bool allowEmpty = false) {
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
    
    // Update record
    bool updateRecord(int id) {
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
        if (newYear > 0) {
            book->setYear(newYear);
        }
        
        std::cout << "Current ISBN: " << book->getIsbn() << "\nNew ISBN: ";
        std::string newIsbn = getValidatedStringInput("", true);
        if (!newIsbn.empty()) {
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
};

//=============================================================================
// MENU CLASS
//=============================================================================

class Menu {
private:
    LibraryManager libraryManager;
    
    // Clear screen (cross-platform)
    void clearScreen() const {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
    
    // Pause screen
    void pauseScreen() const {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
    
    // Display main menu
    void displayMainMenu() const {
        clearScreen();
        std::cout << "\n";
        std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    LIBRARY MANAGEMENT SYSTEM                ║\n";
        std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  1. Add New Book                                             ║\n";
        std::cout << "║  2. Display All Books                                        ║\n";
        std::cout << "║  3. Search Books                                             ║\n";
        std::cout << "║  4. Update Book                                              ║\n";
        std::cout << "║  5. Delete Book                                              ║\n";
        std::cout << "║  6. Sort Books                                               ║\n";
        std::cout << "║  7. Borrow Book                                              ║\n";
        std::cout << "║  8. Return Book                                              ║\n";
        std::cout << "║  9. Export to CSV                                            ║\n";
        std::cout << "║ 10. Library Statistics                                       ║\n";
        std::cout << "║  0. Exit                                                     ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
        std::cout << "Enter your choice: ";
    }
    
    // Display search menu
    void displaySearchMenu() const {
        clearScreen();
        std::cout << "\n";
        std::cout << "╔═══════════════════════════════════════╗\n";
        std::cout << "║             SEARCH MENU               ║\n";
        std::cout << "╠═══════════════════════════════════════╣\n";
        std::cout << "║  1. Search by ID                      ║\n";
        std::cout << "║  2. Search by Title                   ║\n";
        std::cout << "║  3. Search by Author                  ║\n";
        std::cout << "║  0. Back to Main Menu                 ║\n";
        std::cout << "╚═══════════════════════════════════════╝\n";
        std::cout << "Enter your choice: ";
    }
    
    // Display sort menu
    void displaySortMenu() const {
        clearScreen();
        std::cout << "\n";
        std::cout << "╔═══════════════════════════════════════╗\n";
        std::cout << "║              SORT MENU                ║\n";
        std::cout << "╠═══════════════════════════════════════╣\n";
        std::cout << "║  1. Sort by Title                     ║\n";
        std::cout << "║  2. Sort by Author                    ║\n";
        std::cout << "║  3. Sort by Year                      ║\n";
        std::cout << "║  0. Back to Main Menu                 ║\n";
        std::cout << "╚═══════════════════════════════════════╝\n";
        std::cout << "Enter your choice: ";
    }
    
public:
    // Constructor
    Menu() : libraryManager("library_data.bin") {}
    
    // Main menu loop
    void run() {
        int choice;
        
        std::cout << "Welcome to Library Management System!\n";
        std::cout << "Loading existing data...\n";
        pauseScreen();
        
        do {
            displayMainMenu();
            choice = LibraryManager::getValidatedIntInput("", 0, 10);
            
            switch (choice) {
                case 1: {
                    clearScreen();
                    std::cout << "\n=== ADD NEW BOOK ===\n";
                    
                    std::string title = LibraryManager::getValidatedStringInput("Enter book title: ");
                    std::string author = LibraryManager::getValidatedStringInput("Enter author name: ");
                    int year = LibraryManager::getValidatedIntInput("Enter publication year: ", 1000, 2030);
                    std::string isbn = LibraryManager::getValidatedStringInput("Enter ISBN: ");
                    std::string category = LibraryManager::getValidatedStringInput("Enter category: ");
                    
                    if (libraryManager.addRecord(title, author, year, isbn, category)) {
                        std::cout << "Book added successfully!\n";
                    } else {
                        std::cout << "Failed to add book. Please check your input.\n";
                    }
                    
                    pauseScreen();
                    break;
                }
                case 2:
                    clearScreen();
                    libraryManager.displayAllRecords();
                    pauseScreen();
                    break;
                    
                case 3: {
                    int searchChoice;
                    do {
                        displaySearchMenu();
                        searchChoice = LibraryManager::getValidatedIntInput("", 0, 3);
                        
                        switch (searchChoice) {
                            case 1: {
                                clearScreen();
                                std::cout << "\n=== SEARCH BY ID ===\n";
                                
                                int id = LibraryManager::getValidatedIntInput("Enter book ID: ", 1, 9999);
                                Book* book = libraryManager.searchRecordByID(id);
                                
                                if (book) {
                                    std::cout << "\nBook found:\n";
                                    std::cout << std::string(50, '-') << std::endl;
                                    book->displayBook();
                                } else {
                                    std::cout << "Book with ID " << id << " not found.\n";
                                }
                                
                                pauseScreen();
                                break;
                            }
                            case 2: {
                                clearScreen();
                                std::cout << "\n=== SEARCH BY TITLE ===\n";
                                
                                std::string title = LibraryManager::getValidatedStringInput("Enter title (or part of title): ");
                                std::vector<Book*> results = libraryManager.searchRecordsByTitle(title);
                                
                                if (!results.empty()) {
                                    std::cout << "\nFound " << results.size() << " book(s):\n";
                                    std::cout << std::string(50, '-') << std::endl;
                                    for (const auto& book : results) {
                                        book->displayBook();
                                    }
                                } else {
                                    std::cout << "No books found with title containing '" << title << "'.\n";
                                }
                                
                                pauseScreen();
                                break;
                            }
                            case 3: {
                                clearScreen();
                                std::cout << "\n=== SEARCH BY AUTHOR ===\n";
                                
                                std::string author = LibraryManager::getValidatedStringInput("Enter author name (or part of name): ");
                                std::vector<Book*> results = libraryManager.searchRecordsByAuthor(author);
                                
                                if (!results.empty()) {
                                    std::cout << "\nFound " << results.size() << " book(s):\n";
                                    std::cout << std::string(50, '-') << std::endl;
                                    for (const auto& book : results) {
                                        book->displayBook();
                                    }
                                } else {
                                    std::cout << "No books found by author '" << author << "'.\n";
                                }
                                
                                pauseScreen();
                                break;
                            }
                        }
                    } while (searchChoice != 0);
                    break;
                }
                case 4: {
                    clearScreen();
                    std::cout << "\n=== UPDATE BOOK ===\n";
                    
                    libraryManager.displayAllRecords();
                    
                    int id = LibraryManager::getValidatedIntInput("Enter ID of book to update: ", 1, 9999);
                    
                    if (libraryManager.updateRecord(id)) {
                        std::cout << "Book updated successfully!\n";
                    }
                    
                    pauseScreen();
                    break;
                }
                case 5: {
                    clearScreen();
                    std::cout << "\n=== DELETE BOOK ===\n";
                    
                    libraryManager.displayAllRecords();
                    
                    int id = LibraryManager::getValidatedIntInput("Enter ID of book to delete: ", 1, 9999);
                    
                    Book* book = libraryManager.searchRecordByID(id);
                    if (book) {
                        std::cout << "\nYou are about to delete:\n";
                        book->displayBook();
                        std::cout << "\nAre you sure? (y/N): ";
                        std::string confirm;
                        std::getline(std::cin, confirm);
                        
                        if (confirm == "y" || confirm == "Y") {
                            if (libraryManager.deleteRecord(id)) {
                                std::cout << "Book deleted successfully!\n";
                            }
                        } else {
                            std::cout << "Deletion cancelled.\n";
                        }
                    } else {
                        std::cout << "Book with ID " << id << " not found.\n";
                    }
                    
                    pauseScreen();
                    break;
                }
                case 6: {
                    int sortChoice;
                    do {
                        displaySortMenu();
                        sortChoice = LibraryManager::getValidatedIntInput("", 0, 3);
                        
                        switch (sortChoice) {
                            case 1:
                                libraryManager.sortByTitle();
                                std::cout << "Books sorted by title. Use 'Display All Books' to see the sorted list.\n";
                                pauseScreen();
                                break;
                            case 2:
                                libraryManager.sortByAuthor();
                                std::cout << "Books sorted by author. Use 'Display All Books' to see the sorted list.\n";
                                pauseScreen();
                                break;
                            case 3:
                                libraryManager.sortByYear();
                                std::cout << "Books sorted by year. Use 'Display All Books' to see the sorted list.\n";
                                pauseScreen();
                                break;
                        }
                    } while (sortChoice != 0);
                    break;
                }
                case 7: {
                    clearScreen();
                    std::cout << "\n=== BORROW BOOK ===\n";
                    
                    libraryManager.displayAllRecords();
                    
                    int id = LibraryManager::getValidatedIntInput("Enter ID of book to borrow: ", 1, 9999);
                    libraryManager.borrowBook(id);
                    
                    pauseScreen();
                    break;
                }
                case 8: {
                    clearScreen();
                    std::cout << "\n=== RETURN BOOK ===\n";
                    
                    libraryManager.displayAllRecords();
                    
                    int id = LibraryManager::getValidatedIntInput("Enter ID of book to return: ", 1, 9999);
                    libraryManager.returnBook(id);
                    
                    pauseScreen();
                    break;
                }
                case 9: {
                    clearScreen();
                    std::cout << "\n=== EXPORT TO CSV ===\n";
                    
                    std::string filename = LibraryManager::getValidatedStringInput("Enter filename (e.g., books.csv): ");
                    
                    if (filename.find(".csv") == std::string::npos) {
                        filename += ".csv";
                    }
                    
                    if (libraryManager.exportToCSV(filename)) {
                        std::cout << "Export completed successfully!\n";
                    } else {
                        std::cout << "Export failed. Please check the filename and try again.\n";
                    }
                    
                    pauseScreen();
                    break;
                }
                case 10:
                    clearScreen();
                    libraryManager.displayStatistics();
                    pauseScreen();
                    break;
                    
                case 0:
                    clearScreen();
                    std::cout << "Thank you for using Library Management System!\n";
                    std::cout << "Data saved automatically.\n";
                    break;
                    
                default:
                    std::cout << "Invalid choice. Please try again.\n";
                    pauseScreen();
            }
        } while (choice != 0);
    }
};

//=============================================================================
// MAIN FUNCTION
//=============================================================================

int main() {
    try {
        Menu menu;
        menu.run();
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
