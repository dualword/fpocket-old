
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
##	02-09-08	(vp) Created (random date...)
##	
*/

void set_druggability_score(c_lst_pockets *pockets)
{
	if(pockets) {
		node_pocket *cur = pockets->first ;
		while(cur) {
			pcur = cur->pocket ;
			node_vertice *nvcur = pcur->v_lst->first ;
			while(nvcur) {
				//chercher sphere max + plus enfouie
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