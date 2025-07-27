#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "Book.h"
#include <vector>
#include <string>

class DatabaseManager {
private:
    std::string connectionString;
    bool isConnected;
    
public:
    // Constructor
    DatabaseManager(const std::string& host = "localhost", 
                   const std::string& database = "library_db",
                   const std::string& username = "postgres", 
                   const std::string& password = "ValyK.336");
    
    // Destructor
    ~DatabaseManager();
    Valyk.336
    // Connection management
    bool connect();
    void disconnect();
    bool isConnectionActive() const { return isConnected; }
    
    // Database operations (to be implemented with libpq)
    bool createTables();
    bool insertBook(const Book& book);
    bool updateBook(const Book& book);
    bool deleteBook(int id);
    std::vector<Book> getAllBooks();
    Book getBookById(int id);
    std::vector<Book> searchBooks(const std::string& field, const std::string& value);
    
    // Utility methods
    bool testConnection();
    std::string getLastError() const;
    void backup(const std::string& filename);
    void restore(const std::string& filename);
};

// SQL queries for PostgreSQL integration
namespace SQL {
    const std::string CREATE_BOOKS_TABLE = R"(
        CREATE TABLE IF NOT EXISTS books (
            id SERIAL PRIMARY KEY,
            title VARCHAR(255) NOT NULL,
            author VARCHAR(255) NOT NULL,
            year INTEGER CHECK (year >= 1000 AND year <= 2030),
            isbn VARCHAR(20) UNIQUE NOT NULL,
            category VARCHAR(100),
            is_available BOOLEAN DEFAULT TRUE,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        );
    )";
    
    const std::string INSERT_BOOK = R"(
        INSERT INTO books (title, author, year, isbn, category, is_available) 
        VALUES ($1, $2, $3, $4, $5, $6) RETURNING id;
    )";
    
    const std::string UPDATE_BOOK = R"(
        UPDATE books SET title = $1, author = $2, year = $3, isbn = $4, 
                        category = $5, is_available = $6, updated_at = CURRENT_TIMESTAMP 
        WHERE id = $7;
    )";
    
    const std::string DELETE_BOOK = "DELETE FROM books WHERE id = $1;";
    const std::string SELECT_ALL = "SELECT * FROM books ORDER BY id;";
    const std::string SELECT_BY_ID = "SELECT * FROM books WHERE id = $1;";
    const std::string SEARCH_BY_TITLE = "SELECT * FROM books WHERE LOWER(title) LIKE LOWER($1);";
    const std::string SEARCH_BY_AUTHOR = "SELECT * FROM books WHERE LOWER(author) LIKE LOWER($1);";
    const std::string COUNT_TOTAL = "SELECT COUNT(*) FROM books;";
    const std::string COUNT_AVAILABLE = "SELECT COUNT(*) FROM books WHERE is_available = true;";
}

#endif // DATABASE_MANAGER_H

