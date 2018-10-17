/*
 * Author: Logan Thomas
 * Assignment Number: 1
 * Date of Submission:
 * Name of this file: helper.c
 * Description of the program: Provides functions for getting process info
 */


//<editor-fold desc="Includes">

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include <dirent.h>
#include <errno.h>
#include <zconf.h>
#include "helper.h"

//</editor-fold>


const char *PROC_DIRECTORY = "/proc/";

//<editor-fold desc="Function Definitions">

/*
 * Name: set_bits
 * Input: A destination to manipulate and a mask to set bits in the destination
 * Output: None
 * Description: Sets specific bits in a destination according to a bit mask
 */
void set_bits(int *dest, int mask) { *dest = *dest | mask; }

/*
 * Name: clear_bits
 * Input: A destination to manipulate and a mask to clear bits in the destination
 * Output: None
 * Description: Clears specific bits in a destination according to a bit mask
 */
void clear_bits(int *dest, int mask) { *dest = *dest & ~mask; }
/*
 * Name: mask_bits
 * Input: A destination to check and a mask of bits to verify
 * Output: Whether or not the bits according to the mask are set
 * Description: Checks if the bits in the destination are set according to the mask
 */
BOOL mask_bits(int options, int mask) { return options & mask; }

/*
 * Name: get_pid_str_alloc
 * Input: A long pid to convert to a string
 * Output: A pointer to the string representing the process
 * Description: Receives a pid as a long, converts it to a string and builds
 *      the folder path to access files for that pid
 *
 * Note: This function allocates memory, caller must remember to free returned pointer!
 */
char *get_pid_str_alloc(long pid) {
	// Gets the length of the pid
	const size_t len_pid = (size_t) snprintf(NULL, 0, "%ld", pid);
	// Makes sure it is a valid length
	assert(len_pid > 0);
	
	// Allocates memory for the string
	char *pid_str = calloc(len_pid + 1, sizeof(char));
	
	// Converts the pid to a string
	int c = snprintf(pid_str, len_pid + 1, "%ld", pid);
	
	// Makes sure the string is null terminated
	assert(pid_str[len_pid] == '\0');
	// Makes sure the number of characters copied is the correct number
	assert(c == len_pid);
	
	// Return the pointer to the new string
	return pid_str;
}

/*
 * Name: get_pid_directory_alloc
 * Input: The pid as a string
 * Output: A pointer to the string representing the folder path of the process
 * Description: Builds the folder path to access files for the pid
 *
 * Note: This function allocates memory, caller must remember to free returned pointer!
 */
char *get_pid_directory_alloc(const char *pid_str) {
	char *pid_directory = NULL;
	
	size_t len_proc_directory = strlen(PROC_DIRECTORY);
	size_t len_pid_str = strlen(pid_str);
	assert(len_proc_directory > 0);
	assert(len_pid_str > 0);
	size_t len_pid_directory = len_proc_directory + len_pid_str;
	
	pid_directory = calloc(len_pid_directory + 1, sizeof(char));
	
	strcpy(pid_directory, PROC_DIRECTORY);
	strcpy(&pid_directory[len_proc_directory], pid_str);
	assert(pid_directory[len_pid_directory + 1] == '\0');
	
	return pid_directory;
}

/*
 * Name: get_file_path_alloc
 * Input: The pid directory as a string, the filename as a string
 * Output: A pointer to the string representing the file path of the specified file
 * Description: Builds the file path of the specified file
 *
 * Note: This function allocates memory, caller must remember to free returned pointer!
 */
char *get_file_path_alloc(const char *pid_directory, const char *file_name) {
	char *file_path = NULL;
	
	size_t len_pid_directory = strlen(pid_directory);
	size_t len_file_name = strlen(file_name);
	assert(len_pid_directory > 0);
	assert(len_file_name > 0);
	size_t len_total = len_pid_directory + len_file_name;
	
	file_path = calloc(len_total + 1, sizeof(char));
	
	strcpy(file_path, pid_directory);
	strcpy(&file_path[len_pid_directory], file_name);
	assert(file_path[len_total] == '\0');
	
	return file_path;
}

/*
 * Name: check_proc_dir_exists
 * Input: The pid as a string
 * Output: The state if the folder exists or not
 * Description: Checks if the pid folder exists in the /proc directory
 */
PROC_DIR_STATE check_proc_dir_exists(char *pid_str) {
	DIR *dir;
	struct dirent *entry;
	
	if ((dir = opendir(PROC_DIRECTORY)) == NULL) {
		perror("opendir() error");
		exit(EXIT_FAILURE);
	} else {
		while ((entry = readdir(dir)) != NULL) {
			errno = 0;
			if ((entry = readdir(dir)) != NULL) {
				if (strcmp(entry->d_name, pid_str) == 0) {
					switch (entry->d_type) {
						case DT_DIR:
							return FOUND;
						default:
							break;
					}
					closedir(dir);
				}
			} else {
				closedir(dir);
				return errno == 0 ? NOT_FOUND : READ_ERROR;
			}
		}
		return NOT_FOUND;
	}
}

/*
 * Name: get_state
 * Input: The pid directory
 * Output: The state of the process
 * Description: Gets the state of the process
 */
char get_state(char *pid_directory){
	char *file_path = get_file_path_alloc(pid_directory, "/stat");
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
	return buff[0];
}

/*
 * Name: get_time
 * Input: The pid directory
 * Output: The time of the process
 * Description: Gets the running time of the process
 */
unsigned int get_time(char *pid_directory){
	char *file_path = get_file_path_alloc(pid_directory, "/stat");
	FILE *fPtr;
	if ((fPtr = fopen(file_path, "r")) == NULL) {
		printf("Error opening file %s\n", file_path);
		exit(EXIT_FAILURE);
	}
	free(file_path);
	
	char buff[255];
	unsigned int utime = 0;
	unsigned int stime = 0;
	for (int i = 0; i < 14; i++) {
		fscanf(fPtr, "%s", buff);
		utime = strtoul(buff, NULL, 10);
	}
	fscanf(fPtr, "%s", buff);
	stime = strtoul(buff, NULL, 10);
	fclose(fPtr);
	
	return (utime + stime) / sysconf(_SC_CLK_TCK);
}

/*
 * Name: get_virtual_memory
 * Input: The pid directory
 * Output: The virtual memory used by the process
 * Description: Gets the virtual memory used by the process
 */
unsigned int get_virtual_memory(char *pid_directory){
	char *file_path = get_file_path_alloc(pid_directory, "/statm");
	FILE *fPtr;
	if ((fPtr = fopen(file_path, "r")) == NULL) {
		printf("Error opening file %s\n", file_path);
		exit(EXIT_FAILURE);
	}
	free(file_path);
	
	char buff[255];
	fscanf(fPtr, "%s", buff);
	fclose(fPtr);
	
	return strtoul(buff, NULL, 10);
}

/*
 * Name: get_command
 * Input: The pid directory, the character array to store the command in
 * Output: The command that started the process
 * Description: Gets the command that started the process the process
 */
void get_command(char *pid_directory, char return_val[]){
	char *file_path = get_file_path_alloc(pid_directory, "/cmdline");
	FILE *fPtr;
	if ((fPtr = fopen(file_path, "r")) == NULL) {
		printf("Error opening file %s\n", file_path);
		exit(EXIT_FAILURE);
	}
	free(file_path);
	
	fscanf(fPtr, "%s", return_val);
	fclose(fPtr);
}

//</editor-fold>
