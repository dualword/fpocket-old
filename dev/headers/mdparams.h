
/**
    COPYRIGHT DISCLAIMER

    Vincent Le Guilloux, Peter Schmidtke and Pierre Tuffery, hereby
	disclaim all copyright interest in the program “fpocket” (which
	performs protein cavity detection) written by Vincent Le Guilloux and Peter
	Schmidtke.

    Vincent Le Guilloux  28 November 2008
    Peter Schmidtke      28 November 2008
    Pierre Tuffery       28 November 2008

    GNU GPL

    This file is part of the fpocket package.

    fpocket is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    fpocket is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with fpocket.  If not, see <http://www.gnu.org/licenses/>.

**/

#ifndef DH_MDPARAMS
#define DH_MDPARAMS

/* ----------------------------- INCLUDES ------------------------------------*/

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

/* --------------------------- PUBLIC MACROS ---------------------------------*/


/* Options of the test program */
#define M_DPAR_INTERFACE_METHOD1 'e'
#define M_DPAR_INTERFACE_METHOD2 'E'
#define M_DPAR_DISTANCE_CRITERIA 'd'
#define M_DPAR_INPUT_FILE 'f'
#define M_DPAR_OUTPUT_FILE 'o'

/* Two way of defining the interface: */
/* 1 - Get atoms ocontacted by vertices within M_VERT_LIG_NEIG_DIST of the ligand. */
/* 2 - Just get atoms within M_LIG_NEIG_DIST of the ligand, based on each ligand atom. */
#define M_INTERFACE_METHOD1 1
#define M_VERT_LIG_NEIG_DIST 4.0

#define M_INTERFACE_METHOD2 2
#define M_LIG_NEIG_DIST 4.0

#define M_OUTPUT_FILE1_DEFAULT "dpout_explicitp.txt"
#define M_OUTPUT_FILE2_DEFAULT "dpout_fpocketp.txt"
#define M_OUTPUT_FILE3_DEFAULT "dpout_fpocketnp.txt"

#define M_MAX_FILE_NAME_LENGTH 300
#define M_NB_MC_ITER 2500
#define M_MIN_ASPH_RAY 2.8
#define M_MAX_ASPH_RAY 10.0

#define M_MDP_USAGE "\
***** USAGE (mfpocket) *****\n\
\n\
Pocket finding on a MD trajectory -> list of pdb - file(s):             \n\
\t./bin/mdpocket -F pdb_list                                  \n\
\nOPTIONS (find standard parameters in brackets)           \n\n\
\t-m (float)  : Minimum radius of an alpha-sphere.      (3.0)\n\
\t-M (float)  : Maximum radius of an alpha-sphere.      (6.0)\n\
\t-A (int)    : Minimum number of apolar neighbor for        \n\
\t              an a-sphere to be considered as apolar.   (3)\n\
\t-i (int)    : Minimum number of a-sphere per pocket.   (30)\n\
\t-D (float)  : Maximum distance for first clustering        \n\
\t              algorithm.                             (1.73)\n\
\t-s (float)  : Maximum distance for single linkage          \n\
\t              clustering                              (2.5)\n\
\t-n (integer): Minimum number of neighbor close from        \n\
\t              each other (not merged otherwise).        (3)\n\
\t-r (float)  : Maximum distance between two pockets         \n\
\t              barycenter (merged otherwise).          (4.5)\n\
\t-p (float)  : Minimum proportion of apolar sphere in       \n\
\t              a pocket (remove otherwise)             (0.0)\n\
\t-v (integer): Number of Monte-Carlo iteration for the      \n\
\t              calculation of each pocket volume.     (2500)\n\
\t-b (integer): Space approximation for the basic method     \n\
\t              of the volume calculation. Not used by       \n\
\t              default (Monte Carlo approximation is)       \n\
\nSee the manual (man fpocket), or the full documentation for\n\
more information.\n\
***************************\n" /**< the usage print content*/

/* --------------------------- PUBLIC STRUCTURES -----------------------------*/


typedef struct s_mdparams
{
	char **fcomplex,    /**< path of the holo form of the structure */
		 **ligs ;   /**< HET residue name of the ligand */

	char *f_exp,    /**< name of the explicit pocket definition output file*/
		 *f_fpckp, /**< name of the pocket definition output file*/
		 *f_fpcknp ; /**< name of the non pocket definition output file*/

	float interface_dist_crit;  /**< distance for explicit binding pocket definition*/

	int nfiles,         /**< number of files to analyse*/
	    interface_method ;  /**< how to identify the explicit binding pocket*/

	s_fparams *fpar ;   /**< fparams container*/

} s_mdparams ;

/* ----------------------------- PROTOTYPES --------------------------------- */

s_mdparams* init_def_mdparams(void) ;
s_mdparams* get_mdpocket_args(int nargs, char **args) ;

int add_list_complexes(char *str_list_file, s_mdparams *par) ;
int add_complexe(char *complex, char *ligand, s_mdparams *par) ;
int parse_dist_crit(char *str, s_mdparams *p) ;

void print_mdparams(s_mdparams *p, FILE *f) ;
void print_mdpocket_usage(FILE *f) ;
void free_mdparams(s_mdparams *p) ;

#endif
