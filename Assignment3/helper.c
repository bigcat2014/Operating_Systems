/*
 * Author: Logan Thomas
 * Assignment Number: 3
 * Date of Submission: 11/24/18
 * Name of this file: helper.c
 * Description of the program: Provides a function for parsing the processes out of the file
 */

//<editor-fold desc="Includes">

#include "helper.h"
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

//</editor-fold>

//<editor-fold desc="Function definitions">

/*
 * Name: parse_file_alloc
 * Input: The name of the file to parse, a pointer to an array of processes to fill, and a pointer to a count to fill
 * Output: returns void, but fills process array and process count
 * Description: Parses the file and gets the process values and the number of processes
 * Note: This function allocates memory! Caller must remember to free it!
 */
void parse_file_alloc(const char file_name[], Process **processes, unsigned int *process_count) {
	// Variables for reading file line by line
	FILE *file_ptr = NULL;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	
	// Variables to hold process values
	unsigned int process_number;
	unsigned int arrival_time;
	unsigned int priority;
	unsigned int cpu_burst;
	
	// Initial number of processes
	size_t num_processes = 5;
	
	// Allocate the processes array
	*processes = calloc(num_processes, sizeof(Process));
	assert(*processes != NULL);
	
	// Open the file
	file_ptr = fopen(file_name, "r");
	assert(file_ptr != NULL);
	
	// Read the file line by line
	for (*process_count = 0; ((read = getline(&line, &len, file_ptr)) != -1); (*process_count)++) {
		// If there are more processes than originally allocated, reallocate it
		if (*process_count >= (num_processes - 1)) {
			// Double the number of processes
			num_processes += num_processes;
			*processes = realloc(*processes, num_processes * sizeof(Process));
			assert(*processes != NULL);
		}
		// Scan the line for the process information. Simple, known format, assuming no errors, so sscanf was used
		sscanf(line, "%d %d %d %d", &process_number, &arrival_time, &priority, &cpu_burst);
		// Set the process values in the array
		(*processes)[*process_count] = (Process) {.process_number=process_number, .arrival_time=arrival_time, .priority=priority, .cpu_burst=cpu_burst, .pid = 0};
	}
	// Close the file
	fclose(file_ptr);
	
	// Reallocate memory to the actual number of processes parsed
	*processes = realloc(*processes, *process_count * sizeof(Process));
	assert(*processes != NULL);
}

//</editor-fold>
