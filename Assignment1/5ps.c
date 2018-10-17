/*
 * Author: Logan Thomas
 * Assignment Number: 1
 * Date of Submission:
 * Name of this file: 5ps.c
 * Description of the program: Emulates the ps linux function with limited functionality
 */

//<editor-fold desc="Includes">

#include "5ps.h"
#include "parser.h"
#include "helper.h"
#include <stdio.h>
#include <stdlib.h>

//</editor-fold>

int main(int argc, char *argv[]) {
	int options = 0;
	long pid = -1;
	
	if (!parse_args(argc, argv, &options, &pid)) {
		printf("error: garbage option\n\n");
		print_help(argv[0]);
		exit(EXIT_FAILURE);
	}
	// If PID was not set, it is 1
	pid = (pid > 0) ? pid : 1;
	
	char *pid_str = get_pid_str_alloc(pid);
	
	PROC_DIR_STATE found = check_proc_dir_exists(pid_str);
	switch (found) {
		case FOUND:
			break;
		default:
			printf("Process %s not found\n", pid_str);
			free(pid_str);
			exit(EXIT_FAILURE);
	}
	
	char *pid_directory = get_pid_directory_alloc(pid_str);
	free(pid_str);
	
	if (mask_bits(options, fSTATE)) {
		char state = get_state(pid_directory);
		printf("State: %c\n", state);
	}
	if (mask_bits(options, fTIME)) {
		unsigned int seconds = get_time(pid_directory);
		
		unsigned int hours = seconds / 3600;
		unsigned int minutes = (seconds % 3600) / 60;
		seconds = (seconds % 3600) % 60;
		
		printf("Time: %02u:%02u:%02u\n", hours, minutes, seconds);
	}
	if (mask_bits(options, fVIRTUAL)) {
		unsigned int vmemory = get_virtual_memory(pid_directory);
		printf("Virtual memory: %u kB\n", vmemory);
	}
	if (mask_bits(options, fCOMMAND)) {
		char command_line[255];
		get_command(pid_directory, command_line);
		printf("Command: %s\n", command_line);
	}
	
	free(pid_directory);
	exit(EXIT_SUCCESS);
}
