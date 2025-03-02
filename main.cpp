#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdarg.h>

// Color constants
enum TextColor {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7
};

// Function to set text color
void setColor(int textColor, int bgColor = 0) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgColor << 4) | textColor);
}

// Function to reset color to default
void resetColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
}

void H1(const char* format, ...) {
    setColor(RED);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    setColor(GREEN);
}

// Forward declarations
int generateMenu(const char* title, const char* items[], int nItems);
int cursorMenu(const char* title, const char* items[], int nItems);
int condensedMenu(const char* title, const char* items[], int nItems);
void menuSettings();
double** sortCoordinates(double** coordinates, int n, int m);
double** read2DArray(const char* filename, int* n, int* m);
void bubbleSort();

enum Coordinates {x, y};
enum MenuType { CLASSIC, CURSOR, SCROLLING, CONDENSED };
MenuType currentMenuType = CLASSIC;

const char* menuTitle = "Lab 6: Sorting Algorithm Visualizer";
const char* menuItems[] = {
    "1. Bubble Sort",
    "2. Selection Sort",
    "3. Insertion Sort",
    "4. Menu Settings",
    "5. Exit"
};
const int numMenuItems = 5;

int generateMenu(const char* title, const char* items[], int nItems) {
    int selection = 0;
    char input[10];
    bool validInput = false;
    
    while (!validInput) {
        system("cls");
        printf("\n%s\n", title);
        for (int i = 0; i < nItems; i++) {
            printf("%s\n", items[i]);
        }
        
        printf("\nEnter your choice (1-%d): ", nItems);
        if (scanf_s("%s", input, (unsigned)sizeof(input)) == 1) {
            // Check if input is a single digit
            if (strlen(input) == 1 && input[0] >= '1' && input[0] <= '0' + nItems) {
                selection = input[0] - '0';
                
                // Clear input buffer
                while (getchar() != '\n');
                validInput = true;
            } else {
                printf("\nInvalid input. Press any key to try again...");
                while (getchar() != '\n'); // Clear input buffer
                _getch();
            }
        }
    }
    
    return selection;
}

void displayMenu() {
    system("cls");
    setColor(GREEN);
    
    int choice = 0;
    switch (currentMenuType) {
        case CURSOR:
            choice = cursorMenu(menuTitle, menuItems, numMenuItems);
            break;
        case CONDENSED:
            choice = condensedMenu(menuTitle, menuItems, numMenuItems);
            break;
        case CLASSIC:
        default:
            choice = generateMenu(menuTitle, menuItems, numMenuItems);
    }
    
    system("cls");
    switch (choice) {
        case 1:
            bubbleSort();
            break;
        case 2:
            H1("Selection Sort - Coming Soon!\n");
            break;
        case 3:
            H1("Insertion Sort - Coming Soon!\n");
            break;
        case 4:
            menuSettings();
            break;
        case 5:
            H1("Goodbye!\n");
            exit(0);
            break;
        default:
            H1("Invalid choice. Press any key to continue...\n");
    }
    
    if (choice != 5) {
        setColor(GREEN);
        printf("\nPress any key to continue...");
        while (!kbhit());
        getch();
    }
}

void menuSettings() {
    const char* settingsItems[] = {
        "1. Classic Menu",
        "2. Cursor Menu",
        "3. Condensed Menu",
        "4. Back to Main Menu"
    };
    
    static const MenuType menuTypes[] = {CLASSIC, CURSOR, CONDENSED};
    static const char* menuNames[] = {"Classic", "Cursor", "Condensed"};
    
    int choice = currentMenuType == CURSOR ? cursorMenu("Menu Settings", settingsItems, 4) :
                 currentMenuType == CONDENSED ? condensedMenu("Menu Settings", settingsItems, 4) :
                 generateMenu("Menu Settings", settingsItems, 4);
    
    if (choice >= 1 && choice <= 3) {
        currentMenuType = menuTypes[choice - 1];
        H1("\nChanged to %s Menu\n", menuNames[choice - 1]);
    }
}

int condensedMenu(const char* title, const char* items[], int nItems) {
    int selection = 0;
    int ch;
    const char* directions = "\nUse LEFT/RIGHT or W/S to change, or press 1-5. ENTER/D to confirm";
    
    while (1) {
        system("cls");
        printf("\n%s\n\n", title);
        
        // Print options
        printf("Options: ");
        for (int i = 0; i < nItems; i++) {
            // Extract just the number
            printf("%c", items[i][0]);
            if (i < nItems - 1) {
                printf(", ");
            }
        }
        
        // Show current selection
        const char* menuText = items[selection] + 3; // Skip the "N. " prefix
        printf("\n\nSelected: ");
        setColor(BLACK, GREEN);
        printf("%s", menuText);
        setColor(GREEN);
        
        printf("%s", directions);
        
        ch = _getch();
        if (ch == 0 || ch == 224) { // Special key
            ch = _getch();
            switch (ch) {
                case 75: // Left arrow
                case 72: // Up arrow
                    selection = (selection - 1 + nItems) % nItems;
                    break;
                case 77: // Right arrow
                case 80: // Down arrow
                    selection = (selection + 1) % nItems;
                    break;
            }
        } else {
            ch = tolower(ch);
            if (ch >= '1' && ch <= '0' + nItems) {
                selection = ch - '1';
            } else switch (ch) {
                case 'a':
                case 'w':
                    selection = (selection - 1 + nItems) % nItems;
                    break;
                case 'd':
                case 's':
                    selection = (selection + 1) % nItems;
                    break;
                case 13: // Enter
                    return selection + 1;
            }
        }
    }
}

