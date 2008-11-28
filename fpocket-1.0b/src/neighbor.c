
#include "../headers/neighbor.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					neighbor.c
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			28-11-08
##
## ----- SPECIFICATIONS
##
## This file define functions used to perform a space search
## operation like looking for all neighbors of a given molecule
## situated at a given distance. It makes use of a sorting system,
## which sort all atoms/vertices according to each dimension.
##
## See sort.c for information on the sorting procedure.
##
## ----- MODIFICATIONS HISTORY
##
##	28-11-08	(v)  Comments UTD + relooking.
##	01-04-08	(v)  Added template for comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
##
##	(v) Check the algorithms more deeply... Try to make the code more lisible! 
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
	get_mol_atm_neigh
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Return a list of pointer to the atoms situated a distance lower or equal
	to dist_crit from a molecule represented by a list of atoms.

	This functon use a list of atoms that is sorted on the X dimension to accelerate
	the research.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_atm **atoms        : The molecule atoms.
	@ int natoms		   : Number of atoms in the molecule
	@ s_pdb *pdb           : The pdb structure containing all atoms of the system
	@ float dist_crit  	   : The distance criteria.
	@ int *nneigh          : OUTPUT A pointer to the number of neighbour found,
							 will be modified in the function...
   -----------------------------------------------------------------------------
   ## RETURN:
	A tab of pointers to the neighbours, with the number of neighbors stored in
	nneigh
   -----------------------------------------------------------------------------
