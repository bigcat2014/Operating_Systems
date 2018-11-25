/*
 * Author: Logan Thomas
 * Assignment Number: 3
 * Date of Submission: 11/24/18
 * Name of this file: parser.c
 * Description of the program: Provides a function for parsing command line arguments
 */

//<editor-fold desc="Includes">

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//</editor-fold>

//<editor-fold desc="Function definitions">

/*
 * Name: print_usage
 * Input: None
 * Output: None
 * Description: Prints the usage message of the program
 */
void print_usage(char program_name[]) {
	printf("Usage:\n");
	printf("%s file_name\n\n", program_name);
}

/*
 * Name: parse_args
 * Input: The number of arguments in the argument vector, the argument vector, and an
 * 		array to hold the file name
 * Output: None
 * Description: The high level function to parse the file name from the program arguments
 */
void parse_args(int argc, char *argv[], char file_name[]) {
	// If no file name was passed, or too many arguments were given, error out
	if (argc != 2) {
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	
	// Gets the file name
	strcpy(file_name, argv[1]);
	
	// User must pass a valid file name
	if (access(file_name, F_OK | R_OK) == -1) {
		printf("%s not found or not accessible.\n", file_name);
		exit(EXIT_FAILURE);
	}
}

//</editor-fold>
