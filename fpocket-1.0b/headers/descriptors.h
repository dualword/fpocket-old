
#ifndef DH_DESCR
#define DH_DESCR

// -----------------------------------INCLUDES--------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "voronoi.h"
#include "voronoi_lst.h"
#include "atom.h"
#include "aa.h"
#include "utils.h"

// ------------------------------------STRUCTURES-------------------------------------------

typedef struct s_desc 
{
	float hydrophobicity_score,		// Hydropathie score - for each aa
		  volume_score,				// Volume score - for each aa
		  volume,					// Pocket volume
		  flex,						// Flexibility - based on B factors
		  prop_polar_atm,			// Proportion of polar atoms
		  mean_asph_ray,			// Mean alpha sphere radius
		  masph_sacc,	// Mean alpha sphere solvent accessibility
		  apolar_asphere_prop,		// Proportion of apolar alpha spheres
		  mean_loc_hyd_dens ;		// Mean local hydrophobic density (from alpha spheres)
	
	int aa_compo[20] ;				// Absolute amino acid composition
	int nb_asph,					// Number of alpha spheres
		polarity_score,				// Polarity score (based on amino acids properties ; see aa.c & aa.h)
		charge_score ;				// Sum of all net charges at pH = 7 (see aa.c & aa.h)

} s_desc ;

// ------------------------------------PROTOTYPES-------------------------------------------

s_desc* allocate_s_desc(void) ;
void reset_s_desc(s_desc *desc) ;

void set_descriptors(s_atm **atoms, int natoms, s_vvertice **lst_vert, int nvert, s_desc *desc) ;

int get_vert_apolar_density(s_vvertice **lst_vert, int nvert, s_vvertice *vert) ;
void set_atom_based_descriptors(s_atm **atoms, int natoms, s_desc *desc) ;
void set_aa_desc(s_desc *desc, const char *aa_name) ;


#endif
