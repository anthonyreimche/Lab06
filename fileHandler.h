#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdio.h>

class FileHandler {
public:
    // Constructor
    FileHandler();
    
    // File reading functions
    static double** readCoordinates(const char* filename, int* rows, int* cols);
    static void saveCoordinates(const char* filename, double** coordinates, int rows, int cols);
    
    // File existence check
    static bool fileExists(const char* filename);
    
private:
    // Helper functions
    static void freeCoordinates(double** coordinates, int rows);
};

#endif // FILE_HANDLER_H
