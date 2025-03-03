/**
 * @file fileHandler.c
 * @brief Implementation of file handling utilities
 */

#include "fileHandler.h"
#include <stdlib.h>
#include <string.h>

double** FileHandler_readCoordinates(const char* filename, int* rows, int* cols) {
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
    if (!data) {
        fclose(file);
        return NULL;
    }
    
    for (int i = 0; i < *rows; i++) {
        data[i] = (double*)malloc(*cols * sizeof(double));
        if (!data[i]) {
            // Clean up previously allocated memory
            for (int j = 0; j < i; j++) {
                free(data[j]);
            }
            free(data);
            fclose(file);
            return NULL;
        }
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

void FileHandler_saveCoordinates(const char* filename, double** coordinates, int rows, int cols) {
    FILE* file;
    if (fopen_s(&file, filename, "w") != 0) {
        printf("Error creating output file: %s\n", filename);
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%.2f%s", coordinates[i][j], j < cols - 1 ? "," : "");
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
}

int FileHandler_fileExists(const char* filename) {
    FILE* file;
    if (fopen_s(&file, filename, "r") == 0) {
        fclose(file);
        return 1;
    }
    return 0;
}

void FileHandler_freeCoordinates(double** coordinates, int rows) {
    if (coordinates) {
        for (int i = 0; i < rows; i++) {
            free(coordinates[i]);
        }
        free(coordinates);
    }
}
