/*
 * Author: Logan Thomas
 * Assignment Number: 1
 * Date of Submission:
 * Name of this file: Assignment1.c
 * Description of the program: Emulates the ps linux function with limited functionality
 */

//<editor-fold desc="Includes">

#include "Assignment1.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//</editor-fold>

int main(int argc, char *argv[]){
    int options = 0;
    long pid = -1;

    if (!parse_args(argc, argv, &options, &pid)) {
        printf("error: garbage option\n\n");
        print_help(argv[0]);
        return 1;
    }

    return 0;
}
