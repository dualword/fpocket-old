
#ifndef DH_DPARAMS
#define DH_DPARAMS

// ----------------------------------- INCLUDES --------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>

#include "utils.h"
#include "memhandler.h"
#include "fparams.h"

// -------------------------------------- PUBLIC MACROS ---------------------------------------------

// Some macro

// Options of the test program
#define M_DPAR_INTERFACE_METHOD1 'e'
#define M_DPAR_INTERFACE_METHOD2 'E'
#define M_DPAR_DISTANCE_CRITERIA 'd'
#define M_DPAR_INPUT_FILE 'f'
#define M_DPAR_OUTPUT_FILE 'o'

// Two way of getting an interface: 
// 1 - Get atoms ocontacted by vertices within M_VERT_LIG_NEIG_DIST of the ligand.
// 2 - Just get atoms within M_LIG_NEIG_DIST of the ligand, based on each ligand atom.
#define M_INTERFACE_METHOD1 1	
#define M_VERT_LIG_NEIG_DIST 4.0

#define M_INTERFACE_METHOD2 2
#define M_LIG_NEIG_DIST 4.25

#define M_OUTPUT_FILE1_DEFAULT "dpout_explicitp.txt"
#define M_OUTPUT_FILE2_DEFAULT "dpout_fpocketp.txt"
#define M_OUTPUT_FILE3_DEFAULT "dpout_fpocketnp.txt"

#define M_MAX_FILE_NAME_LENGTH 300
#define M_NB_MC_ITER 2500
#define M_MIN_ASPH_RAY 2.8
#define M_MAX_ASPH_RAY 10.0

// ------------------------------- PUBLIC STRUCTURES --------------------------------------


typedef struct s_dparams
{
	char **fcomplex,
		 **ligs ;

	char *f_exp,
		 *f_fpckp,
		 *f_fpcknp ;

	float interface_dist_crit;

	int nfiles,
		interface_method ;

	s_fparams *fpar ;

} s_dparams ;

// ----------------------------------- PROTOTYPES ----------------------------------------

s_dparams* init_def_dparams(void) ;
s_dparams* get_dpocket_args(int nargs, char **args) ;

int add_list_complexes(char *str_list_file, s_dparams *par) ;
int add_complexe(char *complex, char *ligand, s_dparams *par) ;
int parse_interface_method(char *str, s_dparams *p)  ;
int parse_dist_crit(char *str, s_dparams *p) ;

void print_dparams(s_dparams *p, FILE *f) ;
void print_dpocket_usage(FILE *f) ;
void free_dparams(s_dparams *p) ;

#endif
