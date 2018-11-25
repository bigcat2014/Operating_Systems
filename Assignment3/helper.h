/*
 * Author: Logan Thomas
 * Assignment Number: 3
 * Date of Submission: 11/24/18
 * Name of this file: helper.h
 * Description of the program: Provides a function for parsing the processes out of the file
 */

#ifndef ASSIGNMENT3_HELPER_H
#define ASSIGNMENT3_HELPER_H

//<editor-fold desc="Includes">

#include <stdlib.h>

//</editor-fold>

//<editor-fold desc="Structs">

typedef struct __process {
	unsigned int process_number;
	unsigned int arrival_time;
	unsigned int priority;
	unsigned int cpu_burst;
	unsigned int pid;
} Process;

//</editor-fold>

//<editor-fold desc="Function Declarations">

void parse_file_alloc(const char file_name[], Process **processes, unsigned int *process_count);

//</editor-fold>

#endif //ASSIGNMENT3_HELPER_H
