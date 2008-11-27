#ifndef DH_FPARAMS
#define DH_FPARAMS

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

// ----------------------------------- PUBLIC MACROS ---------------------------------------

// Options of the pocket finder program
// standard parameters
//use min alpha sphere radius of : 3.0
#define M_MIN_ASHAPE_SIZE_DEFAULT 3.0
//use max alpha sphere radius of : 6.0
#define M_MAX_ASHAPE_SIZE_DEFAULT 6.0
//use first connection distance (see report) : 2.0
#define M_CLUST_MAX_DIST 2.0
//use second connection distance (see report) : 4.5
#define M_REFINE_DIST 4.5
//at least a proportion of  M_REFINE_MIN_NAPOL_AS apolar alpha spheres in the pocket 0.0
#define M_REFINE_MIN_PROP_APOL_AS 0.0
//single linkage clustering connection distance 2.5
#define M_SLCLUST_MAX_DIST 2.5
//minimum number of common neighbours for single linkage clustering 5
#define M_SLCLUST_MIN_NUM_NEIGH 3
//number of iterations for the Monte Carlo volume calculation 3000
#define M_MC_ITER 3000
//precision for "exact" volume integration, set to -1 if not used -1
#define M_BASIC_VOL_DIVISION -1
//minimum number of alpha spheres for a pocket to be kept 5
#define M_MIN_POCK_NB_ASPH 40
//minimum number of atoms having a low electronegativity in order to declare an alpha sphere to be apolar 3
#define M__MIN_APOL_NEIGH_DEFAULT 3

// flags
#define M_PAR_PDB_FILE 'f'
#define M_PAR_MAX_ASHAPE_SIZE 'M'
#define M_PAR_MIN_ASHAPE_SIZE 'm'
#define M_PAR_MIN_APOL_NEIGH 'A'
#define M_PAR_CLUST_MAX_DIST 'D'
#define M_PAR_SL_MAX_DIST 's'
#define M_PAR_SL_MIN_NUM_NEIGH 'n'
#define M_PAR_MC_ITER 'v'
#define M_PAR_BASIC_VOL_DIVISION 'b'
#define M_PAR_MIN_POCK_NB_ASPH 'i'
#define M_PAR_REFINE_DIST 'r'
#define M_PAR_REFINE_MIN_NAPOL_AS 'p'


// ------------------------------------ PUBLIC STRUCTURES -------------------------------------------

/**
	Structure containing all necessary parameters that can be changed by the user.
	This structure is commun to both programs (validation and pocket finding), even
	if the pocked finding programm doesn't need some parameters.
*/

typedef struct s_fparams
{
	char pdb_path[M_MAX_PDB_NAME_LEN] ;	// The pdb file

	int min_apol_neigh,			// Min number of apolar neighbours for an a-sphere to be an apolar a-sphere
		sl_clust_min_nneigh,		// Min number of neighbours for single linkage clustering
		nb_mcv_iter,			// Number of iteration for the Monte Carlo volume calculation
		basic_volume_div,		// Box division factor for basic volume calculation
		min_pock_nb_asph ;		// Minimump number of alpha spheres per pocket

	float clust_max_dist,			// First clustering distance criterium
		  refine_min_apolar_asphere_prop,		// Min proportion of apolar alpha spheres for each pocket
		  sl_clust_max_dist,		// Single linkage clusturing distance criterium
		  refine_clust_dist,		// Refine clustering distance criterium
		  asph_min_size,	 	// Minimum size of alpha spheres to keep
		  asph_max_size ;		// Maximum size of alpha spheres to keep


} s_fparams ;

// ------------------------------------ PROTOTYPES -------------------------------------------

s_fparams* init_def_fparams(void) ;
s_fparams* get_fpocket_args(int nargs, char **args) ;

int parse_clust_max_dist(char *str, s_fparams *p) ;
int parse_sclust_max_dist(char *str, s_fparams *p) ;
int parse_sclust_min_nneigh(char *str, s_fparams *p) ;
int parse_min_apol_neigh(char *str, s_fparams *p) ;
int parse_asph_min_size(char *str, s_fparams *p) ;
int parse_asph_max_size(char *str, s_fparams *p) ;
int parse_mc_niter(char *str, s_fparams *p) ;
int parse_basic_vol_div(char *str, s_fparams *p)  ;
int parse_refine_dist(char *str, s_fparams *p)  ;
int parse_refine_minaap(char *str, s_fparams *p)  ;
int parse_min_pock_nb_asph(char *str, s_fparams *p) ;

void free_fparams(s_fparams *p) ;
void print_pocket_usage(FILE *f) ;
void print_fparams(s_fparams *p, FILE *f) ;

#endif
