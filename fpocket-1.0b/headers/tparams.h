
#ifndef DH_TPARAMS
#define DH_TPARAMS

// ----------------------------------- INCLUDES --------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>

#include "fparams.h"
#include "utils.h"
#include "memhandler.h"

// -------------------------------------- PUBLIC MACROS ---------------------------------------------

// Options of the test program
#define M_PAR_LIG_NEIG_DIST 'd'
#define M_PAR_VALID_COMPLEX_FILE 'c'
#define M_PAR_VALID_APO_FILE 'a'
#define M_PAR_VALID_LIG_CODE 'l'
#define M_PAR_VALID_INPUT_FILE 'L'
#define M_PAR_P_STATS_OUT 'o'
#define M_PAR_G_STATS_OUT 'e'
#define M_PAR_KEEP_FP_OUTPUT 'k'


//write the statistics output of tpocket to :
#define M_STATS_OUTP "stats_p.txt"
#define M_STATS_OUTG "stats_g.txt"
#define M_MAX_FILE_NAME_LENGTH 300
//#define M_PAR_P_STATS_OUT 'o'
//in order to get the atom set of the pocket, detect around x A of the ligand
#define M_LIG_NEIG_DIST 4.0


// ------------------------------- PUBLIC STRUCTURES --------------------------------------


typedef struct s_tparams
{
	char **fapo,
		 **fcomplex,
		 **fligan;
	char *p_output;
	char *g_output;
	char stats_g[128] ; //M_STATS_OUTG ,
	char stats_p[128] ; //M_STATS_OUTP ;

	float lig_neigh_dist ;
	int nfiles ;
	int keep_fpout ;

// Parameters for the pocket finder program (also needed for validation program...)

	s_fparams *fpar ;

} s_tparams ;

// ----------------------------------- PROTOTYPES ----------------------------------------

s_tparams* init_def_tparams(void) ;
s_tparams* get_tpocket_args(int nargs, char **args) ;
int add_list_data(char *str_list_file, s_tparams *par) ;
int add_prot(char *apo, char *complex, char *ligan, s_tparams *par) ;
int parse_lig_neigh_dist(char *str, s_tparams *p) ;
int parse_keep_fpout(char *str, s_tparams *p) ;
void free_tparams(s_tparams *p);
void print_test_usage(FILE *f) ;
void print_params(s_tparams *p, FILE *f) ;

#endif
