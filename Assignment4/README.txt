/*
 *   Author: Logan Thomas
 *   Assignment number: 4
 *   Date of submission: 12/11/18
 *   Name of this file: README.txt
 */

Setup:
    Compile the Assignment4.c, Assignment4.h files using the following command:
    gcc -std=c99 -o Assignment4 Assignment4.c Assignment4.h

Running:
    Run the Assignment4 executable.
    Program must be run with a file name given as an argument.

    ex: ./Assignment4 addressesModified.txt

Output:
    Outputs a file called output.txt containing the virtual addresses, physical addresses, and value at the address
    Also outputs the percentage of accesses resulted in page faults to standart out

Notes:
    The input file is passed as an argument, not redirected input!

See DEMO.txt for a demo of the program
