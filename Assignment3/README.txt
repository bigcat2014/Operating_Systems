/*
 *   Author: Logan Thomas
 *   Assignment number: 3
 *   Date of submission: 11/24/18
 *   Name of this file: README.txt
 */

Setup:
    Compile the prime.c file, which is the child process using the following command:
    gcc -std=c99 -D_POSIX_C_SOURCE -o prime prime.c

    Then compile the main.c, helper.c, and parser.c files using the following command:
    gcc -std=c99 -D_POSIX_C_SOURCE -o Assignment3 main.c helper.c parser.c

Running:
    Run the Assignment3 executable.
    Program must be run with a file name given as an argument.

    ex: ./Assignment3 Inputs/input.txt

Output:
    Outputs statuses on which processes are being scheduled, resumed, paused, and terminated.
    The child processes spawned output the status of their prime number calculation each time
        they are scheduled, resumed, paused, and terminated.

Notes:
    To view current spawned processes of this assignment in real time, start the program using the following command:
    ./Assignment3 Inputs/input.txt >> output.txt &

    Then use the following command to view the current processes for the assignment as they're spawned and terminated:
    watch -n1 'ps -ely | grep -E "(Assignment3|prime)"'

    Once output from the watch command stops, this means Assignment3 has finished. Use Ctrl+C to stop the watch.
    The output of Assignment3 will be in output.txt

See DEMO.txt to view demos of the program