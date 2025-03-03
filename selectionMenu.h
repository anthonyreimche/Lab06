#ifndef SELECTION_MENU_H
#define SELECTION_MENU_H

#include <stdio.h>
#include <windows.h>

// Constants
#define MAX_PATH_LENGTH 256
#define MAX_FILES 100

// Color constants
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_YELLOW 6
#define COLOR_WHITE 7

// Enums
enum MenuType { CLASSIC, CURSOR, CONDENSED };

// Menu interface
class SelectionMenu {
private:
    MenuType currentMenuType;
    static int menuColor;  // Default text color
    
    // Helper functions
    void displayTitle(const char* title);
    void highlightText(const char* text);
    int handleNavigation(int selection, int nItems, int key);
    int getKey();
    
    // Menu implementations
    int generateClassicMenu(const char* title, const char* items[], int nItems);
    int generateCursorMenu(const char* title, const char* items[], int nItems);
    int generateCondensedMenu(const char* title, const char* items[], int nItems);
    
public:
    // Constructor
    SelectionMenu();
    
    // Menu generation methods
    int showMenu(const char* title, const char* items[], int nItems);
    void setMenuType(MenuType type);
    MenuType getMenuType() const;
    
    // Color methods
    static void setMenuColor(int color);
    static int getMenuColor();
    
    // File handling methods
    char** findFiles(const char* directory, const char* extension, int* fileCount, int maxFiles = MAX_FILES);
    const char** createMenuItems(char** files, int fileCount, int maxPathLength = MAX_PATH_LENGTH);
    void freeMenuItems(const char** menuItems, int count);
    void freeFileList(char** files, int count);
    char* getOutputFilename(const char* defaultName);
    
    // Utility methods
    static void setColor(int textColor, int bgColor = 0);
    static void resetColor();
    static void printColored(int textColor, const char* format, ...);
    static void waitForKey(const char* message = "Press any key to continue...");
    static void clearScreen();
    static bool fileExists(const char* filename);
    static char askYesNo(const char* question);
};

#endif // SELECTION_MENU_H
