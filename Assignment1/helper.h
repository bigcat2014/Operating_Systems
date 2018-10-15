//
// Created by logan on 10/8/18.
//

#ifndef ASSIGNMENT1_HELPER_H
#define ASSIGNMENT1_HELPER_H

//<editor-fold desc="Includes">

#include "5ps.h"
#include <stdlib.h>

//</editor-fold>

//<editor-fold desc="Enumerations">

typedef enum {
    FOUND       = 0,
    NOT_FOUND   = 1 << 0,
    READ_ERROR  = 1 << 1
} PROC_DIR_STATE;

//</editor-fold>

//<editor-fold desc="Function Declarations">

void set_bits(int *dest, int mask);
void clear_bits(int *dest, int mask);
BOOL mask_bits(int options, int mask);

char* get_pid_str_alloc(long pid);
char* get_pid_directory_alloc(const char *pid_str);
char* get_file_path_alloc(const char *pid_directory, const char *file_name);

PROC_DIR_STATE check_proc_dir_exists(char *pid_str);



//</editor-fold>

#endif //ASSIGNMENT1_HELPER_H
