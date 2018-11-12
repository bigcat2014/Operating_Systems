/*
 * Author: Logan Thomas
 * Assignment Number: 2
 * Date of Submission: 11/2/18
 * Name of this file: elevator.c
 * Description of the program: Provides struct types and macros
 */

#ifndef ASSIGNMENT2_ELEVATOR_H
#define ASSIGNMENT2_ELEVATOR_H

//<editor-fold desc="Macro definitions">

#define BOOL int
#define TRUE 1
#define FALSE 0

//</editor-fold>

//<editor-fold desc="Structs">

typedef struct __floor_pair {
	unsigned int floor;
	unsigned int time;
}floor_pair;

typedef struct __person {
	unsigned int n;
	unsigned int n_floors_to_visit;
	floor_pair *previous_floor;
	floor_pair *next_floors;
} Person;

//</editor-fold>

#endif //ASSIGNMENT2_ELEVATOR_H
