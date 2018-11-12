/*
 * Author: Logan Thomas
 * Assignment Number: 2
 * Date of Submission: 11/2/2018
 * Name of this file: elevator.c
 * Description of the program: Simulates an elevator going up and down to pick people up
 * 		and drop them off using threads and semaphores
 */

//<editor-fold desc="Includes">

#include "parser.h"
#include "helper.h"
#include "elevator.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <assert.h>
#include <semaphore.h>

//</editor-fold>

//<editor-fold desc="DEBUG">

#ifdef DEBUG

// The test struct for testing pthread with arguments
typedef struct __test_arg {
	int a;
	int b;
} test_arg;

// the test struct for testing pthread with return values
typedef struct __test_ret {
	int r;
} test_ret;



// The thread to run with arguments and a return value
void *add(void *arg) {
	test_arg *m = (test_arg *) arg;
	test_ret *ans = malloc(sizeof(test_ret));
	ans->r = m->a + m->b;
	return (void *) ans;
}

#endif

//</editor-fold>

//<editor-fold desc="Structs">

// The test struct for elevator thread arguments
typedef struct __elevator_args {
	unsigned int n_people;
	unsigned int n_floors;
	unsigned int max_wait_time;
} elevator_args;

// The test struct for person thread arguments
typedef struct __person_args {
	Person *person;
} person_args;

typedef enum __direction {
	UP = 1,
	DOWN = -1
} Direction;

//</editor-fold>

//<editor-fold desc="Enumerations">

typedef enum __elevator_state{
	NORMAL = 0,
	CHECK_IF_DONE = 1
} elevator_state;

//</editor-fold>

//<editor-fold desc="Function declarations">

void free_person_array(Person **people, size_t n_people);

void *elevator_thread(void *args);

void *person_thread(void *args);

//</editor-fold>

//<editor-fold desc="Global variables">

sem_t *call_list;
sem_t *exit_list;
sem_t calling_mutex;
sem_t exiting_mutex;
int *num_calling;
int *num_exiting;

//</editor-fold>

int main(int argc, char **argv) {
	unsigned int n_people, max_wander_time, n_floors;
	char file_name[PATH_MAX] = "";
	Person **people = NULL;
	
	// Parse command line options
	parse_args(argc, argv, &n_people, &max_wander_time, &n_floors, file_name);
	
	//<editor-fold desc="DEBUG">

#ifdef DEBUG
	printf("Arguments:\nn_people: %u\nmax_wander_time: %u\nn_floors: %u\nfile_name: %s\n", n_people, max_wander_time,
		   n_floors, file_name);
	
	printf("\npthread test:\n");
	pthread_t p;
	
	// Argument struct
	test_arg arg;
	// Return value struct
	test_ret *m;
	
	// Set the values to add in the arg struct
	arg.a = 20;
	arg.b = 35;
	
	printf("a = %d, b = %d\n", arg.a, arg.b);
	
	// Create the thread
	pthread_create(&p, NULL, add, (void *) &arg);
	
	// Join and get the return value
	pthread_join(p, (void **) &m);
	
	// Print the return value
	printf("a + b = %d\n", m->r);
	
	// Free the memory allocated for the return value
	free(m);
#endif
	
	//</editor-fold>
	
	// Parse the file and get the people structs
	people = calloc(n_people, sizeof(Person *));
	parse_file_alloc(file_name, n_people, people);
	
	//<editor-fold desc="DEBUG">

#ifdef DEBUG
	assert(people != NULL);
	printf("\nPeople:\n");
	for (int i = 0; i < n_people; i++) {
		assert(people[i] != NULL);
		printf("\tPerson number: %u\n\tNumber of next_floors visited:%u\n", people[i]->n, people[i]->n_floors_to_visit);
		for (int j = 0; j < people[i]->n_floors_to_visit; j++) {
			printf("\t\tFloor: %d, time:%d\n", people[i]->next_floors[j].floor, people[i]->next_floors[j].time);
		}
		printf("\n");
	}
#endif
	
	//</editor-fold>
	
	// Allocate memory for the semaphores and counters
	call_list = calloc((size_t)n_floors, sizeof(sem_t));
	exit_list = calloc((size_t)n_floors, sizeof(sem_t));
	num_calling = calloc((size_t)n_floors, sizeof(int));
	num_exiting = calloc((size_t)n_floors, sizeof(int));
	
	// Initialize the semaphores
	sem_init(&calling_mutex, 0, 1);
	sem_init(&exiting_mutex, 0, 1);
	for (int i = 0; i < n_floors; i++){
		sem_init(&call_list[i], 0, n_people);
		sem_init(&exit_list[i], 0, n_people);
		for (int j = 0; j < n_people; j++){
			sem_wait(&call_list[i]);
			sem_wait(&exit_list[i]);
		}
	}
	
	// Thread variables
	pthread_t elevator;
	pthread_t person[n_people];
	
	// Elevator thread arguments
	elevator_args e_args = (elevator_args) {.n_people = n_people, .n_floors = n_floors, .max_wait_time = max_wander_time};
	
	// Array people thread arguments
	person_args p_args[n_people];
	
	// Create the people threads
	for (int i = 0; i < n_people; i++) {
		p_args[i] = (person_args) {.person = people[i]};
		pthread_create(&person[i], NULL, person_thread, (void *) &p_args[i]);
	}
	
	// Create the elevator thread
	pthread_create(&elevator, NULL, elevator_thread, (void *) &e_args);
	
	// Wait for all of the people threads to finish
	for (int i = 0; i < n_people; i++) {
		pthread_join(person[i], NULL);
	}
	// Wait for the exit thread to finish
	pthread_join(elevator, NULL);
	
	// Free all of the memory
	free(call_list);
	free(exit_list);
	free(num_calling);
	free(num_exiting);
	free_person_array(people, n_people);
	exit(EXIT_SUCCESS);
}

