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
    "2. Selection Sort",
    "3. Insertion Sort",
    "4. Menu Settings",
    "5. Exit"
};
const int NUM_MENU_ITEMS = 5;

enum Coordinates { x, y };

// Forward declarations
void menuSettings();
void displayMenu();
void bubbleSort();
double** sortCoordinates(double** coordinates, int n, int m);
double** read2DArray(const char* filename, int* n, int* m);
void saveCoordinates(const char* filename, double** coordinates, int n, int m);

// Global menu instance
SelectionMenu menu;

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
            menu.printColored(RED, "\nChanged to Classic Menu\n");
            break;
        case 2:
            menu.setMenuType(CURSOR);
            menu.printColored(RED, "\nChanged to Cursor Menu\n");
            break;
        case 3:
            menu.setMenuType(CONDENSED);
            menu.printColored(RED, "\nChanged to Condensed Menu\n");
            break;
    }
    
    menu.waitForKey();
}

double** read2DArray(const char* filename, int* n, int* m) {
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

double** sortCoordinates(double** coordinates, int n, int m) {
    // Calculate magnitudes
    double* magnitudes = (double*)malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        double x = coordinates[i][0];
        double y = coordinates[i][1];
        magnitudes[i] = sqrt(x * x + y * y);
    }
    
    // Bubble sort coordinates based on magnitudes
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (magnitudes[j] > magnitudes[j + 1]) {
                // Swap magnitudes
                double tempMag = magnitudes[j];
                magnitudes[j] = magnitudes[j + 1];
                magnitudes[j + 1] = tempMag;
                
                // Swap coordinates
                for (int k = 0; k < m; k++) {
                    double tempCoord = coordinates[j][k];
                    coordinates[j][k] = coordinates[j + 1][k];
                    coordinates[j + 1][k] = tempCoord;
                }
            }
        }
    }
    
    // Print sorted coordinates
    printf("\nSorted coordinates by magnitude:\n");
    printf("----------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("    [");
        for (int j = 0; j < m; j++) {
            printf("%8.2f%s", coordinates[i][j], j < m-1 ? " ," : " ]");
        }
        menu.printColored(CYAN, "   magnitude: %8.2f\n", magnitudes[i]);
    }
    printf("----------------------------------------\n\n");
    
    free(magnitudes);
    return coordinates;
}

void saveCoordinates(const char* filename, double** coordinates, int n, int m) {
    FILE* file;
    if (fopen_s(&file, filename, "w") != 0) {
        printf("Error creating output file: %s\n", filename);
        return;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fprintf(file, "%.2f%s", coordinates[i][j], j < m-1 ? "," : "\n");
        }
    }
    
    fclose(file);
    printf("\nCoordinates saved to: %s\n", filename);
}

void bubbleSort() {
    menu.printColored(RED, "Bubble sort chosen.\n");
    menu.setColor(GREEN);

    // Find CSV files
    char currentDir[MAX_PATH_LENGTH];
    GetCurrentDirectory(MAX_PATH_LENGTH, currentDir);
    
    int fileCount;
    char** csvFiles = menu.findFiles(currentDir, ".csv", &fileCount);
    
    if (fileCount == 0) {
        menu.waitForKey("\nNo CSV files found in the current directory.\n");
        menu.freeFileList(csvFiles, 0);
        return;
    }
    
    // Create menu items
    const char** menuItems = menu.createMenuItems(csvFiles, fileCount);
    
    // Show file selection menu using current menu type
    MenuType currentType = menu.getMenuType();
    int selection = menu.showMenu("Select a CSV file to process:", menuItems, fileCount);
    
    // Process selected file if valid
    if (selection > 0 && selection <= fileCount) {
        int n, m;
        double** data = read2DArray(csvFiles[selection - 1], &n, &m);
        if (data) {
            sortCoordinates(data, n, m);
            menu.waitForKey();
            
            // Create yes/no menu items for save prompt
            const char* saveItems[] = {
                "1. Yes",
                "2. No"
            };
            
            // Ask to save file using current menu type
            menu.setMenuType(currentType);
            if (menu.showMenu("Would you like to save the sorted coordinates?", saveItems, 2) == 1) {
                char* outputFile = menu.getOutputFilename("sorted.csv");
                if (outputFile != NULL) {
                    saveCoordinates(outputFile, data, n, m);
                    free(outputFile);
                }
            }
            
            // Cleanup data
            for (int i = 0; i < n; i++) {
                free(data[i]);
            }
            free(data);
        }
    }
    
    // Cleanup menu items
    menu.freeMenuItems(menuItems, fileCount);
    menu.freeFileList(csvFiles, fileCount);
    
    menu.waitForKey();
}

int main() {
    int choice;
    do {
        choice = menu.showMenu(MENU_TITLE, MENU_ITEMS, NUM_MENU_ITEMS);
        
        switch (choice) {
            case 1: bubbleSort(); break;
            case 2: menu.printColored(RED, "Selection sort not implemented yet.\n"); break;
            case 3: menu.printColored(RED, "Insertion sort not implemented yet.\n"); break;
            case 4: menuSettings(); break;
        }
    } while (choice != 5);
    
    return 0;
}