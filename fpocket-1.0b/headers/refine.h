
#ifndef DH_REFINE
#define DH_REFINE



// -----------------------------------INCLUDES--------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "voronoi.h"
#include "calc.h"
#include "pocket.h"
#include "fparams.h"

//maximal distance between two voronoi neighbouring voronoi vertices in order to be considered as neighbours
#define MAX_CLUST_DIST 2.5
//minimum number of alpha spheres in one pocket
#define MIN_NB_ALPHA_SPHERES 15
//minimum number of apolar alpha spheres in one pocket
#define M_MIN_APOL_ALPHA_SPH 5

// ------------------------------------ PUBLIC STRUCTURES -------------------------------------------


// ------------------------------------PROTOTYPES-------------------------------------------

void refinePockets(c_lst_pockets *pockets, s_fparams *params);
void reIndexPockets(c_lst_pockets *pockets);
void dropSmallNpolarPockets(c_lst_pockets *pockets, s_fparams *params);

#endif
