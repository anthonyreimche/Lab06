/**
 * @file selectionMenu.c
 * @brief Implementation of menu system functions
 */

#include "selectionMenu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <stdarg.h>

static int g_menuColor = COLOR_GREEN;  // Default text color is green

void SelectionMenu_init(SelectionMenu* menu) {
    menu->currentMenuType = MENU_CLASSIC;
    SelectionMenu_resetColor();  // Apply the green color immediately
}

void SelectionMenu_setMenuType(SelectionMenu* menu, MenuType type) {
    menu->currentMenuType = type;
}

MenuType SelectionMenu_getMenuType(const SelectionMenu* menu) {
    return menu->currentMenuType;
}

void SelectionMenu_setMenuColor(int color) {
    g_menuColor = color;
}

int SelectionMenu_getMenuColor(void) {
    return g_menuColor;
}

void SelectionMenu_setColor(int textColor, int bgColor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgColor << 4) | textColor);
}

void SelectionMenu_resetColor(void) {
    SelectionMenu_setColor(g_menuColor, COLOR_BLACK);
}

void SelectionMenu_printColored(int textColor, const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    SelectionMenu_setColor(textColor, COLOR_BLACK);
    vprintf(format, args);
    SelectionMenu_resetColor();
    
    va_end(args);
}

void SelectionMenu_waitForKey(const char* message) {
    if (message) {
        printf("\n%s", message);
    }
    _getch();
}

void SelectionMenu_clearScreen(void) {
    system("cls");
}

static void displayTitle(const char* title) {
    printf("\n%s\n", title);
    for (size_t i = 0; i < strlen(title); i++) {
        printf("=");
    }
    printf("\n\n");
}

static void highlightText(const char* text) {
    SelectionMenu_setColor(COLOR_BLACK, COLOR_WHITE);
    printf("%s", text);
    SelectionMenu_resetColor();
}

static int getKey(void) {
    int key = _getch();
    if (key == 0 || key == 224) {  // Special key
        key = _getch();
    }
    return key;
}

static int handleNavigation(int selection, int nItems, int key) {
    switch (key) {
        case 72:  // Up arrow
        case 'w':
        case 'W':
            selection = (selection > 1) ? selection - 1 : nItems;
            break;
            
        case 80:  // Down arrow
        case 's':
        case 'S':
            selection = (selection < nItems) ? selection + 1 : 1;
            break;
            
        case 13:  // Enter
        case 'd':
        case 'D':
            return -selection;  // Return current selection
            
        case 27:  // Escape
        case 'q':
        case 'Q':
            return 0;   // Signal exit
            
        case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
            int num = key - '0';
            if (num <= nItems) {
                return -num;  // Return direct selection
            }
            break;
    }
    return selection;
}

static int generateClassicMenu(SelectionMenu* menu, const char* title, const char* items[], int nItems) {
    int selection = 1;
    char input[32];
    
    do {
        SelectionMenu_clearScreen();
        displayTitle(title);
        
        // Display menu items
        for (int i = 0; i < nItems; i++) {
            printf("%d. %s\n", i + 1, items[i]);
        }
        
        // Get user input
        printf("\nEnter your choice (1-%d, 0 to exit): ", nItems);
        if (fgets(input, sizeof(input), stdin)) {
            // Remove newline
            input[strcspn(input, "\n")] = 0;
            
            // Convert to integer
            selection = atoi(input);
            
            // Validate input
            if (selection >= 0 && selection <= nItems) {
                return selection;
            }
        }
        
        printf("\nInvalid choice. Press any key to try again...");
        _getch();
        
    } while (1);
}

static int generateCursorMenu(SelectionMenu* menu, const char* title, const char* items[], int nItems) {
    int selection = 1;
    int key;
    
    do {
        SelectionMenu_clearScreen();
        displayTitle(title);
        
        // Display menu items
        for (int i = 0; i < nItems; i++) {
            if (i + 1 == selection) {
                printf("> ");
                SelectionMenu_printColored(COLOR_GREEN, "%s\n", items[i]);
            } else {
                printf("  %s\n", items[i]);
            }
        }
        
        key = getKey();
        selection = handleNavigation(selection, nItems, key);
        
        if (selection <= 0) {
            return (selection < 0) ? -selection : 0;
        }
    } while (1);
}

