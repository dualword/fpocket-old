
#include "../headers/fpocket.h" 

/*

## GENERAL INFORMATION
##
## FILE 					fpocket.c
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## SPECIFICATIONS
##
##	Top function(s) to use for looking for pockets in a given protein.
##	This function will call successively all function necessary to
##	perform pocket detection using voronoi vertices.
##
##	No output is writen, just the list of pockets are returned.
##
## MODIFICATIONS HISTORY
##
##	09-02-09	(v)  Drop tiny pocket step added
##	28-11-08	(v)  Comments UTD
##	01-04-08	(v)  Added comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## TODO or SUGGESTIONS
##

*/


/*
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


/**
   ## FUNCTION: 
	pockets search_pocket
  
   ## SPECIFICATION: 
	This function will call all functions needed for the pocket finding algorith
	and will return the list of pockets found on the protein.
  
   ## PARAMETRES:
	@ s_pdb *pdb : The pdb data of the protein to handle.
	@ s_fparams  : Parameters of the algorithm
  
   ## RETURN:
	A chained list of pockets found, sorted according to the current critera
	(the default is a scoring function)
  
*/
c_lst_pockets* search_pocket(s_pdb *pdb, s_fparams *params,s_pdb *pdb_w_lig)
{
/*
	clock_t b, e ;
	time_t bt, et ;
*/
	c_lst_pockets *pockets = NULL ;
/*
        s_clusterlib_vertices *clusterlib_vertices=NULL;
        Node *cluster_tree=NULL;
*/

	/* Calculate and read voronoi vertices comming from qhull */
/*
 	fprintf(stdout,"========= fpocket algorithm begins =========\n") ;

 	fprintf(stdout, "> Calculating vertices ...\n");

	bt = time(NULL) ;
*/
        fprintf(stdout,"first run\n");
        fflush(stdout);
	s_lst_vvertice *lvertStart = load_vvertices(pdb, params->min_apol_neigh,
                                		params->asph_min_size,
						params->asph_max_size,0.0,0.0,0.0) ;
        fprintf(stdout,"second run\n");
        fflush(stdout);

        s_lst_vvertice *lvertShift1 = load_vvertices(pdb, params->min_apol_neigh,
                                		params->asph_min_size,
						params->asph_max_size,1.45,10.0,1.0) ;
        fprintf(stdout,"third run\n");
        fflush(stdout);

        s_lst_vvertice *lvertShift2 = load_vvertices(pdb, params->min_apol_neigh,
                                		params->asph_min_size,
						params->asph_max_size,2.0,0.0,0.0) ;
        fprintf(stdout,"fourth run\n");
        fflush(stdout);
        s_lst_vvertice *lvertShift3 = load_vvertices(pdb, params->min_apol_neigh,
                                		params->asph_min_size,
						params->asph_max_size,0.0,0.0,1.201) ;

        fprintf(stdout,"fifth run\n");
        fflush(stdout);
        s_lst_vvertice *lvertShift4 = load_vvertices(pdb, params->min_apol_neigh,
                                		params->asph_min_size,
						params->asph_max_size,1.0,1.5,1.201) ;


        fprintf(stdout,"sixth run\n");
        fflush(stdout);
        s_lst_vvertice *lvertShift5 = load_vvertices(pdb, params->min_apol_neigh,
                                		params->asph_min_size,
						params->asph_max_size,2.0,0.0,1.201) ;


        /*Usage of this function disrupts functioning of former initial clustering
         * step based on vertice neighbourhood trough index lists, as the ->tr
         * list is not updated here*/
        s_lst_vvertice *lvert2=compare_vvertice_shifted_lists(lvertStart,lvertShift1,1.45,10.0,1.0);
        s_lst_vvertice *lvert3=compare_vvertice_shifted_lists(lvert2,lvertShift2,2.0,0.0,0.0);
        s_lst_vvertice *lvert4=compare_vvertice_shifted_lists(lvert3,lvertShift3,0.0,0.0,1.201);

        s_lst_vvertice *lvert=compare_vvertice_shifted_lists(lvert4,lvertShift4,1.0,1.5,1.201);
/*        s_lst_vvertice *lvert=compare_vvertice_shifted_lists(lvert5,lvertShift5,2.0,0.0,1.201);
*/


        

/*
        clusterlib_vertices=prepare_vertices_for_cluster_lib(lvert);
*/
/*
        cluster_tree=treecluster(lvert->nvert,

                                3,
                                clusterlib_vertices->pos,
                                clusterlib_vertices->mask,
                                clusterlib_vertices->weight,
                                clusterlib_vertices->transpose,
                                clusterlib_vertices->dist,
                                clusterlib_vertices->method,
                                NULL);

        cuttree_distance (lvert->nvert, cluster_tree, 3.0);
*/
        /*
	et = time(NULL) ;
 	fprintf(stdout, "> Vertices successfully calculated in apox. %f sec.\n",
					(float) (et-bt)) ;
*/
	
	if(lvert == NULL) {
		fprintf(stderr, "! Vertice calculation failed!\n");
		return NULL ;
	}
	/* First clustering */
/* 		fprintf(stdout,"> Basic clustering ...\n");

		b = clock() ;
*/
/*
	pockets = clusterPockets(lvert, params);
*/
        
        pockets = assign_pockets(lvert,params);
        

	if(pockets) {

		pockets->vertices = lvert ;
/*
		e = clock() ;
		fprintf(stdout, "> Clustering OK in %f sec.\n",
						((double)e - b) / CLOCKS_PER_SEC) ;
*/

	/* Clustering refinment */

/*
		b = clock() ;
		fprintf(stdout,"> Cluster refinment steps: \n");
*/


		reIndexPockets(pockets) ;


/*
		drop_tiny(pockets) ;	
		reIndexPockets(pockets) ;
*/

/*
		fprintf(stdout,"\t* 2nd refinment step -> clustering : based on barycenters...\n");
*/

		/*refinePockets(pockets, params) ;	/* Refine clustering (rapid) */

/*
		reIndexPockets(pockets) ;
*/

/*
		fprintf(stdout,"\t* 3rd refinment step -> single linkage clusturing...\n");
*/

		pck_final_clust(pockets, params,2.0,1,pdb,pdb_w_lig);	/* Single Linkage Clustering */
                reIndexPockets(pockets) ;

                pck_final_clust(pockets, params,params->sl_clust_max_dist,params->sl_clust_min_nneigh,pdb,pdb_w_lig);	/* Single Linkage Clustering */
                reIndexPockets(pockets) ;
                pck_final_clust(pockets, params,params->sl_clust_max_dist,params->sl_clust_min_nneigh,pdb,pdb_w_lig);	/* Single Linkage Clustering */
                reIndexPockets(pockets) ;

                pck_final_clust(pockets, params,params->sl_clust_max_dist,params->sl_clust_min_nneigh,pdb,pdb_w_lig);	/* Single Linkage Clustering */
                reIndexPockets(pockets) ;

/*
                pck_final_clust(pockets, params,params->sl_clust_max_dist,params->sl_clust_min_nneigh,pdb,pdb_w_lig);
                reIndexPockets(pockets) ;

                pck_final_clust(pockets, params,params->sl_clust_max_dist,params->sl_clust_min_nneigh,pdb,pdb_w_lig);
                reIndexPockets(pockets) ;
*/
	/* Descriptors calculation */
/*
		fprintf(stdout,"> Calculating descriptors and score...\n");
		b = clock() ;
*/
		set_pockets_descriptors(pockets,pdb,params,pdb_w_lig);

                
/*
		e = clock() ;
		fprintf(stdout, "> Descriptors found in %f sec.\n", ((double)e - b) / CLOCKS_PER_SEC) ;

		fprintf(stdout,"> 4th refinment step -> dropping small and polar pockets...\n");
*/

	/* Drop small and too polar binding pockets */


		dropSmallNpolarPockets(pockets, params);


		reIndexPockets(pockets) ;

/*
		e = clock() ;
		fprintf(stdout, "> Refinment OK in %f sec.\n", ((double)e - b) / CLOCKS_PER_SEC) ;
*/

	/* Sorting pockets */

		sort_pockets(pockets, M_SCORE_SORT_FUNCT) ;


/*
		sort_pockets(pockets, M_NASPH_SORT_FUNCT) ;
*/



		reIndexPockets(pockets) ;

/*
		fprintf(stdout,"===== fpocket algorithm ends =====\n");
*/

	}

	return pockets ;
}

