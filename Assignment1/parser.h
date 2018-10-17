/*
 * Author: Logan Thomas
 * Assignment Number: 1
 * Date of Submission:
 * Name of this file: parser.h
 * Description: Header file for parser.c
 */

#ifndef ASSIGNMENT1_PARSER_H
#define ASSIGNMENT1_PARSER_H

//<editor-fold desc="Includes">

#include "5ps.h"
#include "helper.h"

//</editor-fold>

//<editor-fold desc="Enumerations">

// Option flags
typedef enum {
    // The PID flag
    fPID		= 1 << 0,
    // The State flag
    fSTATE		= 1 << 1,
    // The Time flag
    fTIME		= 1 << 2,
    // The Virtual flag
    fVIRTUAL	= 1 << 3,
    // The Command flag
    fCOMMAND	= 1 << 4
} Option;

//</editor-fold>

//<editor-fold desc="Function declarations">

BOOL parse_args(int argc, char *argv[], int *options, long *pid);
void print_help(char program_name[]);

//</editor-fold>

#endif //ASSIGNMENT1_PARSER_H
