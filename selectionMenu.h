/**
 * @file selectionMenu.h
 * @brief A versatile menu system for console applications with multiple display styles
 */

#ifndef SELECTION_MENU_H
#define SELECTION_MENU_H

#include <stdio.h>
#include <windows.h>

/** Maximum length for file paths */
#define MAX_PATH_LENGTH 256
/** Maximum number of files to process */
#define MAX_FILES 100

/** Available console colors for menu customization */
#define COLOR_BLACK     0
#define COLOR_BLUE      9
#define COLOR_GREEN     10
#define COLOR_CYAN      11
#define COLOR_RED       12
#define COLOR_MAGENTA   13
#define COLOR_YELLOW    14
#define COLOR_WHITE     15

/** Menu display styles */
typedef enum {
    MENU_CLASSIC,    ///< Numbered menu items (1, 2, 3...)
    MENU_CURSOR,     ///< Interactive cursor-based selection
    MENU_CONDENSED   ///< Compact display with cursor navigation
} MenuType;

/**
 * @struct SelectionMenu
 * @brief Menu state and configuration
 */
typedef struct {
    MenuType currentMenuType;
} SelectionMenu;

/**
 * @brief Initialize a new menu with default settings
 * @param menu Pointer to menu struct to initialize
 */
void SelectionMenu_init(SelectionMenu* menu);

/**
 * @brief Display a menu and handle user interaction
 * @param menu Menu instance
 * @param title Menu title
 * @param items Array of menu items
 * @param nItems Number of items
 * @return Selected item index (1-based) or 0 for exit
 */
int SelectionMenu_showMenu(SelectionMenu* menu, const char* title, const char* items[], int nItems);

/**
 * @brief Set the menu display style
 * @param menu Menu instance
 * @param type Desired menu style
 */
void SelectionMenu_setMenuType(SelectionMenu* menu, MenuType type);

/**
 * @brief Get the current menu display style
 * @param menu Menu instance
 * @return Current MenuType
 */
MenuType SelectionMenu_getMenuType(const SelectionMenu* menu);

/**
 * @brief Set the menu text color
 * @param color Color constant to use
 */
void SelectionMenu_setMenuColor(int color);

/**
 * @brief Get the current menu text color
 * @return Current color constant
 */
int SelectionMenu_getMenuColor(void);

/**
 * @brief Find files with specific extension in a directory
 * @param menu Menu instance
 * @param directory Directory to search in
 * @param extension File extension to filter by
 * @param fileCount Output parameter for number of files found
 * @param maxFiles Maximum number of files to find
 * @return Array of filenames (must be freed with SelectionMenu_freeFileList)
 */
char** SelectionMenu_findFiles(SelectionMenu* menu, const char* directory, const char* extension, int* fileCount, int maxFiles);

/**
 * @brief Create menu items from file list
 * @param menu Menu instance
 * @param files Array of filenames
 * @param fileCount Number of files
 * @param maxPathLength Maximum length for each menu item
 * @return Array of menu items (must be freed with SelectionMenu_freeMenuItems)
 */
const char** SelectionMenu_createMenuItems(SelectionMenu* menu, char** files, int fileCount, int maxPathLength);

/**
 * @brief Free memory allocated for menu items
 * @param menuItems Array of menu items to free
 * @param count Number of items
 */
void SelectionMenu_freeMenuItems(const char** menuItems, int count);

/**
 * @brief Free memory allocated for file list
 * @param files Array of filenames to free
 * @param count Number of files
 */
void SelectionMenu_freeFileList(char** files, int count);

/**
 * @brief Generate an output filename
 * @param menu Menu instance
 * @param defaultName Default filename to use
 * @return Generated filename (must be freed by caller)
 */
char* SelectionMenu_getOutputFilename(SelectionMenu* menu, const char* defaultName);

/**
 * @brief Set console text and background colors
 * @param textColor Text color constant
 * @param bgColor Background color constant
 */
void SelectionMenu_setColor(int textColor, int bgColor);

/**
 * @brief Reset colors to default menu color
 */
void SelectionMenu_resetColor(void);

/**
 * @brief Print text in specified color
 * @param textColor Color to use
 * @param format Printf-style format string
 */
void SelectionMenu_printColored(int textColor, const char* format, ...);

/**
 * @brief Wait for keypress with optional message
 * @param message Message to display (NULL for no message)
 */
void SelectionMenu_waitForKey(const char* message);

/**
 * @brief Clear the console screen
 */
void SelectionMenu_clearScreen(void);

/**
 * @brief Check if a file exists
 * @param filename Path to file
 * @return true if file exists
 */
int SelectionMenu_fileExists(const char* filename);

/**
 * @brief Prompt for yes/no input
 * @param question Question to display
 * @return 'y' or 'n'
 */
char SelectionMenu_askYesNo(const char* question);

#endif // SELECTION_MENU_H
