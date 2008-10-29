
#ifndef DH_FPOUT
#define DH_FPOUT

// -----------------------------------INCLUDES--------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "pocket.h"
#include "utils.h"
#include "writepdb.h"
#include "writepocket.h"
#include "write_visu.h"
#include "fparams.h"

// ------------------------------------PROTOTYPES-------------------------------------------

void write_out_fpocket(c_lst_pockets *pockets, s_pdb *pdb, s_fparams *params) ;

#endif
