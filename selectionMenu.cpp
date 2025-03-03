/**
 * @file selectionMenu.cpp
 * @brief Implementation of the SelectionMenu class
 */

#include "selectionMenu.h"
#include <stdio.h>
#include <conio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

// Key mapping constants
static const int KEY_UP = 72;      ///< Up arrow key code
static const int KEY_DOWN = 80;    ///< Down arrow key code
static const int KEY_ENTER = 13;   ///< Enter key code
static const int KEY_ESC = 27;     ///< Escape key code
static const int KEY_W = 'w';      ///< Alternative up key
static const int KEY_S = 's';      ///< Alternative down key
static const int KEY_D = 'd';      ///< Alternative enter key
static const int KEY_SPECIAL = 224; ///< Special key prefix
static const int KEY_EXTENDED = 0;  ///< Extended key prefix

// Initialize static member
int SelectionMenu::menuColor = COLOR_GREEN;

/**
 * @brief Constructor for SelectionMenu class
 */
SelectionMenu::SelectionMenu() {
    currentMenuType = CLASSIC;
    setColor(menuColor);  ///< Set initial color
}

/**
 * @brief Set the text color and background color
 * @param textColor Text color to set
 * @param bgColor Background color to set
 */
void SelectionMenu::setColor(int textColor, int bgColor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgColor << 4) | textColor);
}

/**
 * @brief Reset the text color to the default menu color
 */
void SelectionMenu::resetColor() {
    setColor(menuColor);
}

/**
 * @brief Print text with a specified color
 * @param textColor Text color to use
 * @param format Format string for text
 * @param ... Variable arguments for format string
 */
void SelectionMenu::printColored(int textColor, const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    setColor(textColor);
    vprintf(format, args);
    setColor(menuColor);
    
    va_end(args);
}

/**
 * @brief Set the menu color
 * @param color New menu color
 */
void SelectionMenu::setMenuColor(int color) {
    menuColor = color;
    setColor(menuColor);
}

/**
 * @brief Get the current menu color
 * @return Current menu color
 */
int SelectionMenu::getMenuColor() {
    return menuColor;
}

/**
 * @brief Set the menu type
 * @param type New menu type
 */
void SelectionMenu::setMenuType(MenuType type) {
    currentMenuType = type;
}

/**
 * @brief Get the current menu type
 * @return Current menu type
 */
MenuType SelectionMenu::getMenuType() const {
    return currentMenuType;
}

/**
 * @brief Wait for a key press and display a message
 * @param message Message to display
 */
void SelectionMenu::waitForKey(const char* message) {
    if (message) {
        printf("%s", message);
    }
    _getch();
}

/**
 * @brief Find files in a directory with a specified extension
 * @param directory Directory to search
 * @param extension File extension to search for
 * @param fileCount Pointer to store the number of files found
 * @param maxFiles Maximum number of files to find
 * @return Array of file names
 */
char** SelectionMenu::findFiles(const char* directory, const char* extension, int* fileCount, int maxFiles) {
    char** files = (char**)malloc(maxFiles * sizeof(char*));
    *fileCount = 0;
    
    // Create search pattern (e.g., "*.csv")
    char searchPattern[MAX_PATH_LENGTH];
    snprintf(searchPattern, MAX_PATH_LENGTH, "%s\\*%s", directory, extension);
    
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPattern, &findData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        return files;
    }
    
    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            files[*fileCount] = (char*)malloc(MAX_PATH_LENGTH);
            strncpy(files[*fileCount], findData.cFileName, MAX_PATH_LENGTH - 1);
            files[*fileCount][MAX_PATH_LENGTH - 1] = '\0';
            (*fileCount)++;
            
            if (*fileCount >= maxFiles) {
                break;
            }
        }
    } while (FindNextFile(hFind, &findData));
    
    FindClose(hFind);
    return files;
}

/**
 * @brief Create menu items from an array of file names
 * @param files Array of file names
 * @param fileCount Number of file names
 * @param maxPathLength Maximum length of a file name
 * @return Array of menu items
 */
const char** SelectionMenu::createMenuItems(char** files, int fileCount, int maxPathLength) {
    const char** menuItems = (const char**)malloc(fileCount * sizeof(char*));
    for (int i = 0; i < fileCount; i++) {
        if (currentMenuType == CURSOR) {
            char* menuItem = (char*)malloc(maxPathLength);
            strncpy(menuItem, files[i], maxPathLength - 1);
            menuItem[maxPathLength - 1] = '\0';
            menuItems[i] = menuItem;
        } else {
            char* menuItem = (char*)malloc(maxPathLength + 10);
            snprintf(menuItem, maxPathLength + 10, "%d. %s", i + 1, files[i]);
            menuItems[i] = menuItem;
        }
    }
    return menuItems;
}

