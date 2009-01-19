#include "../headers/descriptors.h"

/**

## ----- GENERAL INFORMATION
##
## FILE 					descriptors.c
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## ----- SPECIFICATIONS
##
##	This file is used to calculate descriptors using a set of atoms
##	and/or alpha spheres, using structures defined in atoms.c and
##	voronoi/voronoi_lst.c.
##
##	
##
## ----- MODIFICATIONS HISTORY
##
##	14-01-09	(v)  Added some normalized descriptors
##	28-11-08	(v)  Comments UTD + relooking + mean_asph_ray set to float -_-'
##	01-04-08	(v)  Added comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
##
##  (v) Some descriptors need urgent improvement (amino-acids volume 
##	    score is the best example: maybe replace by the molecular weight 
##	    for example, although the way we use this kind of descriptors 
##		might not be relevant at all anyway...)
##	(v) Possible improvement:
##		Use the sorted structure to calculate the apolar density.

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
	allocate_s_desc 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Allocate a descroptor structure
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   -----------------------------------------------------------------------------
   ## RETURN:
	s_desc*
   -----------------------------------------------------------------------------
*/
s_desc* allocate_s_desc(void) 
{
	s_desc *desc = (s_desc*)my_malloc(sizeof(s_desc)) ;
	
	reset_s_desc(desc) ;

	return desc ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	reset_s_desc
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Reset descriptors to 0 values.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void reset_s_desc(s_desc *desc) 
{
    desc->hydrophobicity_score = 0.0 ;
	desc->volume_score = 0.0 ;
	desc->volume = 0.0 ;
	desc->prop_polar_atm = 0.0 ;
	desc->mean_asph_ray = 0.0 ;
	desc->masph_sacc = 0.0 ;
	desc->apolar_asphere_prop = 0.0 ;
	desc->mean_loc_hyd_dens = 0.0 ;


	desc->flex = 0.0 ;
	desc->nas_norm = 0 ;
	desc->prop_asapol_norm = 0.0 ;
	desc->mean_loc_hyd_dens_norm = -1.0 ;

	desc->nb_asph = 0 ;
	desc->polarity_score  = 0 ;
	desc->charge_score = 0 ;

	int i ;
	for(i = 0 ; i < 20 ; i++) desc->aa_compo[i] = 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	set_descriptors
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Set descriptors using a set of atoms with corresponding voronoi vertices.
	Current descriptors (need to be improved...) includes atom/AA based descriptors
	and voronoi vertice based descriptors.
	A word on several descriptors:
		- The "solvent accessibility" of a sphere (which is rather the buriedness
		  degree...) is defined as the distance of the barycenter of the sphere
		  (defined using the 4 contacted atoms) from the center of the sphere.
		- The mean_loc_hyd_dens is the mean value of apolar density calculated
		  for each vertice and defined in the next function (see comments or doc)
   
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_atm **atoms         : The list of atoms
	@ int natoms            : The number of atoms
	@ s_vvertice **tab_vert : The list of vertices 
	@ int nvert             : The number of vertices
	@ s_desc *desc          : OUTPUT: The descriptor structure to fill
   -----------------------------------------------------------------------------
   ## RETURN:
    void: s_desc is filled
   -----------------------------------------------------------------------------
*/
void set_descriptors(s_atm **atoms, int natoms, s_vvertice **tab_vert, int nvert, 
					 s_desc *desc) 
{
	/* Setting atom-based descriptors */
	set_atom_based_descriptors(atoms, natoms, desc) ;

	/* Setting vertice-based descriptors */
	if(tab_vert) {

		float d = 0.0,
			  masph_sacc = 0.0, /* Mean alpha sphere solvent accessibility */
			  mean_ashape_radius = 0.0 ;

		int i,
			nAlphaApol = 0 ;

		s_vvertice *vcur = NULL ;
		desc->mean_loc_hyd_dens = 0.0 ;

		for(i = 0 ; i < nvert ; i++) {
			vcur = tab_vert[i] ;

			if(vcur->type == M_APOLAR_AS) {
				desc->mean_loc_hyd_dens += (float) get_vert_apolar_density(
														tab_vert, nvert, vcur);
				nAlphaApol += 1 ;
			}
			mean_ashape_radius += vcur->ray ;

			/* Estimating solvent accessibility of the sphere */
			d = dist(vcur->x, vcur->y, vcur->z, 
					 vcur->bary[0], vcur->bary[1], vcur->bary[2]) ;
			masph_sacc += d/vcur->ray ;
		}

		if(nAlphaApol>0) desc->mean_loc_hyd_dens /= (float)nAlphaApol ;
		else desc->mean_loc_hyd_dens= 0.0;
		
		desc->apolar_asphere_prop = (float)nAlphaApol / (float)nvert ;
		desc->masph_sacc =  masph_sacc / nvert ;
		desc->mean_asph_ray = mean_ashape_radius / (float)nvert ;
		desc->nb_asph = nvert ;
	}

	desc->volume = get_verts_volume_ptr(tab_vert, nvert, 3000) ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	get_vert_apolar_density
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Here we calculate the number of apolar vertices (vertices that contact at 
	least 3 atoms having their electronegativity > 2.7) that lie within a range 
	of 0-r () from a given reference vertice, r being its own radius.
 
	It provides the apolar density for a given vertice, that will be used for
	the calculation of the total apolar density of the pocket, defined as the
	mean value of it.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_vvertice **tab_vert : The list of vertices 
	@ int nvert             : The number of vertices
	@ s_vvertice *vert      : The reference vertice.
   -----------------------------------------------------------------------------
   ## RETURN:
	int: The apolar density as defined previously.
   -----------------------------------------------------------------------------
*/
int get_vert_apolar_density(s_vvertice **tab_vert, int nvert, s_vvertice *vert)
{
	int apol_neighbours = 0,
		i = 0 ;

	s_vvertice *vc = NULL ;

	float vx = vert->x, 
		  vy = vert->y,
		  vz = vert->z,
		  vray = vert->ray ;
	
	for(i = 0 ; i < nvert ; i++) {
		vc = tab_vert[i] ;
		if(vc != vert && vc->type == M_APOLAR_AS){
			if(dist(vx, vy, vz, vc->x, vc->y, vc->z)-(vc->ray + vray) <= 0.) {
				apol_neighbours += 1 ;
			}
		}
	}

	return apol_neighbours ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION:
	set_atom_based_descriptors
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Update atomic descriptors of the pocket for the given atom. Here, we just
	update mean bfactor, electronegativity, amino-acids scores...
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_atom *atoms : The atoms
	@ int natoms    : Number of atoms
	@ s_desc *desc  : OUTPUT : The descriptor structure to fill
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void set_atom_based_descriptors(s_atm **atoms, int natoms, s_desc *desc)
{
	s_atm *curatom = NULL ;

	int i,
		res_ids[natoms],	/* Maximum natoms residues contacting the pocket */
		nb_res_ids = 0 ;	/* Current number of residus */

	int nb_polar_atm = 0 ;
 
	for(i = 0 ; i < natoms ; i++) {
		curatom = atoms[i] ;

	/* Setting amino acid descriptor of the current atom */
		if(in_tab(res_ids,  nb_res_ids, curatom->res_id) == 0) {
			set_aa_desc(desc, atoms[i]->res_name) ;
			res_ids[nb_res_ids] = curatom->res_id ;
			nb_res_ids ++ ;
		}

	/* Setting atom descriptor */
		desc->flex += curatom->bfactor ;
		if(curatom->electroneg > 2.7)  nb_polar_atm += 1 ;
	}

	desc->hydrophobicity_score = desc->hydrophobicity_score/ (float) nb_res_ids ;
	desc->volume_score = desc->volume_score / (float) nb_res_ids ;

	desc->flex /= natoms ;
	desc->prop_polar_atm = ((float) nb_polar_atm) / ((float) natoms) * 100.0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	set_aa_desc
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Set amino-acid based descriptors.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   @ s_desc *desc        : OUTPUT: Structure of descriptors to fill
   @ const char *aa_name : The amino acid name
   -----------------------------------------------------------------------------
   ## RETURN:
	s_desc*
   -----------------------------------------------------------------------------
*/
void set_aa_desc(s_desc *desc, const char *aa_name) 
{	
	int aa_idx = -1;
	char l1 = aa_name[0], 
		 l2 = aa_name[1], 
		 l3 = aa_name[2] ;

	/* Ok, lets use the less comparisons possible... (may be used in the aa.c
	 * file later!) 
	 * Only A, C, G, H, I, L, M, P, S, T and V possibility for the first letter:
	 **/
	switch(l1) {
		case 'A' : /* Either ALA, ASP, ASN or ARG */
			if(l2 == 'L') aa_idx = M_ALA_IDX ; 				/* ALA amino acid! */
			else if(l2 == 'R') aa_idx = M_ARG_IDX ; 		/* ARG amino acid! */
			else if(l2 == 'S' && l3 ==  'P') 
				aa_idx = M_ASP_IDX ;						/* ASP amino acid! */
			else aa_idx = M_ASN_IDX ; break ;				/* ASN amino acid! */
			
		case 'C' : aa_idx = M_CYS_IDX ; break ;				/* CYS amino acid! */
			
		case 'G' : /* Either GLU, GLY, or GLN, so just check the 3rd letter */
			if(l3 == 'U') aa_idx = M_GLU_IDX ; 				/* GLU amino acid! */
			else if(l3 == 'Y') aa_idx = M_GLY_IDX ; 		/* GLY amino acid! */
			else aa_idx = M_GLN_IDX ; break ;				/* GLN amino acid! */

		case 'H' : aa_idx = M_HIS_IDX ; break ;				/* HIS amino acid! */
		case 'I' : aa_idx = M_ILE_IDX ; break ;				/* ILE amino acid! */
			
		case 'L' : /* Either ALA, ASP, ASN or ARG */
			if(l2 == 'Y') aa_idx = M_LYS_IDX ; 				/* LYS amino acid! */
			else aa_idx = M_LEU_IDX ; break ;				/* LEU amino acid! */
			
		case 'M' : aa_idx = M_MET_IDX ; break ;				/* MET amino acid! */
		case 'P' : /* Either ALA, ASP, ASN or ARG */
			if(l2 == 'H') aa_idx = M_PHE_IDX ; 				/* PHE amino acid! */
			else aa_idx = M_PRO_IDX ; break ;				/* PRO amino acid! */

		case 'S' : aa_idx = M_SER_IDX ; break ;				/* SER amino acid! */

		case 'T' : /* Either ALA, ASP, ASN or ARG */
			if(l2 == 'H') aa_idx = M_THR_IDX ; 				/* THR amino acid! */
			else if(l2 == 'R') aa_idx = M_TRP_IDX ; 		/* TRP amino acid! */
			else aa_idx = M_TYR_IDX ; break ;				/* TYR amino acid! */

		case 'V' : aa_idx = M_VAL_IDX ; break ;				/* VAL amino acid! */

		default: /*fprintf(stderr, "! Amno acid %s does not exists!\n", aa_name) ;*/ 
			break ;
	}

	/* Ok now we have our amino acid, lets update statistics! */

	if(aa_idx != -1) {
		desc->aa_compo[aa_idx] ++ ;

		desc->hydrophobicity_score += get_hydrophobicity_score_from_idx(aa_idx) ;
		desc->polarity_score += get_polarity_from_idx(aa_idx) ;
		desc->volume_score += get_volume_score_from_idx(aa_idx) ;
		desc->charge_score += get_charge_from_idx(aa_idx) ;
	}
}

