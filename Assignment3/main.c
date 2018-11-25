/*
 * Author: Logan Thomas
 * Assignment Number: 3
 * Date of Submission: 11/24/18
 * Name of this file: main.c
 * Description of the program: The main function of the assignment
 */

//<editor-fold desc="Includes">

#include "parser.h"
#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <sys/time.h>
#include <signal.h>
#include <memory.h>

//</editor-fold>

//<editor-fold desc="Function declarations">

void sigalrm();

int compare_arrival(const void *a, const void *b);

int compare_priority(const void *a, const void *b);

//</editor-fold>

//<editor-fold desc="Enumerations">

typedef enum __scheduler_state {
	NOT_STARTED,
	STARTED
} scheduler_state;

//</editor-fold>

//<editor-fold desc="Global variables">

Process *processes = NULL;
unsigned int process_count;
Process *current_process = NULL;
int current_time = 0;
int done = 0;
scheduler_state state = NOT_STARTED;

//</editor-fold>

//<editor-fold desc="Main">

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

int main(int argc, char **argv) {
	char file_name[PATH_MAX] = "";
	
	parse_args(argc, argv, file_name);
	parse_file_alloc(file_name, &processes, &process_count);
	
	// Sort by arrival time and priority
	qsort(processes, process_count, sizeof(Process), compare_arrival);
	qsort(processes, process_count, sizeof(Process), compare_priority);
	
	struct sigaction sigalrm_action;
	memset(&sigalrm_action, 0, sizeof(sigalrm_action));
	sigalrm_action.sa_handler = &sigalrm;
	sigaction(SIGALRM, &sigalrm_action, NULL);
	
	struct itimerval timer;
	// The timer goes off 1 second after installation of the timer.
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 1;
	// ... and every 1 second after that.
	timer.it_interval.tv_sec = 1;
	timer.it_interval.tv_usec = 0;
	// Start a timer. It counts down whenever this process is executing.
	setitimer(ITIMER_REAL, &timer, NULL);
	
	while (!done) {}
	
	free(processes);
	exit(EXIT_SUCCESS);
}

#pragma clang diagnostic pop

//</editor-fold>

//<editor-fold desc="Function definitions">

/*
 * Name: sigalrm
 * Input: None
 * Output: None
 * Description: The handler for the SIGALRM signal
 */
void sigalrm() {
	// If the scheduler has started at least one process, check if the current process is done
	if (state == STARTED) {
		// If it is done, terminate it
		if (current_process->cpu_burst == 0) {
			printf("Scheduler: Time now: %d seconds\n", current_time);
			printf("Terminating process %d (PID %d)\n\n", current_process->process_number, current_process->pid);
			kill(current_process->pid, SIGTERM);
			current_process->pid = 0;
		}
	}
	
	// Holds the next process to run
	Process *next_process = NULL;
	// Counts the number of finished processes
	int done_count = 0;
	for (int i = 0; i < process_count; i++) {
		if (processes[i].cpu_burst != 0) {
			if (processes[i].arrival_time <= current_time) {
				next_process = &processes[i];
				break;
			}
		} else {
			done_count++;
		}
	}
	
	// If there was a next process found, start determining if a CS is needed
	if (next_process != NULL) {
		// If the next process is not the current process, then pause the current process
		if (next_process != current_process) {
			if (state == STARTED && current_process->pid != 0) {
				// Pause the current process
				kill(current_process->pid, SIGTSTP);
			}
			
			// If the next process has not been started yet, start it and set its PID
			int next_pid = next_process->pid;
			if (next_pid == 0) {
				int pid = fork();
				// If the current process is the child, start the child process
				if (pid == 0) {
					char process_number[10];
					char priority[10];
					snprintf(process_number, 10, "%d", next_process->process_number);
					snprintf(priority, 10, "%d", next_process->priority);
					
					char *args[] = {"./prime", process_number, priority, NULL};
					execvp(args[0], args);
				}
					// Otherwise, set the pid of the next process
				else {
					next_process->pid = (unsigned int) pid;
				}
			}
				// Otherwise, resume it
			else {
				kill(next_process->pid, SIGCONT);
			}
			
			if (state == NOT_STARTED || current_process->cpu_burst == 0) {
				printf("Scheduler: Time now: %d seconds\n", current_time);
				printf("Scheduling process %d (PID %d)\n\n", next_process->process_number, next_process->pid);
				state = STARTED;
			} else {
				printf("Scheduler: Time now: %d seconds\n", current_time);
				printf("Suspending process %d (PID %d) and %s process %d (PID %d)\n\n",
					   current_process->process_number, current_process->pid, next_pid ? "resuming" : "starting",
					   next_process->process_number,
					   next_process->pid);
			}
		}
		// Decriment the cpu burst time
		next_process->cpu_burst--;
		current_process = next_process;
	}
	// Increase the current time
	current_time++;
	// Check if all processes are done
	done = done_count == process_count;
}

/*
 * Name: compare_arrival
 * Input: The two processes to be compared
 * Output: The comparison value between the arrival time of the processes
 * Description: This function compares the arrival time of two processes
 * 		by subtracting the arrival time of the second process from the
 * 		arrival time of the first process
 */
int compare_arrival(const void *a, const void *b) {
	return ((Process *) a)->arrival_time - ((Process *) b)->arrival_time;
}

/*
 * Name: compare_priority
 * Input: The two processes to be compared
 * Output: The comparison value between the priority of the processes
 * Description: This function compares the priority of two processes
 * 		by subtracting the priority of the second process from the
 * 		priority of the first process
 */
int compare_priority(const void *a, const void *b) {
	return ((Process *) a)->priority - ((Process *) b)->priority;
}

//</editor-fold>
