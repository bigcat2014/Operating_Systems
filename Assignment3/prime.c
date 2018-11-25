/*
 * Author: Logan Thomas
 * Assignment Number: 3
 * Date of Submission: 11/24/18
 * Name of this file: prime.c
 * Description of the program: The child process that calculates primes
 */

//<editor-fold desc="Includes">

#include <stdlib.h>
#include <memory.h>
#include <signal.h>
#include <stdio.h>
#include <zconf.h>

//</editor-fold>

//<editor-fold desc="Global variables">

int process_number;
int priority;
int pid;
long unsigned int current_val = 9;
long unsigned int prime_val_found = 0;
int done = 0;

//</editor-fold>

//<editor-fold desc="Function declarations">

void calculate_primes();

void suspend();

void resume();

void terminate();

//</editor-fold>

int main(int argc, char **argv) {
	// Set the action for the SIGTSTP signal
	struct sigaction sigtstp_action;
	memset(&sigtstp_action, 0, sizeof(sigtstp_action));
	sigtstp_action.sa_handler = &suspend;
	sigaction(SIGTSTP, &sigtstp_action, NULL);
	
	// Set the action for the SIGCONT signal
	struct sigaction sigcont_action;
	memset(&sigcont_action, 0, sizeof(sigcont_action));
	sigcont_action.sa_handler = &resume;
	sigaction(SIGCONT, &sigcont_action, NULL);
	
	// Set the action for the SIGTERM signal
	struct sigaction sigterm_action;
	memset(&sigterm_action, 0, sizeof(sigterm_action));
	sigterm_action.sa_handler = &terminate;
	sigaction(SIGTERM, &sigterm_action, NULL);
	
	// Get the process number, priority, and PID of this process
	process_number = (int) strtol(argv[1], NULL, 10);
	priority = (int) strtol(argv[2], NULL, 10);
	pid = getpid();
	
	// Print starting information
	printf("Process %d: My priority is %d, my PID is %d: I just got started.\n", process_number, priority, getpid());
	printf("I am starting with the number %lu to find the next prime number.\n\n", current_val);
	
	// Start the calculation of the primes
	calculate_primes();
	
	exit(EXIT_SUCCESS);
}

//<editor-fold desc="Function definitions">

/*
 * Name: calculate_primes
 * Input: None
 * Output: None
 * Description: Continuously calculates the next prime number after the one given
 */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void calculate_primes() {
	\
    // Flag for if the current value is a prime number
	int is_prime;
	// Continue until terminate signal is received
	while (!done) {
		// Set the prime flag
		is_prime = 1;
		// Increment the current value
		current_val++;
		// Loop from 2 until the value before the current value to check if it is prime
		for (long unsigned int i = 2; i < current_val; i++) {
			// If done signal received, or the current number is not prime, clear the prime flag and break
			if (current_val % i == 0 || done) {
				is_prime = 0;
				break;
			}
		}
		// If the prime flag is set, replact the current prime value
		if (is_prime) { prime_val_found = current_val; }
	}
}

#pragma clang diagnostic pop

/*
 * Name: suspend
 * Input: None
 * Output: None
 * Description: The handler for the SIGTSTP signal
 */
void suspend() {
	printf("Process %d: My priority is %d, my PID is %d: I am about to be suspended.\n", process_number, priority, pid);
	if (prime_val_found == 0) {
		printf("I have not found a prime number yet.\n\n");
	} else {
		printf("Highest prime number I have found is %lu\n\n", prime_val_found);
	}
}

/*
 * Name: resume
 * Input: None
 * Output: None
 * Description: The handler for the SIGCONT signal
 */
void resume() {
	printf("Process %d: My priority is %d, my PID is %d: I just got resumed.\n", process_number, priority, pid);
	
	if (prime_val_found == 0) {
		printf("I have not found a prime number yet.\n\n");
	} else {
		printf("Highest prime number I have found is %lu\n\n", prime_val_found);
	}
}

/*
 * Name: terminate
 * Input: None
 * Output: None
 * Description: The handler for the SIGTERM signal
 */
void terminate() {
	printf("Process %d: My priority is %d, my PID is %d: I completed my task.\n", process_number, priority, pid);
	if (prime_val_found == 0) {
		printf("I never found a prime number.\n\n");
	} else {
		printf("Highest prime number that I found was %lu\n\n", prime_val_found);
	}
	// Sets the done flag so the process can exit
	done = 1;
}

//</editor-fold>