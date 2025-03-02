#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <stdlib.h>

void generateMenu(char *, char*[], int);
void cursorMenu(char *, char*[], int);
int (*sortCoordinates(int coordinates[][2]))[2];
double** read2DArray(char *,int *);

enum Coordinates {x, y};

char titleText[] = "Lab 6: Pointers and Arrays";
const int optionNum[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
char *options[] = {
    "Classic Menu Style..",
    "Cursor Menu Style...",
    "Scrolling Menu Style",
    "Condensed Menu Style"
};


int main() {
    char userInput;

    system("COLOR 0A");
    system("CLS");
    //generateMenu(titleText, options,4);

    int n;
    double** data = read2DArray("coordinates.csv", &n);
    //sortCoordinates(data);



    while(!kbhit());
    userInput = getch();
    printf("%c", userInput);
    
    return 0;
}

/*
Inputs:
• title – a string containing the title for the menu.
• items – a list of strings containing each menu item.
• nItems – the number of items.
Outputs:
• select – the value of the item selected by the user.
*/
void generateMenu(char *title, char *items[], int nItems) {
    printf("\n%s\n", title);
    for (int i = 0; i < nItems; i++) {
        printf("%d. %s\n", i + 1, items[i]);
    }
}

/*
Inputs:
• title – a string containing the title for the menu.
• items – a list of strings containing each menu item.
• nItems – the number of items.
Outputs:
• select – the value of the item selected by the user.
*/
void cursorMenu(char *title, char *items[], int nItems) {
    int selection = 0;
    char ch;
    
    while (1) {
        // Clear screen using system command
        system("cls");
        
        // Display title
        printf("\n%s\n", title);
        
        // Display items with cursor
        for (int i = 0; i < nItems; i++) {
            if (i == selection) {
                printf("> %s\n", items[i]);
            } else {
                printf("  %s\n", items[i]);
            }
        }
        
        // Get user input
        ch = getch();
        
        // Process input
        switch (ch) {
            case 72: // Up arrow
                selection = (selection - 1 + nItems) % nItems;
                break;
            case 80: // Down arrow
                selection = (selection + 1) % nItems;
                break;
            case 13: // Enter key
                return;
        }
    }
}

/*
Description:
• Sorts coordinate [x,y] pairs from an array.
Inputs:
• coordinates – a 2D array containing all x,y pairs.
Outputs:
• select – the value of the item selected by the user.
 *//*
double (*sortCoordinates(int coordinates[][2]))[2] {
    printf("Sorting by magnitude...\n");
    int n = 10; // Number of coordinate pairs
    double *magnitudes = (double *)malloc(n * sizeof(double));
    
    // Calculate magnitudes
    for (int i = 0; i < n; i++) {
        magnitudes[i] = sqrt(pow(coordinates[i][x], 2) + pow(coordinates[i][y], 2));
    }
    
    // Bubble sort both arrays based on magnitudes
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (magnitudes[j] > magnitudes[j+1]) {
                // Swap magnitudes
                double tempMag = magnitudes[j];
                magnitudes[j] = magnitudes[j+1];
                magnitudes[j+1] = tempMag;
                
                // Swap coordinates
                int tempX = coordinates[j][x];
                int tempY = coordinates[j][y];
                coordinates[j][x] = coordinates[j+1][x];
                coordinates[j][y] = coordinates[j+1][y];
                coordinates[j+1][x] = tempX;
                coordinates[j+1][y] = tempY;
            }
        }
    }
    
    free(magnitudes);
    return coordinates;
}
*/
double** read2DArray(char* filename, int* n) {
    printf("Opening %s...\n", filename);
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("File \"%s\" not found!\nMake sure it is in the same directory.",filename);
        return nullptr;
    }

    double** coords = nullptr;
    *n = 0;
    char line[100];

    // Count lines
    while (fgets(line, sizeof(line), file)) {
        (*n)++;
    }

    // Allocate memory
    coords = (double**)malloc(*n * sizeof(double*));
    for (int i = 0; i < *n; i++) {
        coords[i] = (double*)malloc(2 * sizeof(double));
    }

    // Reset file pointer and read coordinates
    rewind(file);
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%lf,%lf", &coords[i][0], &coords[i][1]);
        i++;
    }

    fclose(file);
    printf("Found %d columns:\n\n",*n);
    for(int i = 0; i < *n; i++) {
        printf("[%8.2f ,%8.2f]\n", coords[i][1], coords[i][0]);
    }
    return coords;
}