/**
 * @brief Free an array of menu items
 * @param menuItems Array of menu items to free
 * @param count Number of menu items
 */
void SelectionMenu::freeMenuItems(const char** menuItems, int count) {
    for (int i = 0; i < count; i++) {
        free((void*)menuItems[i]);
    }
    free((void*)menuItems);
}

/**
 * @brief Free an array of file names
 * @param files Array of file names to free
 * @param count Number of file names
 */
void SelectionMenu::freeFileList(char** files, int count) {
    for (int i = 0; i < count; i++) {
        free(files[i]);
    }
    free(files);
}

/**
 * @brief Display a title on the screen
 * @param title Title to display
 */
void SelectionMenu::displayTitle(const char* title) {
    system("cls");
    printf("\n%s\n", title);
}

/**
 * @brief Highlight text on the screen
 * @param text Text to highlight
 */
void SelectionMenu::highlightText(const char* text) {
    setColor(COLOR_BLACK, menuColor);
    printf("%s", text);
    setColor(menuColor);
}

/**
 * @brief Helper function to check if a key is a special key
 * @param key Key code to check
 * @return true if key is special
 */
static int isSpecialKey(int key) {
    return key == KEY_SPECIAL || key == KEY_EXTENDED;
}

/**
 * @brief Helper function to check if a key is an up movement key
 * @param key Key code to check
 * @return true if key is up movement
 */
static int isUpKey(int key) {
    return key == KEY_UP || tolower(key) == KEY_W;
}

/**
 * @brief Helper function to check if a key is a down movement key
 * @param key Key code to check
 * @return true if key is down movement
 */
static int isDownKey(int key) {
    return key == KEY_DOWN || tolower(key) == KEY_S;
}

/**
 * @brief Helper function to check if a key is an enter/select key
 * @param key Key code to check
 * @return true if key is enter/select
 */
static int isEnterKey(int key) {
    return key == KEY_ENTER || tolower(key) == KEY_D;
}

/**
 * @brief Helper function to check if a key is a valid number selection
 * @param key Key code to check
 * @param nItems Number of menu items
 * @return true if key is valid number selection
 */
static int isNumberKey(int key, int nItems) {
    key = tolower(key);
    return key >= '1' && key <= '0' + nItems;
}

/**
 * @brief Helper function to convert number key to selection index
 * @param key Key code to convert
 * @return Zero-based selection index
 */
static int getNumberValue(int key) {
    return key - '1';
}

/**
 * @brief Helper function to check if a key is the escape key
 * @param key Key code to check
 * @return true if key is escape
 */
static int isEscKey(int key) {
    return key == KEY_ESC;
}

/**
 * @brief Get a key press from the user
 * @return Key code of the pressed key
 */
int SelectionMenu::getKey() {
    int key = _getch();
    if (isSpecialKey(key)) {
        key = _getch();
    }
    return key;
}

/**
 * @brief Handle navigation keys and update the selection
 * @param selection Current selection
 * @param nItems Number of menu items
 * @param key Key code of the pressed key
 * @return New selection
 */
int SelectionMenu::handleNavigation(int selection, int nItems, int key) {
    if (isUpKey(key)) {
        return (selection > 1) ? selection - 1 : nItems;
    }
    if (isDownKey(key)) {
        return (selection < nItems) ? selection + 1 : 1;
    }
    if (isEnterKey(key)) {
        return -1;
    }
    if (isEscKey(key)) {
        return 0;  ///< Return 0 for ESC key
    }
    if (isNumberKey(key, nItems)) {
        return getNumberValue(key) + 1;
    }
    return selection;
}

/**
 * @brief Ask the user a yes/no question
 * @param question Question to ask
 * @return 'y' if yes, 'n' if no
 */
char SelectionMenu::askYesNo(const char* question) {
    printf("\n%s (y/n): ", question);
    char response;
    do {
        response = tolower(_getch());
    } while (response != 'y' && response != 'n');
    printf("%c\n", response);
    return response;
}

/**
 * @brief Clear the screen
 */
void SelectionMenu::clearScreen() {
    system("cls");
}

