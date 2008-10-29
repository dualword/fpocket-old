
#ifndef DH_MEMHANDLER
#define DH_MEMHANDLER

// ------------------------------------INCLUDES-------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>


// -------------------------------------MACROS------------------------------------------

#define M_EXIT 1
#define M_CONTINUE 0

// ------------------------------------ PUBLIC STRUCTURES -------------------------------------------

// -------------------------------------PROTOTYPES------------------------------------------

void* my_malloc(size_t nb) ;
void* my_calloc(size_t nb, size_t s) ;
void* my_realloc(void *ptr, size_t nb) ;
void my_free(void *bloc) ;
void my_exit(void) ;

void free_all(void) ;
void print_ptr_lst(void) ;

#endif
