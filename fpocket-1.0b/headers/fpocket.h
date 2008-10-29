
#ifndef DH_FPOCKET
#define DH_FPOCKET

// -----------------------------------INCLUDES--------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "rpdb.h"
#include "voronoi.h"

#include "pocket.h"
#include "cluster.h"
#include "refine.h"
#include "descriptors.h"

#include "fparams.h"
#include "memhandler.h"

// ------------------------------------PROTOTYPES-------------------------------------------

c_lst_pockets* search_pocket(s_pdb *pdb, s_fparams *params) ;

#endif
