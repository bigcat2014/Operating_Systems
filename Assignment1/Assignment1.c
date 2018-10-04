/*
    Author: Logan Thomas
    Assignment Number: 1
    Date of Submission:
    Name of this file: Assignment1.c
    Description of the program: Emulates the ps linux function with limited functionality
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Assignment1.h"


BOOL debug = FALSE;

int main(int argc, char *argv[]){
    int options = fERROR;
    ParsingState parsing_state = IDLE;
    long pid = NULL;

    // Preliminary loop through args to get options and determine debug mode
    for (int i = 1; i < argc; i++){
        // Get options
        get_option(argv[i], &parsing_state, &options, &pid);
    }

    return 0;
}

// Bit manipulation
/*
    Name: set_bits
    Input: A destination to manipulate and a mask to set bits in the destination
    Output: None
    Description: Sets specific bits in a destination according to a bit mask
*/
void set_bits(int *dest, Option mask){ *dest = *dest | mask; }
/*
    Name: clear_bits
    Input: A destination to manipulate and a mask to clear bits in the destination
    Output: None
    Description: Clears specific bits in a destination according to a bit mask
*/
void clear_bits(int *dest, Option mask){ *dest = *dest & ~mask; }
/*
    Name: mask_bits
    Input: A destination to check and a mask of bits to verify
    Output: Whether or not the bits according to the mask are set
    Description: Checks if the bits in the destination are set according to the mask
*/
BOOL mask_bits(int options, Option mask){ return options & mask; }

// Argument parsing
/*
    Name: get_option
    Input: A string of one argument to check, the current state of option parsing, and
        a pointer to the destination of the pid value
    Output: None
    Description: Parses the arguments to find options
*/
void get_option(char *arg, ParsingState *parsing_state, int *options, long *pid){
    // If a -p flag was just received, parse the pid
    if (*parsing_state == P_RECEIVED){
        // Gets the pid value
        *pid = strtol(arg, NULL, 10);
        // Return to idle state
        *parsing_state = IDLE;

        // If a valid pid was given, return...
        if (*pid){ return; }
        // Otherwise, unset the -p flag and continue parsing
        else { clear_bits(options, fPID); }
    }

    // Checks for each of the possible options
    if (!strcmp(arg, DEBUG_OPTION)) { set_bits(options, fDEBUG); }
    else if (!strcmp(arg, STATE_OPTION)) { set_bits(options, fSTATE); }
    else if (!strcmp(arg, TIME_OPTION)) { set_bits(options, fTIME); }
    else if (!strcmp(arg, VIRTUAL_OPTION)) { set_bits(options, fVIRTUAL); }
    else if (!strcmp(arg, COMMAND_OPTION)) { set_bits(options, fCOMMAND); }
    else if (!strcmp(arg, PID_OPTION)) {
        *parsing_state = P_RECEIVED;
        set_bits(options, fPID);
    }
}