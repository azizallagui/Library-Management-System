#ifndef MENU_H
#define MENU_H

#include "LibraryManager.h"

class Menu {
private:
    LibraryManager libraryManager;
    
    // Menu display methods
    void displayMainMenu() const;
    void displaySearchMenu() const;
    void displaySortMenu() const;
    
    // Menu action methods
    void handleAddRecord();
    void handleDisplayAll();
    void handleSearchMenu();
    void handleSearchById();
    void handleSearchByTitle();
    void handleSearchByAuthor();
    void handleDeleteRecord();
    void handleUpdateRecord();
    void handleSortMenu();
    void handleBorrowBook();
    void handleReturnBook();
    void handleExportCSV();
    void handleStatistics();
    
    // Utility methods
    void clearScreen() const;
    void pauseScreen() const;
    
public:
    // Constructor
    Menu();
    
    // Main menu loop
    void run();
};
