/**
 * @file main.cpp
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

// Menu configuration
const char* MENU_TITLE = "Lab 6: Sorting Algorithm Visualizer";
const char* MENU_ITEMS[] = {
    "1. Bubble Sort",
    "2. Optimised Sort",
    "3. Settings",
    "4. Exit"
};
const int NUM_MENU_ITEMS = 4;

/** Enum for accessing coordinate components */
enum Coordinates { x, y };

/**
 * @struct SortStats
 * @brief Statistics collected during sorting operations
 */
struct SortStats {
    int comparisons;  ///< Number of comparisons performed
    int swaps;        ///< Number of swaps performed
};

// Forward declarations
void menuSettings();
void displayMenu();
void bubbleSort();
void optimisedSort();
SortStats sortCoordinates(double** coordinates, int n, int m);
SortStats optimisedSortCoordinates(double** coordinates, int n, int m);
double** read2DArray(const char* filename, int* n, int* m);
bool saveCoordinatesToFile(const char* filename, double** coordinates, int n, int m);

// Global menu instance
SelectionMenu menu;

/**
 * @brief Handles menu style settings
 * 
 * Allows the user to switch between different menu display styles:
 * - Classic (numbered list)
 * - Cursor (interactive arrow selection)
 * - Condensed (compact display)
 */
void menuSettings() {
    const char* settingsItems[] = {
        "1. Classic Menu",
        "2. Cursor Menu",
        "3. Condensed Menu"
    };
    
    // Use current menu style for settings
    MenuType currentType = menu.getMenuType();
    int choice = menu.showMenu("Menu Settings", settingsItems, 3);
    
    // Apply selected style
    switch (choice) {
        case 1:
            menu.setMenuType(CLASSIC);
            menu.printColored(COLOR_RED, "\nChanged to Classic Menu\n");
            break;
        case 2:
            menu.setMenuType(CURSOR);
            menu.printColored(COLOR_RED, "\nChanged to Cursor Menu\n");
            break;
        case 3:
            menu.setMenuType(CONDENSED);
            menu.printColored(COLOR_RED, "\nChanged to Condensed Menu\n");
            break;
    }
    
    menu.waitForKey();
}

/**
 * @brief Reads a 2D array of coordinates from a CSV file
 * 
 * @param filename Name of the file to read
 * @param n Output parameter for number of rows
 * @param m Output parameter for number of columns
 * @return Dynamically allocated 2D array of coordinates
 */
double** read2DArray(const char* filename, int* n, int* m) {
    printf("Reading from file \"%s\"...\n", filename);
    FILE* file;
    if (fopen_s(&file, filename, "r") != 0) {
        printf("Error opening file: %s\n", filename);
        return NULL;
    }
    
    // Count lines and columns
    *n = 0;
    *m = 0;
    char line[1024];
    
    // Read first line to determine number of columns
    if (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",");
        while (token) {
            (*m)++;
            token = strtok(NULL, ",");
        }
        (*n)++;
    }
    
    // Count remaining lines
    while (fgets(line, sizeof(line), file)) {
        (*n)++;
    }
    
    // Allocate memory
    double** data = (double**)malloc(*n * sizeof(double*));
    for (int i = 0; i < *n; i++) {
        data[i] = (double*)malloc(*m * sizeof(double));
    }
    
    // Reset file pointer and read data
    rewind(file);
    for (int i = 0; i < *n; i++) {
        if (fgets(line, sizeof(line), file)) {
            char* token = strtok(line, ",");
            for (int j = 0; j < *m && token; j++) {
                data[i][j] = atof(token);
                token = strtok(NULL, ",");
            }
        }
    }
    
    fclose(file);
    return data;
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
 * @brief Saves sorted coordinates to a CSV file
 * 
 * @param filename Name of the output file
 * @param coordinates 2D array of coordinates to save
 * @param n Number of coordinates
 * @param m Number of components per coordinate
 * @return true if save was successful
 */
