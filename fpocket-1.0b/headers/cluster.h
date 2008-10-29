
#ifndef DH_CLUSTER
#define DH_CLUSTER

// -----------------------------------INCLUDES--------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "voronoi.h"
#include "calc.h"
#include "pocket.h"

//single linkage clustering at 1 A distance
#define MAX_SINGLE_CLUST_DIST 2.75
//cluster only if more than MIN_NUM_NEIGHBOURS vertices are neighbours
#define MIN_NUM_NEIGHBOURS 4
// ------------------------------------PROTOTYPES-------------------------------------------

void pck_sl_clust(c_lst_pockets *pockets, s_fparams *params);

#endif
