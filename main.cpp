/**
 * @file main.c
 * @brief Coordinate sorting visualization program
 * 
 * This program provides an interactive menu system for visualizing different
 * sorting algorithms applied to 2D coordinate data. It supports multiple menu
 * styles and provides detailed statistics about sorting operations.
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "selectionMenu.h"
#include "fileHandler.h"

// Menu configuration
const char* MENU_TITLE = "Lab 6: Sorting Algorithm Visualizer";
const char* MENU_ITEMS[] = {
    "Bubble Sort",
    "Optimised Sort",
    "Settings",
    "Exit"
};
const int NUM_MENU_ITEMS = 4;

/** Enum for accessing coordinate components */
typedef enum {
    COORD_X,
    COORD_Y
} CoordinateAxis;

/**
 * @struct SortStats
 * @brief Statistics collected during sorting operations
 */
typedef struct {
    int comparisons;  ///< Number of comparisons performed
    int swaps;        ///< Number of swaps performed
} SortStats;

// Global menu instance
SelectionMenu g_menu;

// Forward declarations
void menuSettings(void);
void displayMenu(void);
void bubbleSort(void);
void optimisedSort(void);
SortStats sortCoordinates(double** coordinates, int n, int m);
SortStats optimisedSortCoordinates(double** coordinates, int n, int m);
double** read2DArray(const char* filename, int* n, int* m);
int saveCoordinatesToFile(const char* filename, double** coordinates, int n, int m);

/**
 * @brief Handles menu style settings
 * 
 * Allows the user to switch between different menu display styles:
 * - Classic (numbered list)
 * - Cursor (interactive arrow selection)
 * - Condensed (compact display)
 */
void menuSettings(void) {
    const char* settingsItems[] = {
        "Classic",
        "Cursor",
        "Condensed"
    };
    
    // Use current menu style for settings
    MenuType currentType = SelectionMenu_getMenuType(&g_menu);
    int choice = SelectionMenu_showMenu(&g_menu, "Menu Settings", settingsItems, 3);
    
    // Apply selected style
    switch (choice) {
        case 1:
            SelectionMenu_setMenuType(&g_menu, MENU_CLASSIC);
            SelectionMenu_printColored(COLOR_RED, "\nChanged to Classic Menu\n");
            break;
        case 2:
            SelectionMenu_setMenuType(&g_menu, MENU_CURSOR);
            SelectionMenu_printColored(COLOR_RED, "\nChanged to Cursor Menu\n");
            break;
        case 3:
            SelectionMenu_setMenuType(&g_menu, MENU_CONDENSED);
            SelectionMenu_printColored(COLOR_RED, "\nChanged to Condensed Menu\n");
            break;
    }
    
    SelectionMenu_waitForKey(NULL);
}

/**
 * @brief Calculates the sum of values in a row
 * 
 * Used as the comparison metric for sorting coordinates.
 * 
 * @param row Array of values to sum
 * @param m Number of elements in the row
 * @return Sum of all elements
 */
double calculateRowSum(double* row, int m) {
    double sum = 0;
    for (int j = 0; j < m; j++) {
        sum += row[j];
    }
    return sum;
}

/**
 * @brief Sorts coordinates using bubble sort algorithm
 * 
 * Sorts the coordinates based on the sum of their components.
 * Tracks and returns statistics about the sorting operation.
 * 
 * @param coordinates 2D array of coordinates to sort
 * @param n Number of coordinates
 * @param m Number of components per coordinate
 * @return Statistics about the sorting operation
 */
SortStats sortCoordinates(double** coordinates, int n, int m) {
    SortStats stats = {0, 0};  // Initialize counters
    
    // Bubble sort based on row sums
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            double sum1 = calculateRowSum(coordinates[j], m);
            double sum2 = calculateRowSum(coordinates[j + 1], m);
            
            stats.comparisons++;  // Count each comparison
            if (sum1 > sum2) {
                // Swap rows
                double* temp = coordinates[j];
                coordinates[j] = coordinates[j + 1];
                coordinates[j + 1] = temp;
                stats.swaps++;  // Count each swap
            }
        }
    }
    return stats;
}

/**
 * @brief Displays a single coordinate with proper formatting
 * 
 * @param row Array containing the coordinate components
 * @param m Number of components
 */
void displayCoordinate(double* row, int m) {
    printf("[");
    for (int j = 0; j < m; j++) {
        printf("%8.2f%s", row[j], j < m-1 ? " ," : " ]\n");
    }
}

/**
 * @brief Displays a coordinate with its sum
 * 
 * Formats the coordinate according to the user's preferred style:
 * [   x.xx ,    y.yy ]   sum:    z.zz
 * 
 * @param row Array containing the coordinate components
 * @param m Number of components
 */
void displayCoordinateWithSum(double* row, int m) {
    printf("[");
    for (int j = 0; j < m; j++) {
        printf("%8.2f%s", row[j], j < m-1 ? " ," : " ]");
    }
    SelectionMenu_printColored(COLOR_CYAN, "   sum: %8.2f\n", calculateRowSum(row, m));
}

