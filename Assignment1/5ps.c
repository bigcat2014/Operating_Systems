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
#include <string.h>
#include <assert.h>
#include <zconf.h>

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
		char *stat_file = "/stat";
		char *file_path = get_file_path_alloc(pid_directory, stat_file);
		FILE *fPtr;
		if ((fPtr = fopen(file_path, "r")) == NULL) {
			printf("Error opening file %s\n", file_path);
			exit(EXIT_FAILURE);
		}
		free(file_path);
		
		char buff[255];
		for (int i = 0; i < 3; i++) {
			fscanf(fPtr, "%s", buff);
		}
		fclose(fPtr);
		
		assert(buff != NULL);
		printf("State: %c\n", buff[0]);
//		state = buff[0];
	}
	if (mask_bits(options, fTIME)) {
		char *stat_file = "/stat";
		char *file_path = get_file_path_alloc(pid_directory, stat_file);
		FILE *fPtr;
		if ((fPtr = fopen(file_path, "r")) == NULL) {
			printf("Error opening file %s\n", file_path);
			exit(EXIT_FAILURE);
		}
		free(file_path);
		
		char buff[255];
		unsigned int utime = 0;
		unsigned int stime = 0;
		unsigned int seconds = 0;
		unsigned int minutes = 0;
		unsigned int hours = 0;
		
		for (int i = 0; i < 14; i++) {
			fscanf(fPtr, "%s", buff);
			utime = strtoul(buff, NULL, 10);
		}
		fscanf(fPtr, "%s", buff);
		stime = strtoul(buff, NULL, 10);
		fclose(fPtr);
		
		seconds = (utime + stime) / sysconf(_SC_CLK_TCK);
		
		hours = seconds / 3600;
		minutes = (seconds % 3600) / 60;
		seconds = (seconds % 3600) % 60;
		
		printf("Time: %02u:%02u:%02u\n", hours, minutes, seconds);
	}
	if (mask_bits(options, fVIRTUAL)) {
		char *stat_file = "/statm";
		char *file_path = get_file_path_alloc(pid_directory, stat_file);
		FILE *fPtr;
		if ((fPtr = fopen(file_path, "r")) == NULL) {
			printf("Error opening file %s\n", file_path);
			exit(EXIT_FAILURE);
		}
		free(file_path);
		
		unsigned int vmemory;
		char buff[255];
		fscanf(fPtr, "%s", buff);
		vmemory = strtoul(buff, NULL, 10);
		fclose(fPtr);
		
		printf("Virtual memory: %u kB\n", vmemory);
	}
	if (mask_bits(options, fCOMMAND)) {
		char *stat_file = "/cmdline";
		char *file_path = get_file_path_alloc(pid_directory, stat_file);
		FILE *fPtr;
		if ((fPtr = fopen(file_path, "r")) == NULL) {
			printf("Error opening file %s\n", file_path);
			exit(EXIT_FAILURE);
		}
		free(file_path);
		
		char command_line[255];
		fscanf(fPtr, "%s", command_line);
		fclose(fPtr);
		
		printf("Command: %s\n", command_line);
	}
	
	free(pid_directory);
	exit(EXIT_SUCCESS);
}
