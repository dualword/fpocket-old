 
#include "../headers/cluster.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					cluster.h
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## ----- SPECIFICATIONS
##
##	This file contains currently only one function, providing
##	a single linkage clustering algorithm performed on a list
##	of pockets.
##
## ----- MODIFICATIONS HISTORY
##
##	01-04-08	(v)  singleLinkageClustering -> pck_sl_clust
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
	@ c_lst_pockets *pockets: The list of pockets
	@ s_fparams *params: Parameters of the program, including single linkage 
						 parameters
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

	int distFlag;						//flag to know if two clusters are next to each other by single linkage clustering...or not
	if(pockets) {
		pcur = pockets->first ;				//set first pocket
		while(pcur) {
			curMobilePocket = pcur->next ;		//set the second pocket
			while(curMobilePocket) {
				distFlag = 0 ;
 
				vcur = pcur->pocket->v_lst->first ;	//set the first vertice of the first pocket
				while(vcur && distFlag <= params->sl_clust_min_nneigh){
					curMobileVertice = curMobilePocket->pocket->v_lst->first;	//set the first vertice of the second pocket
					vvcur = vcur->vertice ;
					vcurx = vvcur->x ;
					vcury = vvcur->y ;
					vcurz = vvcur->z ;
					
					while(curMobileVertice && (distFlag <= params->sl_clust_min_nneigh)){					//double loop for vertices -> if not near
						mvvcur = curMobileVertice->vertice ;
						if(dist(vcurx, vcury, vcurz, mvvcur->x, mvvcur->y, mvvcur->z) < params->sl_clust_max_dist) {
							distFlag++;
						}
						curMobileVertice = curMobileVertice->next;
					}
					vcur = vcur->next ;
				}
				
				pnext =  curMobilePocket->next ;
				if(distFlag >= params->sl_clust_min_nneigh) {
					mergePockets(pcur,curMobilePocket,pockets);	//if they are near to each other, merge them
				}
				curMobilePocket = pnext ;
			}

			pcur = pcur->next ;
		}
	}
	else {
		printf("> Incorrect argument during Single Linkage Clustering.\n") ; //exit(0);
	}
}