//<editor-fold desc="Function definitions">

/*
 * Name: free_person_array
 * Input: Person array and number of people
 * Output: None
 * Description: Frees an array of people
 */
void free_person_array(Person **people, size_t n_people) {
	for (int i = 0; i < n_people; i++) {
		free(people[i]->next_floors);
		free(people[i]);
	}
	free(people);
}

/*
 * Name: elevator_thread
 * Input: Elevator args
 * Output: None
 * Description: The elevator thread that runs
 */
void *elevator_thread(void *args) {
	elevator_state state = NORMAL;
	elevator_args *e_args = (elevator_args *) args;
	unsigned int current_floor = 0;
	Direction direction = UP;
	int num_people_calling = 0;
	int num_people_exiting = 0;
	
	while (1) {
		// If it is the first or last floor, print all elevator calls
		if (current_floor == 0 || current_floor == e_args->n_floors - 1){
			printf("Number of people waiting at floor\n");
			printf("Floor number\t\tNumber of people\n");
			for (int i = 0; i < e_args->n_floors; i++){
				sem_wait(&calling_mutex);
				printf("%u\t\t\t\t\t%u\n", i, num_calling[i]);
				sem_post(&calling_mutex);
			}
		}
		
		// Get the number of people who want to get on
		sem_wait(&calling_mutex);
		num_people_calling = num_calling[current_floor];
		sem_post(&calling_mutex);
		
		// Get the number of people who want to get off
		sem_wait(&exiting_mutex);
		num_people_exiting = num_exiting[current_floor];
		sem_post(&exiting_mutex);
		
		
		// Check if people want to get on or off
		if (num_people_calling || num_people_exiting ) {
			// Open the doors and wait
			printf("Elevator opening doors at floor %u\n", current_floor);
			state = NORMAL;
			sleep(1);
			for(int i = 0; i < num_people_calling; i++){
				sem_post(&call_list[current_floor]);
			}
			for(int i = 0; i < num_people_exiting; i++){
				sem_post(&exit_list[current_floor]);
			}
		}
		
		// Travel between floors
		sleep(1);
		
		// Check elevator bounds
		if (current_floor == 0 && direction == DOWN) {
			
			// If the elevator is already checking if it's done...
			if (state == CHECK_IF_DONE){
				// Wait for the max wander time
				sleep(e_args->max_wait_time);
				
				// See if anyone is calling the elevator
				int total_calling = 0;
				for(int i = 0; i < e_args->n_floors; i++){
					sem_wait(&calling_mutex);
					total_calling += num_calling[i];
					sem_post(&calling_mutex);
				}
				
				// If not, exit
				if (total_calling == 0){
					break;
				}
			}
			// Change the state and direction of the elevator
			printf("Elevator at floor 0 heading to floor %u\n", (e_args->n_floors - 1));
			state = CHECK_IF_DONE;
			direction = UP;
		}
		else if (current_floor == e_args->n_floors - 1 && direction == UP) {
			printf("Elevator at floor %u heading to floor 0\n", (e_args->n_floors - 1));
			// Change the direction of the elevator
			direction = DOWN;
		}
		// Increment the floor
		current_floor += direction * 1;
		
	}
	// Leave the system
	printf("Elevator leaving the system");
	return NULL;
}

/*
 * Name: person_thread
 * Input: Person args
 * Output: None
 * Description: The person thread that runs
 */
void *person_thread(void *args) {
	person_args *p_args = (person_args *) args;
	Person *person = p_args->person;
	person->previous_floor = &((floor_pair) {.floor = 0, .time = 0});
	
	// Loop through each floor
	for (int i = 0; i < person->n_floors_to_visit; i++) {
		// If the person is supposed to wait, then sleep
		if(person->previous_floor->time != 0) {
			printf("Person %u wandering for %u seconds\n", person->n, person->previous_floor->time);
			sleep(person->previous_floor->time);
		}
		
		// Call elevator and wait
		printf("Person %u waiting for elevator at floor %u\n", person->n, person->previous_floor->floor);
		
		// Increment the number of people waiting at this floor
		sem_wait(&calling_mutex);
		num_calling[person->previous_floor->floor]++;
		sem_post(&calling_mutex);
		
		// Wait for the elevator
		sem_wait(&call_list[person->previous_floor->floor]);
		
		// Get on the elevator and decrement the number of people waiting for the elevator at this floor
		sem_wait(&calling_mutex);
		num_calling[person->previous_floor->floor]--;
		sem_post(&calling_mutex);
		
		// The previous floor is the floor we just got off
		person->previous_floor = &person->next_floors[i];

		// Get on elevator to reach destination floor
		printf("Person %u taking elevator to floor %u\n", person->n, person->next_floors[i].floor);
		printf("Person %u waiting to get off at floor %u\n", person->n, person->next_floors[i].floor);
		
		// Increment the number of people waiting to get off the elevator
		sem_wait(&exiting_mutex);
		num_exiting[person->next_floors[i].floor]++;
		sem_post(&exiting_mutex);
		
		// Wait to get off
		sem_wait(&exit_list[person->next_floors[i].floor]);
		
		// Get off elevator and decrement the number of people waiting to get off the elevator
		sem_wait(&exiting_mutex);
		num_exiting[person->next_floors[i].floor]--;
		sem_post(&exiting_mutex);
	}
	
	// Leave the system
	printf("Person %u leaving the system. Goodbye!\n", person->n);
	return NULL;
}

//</editor-fold>
