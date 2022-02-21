/******************************************************************************
 * @file: latin_square_functions.c
 *
 * WISC NETID:			hchoi256
 * CANVAS USERNAME		Eric Choi
 * WISC ID NUMBER		9082005175
 * OTHER COMMENTS FOR THE GRADER (OPTIONAL)
 *
 * @creator: Eric Choi (hchoi256@wisc.edu)
 * @modified: 11/06/2021
 *****************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "latin_square_functions.h"

 // Set this to 1 to enable dbgprintf statements, make sure to set it back to 0 
 // before submitting!
#define DEBUG               0 
#define dbgprintf(...)      if (DEBUG) { printf(__VA_ARGS__); }
#define dbgprint_latin_square(n, LS) if (DEBUG) { Print_Latin_Square(n, LS); }
#define LINE 1002

/******************************************************************************
 * Data Types and Structures
 *****************************************************************************/

 // ADD ANY ADDITIONAL DATA TYPES OR STRUCTSvHERE 

 /******************************************************************************
  * Globals
  *****************************************************************************/
extern void Print_Latin_Square(const size_t n, char **latin_square);

/******************************************************************************
 * Helper functions
 *****************************************************************************/

 // ADD ANY HELPER FUNCTIONS YOU MIGHT WRITE HERE 
//int length_of_string(char *p)
//{
//	int count = 0;
//	while (*p != '\0') {
//		count++;
//		p++;
//	}
//
//	return count;
//}

/******************************************************************************
 * Verificaiton functions
 *****************************************************************************/

 /*
  * This function takes the name of the file containing the latin square
  * and reads in the data to the the latin_square parameter.
  *
  * There are many approaches that will work to read in the latin square data.
  * In any approach you choose, you will need to do at least the following:
  *     1) open the file
  *     2) read in the text from the file
  *     3) figure out the dimensions of the latin square (n)
  *     4) reserve memory for the latin_square. This requires 2 steps
  *         4a) reserve an array of pointers to the rows
  *         4b) reserve an array of characters for each row
  *     5) fill in the latin_square data structure
  *     6) close the file
  *
  * @param filename The name of the file to read in
  * @param latin_square_in A pointer to the latin square variable in main
  * @param n The value of both dimensions of the latin square (i.e. nxn)
  */
void Read_Latin_Square_File(const char *filename,
	char ***latin_square_in,
	size_t *n) {
	FILE *fp = fopen(filename, "r");

	if (fp == NULL) {
		*n = 0, *latin_square_in = NULL;
		printf("Error in file %s\n", filename);
		fclose(fp);

		return;
	}

	char *line = (char*)malloc(LINE * sizeof(char));

	if (fgets(line, LINE, fp) == NULL) {
		// no input in the file
		*n = 0, *latin_square_in = NULL;
		free(line); 
		fclose(fp);
		return;
	}

	// *n = length_of_string(line) - 1;	// -1 for '\n'
	*n = strlen(line) - 1;
	*latin_square_in = (char **)malloc((*n) * sizeof(char *));	// allocate memory for rows

	// allocate memory for columns
	for (int i = 0; i < *n; i++) {
		*(*latin_square_in + i) = (char *)malloc((*n) * sizeof(char));
	}

	// copy the latin square
	for (int i = 0; i < *n; i++) {
		for (int j = 0; j < *n; j++) {
			*(*(*latin_square_in + i) + j) = *(line + j);
		}

		fgets(line, LINE, fp);
	}

	free(line);
	fclose(fp);
	return;
}

/*
 * This function checks to see that exactly n symbols are used and that
 * each symbol is used exactly n times.
 *
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 * @return 1 if valid, 0 if not
 */
int Verify_Alphabet(const size_t n, char **latin_square) {
	if (latin_square == NULL) {
		printf("Verify_Alphabet - latin_square is NULL\n");
		return 0;
	}

	int chars = 126 - 33 + 1;
	int *freq = (int *)malloc(chars * sizeof(int));	// frequency of each character

	// array with zero values
	for (int i = 0; i < chars; i++) {
		*(freq + i) = 0;
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int index = (int)(*(*(latin_square + i) + j)) - 33;

			if ((*(*(latin_square + i) + j)) < 33 || (*(*(latin_square + i) + j)) > 126)
				return 0;

			*(freq + index) += 1;
		}
	}

	// check if frequency is either 0 or n
	for (int i = 0; i < chars; i++) {
		if (*(freq + i) != 0 && *(freq + i) != n) {
			free(freq);
			return 0;
		}
	}

	free(freq);
	return 1;
}

/*
 * This function verifies that no symbol is used twice in a row or column.
 * It prints an error message alerting the user which rows or columns have
 * duplicate symbols. This means that you will have at most n row error prints
 * and n column error prints.
 *
 * Note: Test all rows first then test all columns.
 *
 * Error messages have been included for you. Do not change the format out the
 * print statements, as our grading scripts will use exact string matching. You
 * change the variable passed as the formatting argument to printf.
 *
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 * @return 1 if valid, 0 if not
 */
int Verify_Rows_and_Columns(const size_t n, char **latin_square) {
	if (latin_square == NULL) {
		printf("Verify_Rows_and_Columns - latin_square is NULL\n");
		return 0;
	}

	int chars = 126 - 33 + 1, index, dup = 0, res = 1;
	int *freq = (int *)malloc(chars * sizeof(int));

	for (int r = 0; r < n; r++) {
		dup = 0;
		for (int i = 0; i < chars; i++) {
			*(freq + i) = 0;
		}

		for (int c = 0; c < n; c++) {
			index = (int)(*(*(latin_square + r) + c)) - 33;

			// if duplicated, the character already counted, such that *(freq + index) > 0
			if (*(freq + index) != 0 && dup == 0) {
				dup = 1, res = 0;
				printf("Error in row %d\n", r);
			}

			*(freq + index) += 1;
		}
	}

	for (int c = 0; c < n; c++) {
		dup = 0;
		for (int i = 0; i < chars; i++) {
			*(freq + i) = 0;
		}

		for (int r = 0; r < n; r++) {
			index = (int)(*(*(latin_square + r) + c)) - 33;

			// if duplicated, the character already counted, such that *(freq + index) > 0
			if (*(freq + index) != 0 && dup == 0) {
				dup = 1, res = 0;
				printf("Error in column %d\n", c);
			}

			*(freq + index) += 1;
		}
	}

	free(freq);
	return res;
}

/*
 * This function calls free to allow all memory used by the latin_square
 * verification program to be reclaimed.
 *
 * Note: you will have n+1 calls to free
 *
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 */
void Free_Memory(const size_t n, char **latin_square) {
	for (int r = 0; r < n; r++) {
		free(*(latin_square + r));
	}

	free(latin_square);
	return;
}


