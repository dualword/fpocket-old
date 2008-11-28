
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

#ifndef DH_DPOCKET
#define DH_DPOCKET

/* ----------------------------INCLUDES-------------------------------------- */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dparams.h"
#include "descriptors.h"
#include "neighbor.h"
#include "pocket.h"
#include "cluster.h"
#include "refine.h"
#include "aa.h"
#include "utils.h"

#include "memhandler.h"

/* ----------------------------------MACROS----------------------------------*/

#define M_DP_EXPLICIT 1
#define M_DP_POCKET   2
#define M_DP_POCETLIG 3

#define M_DP_OUTP_HEADER "pdb ligand overlap lig_vol pocket_vol nb_alpha_spheres mean_asph_ray mean_asph_solv_acc apolar_asphere_prop mean_loc_hyd_dens hydrophobicity_score volume_score polarity_score charge_score flex prop_polar_atm"

#define M_DP_OUTP_FORMAT "%s %s %6.2f %8.2f %8.2f %5d %5.2f %5.2f %4.2f %6.2f %8.2f %6.2f %4d %4d %5.2f %6.2f"
#define M_DP_OUTP_VAR(fc, l, ovlp, lv, d) fc, l, ovlp, lv, d->volume, d->nb_asph, \
									 d->mean_asph_ray, \
									 d->masph_sacc, d->apolar_asphere_prop, \
									 d->mean_loc_hyd_dens, d->hydrophobicity_score, \
									 d->volume_score, d->polarity_score, d->charge_score, \
									 d->flex, d->prop_polar_atm

/* ------------------------------PROTOTYPES-----------------------------------*/

void dpocket(s_dparams *par) ;
void desc_pocket(const char fcomplexe[], const char ligname[], s_dparams *par, 
				 FILE *f[3]) ;

s_atm** get_explicit_desc(s_pdb *pdb_cplx_l, s_lst_vvertice *verts, s_atm **lig, 
						  int nal, s_dparams *par, int *nai, s_desc *desc) ;

void write_pocket_desc(const char fc[], const char l[], s_desc *d, float lv, 
					   float ovlp, FILE *f) ;

#endif
