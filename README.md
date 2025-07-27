# Library Management System

A comprehensive file-based library management system built in C++ with Object-Oriented Programming principles. This system allows you to manage book records with full CRUD (Create, Read, Update, Delete) functionality, search capabilities, sorting options, and CSV export features.

## Features

### Core Functionality
- ✅ **Add Records**: Add new books with validation
- ✅ **Display All Records**: View all books in a formatted table
- ✅ **Search Records**: Search by ID, title, or author
- ✅ **Update Records**: Edit existing book information
- ✅ **Delete Records**: Remove books with confirmation
- ✅ **Sort Records**: Sort by title, author, or year
- ✅ **Export to CSV**: Export library data to CSV format

### Advanced Features
-  **Borrow/Return System**: Track book availability
-  **Statistics**: View library statistics
-  **Input Validation**: Comprehensive input validation and error handling
-  **Persistent Storage**: Binary file storage for efficiency
-  **User-Friendly Interface**: Clean CLI with menu-driven navigation

### Book Information
Each book record contains:
- **ID**: Unique identifier (auto-generated)
- **Title**: Book title
- **Author**: Author name
- **Year**: Publication year (1000-2030)
- **ISBN**: International Standard Book Number
- **Category**: Book category/genre
- **Availability**: Available/Borrowed status

## System Architecture

### Object-Oriented Design
The system follows OOP principles with the following classes:

1. **Book Class** (`Book.h`, `Book.cpp`)
   - Encapsulates book data and operations
   - Handles file I/O operations
   - Provides data validation methods

2. **LibraryManager Class** (`LibraryManager.h`, `LibraryManager.cpp`)
   - Core business logic
   - CRUD operations
   - Search and sort functionality
   - File management

3. **Menu Class** (`Menu.h`, `Menu.cpp`)
   - User interface management
   - Menu navigation
   - User input handling

## Getting Started

### Prerequisites
- C++ compiler with C++17 support (GCC, Clang, or MSVC)
- Make utility (optional, for easier building)

### Building the Project

#### Using Make 
```bash
# Build the project
make

# Build and run
make run

# Build debug version
make debug

# Build release version
make release

# Clean build files
make clean
```

#### Manual Compilation
```bash
# Create directories
mkdir -p obj bin

# Compile source files
g++ -std=c++17 -Wall -Wextra -O2 -c src/Book.cpp -o obj/Book.o
g++ -std=c++17 -Wall -Wextra -O2 -c src/LibraryManager.cpp -o obj/LibraryManager.o
g++ -std=c++17 -Wall -Wextra -O2 -c src/Menu.cpp -o obj/Menu.o
g++ -std=c++17 -Wall -Wextra -O2 -c src/main.cpp -o obj/main.o

# Link and create executable
g++ obj/*.o -o bin/library_manager
```

### Running the Application
```bash
# Run the executable
./bin/library_manager
```

### Main Menu Options

1. **Add New Book**
   - Enter book details (title, author, year, ISBN, category)
   - System validates input and prevents duplicates
   - Auto-generates unique ID

2. **Display All Books**
   - Shows all books in formatted table
   - Displays total count and availability status

3. **Search Books**
   - Search by ID (exact match)
   - Search by title (partial match)
   - Search by author (partial match)

4. **Update Book**
   - Select book by ID
   - Modify any field (press Enter to keep current value)
   - Validates new input

5. **Delete Book**
   - Select book by ID
   - Confirmation required before deletion

6. **Sort Books**
   - Sort by title (alphabetical)
   - Sort by author (alphabetical)
   - Sort by year (chronological)

7. **Borrow/Return Book**
   - Track book availability status
   - Prevent borrowing unavailable books

8. **Export to CSV**
   - Export all book data to CSV format
   - Specify custom filename

9. **Library Statistics**
   - Total books count
   - Available vs borrowed books

### Sample Book Data
For testing, you can add these sample books:

| Title | Author | Year | ISBN | Category |
|-------|--------|------|------|----------|
| The Great Gatsby | F. Scott Fitzgerald | 1925 | 9780743273565 | Fiction |
| To Kill a Mockingbird | Harper Lee | 1960 | 9780061120084 | Fiction |
| 1984 | George Orwell | 1949 | 9780451524935 | Dystopian |
| Pride and Prejudice | Jane Austen | 1813 | 9780141439518 | Romance |

## File Structure
```
task 55/
├── src/
│   ├── Book.h              # Book class header
│   ├── Book.cpp            # Book class implementation
│   ├── LibraryManager.h    # Library manager header
│   ├── LibraryManager.cpp  # Library manager implementation
│   ├── Menu.h              # Menu class header
│   ├── Menu.cpp            # Menu class implementation
│   └── main.cpp            # Main entry point
├── obj/                    # Object files (generated)
├── bin/                    # Executable (generated)
├── Makefile               # Build configuration
├── README.md              # This file
└── library_data.bin       # Binary data file (generated)
```

## Data Storage

### Binary File Format
- Books are stored in binary format for efficiency
- File: `library_data.bin`
- Auto-saves on program exit
- Auto-loads on program start

### CSV Export Format
```csv
ID,Title,Author,Year,ISBN,Category,Status
1,The Great Gatsby,F. Scott Fitzgerald,1925,9780743273565,Fiction,Available
```

## Input Validation

The system includes comprehensive input validation:

- **Year**: Must be between 1000 and 2030
- **ISBN**: Basic format validation (10 or 13 digits)
- **Text fields**: Non-empty validation
- **Numbers**: Range validation
- **Duplicate prevention**: ISBN uniqueness check

## Error Handling

- File I/O error handling
- Input validation with user-friendly messages
- Graceful handling of edge cases
- Memory management with RAII principles


### Planned Features
- **PostgreSQL Integration**: Database storage option
- **Advanced Search**: Multiple criteria search
- **Due Date Management**: Track borrowed book due dates
- **User Management**: Library member system
- **API Interface**: REST API for web integration
- **Authentication**: User roles and permissions

### Database Schema (PostgreSQL)
```sql
CREATE TABLE books (
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
```

## Development

### Code Style
- C++17 standard
- Object-oriented design principles
- RAII for resource management
- Const-correctness
- Exception safety

### Testing
The system has been tested with:
- Various input combinations
- Edge cases and error conditions
- File I/O operations
- Memory management

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request


### Main Menu
```
╔══════════════════════════════════════════════════════════════╗
║                    LIBRARY MANAGEMENT SYSTEM                ║
╠══════════════════════════════════════════════════════════════╣
║  1. Add New Book                                             ║
║  2. Display All Books                                        ║
║  3. Search Books                                             ║
║  4. Update Book                                              ║
║  5. Delete Book                                              ║
║  6. Sort Books                                               ║
║  7. Borrow Book                                              ║
║  8. Return Book                                              ║
║  9. Export to CSV                                            ║
║ 10. Library Statistics                                       ║
║  0. Exit                                                     ║
╚══════════════════════════════════════════════════════════════╝
