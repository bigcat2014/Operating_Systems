//
// Created by logan on 10/3/18.
//

#ifndef ASSIGNMENT1_ASSIGNMENT1_H
#define ASSIGNMENT1_ASSIGNMENT1_H

#define BOOL int
#define TRUE 1
#define FALSE 0

#define DEBUG_OPTION "-d"
#define PID_OPTION "-p"
#define STATE_OPTION "-s"
#define TIME_OPTION "-t"
#define VIRTUAL_OPTION "-v"
#define COMMAND_OPTION "-c"

typedef enum {
    fERROR		= 0,
    fDEBUG		= 1 << 0,
    fPID		= 1 << 1,
    fSTATE		= 1 << 2,
    fTIME		= 1 << 3,
    fVIRTUAL	= 1 << 4,
    fCOMMAND	= 1 << 5
} Option;

typedef enum {
    IDLE       = 0,
    P_RECEIVED = 1 << 0
} ParsingState;

void set_bits(int *dest, Option mask);
void clear_bits(int *dest, Option mask);
int mask_bits(int options, Option mask);

void get_option(char *arg, ParsingState *parsing_state, int *options, long *pid);

#endif //ASSIGNMENT1_ASSIGNMENT1_H
