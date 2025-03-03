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
 * @brief Reads coordinates from a CSV file
 * @param filename Path to the input file
 * @param rows Output parameter for number of rows read
 * @param cols Output parameter for number of columns read
 * @return 2D array of coordinates, or NULL if file cannot be opened
 * @note Caller is responsible for freeing the returned array using FileHandler_freeCoordinates
 */
double** FileHandler_readCoordinates(const char* filename, int* rows, int* cols);

/**
 * @brief Saves coordinates to a CSV file
 * @param filename Path to the output file
 * @param coordinates 2D array of coordinates to save
 * @param rows Number of rows in the array
 * @param cols Number of columns in the array
 */
void FileHandler_saveCoordinates(const char* filename, double** coordinates, int rows, int cols);

/**
 * @brief Checks if a file exists
 * @param filename Path to the file to check
 * @return 1 if file exists and can be opened for reading, 0 otherwise
 */
int FileHandler_fileExists(const char* filename);

/**
 * @brief Frees memory allocated for coordinate array
 * @param coordinates 2D array to free
 * @param rows Number of rows in the array
 */
void FileHandler_freeCoordinates(double** coordinates, int rows);

#endif // FILE_HANDLER_H
