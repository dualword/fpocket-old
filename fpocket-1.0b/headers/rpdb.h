
#ifndef DH_RPDBB
#define DH_RPDBB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atom.h"
#include "pertable.h"
#include "utils.h"
#include "memhandler.h"


#define M_PDB_LINE_LEN 80   /* actual record size */
#define M_PDB_BUF_LEN  83    /* size need to buffer + CR, LF, and NUL */

#define M_KEEP_LIG  1
#define M_DONT_KEEP_LIG 0

#define M_PDB_HEADER  1
#define M_PDB_REMARK  2
#define M_PDB_ATOM    3
#define M_PDB_CONECT  4
#define M_PDB_HETATM  5
#define M_PDB_CRYST1  6
#define M_PDB_EOF     7
#define M_PDB_END     8
#define M_PDB_UNKNOWN 9

/*
 * API functions start here
 */

typedef struct s_pdb
{
	FILE *fpdb ;

	s_atm *latoms ;			// The list of atoms: contains all atoms!
	s_atm **lhetatm ;		// List of hetatm. Contains pointers to atoms in the latoms list!
	s_atm **latm_lig ;		// List of ligand atoms. Contains pointers to atoms in the latoms list!

	int natoms,			// Number of atoms
		nhetatm,		// Number of HETATM
		natm_lig ;		// Number of ligand atoms

	float A, B, C, 				// Side lengths of the unit cell
		  alpha, beta, gamma ;		// Angle between B and C, A and C, A and C

	char header[M_PDB_BUF_LEN] ;

} s_pdb ;


// ------------------------------------ PUBLIC FUNCTIONS -------------------------------------------

s_pdb* rpdb_open(const char *fpath, const char *ligan, const int keep_lig) ;
void rpdb_read(s_pdb *pdb, const char *ligan, const int keep_lig) ;

void rpdb_extract_cryst1(char *rstr, float *alpha, float *beta, float *gamma, 
						 float *a, float *b, float *c) ;
void rpdb_extract_atom_values(char *pdb_line, float *x, float *y, float *z,
							  float *occ, float *beta) ;
void rpdb_extract_pdb_atom(char *pdb_line, char *type, int *atm_id, char *name, char *alt_loc, 
						   char *res_name, char *chain, int *res_id, char *insert, 
						   float *x, float *y, float *z, float *occ, float *bfactor, 
						   char *symbol, int *charge) ;
void print_pdb_atoms(FILE *f, s_atm *atoms, int natoms) ;

int is_in_lst_atm(s_atm **lst_atm, int nb_atm, int atm_id) ;

float get_mol_mass(s_atm *latoms, int natoms) ;
float get_mol_mass_ptr(s_atm **latoms, int natoms) ;
float get_mol_volume_ptr(s_atm **atoms, int natoms, int niter) ;

void free_pdb_atoms(s_pdb *pdb) ;

#endif
