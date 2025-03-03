# Selection Menu Library

A versatile C menu library that provides multiple menu styles and file handling utilities for console applications.

## Features

- Three menu styles:
  - Classic: Numbered menu items (1, 2, 3...)
  - Cursor: Interactive cursor-based selection
  - Condensed: Compact display with cursor navigation
- Color support with customizable menu colors
- File handling utilities:
  - File search with extension filtering
  - File existence checking
  - Output filename generation
- Cross-platform keyboard input handling
- Memory-safe dynamic allocation

## Usage

### Basic Menu Creation

```c
#include "selectionMenu.h"

SelectionMenu menu;
const char* items[] = {"Option 1", "Option 2", "Option 3"};
int selection = menu.showMenu("Main Menu", items, 3);
```

### Menu Styles

```c
// Change menu style
menu.setMenuType(CURSOR);      // For cursor-based navigation
menu.setMenuType(CLASSIC);     // For numbered menu items
menu.setMenuType(CONDENSED);   // For compact display
```

### Color Customization

```c
// Available colors: BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE
SelectionMenu_setMenuColor(COLOR_CYAN);
```

### File Operations

```c
// Find files with specific extension
int fileCount;
char** files = menu.findFiles("./data", ".txt", &fileCount);

// Create menu items from files
const char** menuItems = menu.createMenuItems(files, fileCount);

// Clean up
menu.freeMenuItems(menuItems, fileCount);
menu.freeFileList(files, fileCount);
```

## Navigation

- Arrow Up/W: Move selection up
- Arrow Down/S: Move selection down
- Enter/D: Select item
- ESC: Exit menu
- Number keys: Direct selection (Classic mode)

## Memory Management

The library handles memory allocation internally. Always use the provided free functions:
- `freeMenuItems()` for menu item arrays
- `freeFileList()` for file lists

## Error Handling

- File operations return NULL or empty arrays on failure
- Menu operations return 0 on ESC or error
- Selected item indices are 1-based (0 indicates error/exit)