*/
s_atm** get_mol_atm_neigh(s_atm **atoms, int natoms, s_pdb *pdb, float dist_crit, 
						  int *nneigh) 
{	
	/* No vertices, we only search atoms... */
	s_vsort *lsort =  get_sorted_list(pdb, NULL) ;	
	s_vect_elem *xsort = lsort->xsort ;

	int ip, im,
		stopm, stopp,
		nb_neigh,
		sort_x,
		ntest_x,
		dim = lsort->nelem ;
	
	float vvalx, vvaly, vvalz;
	
	int real_size = 10,
		i, seen ;
	
	s_atm *curap = NULL, *curam = NULL ;
	s_atm **neigh = (s_atm**)my_malloc(sizeof(s_atm*)*real_size) ;
	
	nb_neigh = 0 ;
	for(i = 0 ; i < natoms ; i++) {
		s_atm *cur = atoms[i] ;
	
	/* Reinitialize variables */
		stopm = 0; stopp = 0;
		sort_x = cur->sort_x ;
		ntest_x = 1 ;
		vvalx = cur->x ; vvaly = cur->y ; vvalz = cur->z ;

	/* Search neighbors */
		while(!stopm || !stopp) {
		/* Do the neighbor search while wa are in tab and we dont reach a 
		 * distance up to our criteria */
			ip = ntest_x + sort_x ;
			if(ip >= dim) stopp = 1 ;
	
			if(stopp == 0) {
				curap = (s_atm*) xsort[ip].data ;

				if(curap->x - vvalx > dist_crit) stopp = 1 ;
				else {
				/* OK we have an atom which is near our vertice on X, so 
				 * calculate real distance */
					if(dist(curap->x, curap->y, curap->z, vvalx, vvaly, vvalz) < dist_crit) {
					/* Distance OK, see if the molecule is not one part of the 
						input, and if we have not already seen it. */
						seen = is_in_lst_atm(atoms, natoms, curap->id) 
							   + is_in_lst_atm(neigh, nb_neigh, curap->id) ;
						if(!seen) {
							if(nb_neigh >= real_size-1) {
								real_size *= 2 ;
								neigh =(s_atm**) my_realloc(neigh, sizeof(s_atm)*real_size) ;
							}
							neigh[nb_neigh] = curap ;
							nb_neigh ++ ;
						}
					}
				}
			}
	
			im = sort_x - ntest_x ;
			if(im < 0) stopm = 1 ;

			if(stopm == 0) {
				curam = (s_atm*) xsort[im].data ;
	
				if(vvalx - curam->x > dist_crit) stopm = 1 ;
				else {
				/* OK we have an atom which is near our vertice on X, so 
				 * calculate real distance */
					if(dist(curam->x, curam->y, curam->z, vvalx, vvaly, vvalz) < dist_crit){
					/* Distance OK, see if the molecule is not one part of the 
						input, and if we have not already seen it. */
						seen = is_in_lst_atm(atoms, natoms, curam->id) 
							   + is_in_lst_atm(neigh, nb_neigh, curam->id) ;
						if(!seen) {
							if(nb_neigh >= real_size-1) {
								real_size *= 2 ;
								neigh = (s_atm**)my_realloc(neigh, sizeof(s_atm)*real_size) ;
							}
							neigh[nb_neigh] = curam ;
							nb_neigh ++ ;
						}
					}
				}
			}
			
			ntest_x += 1 ;
		}
	}
	
	*nneigh = nb_neigh ;
	free_s_vsort(lsort) ;

	return neigh ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	get_mol_vert_neigh
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Return a list of pointer to the vertices situated a distance lower or equal
	to dist_crit of a molecule represented by it's list of atoms.

	This functon use a list of atoms that is sorted on the X dimension to accelerate
	the research.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_atm **atoms         : The molecule.
	@ int natoms		    : Number of atoms in the molecule
    @ s_lst_vvertice *lvert : Full list of vertices present in the system 
	@ s_pdb *pdb            : The pdb structure containing all atoms of the system
	@ float dist_crit       : The distance criteria.
	@ int *nneigh           : OUTPUT A pointer to the number of neighbour found,
							  will be modified in the function...
   -----------------------------------------------------------------------------
   ## RETURN:
	A tab of pointers to the neighbours.
   -----------------------------------------------------------------------------
*/
s_vvertice** get_mol_vert_neigh(s_atm **atoms, int natoms, s_lst_vvertice *lvert, 
								s_pdb *pdb, float dist_crit, int *nneigh) 
{	
	s_vsort *lsort =  get_sorted_list(pdb, lvert) ;	
	s_vect_elem *xsort = lsort->xsort ;

	int ip, im,
		stopm, stopp,
		nb_neigh,
		sort_x,
		ntest_x,
		dim = lsort->nelem ;
	
	float vvalx, vvaly, vvalz;
	
	int real_size = 10,
		i, seen ;
	
	s_vvertice *curvp = NULL, *curvm = NULL ;
	s_vvertice **neigh = (s_vvertice**)my_malloc(sizeof(s_vvertice*)*real_size) ;
	
	nb_neigh = 0 ;
	for(i = 0 ; i < natoms ; i++) {
		s_atm *cur = atoms[i] ;
	
	/* Reinitialize variables */
		stopm = 0; stopp = 0;
		sort_x = cur->sort_x ;
		ntest_x = 1 ;
		vvalx = cur->x ; vvaly = cur->y ; vvalz = cur->z ;

	/* Search neighbors */
		while(!stopm || !stopp) {
		/* Do the neighbor search while wa are in tab and we dont reach a 
		 * distance up to our criteria */
			ip = ntest_x + sort_x ;
			if(ip >= dim) stopp = 1 ;
	
			if(stopp == 0 && xsort[ip].type == M_VERTICE_TYPE) {
				curvp = (s_vvertice*) xsort[ip].data ;

				if(curvp->x - vvalx > dist_crit) stopp = 1 ;
				else {
				/* OK we have an atom which is near our vertice on X, so 
				 * calculate real distance */
					if(dist(curvp->x, curvp->y, curvp->z, vvalx, vvaly, vvalz) < dist_crit) {
					/* Distance OK, see if the molecule have not been already seen. */
						seen = is_in_lst_vert(neigh, nb_neigh, curvp->id) ;
						if(!seen) {
							if(nb_neigh >= real_size-1) {
								real_size *= 2 ;
								neigh = (s_vvertice **)my_realloc(neigh, 
												sizeof(s_vvertice)*real_size) ;
							}
							neigh[nb_neigh] = curvp ;
							nb_neigh ++ ;
						}
					}
				}
			}
	
			im = sort_x - ntest_x ;
			if(im < 0) stopm = 1 ;

			if(stopm == 0 && xsort[im].type == M_VERTICE_TYPE) {
				curvm = (s_vvertice*) xsort[im].data ;
	
				if(vvalx - curvm->x > dist_crit) stopm = 1 ;
				else {
				/* OK we have an atom which is near our vertice on X, so 
				 * calculate real distance */
					if(dist(curvm->x, curvm->y, curvm->z, vvalx, vvaly, vvalz) < dist_crit){
					/* Distance OK, see if the molecule is not one part of the 
					 * input, and if we have not already seen it. */
						seen = is_in_lst_vert(neigh, nb_neigh, curvm->id) ;
						if(!seen) {
							if(nb_neigh >= real_size-1) {
								real_size *= 2 ;
								neigh = (s_vvertice**)my_realloc(neigh, sizeof(s_vvertice)*real_size) ;
							}
							neigh[nb_neigh] = curvm ;
							nb_neigh ++ ;
						}
					}
				}
			}
			
			ntest_x += 1 ;
		}
	}
	
	*nneigh = nb_neigh ;
	free_s_vsort(lsort) ;

	return neigh ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	get_mol_ctd_atm_neigh
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Return a list of atoms contacted by voronoi vertices situated at dist_crit
	of a given molecule represented by a list of its atoms.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_atm **atoms         : The molecule.
	@ int natoms		    : Number of atoms in the molecule
    @ s_lst_vvertice *lvert : Full list of vertices present in the system 
	@ s_pdb *pdb            : The pdb structure containing all atoms of the system
	@ float dist_crit       : The distance criteria.
    @ int interface_search  : Perform an interface-type search ?
	@ int *nneigh           : OUTPUT A pointer to the number of neighbour found,
							  will be modified in the function...
   -----------------------------------------------------------------------------
   ## RETURN:
	A tab of pointers to atoms describing the pocket.
   -----------------------------------------------------------------------------
*/
s_atm** get_mol_ctd_atm_neigh(s_atm **atoms, int natoms, s_pdb *pdb, 
							  s_lst_vvertice *lvert, float vdist_crit, 
							  int interface_search, int *nneigh) 
{
	s_vsort *lsort =  get_sorted_list(pdb, lvert) ;
	s_vect_elem *xsort = lsort->xsort ;

	int ip, im,			/* Current indexes in the tab (start at the current 
						   atom position, and check the tab in each directions */
		stopm, stopp,	/* Say weteher the algorithm has to continue the search 
						   in each direction */
		nb_neigh = 0,
		sort_x = 0,		/* Index of the current ligand atom in the sorted list.*/
		ntest_x = 0,
		dim = lsort->nelem,
		real_size = 10,
		i, j ;

	float lx, ly, lz;
	
	s_vvertice *curvp = NULL, *curvm = NULL ;
	s_atm *curatm = NULL ;
	s_atm **neigh = (s_atm**)my_malloc(sizeof(s_atm*)*real_size) ;

	for(i = 0 ; i < natoms ; i++) {
		s_atm *cur = atoms[i] ;
	
	/* Reinitialize variables */
		stopm = 0; stopp = 0;		/* We can search in each directions */
		sort_x = cur->sort_x ;		/* Get the index of the current lidang atom 
									   in the sorted list. */
		ntest_x = 1 ;

		/* Coordinates of the current atom of the ligand */
		lx = cur->x ; ly = cur->y ; lz = cur->z ;

	/* Search neighbors */
		while(!stopm || !stopp) {
		/* Do the neighbor search while we are in tab and we dont reach a 
		 * distance up to our criteria */
			ip = ntest_x + sort_x ;
			
			/* If we are out of the tab, stop the search in this direction */
			if(ip >= dim) stopp = 1 ;	
	
			if(stopp == 0 && xsort[ip].type == M_VERTICE_TYPE) {
				curvp = (s_vvertice*) xsort[ip].data ;
				
				/* Stop when the distance reaches the criteria */
				if(curvp->x - lx > vdist_crit) stopp = 1 ; 
				else {
				/*OK we have a vertice which is near our atom on the X axe, 
				  so calculate real distance */
					if(dist(curvp->x, curvp->y, curvp->z, lx, ly, lz) < vdist_crit) {
					/* If the distance from the atom to the vertice is small enough*/
						for(j = 0 ; j < 4 ; j++) {
							curatm = curvp->neigh[j] ;
							if(! is_in_lst_atm(neigh, nb_neigh, curatm->id)) {
								if(interface_search && 
								dist(curatm->x, curatm->y, curatm->z, lx, ly, lz) 
										< M_INTERFACE_SEARCH_DIST) {
								/* If we have not seen yet this atom and if he 
								 * is not too far away from the ligand, add it*/
									if(nb_neigh >= real_size-1) {
										real_size *= 2 ;
										neigh = (s_atm**)my_realloc(neigh, sizeof(s_atm)*real_size) ;
									}
									neigh[nb_neigh] = curatm ;
									nb_neigh ++ ;
								}
								else if(!interface_search) {
								/* If we have not seen yet this atom and if he 
								 * is not too far away from the ligand, add it*/
									if(nb_neigh >= real_size-1) {
										real_size *= 2 ;
										neigh = (s_atm**)my_realloc(neigh, sizeof(s_atm)*real_size) ;
									}
									neigh[nb_neigh] = curatm ;
									nb_neigh ++ ;
								}
							} 
						}
					}
				}
			}
	
			im = sort_x - ntest_x ;
			/* If we are out of the tab, stop the search in this direction */
			if(im < 0) stopm = 1 ;		

			if(stopm == 0 && xsort[im].type == M_VERTICE_TYPE) {
				curvm = (s_vvertice*) xsort[im].data ;
	
				if(lx - curvm->x > vdist_crit) stopm = 1 ;
				else {
				/* OK we have a vertice which is near our atom on the X axe, 
				 * so calculate real distance*/
					if(dist(curvm->x, curvm->y, curvm->z, lx, ly, lz) < vdist_crit) {
					/* If the distance from the atom to the vertice is small enough */
						for(j = 0 ; j < 4 ; j++) {
							curatm = curvm->neigh[j] ;
							if(! is_in_lst_atm(neigh, nb_neigh, curatm->id)) {
								if(interface_search && 
								dist(curatm->x, curatm->y, curatm->z, lx, ly, lz) 
										< M_INTERFACE_SEARCH_DIST) { 
								/* If we have not seen yet this atom and if he 
								 * is not too far away from the ligand, add it */
									if(nb_neigh >= real_size-1) {
										real_size *= 2 ;
										neigh = (s_atm**)
										my_realloc(neigh, sizeof(s_atm)*real_size) ;
									}
									neigh[nb_neigh] = curatm ;
									nb_neigh ++ ;
								}
								else if(!interface_search) {
								/* If we have not seen yet this atom and if he 
								 * is not too far away from the ligand, add it */
									if(nb_neigh >= real_size-1) {
										real_size *= 2 ;
										neigh = (s_atm**)my_realloc(neigh, sizeof(s_atm)*real_size) ;
									}
									neigh[nb_neigh] = curatm ;
									nb_neigh ++ ;
								}
							} 
						}
					}
				}
			}
			
			ntest_x += 1 ;
		}
	}
	
	*nneigh = nb_neigh ;
	free_s_vsort(lsort) ;

	return neigh ;
}