/**
 * @brief Handles the bubble sort visualization option
 * 
 * Allows the user to:
 * 1. Select a CSV file
 * 2. View the original coordinates
 * 3. Sort the coordinates using bubble sort
 * 4. View sorting statistics
 * 5. Save the sorted coordinates
 */
void bubbleSort(void) {
    char** files = NULL;
    const char** menuItems = NULL;
    int fileCount = 0;
    
    // Find CSV files
    files = SelectionMenu_findFiles(&g_menu, ".", "csv", &fileCount, MAX_FILES);
    if (!files || fileCount == 0) {
        SelectionMenu_printColored(COLOR_RED, "\nNo CSV files found!\n");
        SelectionMenu_waitForKey(NULL);
        return;
    }
    
    // Create menu items from filenames
    menuItems = SelectionMenu_createMenuItems(&g_menu, files, fileCount, MAX_PATH_LENGTH);
    if (!menuItems) {
        SelectionMenu_freeFileList(files, fileCount);
        return;
    }
    
    // Let user select a file
    int choice = SelectionMenu_showMenu(&g_menu, "Select CSV File", menuItems, fileCount);
    if (choice <= 0) {
        SelectionMenu_freeMenuItems(menuItems, fileCount);
        SelectionMenu_freeFileList(files, fileCount);
        return;
    }
    
    // Read coordinates from file
    int n = 0, m = 0;
    double** coordinates = FileHandler_readCoordinates(files[choice-1], &n, &m);
    if (!coordinates) {
        SelectionMenu_printColored(COLOR_RED, "\nFailed to read coordinates!\n");
        SelectionMenu_freeMenuItems(menuItems, fileCount);
        SelectionMenu_freeFileList(files, fileCount);
        SelectionMenu_waitForKey(NULL);
        return;
    }
    
    // Display original coordinates
    SelectionMenu_clearScreen();
    printf("\nOriginal coordinates:\n\n");
    for (int i = 0; i < n; i++) {
        displayCoordinate(coordinates[i], m);
    }
    SelectionMenu_waitForKey("\nPress any key to start sorting...");
    
    // Sort coordinates and get statistics
    SortStats stats = sortCoordinates(coordinates, n, m);
    
    // Display sorted coordinates with sums
    SelectionMenu_clearScreen();
    printf("\nSorted coordinates:\n\n");
    for (int i = 0; i < n; i++) {
        displayCoordinateWithSum(coordinates[i], m);
    }
    
    // Display statistics
    printf("\nSort Statistics:\n");
    printf("Comparisons: %d\n", stats.comparisons);
    printf("Swaps: %d\n", stats.swaps);
    
    // Save sorted coordinates
    char response = SelectionMenu_askYesNo("\nSave sorted coordinates? ");
    if (response == 'y') {
        char* outputFile = SelectionMenu_getOutputFilename(&g_menu, "sorted.csv");
        if (outputFile) {
            if (saveCoordinatesToFile(outputFile, coordinates, n, m)) {
                SelectionMenu_printColored(COLOR_GREEN, "\nCoordinates saved to %s\n", outputFile);
            } else {
                SelectionMenu_printColored(COLOR_RED, "\nFailed to save coordinates!\n");
            }
            free(outputFile);
        }
    }
    
    // Cleanup
    FileHandler_freeCoordinates(coordinates, n);
    SelectionMenu_freeMenuItems(menuItems, fileCount);
    SelectionMenu_freeFileList(files, fileCount);
    SelectionMenu_waitForKey(NULL);
}

/**
 * @brief Optimized sorting algorithm for coordinates
 * 
 * Implements an optimized version of bubble sort that:
 * 1. Pre-calculates row sums to avoid redundant calculations
 * 2. Uses early termination when no swaps are needed
 * 3. Reduces the search range after each pass
 * 
 * @param coordinates 2D array of coordinates to sort
 * @param n Number of coordinates
 * @param m Number of components per coordinate
 * @return Statistics about the sorting operation
 */
SortStats optimisedSortCoordinates(double** coordinates, int n, int m) {
    SortStats stats = {0, 0};
    
    // Create temporary array to store coordinates and their sums
    typedef struct {
        double* coords;
        double sum;
        int originalIndex;
    } CoordInfo;
    
    CoordInfo* tempArray = (CoordInfo*)malloc(n * sizeof(CoordInfo));
    
    // Calculate sums and store original indices
    for (int i = 0; i < n; i++) {
        tempArray[i].coords = coordinates[i];
        tempArray[i].sum = calculateRowSum(coordinates[i], m);
        tempArray[i].originalIndex = i;
    }
    
    // Sort using selection sort approach to minimize swaps
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            stats.comparisons++;
            if (tempArray[j].sum < tempArray[minIdx].sum) {
                minIdx = j;
            }
        }
        
        if (minIdx != i) {
            // Swap in temporary array
            CoordInfo temp = tempArray[i];
            tempArray[i] = tempArray[minIdx];
            tempArray[minIdx] = temp;
            stats.swaps++;
        }
    }
    
    // Copy back to original array in sorted order
    double** tempCoords = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        tempCoords[i] = tempArray[i].coords;
    }
    
    // Copy pointers back to original array
    for (int i = 0; i < n; i++) {
        coordinates[i] = tempCoords[i];
    }
    
    // Cleanup
    free(tempCoords);
    free(tempArray);
    
    return stats;
}

