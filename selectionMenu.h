/**
 * @file selectionMenu.h
 * @brief A versatile menu system for console applications with multiple display styles and file handling
 */

#ifndef SELECTION_MENU_H
#define SELECTION_MENU_H

#include <stdio.h>
#include <windows.h>

// Constants
/** Maximum length for file paths */
#define MAX_PATH_LENGTH 256
/** Maximum number of files to process */
#define MAX_FILES 100

// Color constants
/** Available console colors for menu customization */
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_YELLOW 6
#define COLOR_WHITE 7

/** Menu display styles */
enum MenuType { 
    CLASSIC,    ///< Numbered menu items (1, 2, 3...)
    CURSOR,     ///< Interactive cursor-based selection
    CONDENSED   ///< Compact display with cursor navigation
};

/**
 * @class SelectionMenu
 * @brief Provides an interactive menu system with multiple display styles and file handling capabilities
 */
class SelectionMenu {
private:
    MenuType currentMenuType;
    static int menuColor;  // Default text color
    
    /** @brief Displays the menu title with proper formatting
     * @param title The title text to display */
    void displayTitle(const char* title);
    
    /** @brief Highlights text for cursor-based menu styles
     * @param text The text to highlight */
    void highlightText(const char* text);
    
    /** @brief Processes keyboard input for menu navigation
     * @param selection Current selected item
     * @param nItems Total number of menu items
     * @param key The key that was pressed
     * @return New selection index or special value (-1 for enter, 0 for escape) */
    int handleNavigation(int selection, int nItems, int key);
    
    /** @brief Gets a keypress, handling special keys
     * @return The key code */
    int getKey();
    
    /** @brief Generates a classic numbered menu
     * @param title Menu title
     * @param items Array of menu items
     * @param nItems Number of items
     * @return Selected item index (1-based) or 0 for exit */
    int generateClassicMenu(const char* title, const char* items[], int nItems);
    
    /** @brief Generates a cursor-based menu
     * @param title Menu title
     * @param items Array of menu items
     * @param nItems Number of items
     * @return Selected item index (1-based) or 0 for exit */
    int generateCursorMenu(const char* title, const char* items[], int nItems);
    
    /** @brief Generates a condensed menu
     * @param title Menu title
     * @param items Array of menu items
     * @param nItems Number of items
     * @return Selected item index (1-based) or 0 for exit */
    int generateCondensedMenu(const char* title, const char* items[], int nItems);
    
public:
    /** @brief Initializes menu with default style and color */
    SelectionMenu();
    
    /** @brief Displays a menu and handles user interaction
     * @param title Menu title
     * @param items Array of menu items
     * @param nItems Number of items
     * @return Selected item index (1-based) or 0 for exit */
    int showMenu(const char* title, const char* items[], int nItems);
    
    /** @brief Sets the menu display style
     * @param type The desired menu style */
    void setMenuType(MenuType type);
    
    /** @brief Gets the current menu display style
     * @return Current MenuType */
    MenuType getMenuType() const;
    
    /** @brief Sets the menu text color
     * @param color Color constant to use */
    static void setMenuColor(int color);
    
    /** @brief Gets the current menu text color
     * @return Current color constant */
    static int getMenuColor();
    
    /** @brief Searches for files with specific extension
     * @param directory Directory to search in
     * @param extension File extension to filter by
     * @param fileCount Output parameter for number of files found
     * @param maxFiles Maximum number of files to return
     * @return Array of filenames (must be freed with freeFileList) */
    char** findFiles(const char* directory, const char* extension, int* fileCount, int maxFiles = MAX_FILES);
    
    /** @brief Creates menu items from file list
     * @param files Array of filenames
     * @param fileCount Number of files
     * @param maxPathLength Maximum length for each menu item
     * @return Array of menu items (must be freed with freeMenuItems) */
    const char** createMenuItems(char** files, int fileCount, int maxPathLength = MAX_PATH_LENGTH);
    
    /** @brief Frees memory allocated for menu items
     * @param menuItems Array of menu items to free
     * @param count Number of items */
    void freeMenuItems(const char** menuItems, int count);
    
    /** @brief Frees memory allocated for file list
     * @param files Array of filenames to free
     * @param count Number of files */
    void freeFileList(char** files, int count);
    
    /** @brief Generates an output filename
     * @param defaultName Default filename to use
     * @return Generated filename (must be freed by caller) */
    char* getOutputFilename(const char* defaultName);
    
    /** @brief Sets console text and background colors
     * @param textColor Text color constant
     * @param bgColor Background color constant */
    static void setColor(int textColor, int bgColor = 0);
    
    /** @brief Resets colors to default menu color */
    static void resetColor();
    
    /** @brief Prints text in specified color
     * @param textColor Color to use
     * @param format Printf-style format string
     * @param ... Format arguments */
    static void printColored(int textColor, const char* format, ...);
    
    /** @brief Waits for keypress with optional message
     * @param message Message to display (nullptr for no message) */
    static void waitForKey(const char* message = "Press any key to continue...");
    
    /** @brief Clears the console screen */
    static void clearScreen();
    
    /** @brief Checks if a file exists
     * @param filename Path to file
     * @return true if file exists */
    static bool fileExists(const char* filename);
    
    /** @brief Prompts for yes/no input
     * @param question Question to display
     * @return 'y' or 'n' */
    static char askYesNo(const char* question);
};

#endif // SELECTION_MENU_H