static int generateCondensedMenu(SelectionMenu* menu, const char* title, const char* items[], int nItems) {
    int selection = 1;
    int key;
    int startItem = 0;
    const int ITEMS_PER_PAGE = 3;
    
    do {
        SelectionMenu_clearScreen();
        displayTitle(title);
        
        // Calculate visible range
        if (selection > startItem + ITEMS_PER_PAGE) {
            startItem = selection - ITEMS_PER_PAGE;
        } else if (selection <= startItem) {
            startItem = selection - 1;
        }
        
        // Display menu items
        for (int i = startItem; i < startItem + ITEMS_PER_PAGE && i < nItems; i++) {
            if (i + 1 == selection) {
                printf("> ");
                SelectionMenu_printColored(COLOR_GREEN, "%s\n", items[i]);
            } else {
                printf("  %s\n", items[i]);
            }
        }
        
        // Display navigation help
        if (startItem > 0) {
            printf("\n^ More items above");
        }
        if (startItem + ITEMS_PER_PAGE < nItems) {
            printf("\nv More items below");
        }
        
        key = getKey();
        selection = handleNavigation(selection, nItems, key);
        
        if (selection <= 0) {
            return (selection < 0) ? -selection : 0;
        }
    } while (1);
}

int SelectionMenu_showMenu(SelectionMenu* menu, const char* title, const char* items[], int nItems) {
    switch (menu->currentMenuType) {
        case MENU_CURSOR:
            return generateCursorMenu(menu, title, items, nItems);
        case MENU_CONDENSED:
            return generateCondensedMenu(menu, title, items, nItems);
        default:
            return generateClassicMenu(menu, title, items, nItems);
    }
}

char** SelectionMenu_findFiles(SelectionMenu* menu, const char* directory, const char* extension, int* fileCount, int maxFiles) {
    char searchPath[MAX_PATH_LENGTH];
    WIN32_FIND_DATA findData;
    HANDLE hFind;
    char** files = NULL;
    int count = 0;
    
    // Create search path with wildcard
    snprintf(searchPath, MAX_PATH_LENGTH, "%s\\*%s", directory, extension);
    
    // Start file search
    hFind = FindFirstFile(searchPath, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        *fileCount = 0;
        return NULL;
    }
    
    // Count matching files
    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            count++;
        }
    } while (count < maxFiles && FindNextFile(hFind, &findData));
    
    // Allocate memory for file list
    files = (char**)malloc(count * sizeof(char*));
    if (!files) {
        FindClose(hFind);
        *fileCount = 0;
        return NULL;
    }
    
    // Reset search and store filenames
    FindClose(hFind);
    hFind = FindFirstFile(searchPath, &findData);
    count = 0;
    
    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            files[count] = (char*)malloc(strlen(findData.cFileName) + 1);
            if (files[count]) {
                strcpy(files[count], findData.cFileName);
                count++;
            }
        }
    } while (count < maxFiles && FindNextFile(hFind, &findData));
    
    FindClose(hFind);
    *fileCount = count;
    return files;
}

const char** SelectionMenu_createMenuItems(SelectionMenu* menu, char** files, int fileCount, int maxPathLength) {
    const char** menuItems = (const char**)malloc(fileCount * sizeof(char*));
    if (!menuItems) return NULL;
    
    for (int i = 0; i < fileCount; i++) {
        menuItems[i] = files[i];
    }
    
    return menuItems;
}

void SelectionMenu_freeMenuItems(const char** menuItems, int count) {
    if (menuItems) {
        free((void*)menuItems);
    }
}

void SelectionMenu_freeFileList(char** files, int count) {
    if (files) {
        for (int i = 0; i < count; i++) {
            free(files[i]);
        }
        free(files);
    }
}

char* SelectionMenu_getOutputFilename(SelectionMenu* menu, const char* defaultName) {
    char* filename = (char*)malloc(MAX_PATH_LENGTH);
    if (!filename) return NULL;
    
    strncpy(filename, defaultName, MAX_PATH_LENGTH - 1);
    filename[MAX_PATH_LENGTH - 1] = '\0';
    
    return filename;
}

int SelectionMenu_fileExists(const char* filename) {
    FILE* file;
    if (fopen_s(&file, filename, "r") == 0) {
        fclose(file);
        return 1;
    }
    return 0;
}

char SelectionMenu_askYesNo(const char* question) {
    char response;
    printf("%s (y/n): ", question);
    do {
        response = (char)tolower(_getch());
    } while (response != 'y' && response != 'n');
    printf("%c\n", response);
    return response;
}
