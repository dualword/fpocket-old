
#include "../headers/fpocket.h" 

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					fpmain.h
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## ----- SPECIFICATIONS
##
##	Top function(s) to use for looking for pockets in a given protein.
##	This function will call successively all function necessary to
##	perform pocket detection using voronoi vertices.
##
##	No output is writen, just the list of pockets are returned.
##
## ----- MODIFICATIONS HISTORY
##
##	01-04-08	(v)  Added comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
##

*/

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	c_lst_pockets *pockets search_pocket(s_pdb *pdb, s_fparams *params)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	This function will call all functions needed for the pocket finding algorith
	and will return the list of pockets found on the protein.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_pdb *pdb: The pdb data of the protein to handle.
   -----------------------------------------------------------------------------
   ## RETURN:
	A list of pockets found.
   -----------------------------------------------------------------------------
*/
c_lst_pockets* search_pocket(s_pdb *pdb, s_fparams *params)
{
	clock_t b, e ;
	time_t bt, et ;
	c_lst_pockets *pockets = NULL ;

	/* Calculate and read voronoi vertices coming from qhull */
 	//fprintf(stdout,"========= fpocket algorithm begins =========\n") ;

 	//fprintf(stdout, "> Calculating vertices ...\n");
	bt = time(NULL) ;
	s_lst_vvertice *lvert = load_vvertices(pdb, params->min_apol_neigh, 
												params->asph_min_size, 
												params->asph_max_size) ;
	et = time(NULL) ;
 	//fprintf(stdout, "> Vertices successfully calculated in apox. %f sec.\n",
	//				(float) (et-bt)) ;
	
	if(lvert) {
	/* First clustering */
 		//fprintf(stdout,"> Basic clustering ...\n");
		b = clock() ;
		pockets = clusterPockets(lvert, params);

		if(pockets) {
			pockets->vertices = lvert ;
			e = clock() ;
 			//fprintf(stdout, "> Clustering OK in %f sec.\n",
			//				((double)e - b) / CLOCKS_PER_SEC) ;
		
		/* Clustering refinment */

			b = clock() ;
 			//fprintf(stdout,"> Cluster refinment steps: \n");
			reIndexPockets(pockets) ;/* Create index and calculate statistics */

 			//fprintf(stdout,"\t* 2nd refinment step -> clustering : based on barycenters...\n");
			refinePockets(pockets, params) ;	/* Refine clustering (rapid) */
			reIndexPockets(pockets) ;

 			//fprintf(stdout,"\t* 3rd refinment step -> single linkage clusturing...\n");
			pck_sl_clust(pockets, params);	/* Single Linkage Clustering */
			reIndexPockets(pockets) ;

		/* Descriptors calculation */
 			//fprintf(stdout,"> Calculating descriptors and score...\n");
			b = clock() ;
			set_pockets_descriptors(pockets);		
			e = clock() ;
 			//fprintf(stdout, "> Descriptors found in %f sec.\n", ((double)e - b) / CLOCKS_PER_SEC) ;

 			//fprintf(stdout,"> 4th refinment step -> dropping small and polar pockets...\n");

		/* Drop small and too polar binding pockets */
			dropSmallNpolarPockets(pockets, params);
			reIndexPockets(pockets) ;
			e = clock() ;
 			//fprintf(stdout, "> Refinment OK in %f sec.\n", ((double)e - b) / CLOCKS_PER_SEC) ;

		/* Sorting pockets */
			sort_pockets(pockets, M_SCORE_SORT_FUNCT) ;
			reIndexPockets(pockets) ;
 			//fprintf(stdout,"===== fpocket algorithm ends =====\n");
		}
	}
	else fprintf(stderr, "! Vertice calculation failed!\n");
	return pockets ;
}

