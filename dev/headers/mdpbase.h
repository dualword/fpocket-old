
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

#ifndef DH_MDPBASE
#define DH_MDPBASE

/* ----------------------------- INCLUDES ------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

#include "fpocket.h"
#include "fpout.h"
#include "writepocket.h"
#include "tpocket.h"
#include "dparams.h"
#include "descriptors.h"
#include "neighbor.h"
#include "pocket.h"
#include "cluster.h"
#include "refine.h"
#include "aa.h"
#include "utils.h"
#include "mdparams.h"
#include "memhandler.h"

/* ---------------------------------MACROS----------------------------------*/

#define M_MDP_GRID_RESOLUTION 1.0   /**< grid resolution in Angstroems*/
#define M_MDP_CUBE_SIDE 2.0         /**< size of the side of the cube to count vvertices*/
#define M_MDP_WP_ATOM_DIST 4.0      /**< max distance for constructing the atom set of the pocket with voronoi vertices*/
/* -------------------------------STRUCTURES--------------------------------*/

/**
 Structure handle for the md concat object
 */
typedef struct s_mdconcat
{
    float **vertpos;    /**< nx4 array of alpha sphere center + radius */
    size_t n_vertpos;   /**< number of vertices in all snapshots*/
    int n_snapshots;    /**< number of snapshots of the trajectory*/
} s_mdconcat ;

/**
 Structure handle for the grid
 */
typedef struct s_mdgrid
{
    float ***gridvalues;    /**< values of the md grid (i.e. number of alpha spheres nearby*/
    float *origin;          /**< origin of the grid (3 positons, xyz)*/
    int nx,ny,nz;           /**< gridsize at the x, y, z axis*/
    float resolution;       /**< resolution of the grid; in general 1A*/
    int n_snapshots;        /**< number of snapshots of the trajectory*/
} s_mdgrid;

/* -------------------------------PROTOTYPES--------------------------------*/

void store_vertice_positions(s_mdconcat *m,c_lst_pockets *pockets);

s_mdgrid *calculate_md_grid(s_mdconcat *mdconcat);
s_mdconcat *init_md_concat(void);
s_mdgrid *init_md_grid(s_mdconcat *mdc);
void alloc_first_md_concat(s_mdconcat *m,size_t n);
void realloc_md_concat(s_mdconcat *m,size_t n);
void free_mdconcat(s_mdconcat *m);


#endif
