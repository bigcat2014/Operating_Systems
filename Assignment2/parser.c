/*
 * Author: Logan Thomas
 * Assignment Number: 2
 * Date of Submission: 11/2/18
 * Name of this file: parser.c
 * Description of the program: Provides a function for parsing command line arguments
 */

//<editor-fold desc="Includes">

#include "helper.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//</editor-fold>

//<editor-fold desc="Define constants">

// Possible options
#define N_PEOPLE_OPTION "-p"
#define MAX_WANDER_TIME_OPTION "-w"
#define N_FLOORS_OPTION "-f"

#define DEFAULT_N_PEOPLE 1
#define DEFAULT_MAX_WAIT_TIME 10
#define DEFAULT_N_FLOORS 10

//</editor-fold>

//<editor-fold desc="Enumerations">

// Parser states
typedef enum {
	// The error state
	ERROR      = 0,
	
	// The idle state
	IDLE       = 1 << 0,
	
	// The -p flag was received state
	P_RECEIVED = 1 << 1,
	
	// The -p flag was received state
	W_RECEIVED = 1 << 2,
	
	// The -p flag was received state
	F_RECEIVED = 1 << 3
} ParsingState;

//</editor-fold>

//<editor-fold desc="Function definitions">

/*
 * Name: print_usage
 * Input: None
 * Output: None
 * Description: Prints the usage message of the program
 */
void print_usage(char program_name[]){
	printf("Usage:\n");
	printf("%s [options] <FILE>\n\n", program_name);
	printf("Options:\n");
	
	printf("\t%s [N]\t"
		   "The number of preople in the building\n",
		   N_PEOPLE_OPTION);
	
	printf("\t%s [T]\t"
		   "The max amount of time people can wander on a floor\n",
		   MAX_WANDER_TIME_OPTION);
	
	printf("\t%s [N]\t"
		   "The number of next_floors in the building\n",
		   N_FLOORS_OPTION);
}

/*
 * Name: get_option
 * Input: A string of one argument to check, the current state of the parser, and
 *     a pointer to the destination of the pid value
 * Output: The next state of the parser
 * Description: Parses the argument to find an option
 */
ParsingState get_option(char *arg, ParsingState parsing_state, unsigned int *n_people, unsigned int *max_wander_time, unsigned int *n_floors, char file_name[]) {
	ParsingState next_state = parsing_state;
	
	// If a flag was just received, parse the argument...
	switch(parsing_state){
		case P_RECEIVED:
			// Gets the number of people
			*n_people = (unsigned int)strtoul(arg, NULL, 10);
			next_state = IDLE;
			
			// If a valid number of people was not given, next state is error
			if (!(*n_people)) { next_state = ERROR; }
			break;
			
		case W_RECEIVED:
			// Gets the max wander time
			*max_wander_time = (unsigned int)strtoul(arg, NULL, 10);
			next_state = IDLE;
			
			// If a valid max wander time was not given, next state is error
			if (!(*max_wander_time)) { next_state = ERROR; }
			break;
			
		case F_RECEIVED:
			// Gets the number of next_floors
			*n_floors = (unsigned int)strtoul(arg, NULL, 10);
			next_state = IDLE;
			
			// If a valid number of next_floors was not given, next state is error
			if (!(*n_floors)) { next_state = ERROR; }
			break;
			
		// Otherwise, check for another option
		default:
			// Checks for each of the possible options
			if (!strcmp(arg, N_PEOPLE_OPTION)) {
				next_state = P_RECEIVED;
			} else if (!strcmp(arg, MAX_WANDER_TIME_OPTION)) {
				next_state = W_RECEIVED;
			} else if (!strcmp(arg, N_FLOORS_OPTION)) {
				next_state = F_RECEIVED;
			} else {
				strcpy(file_name, arg);
			}
	}
	
	// Return the next state of the parser
	return next_state;
}

/*
 * Name: parse_args
 * Input: The number of arguments in the argument vector, the argument vector, a pointer
 *      to the options, and pointers to the values of the possible options/arguments
 * Output: None
 * Description: The high level function to parse all of the arguments and set all
 *      of the flags and values.
 */
void parse_args(int argc, char *argv[], unsigned int *n_people, unsigned int *max_wander_time, unsigned int *n_floors, char file_name[]){
	ParsingState parsing_state = IDLE;
	
	// Set defaults
	*n_people = DEFAULT_N_PEOPLE;
	*max_wander_time = DEFAULT_MAX_WAIT_TIME;
	*n_floors = DEFAULT_N_FLOORS;
	
	// Loop through args to get options
	for (int i = 1; i < argc; i++) {
		// Get options
		parsing_state = get_option(argv[i], parsing_state, n_people, max_wander_time, n_floors, file_name);
		
		// If there was an error parsing, show the usage and error out
		if (parsing_state == ERROR) {
			print_usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	// User must pass a file name
	if (strcmp(file_name, "") == 0){
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	} else if (access(file_name, F_OK|R_OK) == -1){
		printf("%s not found or not accessible.\n", file_name);
		exit(EXIT_FAILURE);
	}
}

//</editor-fold>
