/**
 * @file fileHandler.h
 * @brief File handling utilities for coordinate data management
 * 
 * This module provides functionality for reading and writing coordinate data
 * from/to CSV files. It handles memory allocation and cleanup for 2D coordinate arrays.
 */

#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdio.h>

/**
 * @class FileHandler
 * @brief Manages file operations for coordinate data
 * 
 * Provides static methods for reading and writing coordinate data from/to files,
 * with proper memory management and error handling.
 */
class FileHandler {
public:
    /**
     * @brief Default constructor
     */
    FileHandler();
    
    /**
     * @brief Reads coordinates from a CSV file
     * @param filename Path to the input file
     * @param rows Output parameter for number of rows read
     * @param cols Output parameter for number of columns read
     * @return 2D array of coordinates, or NULL if file cannot be opened
     * @note Caller is responsible for freeing the returned array using freeCoordinates
     */
    static double** readCoordinates(const char* filename, int* rows, int* cols);
    
    /**
     * @brief Saves coordinates to a CSV file
     * @param filename Path to the output file
     * @param coordinates 2D array of coordinates to save
     * @param rows Number of rows in the array
     * @param cols Number of columns in the array
     */
    static void saveCoordinates(const char* filename, double** coordinates, int rows, int cols);
    
    /**
     * @brief Checks if a file exists
     * @param filename Path to the file to check
     * @return true if file exists and can be opened for reading
     */
    static bool fileExists(const char* filename);
    
private:
    /**
     * @brief Frees memory allocated for coordinate array
     * @param coordinates 2D array to free
     * @param rows Number of rows in the array
     */
    static void freeCoordinates(double** coordinates, int rows);
};

#endif // FILE_HANDLER_H
