
#ifndef DH_ATOM
#define DH_ATOM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"

// ------------------------------------MACROS-------------------------------------------



// ------------------------------------ PUBLIC STRUCTURES -------------------------------------------

/***
	A structure for the modelisation of an atom
**/
typedef struct s_atm
{
	int sort_x;					// Index in the sorted tab by X coord
	
	float x, y, z ;				// Coords
	char name[5],				// Atom name
		 type[7],				// Atom type
		 chain[2],				// Chain name
		 symbol[3],				// Chemical symbol of the atom
		 res_name[8];			// Atom residue name

	int id,						// Atom id
		res_id,					// Atom residue ID
		atype,
		charge ;				// Atom charge

	// Optional fields
	float mass,					// Mass
		  radius,				// Vdw radius
		  electroneg,			// Electronegativity
		  occupancy,			// Occupancy
		  bfactor ;				// B-factor for christal structures
	
	char pdb_insert, 			// PDB insertion code
		 pdb_aloc;				// PDB alternate location code
	
	int atomic_num ;   			// Atomic number

} s_atm ;

// ------------------------------------PROTOTYPES-------------------------------------------

float get_mol_mass(s_atm *latoms, int natoms) ;
float get_mol_mass_ptr(s_atm **latoms, int natoms);
void set_mol_barycenter_ptr(s_atm **latoms, int natoms, float bary[]) ;
float get_mol_volume_ptr(s_atm **atoms, int natoms, int niter) ;

int is_in_lst_atm(s_atm **lst_atm, int nb_atm, int atm_id) ;	
float atm_corsp(s_atm **al1, int nl1, s_atm **pocket_neigh, int nal2) ; 

void print_atoms(FILE *f, s_atm *atoms, int natoms) ;

#endif
