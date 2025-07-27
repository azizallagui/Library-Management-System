#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>
#include <fstream>

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
    Book();
    Book(int id, const std::string& title, const std::string& author, 
         int year, const std::string& isbn, const std::string& category, bool available = true);
    
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
    
    // Utility methods
    void displayBook() const;
    std::string toString() const;
    std::string toCSV() const;
    
    // File I/O methods
    void writeToFile(std::ofstream& out) const;
    void readFromFile(std::ifstream& in);
    
    // Operators
    bool operator==(const Book& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Book& book);
};

#endif // BOOK_H
