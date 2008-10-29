 

#ifndef DH_WRITEPDB
#define DH_WRITEPDB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------------ PUBLIC FUNCTIONS -------------------------------------------

void write_pdb_atom_line(FILE *f, const char rec_name[], int id, const char atom_name[], 
						 char alt_loc, const char res_name[], const char chain[], 
						 int res_id, const char insert, float x, float y, float z, float occ, 
						 float bfactor,	const char *symbol, int charge)  ;
void write_pqr_atom_line(FILE *f, const char *rec_name, int id, const char *atom_name, 
						 char alt_loc, const char *res_name, const char *chain, 
						 int res_id, const char insert, float x, float y, float z, float charge, 
						 float radius);
#endif