/**
 * @brief Check if a file exists
 * @param filename File name to check
 * @return true if file exists
 */
bool SelectionMenu::fileExists(const char* filename) {
    FILE* file;
    if (fopen_s(&file, filename, "r") == 0) {
        fclose(file);
        return true;
    }
    return false;
}

/**
 * @brief Get the output file name
 * @param defaultName Default file name
 * @return File name
 */
char* SelectionMenu::getOutputFilename(const char* defaultName) {
    char* filename = (char*)malloc(MAX_PATH_LENGTH);
    strncpy(filename, defaultName, MAX_PATH_LENGTH - 1);
    filename[MAX_PATH_LENGTH - 1] = '\0';
    return filename;
}

/**
 * @brief Show a menu to the user
 * @param title Menu title
 * @param items Array of menu items
 * @param nItems Number of menu items
 * @return Selected menu item index
 */
int SelectionMenu::showMenu(const char* title, const char* items[], int nItems) {
    switch (currentMenuType) {
        case CURSOR:
            return generateCursorMenu(title, items, nItems);
        case CONDENSED:
            return generateCondensedMenu(title, items, nItems);
        default:
            return generateClassicMenu(title, items, nItems);
    }
}

/**
 * @brief Generate a classic menu
 * @param title Menu title
 * @param items Array of menu items
 * @param nItems Number of menu items
 * @return Selected menu item index
 */
int SelectionMenu::generateClassicMenu(const char* title, const char* items[], int nItems) {
    int selection = 0;
    
    while (1) {
        displayTitle(title);
        
        for (int i = 0; i < nItems; i++) {
            printf("%s\n", items[i]);
        }
        
        printf("\nEnter your choice (1-%d): ", nItems);
        int key = _getch();
        
        if (isNumberKey(key, nItems)) {
            selection = getNumberValue(key) + 1;
            printf("%d", selection);  ///< Echo the number
            
            // Wait for enter key
            while ((key = _getch()) != KEY_ENTER) {
                // Ignore all other keys
            }
            return selection;
        }
    }
}

/**
 * @brief Generate a condensed menu
 * @param title Menu title
 * @param items Array of menu items
 * @param nItems Number of menu items
 * @return Selected menu item index
 */
int SelectionMenu::generateCondensedMenu(const char* title, const char* items[], int nItems) {
    int currentSelection = 1;
    int selected = 0;
    
    while (!selected) {
        displayTitle(title);
        
        // Show options
        printf("Options (");
        for (int i = 0; i < nItems; i++) {
            printf("%d", i + 1);
            if (i < nItems - 1) printf(",");
        }
        printf(")\n\n");
        
        // Show preview of current selection
        printf("Selected: ");
        setColor(COLOR_BLACK, menuColor);
        printf("%s", items[currentSelection - 1]);
        setColor(menuColor);
        printf("\n\n");
        
        printf("Enter number or use W/S to navigate, Enter to select, ESC to go back\n");
        
        int newSelection = handleNavigation(currentSelection, nItems, getKey());
        if (newSelection == -1) {
            selected = 1;
        } else if (newSelection == 0) {
            return 0;  ///< Return 0 for ESC key
        } else {
            currentSelection = newSelection;
        }
    }
    
    clearScreen();
    return currentSelection;
}

/**
 * @brief Generate a cursor menu
 * @param title Menu title
 * @param items Array of menu items
 * @param nItems Number of menu items
 * @return Selected menu item index
 */
int SelectionMenu::generateCursorMenu(const char* title, const char* items[], int nItems) {
    int currentSelection = 1;
    int selected = 0;
    
    while (!selected) {
        displayTitle(title);
        
        // Show all items
        for (int i = 0; i < nItems; i++) {
            if (i + 1 == currentSelection) {
                setColor(COLOR_BLACK, menuColor);
                printf("%s\n", items[i]);
                setColor(menuColor);
            } else {
                printf("%s\n", items[i]);
            }
        }
        
        printf("\nUse arrow keys to preview, Enter/D to confirm, ESC to go back\n");
        
        int key = getKey();
        if (isEnterKey(key)) {
            selected = 1;
        } else if (isEscKey(key)) {
            return 0;  ///< Return 0 for ESC key
        } else {
            int newSelection = handleNavigation(currentSelection, nItems, key);
            if (newSelection != currentSelection) {
                currentSelection = newSelection;
                clearScreen();
            }
        }
    }
    
    return currentSelection;
}
