#ifndef DH_WRITE_VISU
#define DH_WRITE_VISU

// ----------------------------------- INCLUDES --------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

// -------------------------------------- PUBLIC MACROS ---------------------------------------------

// ------------------------------------ PUBLIC STRUCTURES -------------------------------------------


// ------------------------------------ PROTOTYPES -------------------------------------------


void write_visualization(char *pdb_name,char *pdb_out_name);
void write_vmd(char *pdb_name,char *pdb_out_name);
void write_pymol(char *pdb_name,char *pdb_out_name);

#endif
 
