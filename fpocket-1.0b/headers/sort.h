
#ifndef DH_SORT
#define DH_SORT

// -----------------------------------INCLUDES--------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "rpdb.h"
#include "voronoi.h"

#include "memhandler.h"

// ------------------------------------MACROS-------------------------------------------

#define M_ATOM_TYPE 0
#define M_VERTICE_TYPE 1

#define M_SORT_X 1
#define M_SORT_Y 2
#define M_SORT_Z 3

// ------------------------------------STRUCTURES-------------------------------------------
/**
	A vector (basicly here will be  either atoms or vertices)
*/
typedef struct s_vect_elem 
{
	void *data ;		// Pointer to data
	int type ;		// Type of data (either s_atm or s_vvertice)

} s_vect_elem ;

/**
	A list of vector (basicly this structure contains atoms and vertices)
*/
typedef struct s_vsort
{
	s_vect_elem *xsort ;	// Elements sorted by x coord
	int nelem ;		// Number of elements

} s_vsort ;


// ------------------------------------PROTOTYPES-------------------------------------------

s_vsort* get_sorted_list(s_pdb *pdb, s_lst_vvertice *lvert) ;
void print_sorted_lst(s_vsort *lsort, FILE *buf) ;
void free_s_vsort(s_vsort *lsort) ;

#endif
