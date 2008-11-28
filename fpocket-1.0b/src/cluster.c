 
#include "../headers/cluster.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					cluster.h
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			28-11-08
##
## ----- SPECIFICATIONS
##
##	This file contains currently only one function, providing
##	a single linkage clustering algorithm performed on a list
##	of pockets.
##
## ----- MODIFICATIONS HISTORY
##
##	28-11-08	(v)  Comments UTD + minor relooking
##	11-05-08	(v)  singleLinkageClustering -> pck_sl_clust
##	01-04-08	(v)  Added comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
##
##	(v) Possible improvement:
##		Use the sorted structure to find neighbors in a more
##		efficient way.
##	(v) Rename the file ! (slcluster.c for example...)
##		Or maybe move this function into pocket.c, as the
##		algorithm deals with pockets only...
##	(v) Check and update if necessary comments of each function!!

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
   ## FONCTION: 
	void pck_sl_clust(c_lst_pockets *pockets, s_fparams *params)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	This function will apply a single linkage clusturing algorithm on the given
	list of pockets. Considering two pockets, if params->sl_clust_min_nneigh
	alpha spheres are separated by a distance lower than params->sl_clust_max_dist,
	then merge the two pockets.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_pockets *pockets  : The list of pockets
	@ s_fparams *params       : Parameters of the program, including single  
								linkage parameters
   -----------------------------------------------------------------------------
   ## RETURN: 
	void
   -----------------------------------------------------------------------------
*/
void pck_sl_clust(c_lst_pockets *pockets, s_fparams *params)
{
	node_pocket *pcur = NULL,
				*pnext = NULL ,
				*curMobilePocket = NULL ;

	node_vertice *vcur = NULL ;
	node_vertice *curMobileVertice = NULL ;

	s_vvertice *vvcur = NULL,
			   *mvvcur = NULL ;
	float vcurx,
		  vcury,
		  vcurz ;
	
	/* Flag to know if two clusters are next to each other by single linkage 
	 * clustering...or not */
	int distFlag;
	if(pockets) {
		/* Set the first pocket */
		pcur = pockets->first ;				
		while(pcur) {
			/* Set the second pocket */
			curMobilePocket = pcur->next ;	
			while(curMobilePocket) {
				distFlag = 0 ;
				/* Set the first vertice of the first pocket */
				vcur = pcur->pocket->v_lst->first ;	
				while(vcur && distFlag <= params->sl_clust_min_nneigh){
					/* Set the first vertice of the second pocket */
					curMobileVertice = curMobilePocket->pocket->v_lst->first ;
					vvcur = vcur->vertice ;
					vcurx = vvcur->x ;
					vcury = vvcur->y ;
					vcurz = vvcur->z ;
					
					/* Double loop for vertices -> if not near */
					while(curMobileVertice && (distFlag <= params->sl_clust_min_nneigh)){					
						mvvcur = curMobileVertice->vertice ;
						if(dist(vcurx, vcury, vcurz, mvvcur->x, mvvcur->y, mvvcur->z) 
							< params->sl_clust_max_dist) {
							distFlag++;
						}
						curMobileVertice = curMobileVertice->next;
					}
					vcur = vcur->next ;
				}
				
				pnext =  curMobilePocket->next ;
				if(distFlag >= params->sl_clust_min_nneigh) {
					/* If they are next to each other, merge them */
					mergePockets(pcur,curMobilePocket,pockets);
				}
				curMobilePocket = pnext ;
			}

			pcur = pcur->next ;
		}
	}
	else {
		fprintf(stderr, "! Incorrect argument during Single Linkage Clustering.\n") ;
	}
}
