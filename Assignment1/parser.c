/*
 * Author: Logan Thomas
 * Assignment Number: 1
 * Date of Submission:
 * Name of this file: parser.c
 * Description of the program: Provides a function for parsing command line arguments
 */

//<editor-fold desc="Includes">

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//</editor-fold>

//<editor-fold desc="Define constants">

// Possible options
#define PID_OPTION "-p"
#define STATE_OPTION "-s"
#define TIME_OPTION "-t"
#define VIRTUAL_OPTION "-v"
#define COMMAND_OPTION "-c"

//</editor-fold>

//<editor-fold desc="Enumerations">

// Parser states
typedef enum {
    // The error state
    ERROR      = 0,
    // The idle state
    IDLE       = 1 << 0,
    // The -p flag was received state
    P_RECEIVED = 1 << 1
} ParsingState;

//</editor-fold>

//<editor-fold desc="Argument parsing">

/*
 * Name: get_option
 * Input: A string of one argument to check, the current state of the parser, and
 *     a pointer to the destination of the pid value
 * Output: The next state of the parser
 * Description: Parses the argument to find an option
 */
ParsingState get_option(char *arg, ParsingState parsing_state, int *options, long *pid) {
    ParsingState next_state = parsing_state;

    // If a -p flag was just received, parse the pid...
    if (parsing_state == P_RECEIVED) {
        // Gets the pid value
        *pid = strtol(arg, NULL, 10);
        next_state = IDLE;

        // If a valid pid was given, return...
        if (!(*pid)) { next_state = ERROR; }

    }
    // Otherwise, check for another argument
    else {
        // Checks for each of the possible options
        if (!strcmp(arg, STATE_OPTION)) { set_bits(options, fSTATE); }
        else if (!strcmp(arg, TIME_OPTION)) { set_bits(options, fTIME); }
        else if (!strcmp(arg, VIRTUAL_OPTION)) { set_bits(options, fVIRTUAL); }
        else if (!strcmp(arg, COMMAND_OPTION)) { set_bits(options, fCOMMAND); }
        else if (!strcmp(arg, PID_OPTION)) {
            next_state = P_RECEIVED;
            set_bits(options, fPID);
        } else {
            next_state = ERROR;
        }
    }

    // Return the next state of the parser
    return next_state;
}

/*
 * Name: parse_args
 * Input: The number of arguments in the argument vector, the argument vector, a pointer
 *      to the options, and a pointer to the pid
 * Output: BOOL representing if parsing passed or failed
 * Description: The high level function to parse all of the arguments and set all
 *      of the flags and values.
 */
BOOL parse_args(int argc, char *argv[], int *options, long *pid){
    ParsingState parsing_state = IDLE;

    // Loop through args to get options
    for (int i = 1; i < argc; i++){
        // Get options
        parsing_state = get_option(argv[i], parsing_state, options, pid);
        if (parsing_state == ERROR){
            return FALSE;
        }
    }

    // If no options are present, exit the program
    return (*options && (mask_bits(*options, fPID) ? (*pid > 0) : TRUE));
}

/*
 * Name: print_help
 * Input: None
 * Output: None
 * Description: Prints the help message of the program
 */
void print_help(char program_name[]){
    printf("Usage:\n");
    printf("%s [options]\n\n", program_name);
    printf("Options:\n");

    // PID help message
    printf("\t%s [PID]\t"
           "Display process information only for the PID specified\n",
           PID_OPTION);

    // State help message
    printf("\t%s\t\t"
           "Display the single-character state information about the process\n",
           STATE_OPTION);

    // Time help message
    printf("\t%s\t\t"
           "Display the amount of time consumed by this process in hours:min:seconds format\n",
           TIME_OPTION);

    // Virtual help message
    printf("\t%s\t\t"
           "Display the amount of virtual memory currently being used (in pages) by this program\n",
           VIRTUAL_OPTION);

    // Command help message
    printf("\t%s\t\t"
           "Display the command-line that started this program\n",
           COMMAND_OPTION);
}

//</editor-fold>