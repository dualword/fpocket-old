
#ifndef DH_WRITEPOCKET
#define DH_WRITEPOCKET

// -----------------------------------INCLUDES--------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "voronoi.h"
#include "pocket.h"
#include "writepdb.h"
#include "utils.h"

// ------------------------------------ PUBLIC STRUCTURES -------------------------------------------

// ------------------------------------PROTOTYPES-------------------------------------------

void write_pockets_single_pdb(const char out[], s_pdb *pdb, c_lst_pockets *pockets)  ;
void write_pockets_single_pqr(const char out[], c_lst_pockets *pockets);

void write_each_pocket(const char out_path[], c_lst_pockets *pockets) ;
void write_pocket_pdb(const char out[], s_pocket *pocket) ;
void write_pocket_pqr(const char out[], s_pocket *pocket) ;

void write_pdb_atoms(FILE *f, s_atm *atoms, int natoms) ;

#endif
