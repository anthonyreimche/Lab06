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
- Coordinate sorting algorithms:
  - Basic bubble sort
  - Optimized bubble sort with early termination

## Menu System

### Basic Menu Creation

```c
#include "selectionMenu.h"

SelectionMenu menu;
SelectionMenu_init(&menu);  // Initialize menu
const char* items[] = {"Option 1", "Option 2", "Option 3"};
int selection = SelectionMenu_showMenu(&menu, "Main Menu", items, 3);
```

### Menu Styles

```c
// Change menu style
SelectionMenu_setMenuType(&menu, MENU_CURSOR);      // For cursor-based navigation
SelectionMenu_setMenuType(&menu, MENU_CLASSIC);     // For numbered menu items
SelectionMenu_setMenuType(&menu, MENU_CONDENSED);   // For compact display
```

### Color Customization

```c
// Available colors: BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE
SelectionMenu_setMenuColor(COLOR_CYAN);
```

### Navigation

- Arrow Up/W: Move selection up
- Arrow Down/S: Move selection down
- Enter/D: Select item
- ESC: Exit menu
- Number keys: Direct selection (Classic mode)

## File Handling

The FileHandler module provides functions for reading and writing coordinate data in CSV format.

### Reading Coordinates

```c
#include "fileHandler.h"

int rows, cols;
double** coords = FileHandler_readCoordinates("input.csv", &rows, &cols);
if (coords) {
    // Process coordinates...
    FileHandler_freeCoordinates(coords, rows);
}
```

### Writing Coordinates

```c
double** coords = /* your coordinate array */;
FileHandler_saveCoordinates("output.csv", coords, rows, cols);
```

### File Operations

```c
// Find files with specific extension
int fileCount;
char** files = SelectionMenu_findFiles(&menu, "./data", ".txt", &fileCount);

// Create menu items from files
const char** menuItems = SelectionMenu_createMenuItems(&menu, files, fileCount);

// Clean up
SelectionMenu_freeMenuItems(menuItems, fileCount);
SelectionMenu_freeFileList(files, fileCount);
```

### CSV File Format

- Each line represents one coordinate
- Values are comma-separated
- Example:
  ```
  1.23,4.56
  7.89,0.12
  3.45,6.78
  ```

## Sorting Functionality

The library includes two sorting algorithms for coordinate data:

### Basic Bubble Sort

```c
SortStats stats = sortCoordinates(coords, rows, cols);
printf("Comparisons: %d, Swaps: %d\n", stats.comparisons, stats.swaps);
```

Features:
- Sorts coordinates based on magnitude (sum of components)
- Tracks number of comparisons and swaps
- Stable sort (maintains relative order of equal elements)

### Optimized Bubble Sort

```c
SortStats stats = optimisedSortCoordinates(coords, rows, cols);
printf("Comparisons: %d, Swaps: %d\n", stats.comparisons, stats.swaps);
```

Optimizations:
- Pre-calculates row sums to avoid redundant calculations
- Uses early termination when no swaps are needed
- Reduces the search range after each pass
- Typically 40-60% faster than basic bubble sort

### Coordinate Display Format

Coordinates are displayed in a standardized format:
```
[   x.xx ,    y.yy ]   magnitude:    z.zz
```
Where:
- Values are right-aligned in 8-character fields
- Two decimal places are shown
- Magnitude is displayed in cyan color
- No list numbers are shown

## Memory Management

The library handles memory allocation internally. Always use the provided free functions:
- `SelectionMenu_freeMenuItems()` for menu item arrays
- `SelectionMenu_freeFileList()` for file lists
- `FileHandler_freeCoordinates()` for coordinate arrays

## Error Handling

- File operations return NULL or empty arrays on failure
- Menu operations return 0 on ESC or error
- Selected item indices are 1-based (0 indicates error/exit)
- Memory allocation failures are reported via console messages
