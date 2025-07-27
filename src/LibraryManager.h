#ifndef LIBRARY_MANAGER_H
#define LIBRARY_MANAGER_H

#include "Book.h"
#include <vector>
#include <string>
#include <functional>

class LibraryManager {
private:
    std::vector<Book> books;
    std::string dataFile;
    int nextId;
    
    // Private helper methods
    int generateNextId();
    bool isValidYear(int year) const;
    bool isValidISBN(const std::string& isbn) const;
    void loadBooksFromFile();
    void saveBooksToFile();
    
public:
    // Constructor
    LibraryManager(const std::string& filename = "library_data.bin");
    
    // Destructor
    ~LibraryManager();
    
    // Core CRUD operations
    bool addRecord(const std::string& title, const std::string& author, 
                   int year, const std::string& isbn, const std::string& category);
    void displayAllRecords() const;
    Book* searchRecordByID(int id);
    std::vector<Book*> searchRecordsByTitle(const std::string& title);
    std::vector<Book*> searchRecordsByAuthor(const std::string& author);
    bool deleteRecord(int id);
    bool updateRecord(int id);
    
    // Advanced operations
    void sortByTitle();
    void sortByAuthor();
    void sortByYear();
    void sortBy(std::function<bool(const Book&, const Book&)> comparator);
    
    // Export/Import operations
    bool exportToCSV(const std::string& filename) const;
    bool importFromCSV(const std::string& filename);
    
    // Utility methods
    int getTotalBooks() const { return books.size(); }
    int getAvailableBooks() const;
    int getBorrowedBooks() const;
    void displayStatistics() const;
    bool borrowBook(int id);
    bool returnBook(int id);
    
    // Input validation helpers
    static bool validateInput(const std::string& input, const std::string& type);
    static int getValidatedIntInput(const std::string& prompt, int min = 0, int max = 9999);
    static std::string getValidatedStringInput(const std::string& prompt, bool allowEmpty = false);
};

#endif // LIBRARY_MANAGER_H
