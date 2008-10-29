
#ifndef DH_NEIGHBOR
#define DH_NEIGHBOR

// ------------------------------------INCLUDES-------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "voronoi.h"
#include "atom.h"
#include "rpdb.h"
#include "sort.h"
#include "memhandler.h"

// -------------------------------------MACROS------------------------------------------

#define M_INTERFACE_SEARCH_DIST 8.0
#define M_INTERFACE_SEARCH 1
#define M_NO_INTERFACE_SEARCH 0

// ------------------------------------ PUBLIC STRUCTURES -------------------------------------------

// -------------------------------------PROTOTYPES------------------------------------------

s_atm** get_mol_atm_neigh(s_atm **atoms, int natoms, s_pdb *pdb, float dist_crit, int *nneigh)  ;
s_atm** get_mol_ctd_atm_neigh(s_atm **atoms, int natoms, s_pdb *pdb, s_lst_vvertice *lvert, float vdist_crit, 
							   int interface_search, int *nneigh) ;
s_vvertice** get_mol_vert_neigh(s_atm **atoms, int natoms, s_lst_vvertice *lvert, s_pdb *pdb, float dist_crit, int *nneigh) ;

#endif
