/*
 * Author: Logan Thomas
 * Assignment Number: 2
 * Date of Submission: 11/2/18
 * Name of this file: helper.c
 * Description of the program: Provides functions for parsing the file and freeing memory
 */

//<editor-fold desc="Includes">

#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

//</editor-fold>

//<editor-fold desc="Function declarations">

void get_file_contents(const char file_name[], unsigned int **contents, unsigned int n_lines);
void free2D(unsigned int **array, size_t numRows);

//</editor-fold>

//<editor-fold desc="Function definitions">

/*
 * Name: parse_file_alloc
 * Input: file name as char array, number of people, array of Person structs
 * Output: Array of person structs holding their floors and other relevant data
 * Description: Parses the file for people and the floors they each visit and for how long
 */
void parse_file_alloc(const char file_name[], const unsigned int n_people, Person **people) {
	unsigned int **contents = calloc((size_t) n_people, sizeof(int *));
	
	get_file_contents(file_name, contents, n_people);
	
	//<editor-fold desc="DEBUG">

#ifdef DEBUG
	int debug_n_moves = 0;
	assert(contents != NULL);
	printf("\nFile contents:\n");
	for (int i = 0; i < n_people; i++) {
		assert(contents[i] != NULL);
		debug_n_moves = 2 * contents[i][0];
		for (int j = 0; j <= debug_n_moves; j++) {
			printf("%u ", contents[i][j]);
		}
		printf("\n");
	}
#endif
	
	//</editor-fold>
	
	unsigned int n_floors_visited = 0;
	assert(contents != NULL);
	for (int i = 0; i < n_people; i++) {
		assert(contents[i] != NULL);
		
		people[i] = calloc(1, sizeof(Person));
		people[i]->n = (unsigned int)i;
		
		n_floors_visited = contents[i][0];
		people[i]->n_floors_to_visit = n_floors_visited;
		people[i]->next_floors = calloc((size_t)n_floors_visited, sizeof(floor_pair));
		for (int j = 1; j <= (2 * n_floors_visited); j+=2) {
			people[i]->next_floors[j/2].floor = contents[i][j];
			people[i]->next_floors[j/2].time = contents[i][j + 1];
		}
	}
	
	free2D(contents, (size_t) n_people);
}

/*
 * Name: get_file_contents
 * Input: file name as char array, pointer to hold the contents of the file, int number of lines in the file
 * Output: 2D array of ints as the contents of the file
 * Description: Parses the file to get the contents
 */
void get_file_contents(const char file_name[], unsigned int **contents, const unsigned int n_lines) {
	FILE *file_ptr = NULL;
	int count = 0;
	char temp_str[256];
	char temp_char;
	int num_scanned;
	
#ifdef DEBUG
	unsigned int temp2;
#endif
	// Open the file
	file_ptr = fopen(file_name, "r");
	
	// For each line...
	for (int i = 0; i < n_lines; i++) {
		count = 0;
		contents[i] = calloc(256, sizeof(unsigned int));
		// Scan the number...
		do {
			
			num_scanned = fscanf(file_ptr, "%s%c", temp_str, &temp_char);
			if (num_scanned > 0) {
#ifdef DEBUG
				// Parse the value of the number
				temp2 = (unsigned int) strtol(temp_str, NULL, 10);
				// Add it to contents
				contents[i][count++] = temp2;
#else
				contents[i][count++] = (unsigned int) strtol(temp_str, NULL, 10);
#endif
			}
		} while (temp_char != '\n' && num_scanned > 0);
		
		// Reallocate contents to the proper size
		contents[i] = realloc(contents[i], ((2 * contents[i][0]) + 1) * sizeof(unsigned int));
	}
	
	// Close the file
	fclose(file_ptr);
}

/*
 * Name: free2D
 * Input: 2D char array (jagged array of strings) and the number of strings
 * Output: None
 * Description: Frees a 2D jagged array of strings
 */
void free2D(unsigned int **array, size_t numRows) {
	// For each row, free the memory
	for (int i = 0; i < numRows; i++) {
		free(array[i]);
	}
	// Free the now empty array
	free(array);
}

//</editor-fold>