int cursorMenu(const char* title, const char* items[], int nItems) {
    int selection = 0;
    int ch;
    
    while (1) {
        system("cls");
        printf("\n%s\n", title);
        
        for (int i = 0; i < nItems; i++) {
            const char* menuText = items[i] + 3; // Skip the "N. " prefix
            if (i == selection) {
                setColor(BLACK, GREEN); // Invert colors for selected item
                printf("%s\n", menuText);
                setColor(GREEN); // Reset to normal colors
            } else {
                printf("%s\n", menuText);
            }
        }
        
        printf("\nUse UP/DOWN or W/S to move, D or ENTER to select");
        
        ch = _getch(); // Get the first byte
        if (ch == 0 || ch == 224) { // Check if it's a special key
            ch = _getch(); // Get the second byte for special keys
            switch (ch) {
                case 72: // Up arrow
                    selection = (selection - 1 + nItems) % nItems;
                    break;
                case 80: // Down arrow
                    selection = (selection + 1) % nItems;
                    break;
            }
        } else {
            ch = tolower(ch);
            switch (ch) {
                case 'w':
                    selection = (selection - 1 + nItems) % nItems;
                    break;
                case 's':
                    selection = (selection + 1) % nItems;
                    break;
                case 'd':
                case 13: // Enter key
                    return selection + 1; // Return 1-based choice number
            }
        }
    }
}

/*
Description:
• Sorts coordinate rows by magnitude (Euclidean distance from origin).
Inputs:
• coordinates – a 2D array containing coordinate rows
• n – number of rows
• m – number of columns (dimensions) per row
Outputs:
• coordinates – sorted array by magnitude
 */
double** sortCoordinates(double** coordinates, int n, int m) {
    H1("Sorting coordinates by magnitude...\n");
    if (!coordinates || n <= 0 || m <= 0) return coordinates;

    double* magnitudes = (double*)malloc(n * sizeof(double));
    if (!magnitudes) {
        printf("Memory allocation failed for magnitudes!\n");
        return coordinates;
    }
    
    // Calculate magnitudes using Euclidean distance formula for any number of dimensions
    for (int i = 0; i < n; i++) {
        magnitudes[i] = 0;
        for (int j = 0; j < m; j++) {
            magnitudes[i] += pow(coordinates[i][j], 2);
        }
        magnitudes[i] = sqrt(magnitudes[i]);
    }
    
    // Bubble sort both arrays based on magnitudes
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (magnitudes[j] > magnitudes[j+1]) {
                // Swap magnitudes
                double tempMag = magnitudes[j];
                magnitudes[j] = magnitudes[j+1];
                magnitudes[j+1] = tempMag;
                
                // Swap all coordinates in the row
                for (int k = 0; k < m; k++) {
                    double tempCoord = coordinates[j][k];
                    coordinates[j][k] = coordinates[j+1][k];
                    coordinates[j+1][k] = tempCoord;
                }
            }
        }
    }

    // Print sorted coordinates with their magnitudes
    printf("Sorted coordinates:\n");
    for (int i = 0; i < n; i++) {
        printf("[");
        for (int j = 0; j < m; j++) {
            printf("%8.2f%s", coordinates[i][j], (j < m-1) ? "," : "]");
        }
        setColor(CYAN);
        printf("   magnitude: %8.2f", magnitudes[i]);
        setColor(GREEN);
        printf("\n");
    }
    printf("\n");
    
    free(magnitudes);
    resetColor();
    return coordinates;
}

double** read2DArray(const char* filename, int* n, int* m) {
    H1("Opening \"%s\"...\n", filename);
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("File \"%s\" not found!\nMake sure it is in the same directory.", filename);
        return nullptr;
    }

    double** data = nullptr;
    *n = 0;
    *m = 0;
    char line[1024];  // Increased buffer size for longer lines

    // Count lines and determine number of columns from first line
    if (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",");
        while (token) {
            (*m)++;
            token = strtok(NULL, ",");
        }
        (*n)++;  // Count first line
    }

    // Count remaining lines
    while (fgets(line, sizeof(line), file)) {
        (*n)++;
    }

    // Allocate memory for rows
    data = (double**)malloc(*n * sizeof(double*));
    if (!data) {
        printf("Memory allocation failed for rows!\n");
        fclose(file);
        return nullptr;
    }

    // Allocate memory for columns
    for (int i = 0; i < *n; i++) {
        data[i] = (double*)malloc(*m * sizeof(double));
        if (!data[i]) {
            printf("Memory allocation failed for columns at row %d!\n", i);
            // Free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(data[j]);
            }
            free(data);
            fclose(file);
            return nullptr;
        }
    }

    // Reset file pointer and read data
    rewind(file);
    int row = 0;
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",");
        int col = 0;
        while (token && col < *m) {
            data[row][col] = atof(token);
            token = strtok(NULL, ",");
            col++;
        }
        row++;
    }

    fclose(file);
    printf("Found %d rows with %d columns each:\n\n", *n, *m);
    for (int i = 0; i < *n; i++) {
        printf("[");
        for (int j = 0; j < *m; j++) {
            printf("%8.2f%s", data[i][j], (j < *m-1) ? "," : "");
        }
        printf("]\n");
    }
    printf("\n");
    return data;
}

void bubbleSort() {
    H1("Bubble sort chosen.\n");
    setColor(GREEN);

    int n, m;
    double** data = read2DArray("coordinates.csv", &n, &m);
    if (data) {
        sortCoordinates(data, n, m);

        // Free memory
        for (int i = 0; i < n; i++) {
            free(data[i]);
        }
        free(data);
    }
}

int main() {
    while (1) {
        displayMenu();
    }
    
    resetColor();
    return 0;
}