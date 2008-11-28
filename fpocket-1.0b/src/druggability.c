
#include "../headers/druggability.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					druggability.c
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			02-09-08
##
## ----- SPECIFICATIONS
##
##	This file contains necessary functions to calculate druggability index
##  according to Cheng AC et al. Nat Comp Biol, 25, 1, 2007
##  This index contains information about the apolar solvent accessible 
##  surface of the cavity and the curvature of the cavity.
##
## ----- MODIFICATIONS HISTORY
##
##	02-09-08	(vp) Created
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



void set_druggability_score(c_lst_pockets *pockets)
{
	if(pockets) {
		node_pocket *cur = pockets->first ;
		while(cur) {
			pcur = cur->pocket ;
			node_vertice *nvcur = pcur->v_lst->first ;
			while(nvcur) {
				/* chercher sphere max + plus enfouie */
				nvcur = nvcur->next ;
			}
			print_pocket(f, cur->pocket) ;
			cur = cur->next ;
		}
	}
	else {
		fprintf(f, "\n## NO POCKETS IN THE LIST ##\n");
	}
}