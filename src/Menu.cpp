#include "Menu.h"
#include <iostream>
#include <limits>
#include <cstdlib>

// Constructor
Menu::Menu() : libraryManager("library_data.bin") {}

// Clear screen (cross-platform)
void Menu::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Pause screen
void Menu::pauseScreen() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Display main menu
void Menu::displayMainMenu() const {
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
void Menu::displaySearchMenu() const {
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
void Menu::displaySortMenu() const {
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

// Handle add record
void Menu::handleAddRecord() {
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
}

// Handle display all records
void Menu::handleDisplayAll() {
    clearScreen();
    libraryManager.displayAllRecords();
    pauseScreen();
}

// Handle search menu
void Menu::handleSearchMenu() {
    int choice;
    do {
        displaySearchMenu();
        choice = LibraryManager::getValidatedIntInput("", 0, 3);
        
        switch (choice) {
            case 1:
                handleSearchById();
                break;
            case 2:
                handleSearchByTitle();
                break;
            case 3:
                handleSearchByAuthor();
                break;
            case 0:
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                pauseScreen();
        }
    } while (choice != 0);
}

// Handle search by ID
void Menu::handleSearchById() {
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
}

// Handle search by title
void Menu::handleSearchByTitle() {
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
}

// Handle search by author
void Menu::handleSearchByAuthor() {
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
}

// Handle delete record
void Menu::handleDeleteRecord() {
    clearScreen();
    std::cout << "\n=== DELETE BOOK ===\n";
    
    // First, show current books
    libraryManager.displayAllRecords();
    
    int id = LibraryManager::getValidatedIntInput("Enter ID of book to delete: ", 1, 9999);
    
    // Confirm deletion
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
}

// Handle update record
void Menu::handleUpdateRecord() {
    clearScreen();
    std::cout << "\n=== UPDATE BOOK ===\n";
    
    // Show current books
    libraryManager.displayAllRecords();
    
    int id = LibraryManager::getValidatedIntInput("Enter ID of book to update: ", 1, 9999);
    
    if (libraryManager.updateRecord(id)) {
        std::cout << "Book updated successfully!\n";
    }
    
    pauseScreen();
}

// Handle sort menu
void Menu::handleSortMenu() {
    int choice;
    do {
        displaySortMenu();
        choice = LibraryManager::getValidatedIntInput("", 0, 3);
        
        switch (choice) {
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
            case 0:
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                pauseScreen();
        }
    } while (choice != 0);
}

// Handle borrow book
void Menu::handleBorrowBook() {
    clearScreen();
    std::cout << "\n=== BORROW BOOK ===\n";
    
    // Show available books
    libraryManager.displayAllRecords();
    
    int id = LibraryManager::getValidatedIntInput("Enter ID of book to borrow: ", 1, 9999);
    libraryManager.borrowBook(id);
    
    pauseScreen();
}

// Handle return book
void Menu::handleReturnBook() {
    clearScreen();
    std::cout << "\n=== RETURN BOOK ===\n";
    
    // Show all books
    libraryManager.displayAllRecords();
    
    int id = LibraryManager::getValidatedIntInput("Enter ID of book to return: ", 1, 9999);
    libraryManager.returnBook(id);
    
    pauseScreen();
}

// Handle export to CSV
void Menu::handleExportCSV() {
    clearScreen();
    std::cout << "\n=== EXPORT TO CSV ===\n";
    
    std::string filename = LibraryManager::getValidatedStringInput("Enter filename (e.g., books.csv): ");
    
    // Add .csv extension if not present
    if (filename.find(".csv") == std::string::npos) {
        filename += ".csv";
    }
    
    if (libraryManager.exportToCSV(filename)) {
        std::cout << "Export completed successfully!\n";
    } else {
        std::cout << "Export failed. Please check the filename and try again.\n";
    }
    
    pauseScreen();
}

// Handle statistics
void Menu::handleStatistics() {
    clearScreen();
    libraryManager.displayStatistics();
    pauseScreen();
}

// Main menu loop
void Menu::run() {
    int choice;
    
    std::cout << "Welcome to Library Management System!\n";
    std::cout << "Loading existing data...\n";
    pauseScreen();
    
    do {
        displayMainMenu();
        choice = LibraryManager::getValidatedIntInput("", 0, 10);
        
        switch (choice) {
            case 1:
                handleAddRecord();
                break;
            case 2:
                handleDisplayAll();
                break;
            case 3:
                handleSearchMenu();
                break;
            case 4:
                handleUpdateRecord();
                break;
            case 5:
                handleDeleteRecord();
                break;
            case 6:
                handleSortMenu();
                break;
            case 7:
                handleBorrowBook();
                break;
            case 8:
                handleReturnBook();
                break;
            case 9:
                handleExportCSV();
                break;
            case 10:
                handleStatistics();
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
