#ifndef DH_UTILS
#define DH_UTILS

// ----------------------------------- INCLUDES --------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>

#ifdef MD_USE_GSL 	/* GSL */

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#endif 				/* /GSL */

#include "memhandler.h"

// -------------------------------------- PUBLIC MACROS ---------------------------------------------

#define M_MAX_PDB_NAME_LEN 200

#define M_SIGN 1
#define M_NO_SIGN 0


#ifdef MD_USE_GSL	/* GSL */

#define M_GEN_MTWISTER gsl_rng_mt19937
#define M_GEN_GFSR4 gsl_rng_gfsr4
#define M_GEN_TAUS gsl_rng_taus2
#define M_GEN_RANLXS0 gsl_rng_ranlxs0
#define M_GEN_RANLXS1 gsl_rng_ranlxs1

#endif				/* /GSL */

// ------------------------------------ PUBLIC STRUCTURES -------------------------------------------

typedef struct tab_str 
{
	char **t_str ;
	int nb_str ;

} tab_str ; 


// ------------------------------------ PROTOTYPES -------------------------------------------

FILE* my_fopen(const char *fpath, const char *mode) ;

int str_is_number(const char *str, const int sign) ;
int str_is_float(const char *str, const int sign) ;
void str_trim(char *str) ;

tab_str* f_readl(const char *fpath, int nchar_max) ;
void free_tab_str(tab_str *tstr) ;
void print_tab_str(tab_str* strings) ;

int in_tab(int *tab, int size, int val) ;
int index_of(int *tab, int size, int val)  ;


void remove_ext(char *str) ;
void remove_path(char *str) ;
void extract_ext(char *str, char *dest) ;
void extract_path(char *str, char *dest) ;

void start_rand_generator(void) ;
float rand_uniform(float min, float max) ;

FILE* fopen_pdb_check_case(char *name, const char *mode)  ;

#endif
