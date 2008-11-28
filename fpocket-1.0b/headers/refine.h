
/**
    COPYRIGHT DISCLAIMER

    Vincent Le Guilloux, Peter Schmidtke and Pierre Tuffery, hereby
	disclaim all copyright interest in the program “fpocket” (which
	performs protein cavity detection) written by Vincent Le Guilloux and Peter
	Schmidtke.

    Vincent Le Guilloux  28 November 2008
    Peter Schmidtke      28 November 2008
    Pierre Tuffery       28 November 2008

    GNU GPL

    This file is part of the fpocket package.

    fpocket is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    fpocket is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with fpocket.  If not, see <http://www.gnu.org/licenses/>.

**/

#ifndef DH_REFINE
#define DH_REFINE



/* -------------------------------INCLUDES--------------------------------- */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "voronoi.h"
#include "calc.h"
#include "pocket.h"
#include "fparams.h"

/* Maximal distance between two voronoi neighbouring voronoi vertices in order 
 * to be considered as neighbours */
#define MAX_CLUST_DIST 2.5

/* Minimum number of alpha spheres in one pocket */
#define MIN_NB_ALPHA_SPHERES 15

/*minimum number of apolar alpha spheres in one pocket */
#define M_MIN_APOL_ALPHA_SPH 5

/* -------------------------- PUBLIC STRUCTURES ------------------------------*/


/* --------------------------PROTOTYPES---------------------------------------*/

void refinePockets(c_lst_pockets *pockets, s_fparams *params);
void reIndexPockets(c_lst_pockets *pockets);
void dropSmallNpolarPockets(c_lst_pockets *pockets, s_fparams *params);

#endif