/**
 * @brief Handles the optimized sort visualization option
 * 
 * Allows the user to:
 * 1. Select a CSV file
 * 2. View the original coordinates
 * 3. Sort the coordinates using optimized sort
 * 4. View sorting statistics
 * 5. Save the sorted coordinates
 */
void optimisedSort(void) {
    char** files = NULL;
    const char** menuItems = NULL;
    int fileCount = 0;
    
    // Find CSV files
    files = SelectionMenu_findFiles(&g_menu, ".", "csv", &fileCount, MAX_FILES);
    if (!files || fileCount == 0) {
        SelectionMenu_printColored(COLOR_RED, "\nNo CSV files found!\n");
        SelectionMenu_waitForKey(NULL);
        return;
    }
    
    // Create menu items from filenames
    menuItems = SelectionMenu_createMenuItems(&g_menu, files, fileCount, MAX_PATH_LENGTH);
    if (!menuItems) {
        SelectionMenu_freeFileList(files, fileCount);
        return;
    }
    
    // Let user select a file
    int choice = SelectionMenu_showMenu(&g_menu, "Select CSV File", menuItems, fileCount);
    if (choice <= 0) {
        SelectionMenu_freeMenuItems(menuItems, fileCount);
        SelectionMenu_freeFileList(files, fileCount);
        return;
    }
    
    // Read coordinates from file
    int n = 0, m = 0;
    double** coordinates = FileHandler_readCoordinates(files[choice-1], &n, &m);
    if (!coordinates) {
        SelectionMenu_printColored(COLOR_RED, "\nFailed to read coordinates!\n");
        SelectionMenu_freeMenuItems(menuItems, fileCount);
        SelectionMenu_freeFileList(files, fileCount);
        SelectionMenu_waitForKey(NULL);
        return;
    }
    
    // Display original coordinates
    SelectionMenu_clearScreen();
    printf("\nOriginal coordinates:\n\n");
    for (int i = 0; i < n; i++) {
        displayCoordinate(coordinates[i], m);
    }
    SelectionMenu_waitForKey("\nPress any key to start sorting...");
    
    // Sort coordinates and get statistics
    SortStats stats = optimisedSortCoordinates(coordinates, n, m);
    
    // Display sorted coordinates with sums
    SelectionMenu_clearScreen();
    printf("\nSorted coordinates:\n\n");
    for (int i = 0; i < n; i++) {
        displayCoordinateWithSum(coordinates[i], m);
    }
    
    // Display statistics
    printf("\nSort Statistics:\n");
    printf("Comparisons: %d\n", stats.comparisons);
    printf("Swaps: %d\n", stats.swaps);
    
    // Save sorted coordinates
    char response = SelectionMenu_askYesNo("\nSave sorted coordinates? ");
    if (response == 'y') {
        char* outputFile = SelectionMenu_getOutputFilename(&g_menu, "sorted.csv");
        if (outputFile) {
            if (saveCoordinatesToFile(outputFile, coordinates, n, m)) {
                SelectionMenu_printColored(COLOR_GREEN, "\nCoordinates saved to %s\n", outputFile);
            } else {
                SelectionMenu_printColored(COLOR_RED, "\nFailed to save coordinates!\n");
            }
            free(outputFile);
        }
    }
    
    // Cleanup
    FileHandler_freeCoordinates(coordinates, n);
    SelectionMenu_freeMenuItems(menuItems, fileCount);
    SelectionMenu_freeFileList(files, fileCount);
    SelectionMenu_waitForKey(NULL);
}

/**
 * @brief Saves sorted coordinates to a CSV file
 * 
 * @param filename Name of the output file
 * @param coordinates 2D array of coordinates to save
 * @param n Number of coordinates
 * @param m Number of components per coordinate
 * @return true if save was successful
 */
int saveCoordinatesToFile(const char* filename, double** coordinates, int n, int m) {
    FILE* file;
    if (fopen_s(&file, filename, "w") != 0) {
        printf("Error creating output file: %s\n", filename);
        return 0;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fprintf(file, "%.2f%s", coordinates[i][j], j < m - 1 ? "," : "");
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
    return 1;
}

/**
 * @brief Main program entry point
 * 
 * Initializes the menu system and handles the main program loop.
 * 
 * @return 0 on successful execution
 */
int main(void) {
    SelectionMenu_init(&g_menu);  // Initialize menu
    SelectionMenu_setMenuColor(COLOR_GREEN);  // Set default text color to green
    
    int choice;
    do {
        choice = SelectionMenu_showMenu(&g_menu, MENU_TITLE, MENU_ITEMS, NUM_MENU_ITEMS);
        
        switch (choice) {
            case 1:
                bubbleSort();
                break;
            case 2:
                optimisedSort();
                break;
            case 3:
                menuSettings();
                break;
        }
    } while (choice != 4 && choice != 0);
    
    return 0;
}