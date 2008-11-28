
#include "../headers/pscoring.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					pscoring.c
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			28-11-08
##
## ----- SPECIFICATIONS
##
## This file stores scoring functions for pockets.
##
## ----- MODIFICATIONS HISTORY
##
##	28-11-08	(v) Created + Comments UTD
##	
## ----- TODO or SUGGESTIONS
##
##

*/

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

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void set_pocket_score(s_desc *pdesc) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Set a score to a given pocket. The current scoring function has been determined
	using a logistic regression based on an analysis of pocket descriptors.

   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_desc *pdesc: The pocket
   -----------------------------------------------------------------------------
   ## RETURN:
	float: The score
   -----------------------------------------------------------------------------
*/
float score_pocket(s_desc *pdesc) 
{

	double e_tmp = -26.57 + 0.55    * (float)pdesc->nb_asph + 
					        0.97    * (float)pdesc->mean_loc_hyd_dens + 
					        8.42e-2 * (float)pdesc->mean_asph_ray - 
					        1.58e-15* (float)pdesc->masph_sacc - 
							5.85	* (float)pdesc->flex - 
							1.49e-16* (float)pdesc->hydrophobicity_score + 
							1.41e-14* (float)pdesc->prop_polar_atm - 
							2.0		* (float)pdesc->volume_score + 
							3.03e-16* (float)pdesc->charge_score ;
	
	return e_tmp/(1.0 + e_tmp) ;
}


/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	set_pocket_score2
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Set a score to a given pocket. The current scoring function has been determined
	using a logistic regression based on an analysis of pocket descriptors.

   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_pocket *pocket: The pocket
   -----------------------------------------------------------------------------
   ## RETURN:
	float: The score
   -----------------------------------------------------------------------------
*/
float score_pocket2(s_desc *pdesc) 
{
/*
 *	
	double e_tmp = -14.835 + 0.29*(float)pdesc->nb_asph +
							 0.52*(float)pdesc->mean_loc_hyd_dens-
							 51.77*(float)pdesc->masph_sacc+
							 10.678*(float)pdesc->mean_asph_ray;
*/

/*	
	double e_tmp = -14.835 + 0.56*(float)pdesc->nb_asph +
							 0.72*(float)pdesc->mean_loc_hyd_dens-
							 59.77*(float)pdesc->masph_sacc+
							 6.678*(float)pdesc->mean_asph_ray;
*/
	
	double e_tmp = -14.835 + 0.56  *(float)pdesc->nb_asph +
							 0.72  *(float)pdesc->mean_loc_hyd_dens-
							 59.77 *(float)pdesc->masph_sacc+
							 6.678 *(float)pdesc->mean_asph_ray;
	
	return e_tmp ;
}
