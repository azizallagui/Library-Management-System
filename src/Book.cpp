#include "Book.h"
#include <sstream>
#include <iomanip>

// Default constructor
Book::Book() : id(0), title(""), author(""), year(0), isbn(""), category(""), isAvailable(true) {}

// Parameterized constructor
Book::Book(int id, const std::string& title, const std::string& author, 
           int year, const std::string& isbn, const std::string& category, bool available)
    : id(id), title(title), author(author), year(year), isbn(isbn), category(category), isAvailable(available) {}

// Display book information
void Book::displayBook() const {
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
std::string Book::toString() const {
    std::ostringstream oss;
    oss << "ID: " << id << ", Title: " << title << ", Author: " << author 
        << ", Year: " << year << ", ISBN: " << isbn << ", Category: " << category 
        << ", Status: " << (isAvailable ? "Available" : "Borrowed");
    return oss.str();
}

// Convert book to CSV format
std::string Book::toCSV() const {
    std::ostringstream oss;
    oss << id << "," << title << "," << author << "," << year << "," 
        << isbn << "," << category << "," << (isAvailable ? "Available" : "Borrowed");
    return oss.str();
}

// Write book data to binary file
void Book::writeToFile(std::ofstream& out) const {
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
void Book::readFromFile(std::ifstream& in) {
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
bool Book::operator==(const Book& other) const {
    return id == other.id;
}

// Stream output operator
std::ostream& operator<<(std::ostream& os, const Book& book) {
    os << book.toString();
    return os;
}
