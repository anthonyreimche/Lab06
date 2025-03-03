/**
 * @file fileHandler.cpp
 * @brief Implementation of the FileHandler class for coordinate data management
 */

#include "fileHandler.h"
#include <stdlib.h>
#include <string.h>

FileHandler::FileHandler() {
    // Constructor implementation
}

/**
 * @brief Reads coordinate data from a CSV file
 * 
 * This function performs the following steps:
 * 1. Opens the specified file
 * 2. Counts the number of rows and columns
 * 3. Allocates memory for the coordinate array
 * 4. Reads and parses the coordinate data
 * 
 * @param filename Path to the input CSV file
 * @param rows Output parameter for number of rows
 * @param cols Output parameter for number of columns
 * @return Dynamically allocated 2D array of coordinates
 */
double** FileHandler::readCoordinates(const char* filename, int* rows, int* cols) {
    FILE* file;
    if (fopen_s(&file, filename, "r") != 0) {
        printf("Error opening file: %s\n", filename);
        return NULL;
    }
    
    // Count lines and columns
    *rows = 0;
    *cols = 0;
    char line[1024];
    
    // Read first line to determine number of columns
    if (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",");
        while (token) {
            (*cols)++;
            token = strtok(NULL, ",");
        }
        (*rows)++;
    }
    
    // Count remaining lines
    while (fgets(line, sizeof(line), file)) {
        (*rows)++;
    }
    
    // Allocate memory
    double** data = (double**)malloc(*rows * sizeof(double*));
    for (int i = 0; i < *rows; i++) {
        data[i] = (double*)malloc(*cols * sizeof(double));
    }
    
    // Reset file pointer and read data
    rewind(file);
    for (int i = 0; i < *rows; i++) {
        if (fgets(line, sizeof(line), file)) {
            char* token = strtok(line, ",");
            for (int j = 0; j < *cols && token; j++) {
                data[i][j] = atof(token);
                token = strtok(NULL, ",");
            }
        }
    }
    
    fclose(file);
    return data;
}

/**
 * @brief Saves coordinate data to a CSV file
 * 
 * Writes the coordinate array to a file in CSV format, with each row
 * on a new line and values separated by commas.
 * 
 * @param filename Path to the output file
 * @param coordinates 2D array of coordinates to save
 * @param rows Number of rows in the array
 * @param cols Number of columns in the array
 */
void FileHandler::saveCoordinates(const char* filename, double** coordinates, int rows, int cols) {
    FILE* file;
    if (fopen_s(&file, filename, "w") != 0) {
        printf("Error creating output file: %s\n", filename);
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%.2f%s", coordinates[i][j], j < cols-1 ? "," : "\n");
        }
    }
    
    fclose(file);
    printf("\nCoordinates saved to: %s\n", filename);
}

/**
 * @brief Checks if a file exists and is accessible
 * 
 * @param filename Path to the file to check
 * @return true if file exists and can be opened for reading
 */
bool FileHandler::fileExists(const char* filename) {
    FILE* file;
    if (fopen_s(&file, filename, "r") == 0) {
        fclose(file);
        return true;
    }
    return false;
}

/**
 * @brief Frees memory allocated for a coordinate array
 * 
 * @param coordinates 2D array to free
 * @param rows Number of rows in the array
 */
void FileHandler::freeCoordinates(double** coordinates, int rows) {
    if (coordinates) {
        for (int i = 0; i < rows; i++) {
            free(coordinates[i]);
        }
        free(coordinates);
    }
}