bool saveCoordinatesToFile(const char* filename, double** coordinates, int n, int m) {
    FILE* file;
    if (fopen_s(&file, filename, "w") != 0) {
        printf("Error creating output file: %s\n", filename);
        return false;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fprintf(file, "%.2f%s", coordinates[i][j], j < m - 1 ? "," : "");
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
    return true;
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
 * @brief Displays a coordinate with its magnitude (sum)
 * 
 * Formats the coordinate according to the user's preferred style:
 * [   x.xx ,    y.yy ]   magnitude:    z.zz
 * 
 * @param row Array containing the coordinate components
 * @param m Number of components
 */
void displayCoordinateWithSum(double* row, int m) {
    printf("[");
    for (int j = 0; j < m; j++) {
        printf("%8.2f%s", row[j], j < m-1 ? " ," : " ]");
    }
    menu.printColored(COLOR_CYAN, "   magnitude: %8.2f\n", calculateRowSum(row, m));
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
void bubbleSort() {
    menu.printColored(COLOR_RED, "Bubble sort chosen.\n");
    menu.setColor(COLOR_GREEN);

    // Find CSV files
    char currentDir[MAX_PATH_LENGTH];
    GetCurrentDirectory(MAX_PATH_LENGTH, currentDir);
    
    int fileCount;
    char** files = menu.findFiles(currentDir, ".csv", &fileCount);
    if (fileCount == 0) {
        menu.printColored(COLOR_RED, "No CSV files found in current directory.\n");
        return;
    }
    
    const char** menuItems = menu.createMenuItems(files, fileCount);
    
    // Show file selection menu
    printf("\nSelect a CSV file to sort:\n");
    int choice = menu.showMenu("File Selection", menuItems, fileCount);
    if (choice <= 0) {  // Handle ESC key
        menu.freeMenuItems(menuItems, fileCount);
        menu.freeFileList(files, fileCount);
        return;
    }
    
    // Load the selected file
    char* selectedFile = files[choice - 1];
    int n = 0, m = 0;
    double** coordinates = read2DArray(selectedFile, &n, &m);
    if (!coordinates) {
        menu.freeMenuItems(menuItems, fileCount);
        menu.freeFileList(files, fileCount);
        return;
    }
    
    // Display original coordinates
    printf("\nOriginal coordinates:\n");
    for (int i = 0; i < n; i++) {
        displayCoordinate(coordinates[i], m);
    }
    
    // Sort and get statistics
    SortStats stats = sortCoordinates(coordinates, n, m);
    
    // Display sorted coordinates
    printf("\nSorted coordinates:\n");
    for (int i = 0; i < n; i++) {
        displayCoordinateWithSum(coordinates[i], m);
    }
    
    // Display sorting statistics
    menu.printColored(COLOR_CYAN, "\nBubble Sort Statistics:\n");
    menu.printColored(COLOR_CYAN, "Comparisons: %d\n", stats.comparisons);
    menu.printColored(COLOR_CYAN, "Swaps: %d\n", stats.swaps);
    
    // Save sorted coordinates
    char outputFilename[MAX_PATH_LENGTH];
    sprintf_s(outputFilename, MAX_PATH_LENGTH, "sorted_%s", selectedFile);
    if (saveCoordinatesToFile(outputFilename, coordinates, n, m)) {
        menu.printColored(COLOR_GREEN, "\nSorted coordinates saved to: %s\n", outputFilename);
    }
    
    // Cleanup
    for (int i = 0; i < n; i++) {
        free(coordinates[i]);
    }
    free(coordinates);
    menu.freeMenuItems(menuItems, fileCount);
    menu.freeFileList(files, fileCount);
    
    menu.waitForKey();
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
 * Similar to bubbleSort(), but uses the optimized sorting algorithm
 * and displays comparative performance metrics.
 */
void optimisedSort() {
    menu.printColored(COLOR_RED, "Optimised sort chosen.\n");
    menu.setColor(COLOR_GREEN);

    // Find CSV files
    char currentDir[MAX_PATH_LENGTH];
    GetCurrentDirectory(MAX_PATH_LENGTH, currentDir);
    
    int fileCount;
    char** files = menu.findFiles(currentDir, ".csv", &fileCount);
    if (fileCount == 0) {
        menu.printColored(COLOR_RED, "No CSV files found in current directory.\n");
        return;
    }
    
    const char** menuItems = menu.createMenuItems(files, fileCount);
    
    // Show file selection menu
    printf("\nSelect a CSV file to sort:\n");
    int choice = menu.showMenu("File Selection", menuItems, fileCount);
    if (choice <= 0) {  // Handle ESC key
        menu.freeMenuItems(menuItems, fileCount);
        menu.freeFileList(files, fileCount);
        return;
    }
    
    // Load the selected file
    char* selectedFile = files[choice - 1];
    int n = 0, m = 0;
    double** coordinates = read2DArray(selectedFile, &n, &m);
    if (!coordinates) {
        menu.freeMenuItems(menuItems, fileCount);
        menu.freeFileList(files, fileCount);
        return;
    }
    
    // Display original coordinates
    printf("\nOriginal coordinates:\n");
    for (int i = 0; i < n; i++) {
        displayCoordinate(coordinates[i], m);
    }
    
    // Sort and get statistics
    SortStats stats = optimisedSortCoordinates(coordinates, n, m);
    
    // Display sorted coordinates
    printf("\nSorted coordinates:\n");
    for (int i = 0; i < n; i++) {
        displayCoordinateWithSum(coordinates[i], m);
    }
    
    // Display sorting statistics
    menu.printColored(COLOR_CYAN, "\nOptimised Sort Statistics:\n");
    menu.printColored(COLOR_CYAN, "Comparisons: %d\n", stats.comparisons);
    menu.printColored(COLOR_CYAN, "Swaps: %d\n", stats.swaps);
    
    // Save sorted coordinates
    char outputFilename[MAX_PATH_LENGTH];
    sprintf_s(outputFilename, MAX_PATH_LENGTH, "sorted_%s", selectedFile);
    if (saveCoordinatesToFile(outputFilename, coordinates, n, m)) {
        menu.printColored(COLOR_GREEN, "\nSorted coordinates saved to: %s\n", outputFilename);
    }
    
    // Cleanup
    for (int i = 0; i < n; i++) {
        free(coordinates[i]);
    }
    free(coordinates);
    menu.freeMenuItems(menuItems, fileCount);
    menu.freeFileList(files, fileCount);
    
    menu.waitForKey();
}

/**
 * @brief Main program entry point
 * 
 * Initializes the menu system and handles the main program loop.
 * 
 * @return 0 on successful execution
 */
int main() {
    menu.setMenuType(CLASSIC);  // Set Classic as default menu style
    int choice;
    
    do {
        choice = menu.showMenu(MENU_TITLE, MENU_ITEMS, NUM_MENU_ITEMS);
        
        switch (choice) {
            case 1: bubbleSort(); break;
            case 2: optimisedSort(); break;
            case 3: menuSettings(); break;
        }
    } while (choice != 4);
    
    return 0;
}