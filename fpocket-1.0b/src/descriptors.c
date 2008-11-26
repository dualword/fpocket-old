#include "../headers/descriptors.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					descriptors.h
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
##	01-04-08	(v)  Added comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
##
##	(v) Possible improvement:
##		Use the sorted structure to calculate the apolar density.
##	(v) Check and update if necessary comments of each function!!

*/

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	s_desc* allocate_s_desc(void) 
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
	s_desc* reset_s_desc(s_desc *desc) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Reset a descroptor structure
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   -----------------------------------------------------------------------------
   ## RETURN:
	s_desc*
   -----------------------------------------------------------------------------
*/
void reset_s_desc(s_desc *desc) 
{
	desc->hydrophobicity_score = 0.0 ;
	desc->volume_score = 0.0 ;
	desc->volume = 0.0 ;
	desc->flex = 0.0 ;
	desc->prop_polar_atm = 0.0 ;
	desc->mean_asph_ray = 0.0 ;
	desc->masph_sacc = 0.0 ;
	desc->apolar_asphere_prop = 0.0 ;
	desc->mean_loc_hyd_dens = 0.0 ;

	desc->nb_asph = 0 ;
	desc->polarity_score  = 0 ;
	desc->charge_score = 0 ;

	int i ;
	for(i = 0 ; i < 20 ; i++) desc->aa_compo[i] = 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void set_descriptors(s_atm **atoms, int natoms, s_vvertice **lst_vert, int nvert, s_desc *desc)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   -----------------------------------------------------------------------------
   ## RETURN:
	s_desc*
   -----------------------------------------------------------------------------
*/
void set_descriptors(s_atm **atoms, int natoms, s_vvertice **tab_vert, int nvert, s_desc *desc) 
{
// Setting atom-based descriptors

	set_atom_based_descriptors(atoms, natoms, desc) ;

// Setting vertice-based descriptors

	if(tab_vert) {

		float d = 0.0,
			  masph_sacc = 0.0 ; 	// Mean alpha sphere solvent accessibility

		int i,
			nAlphaApol = 0,
			mean_ashape_radius = 0.0 ;

		s_vvertice *vcur = NULL ;
		desc->mean_loc_hyd_dens = 0.0 ;

		for(i = 0 ; i < nvert ; i++) {
			vcur = tab_vert[i] ;

			if(vcur->type == M_APOLAR_AS) {
				desc->mean_loc_hyd_dens += (float) get_vert_apolar_density(tab_vert, nvert, vcur);
				nAlphaApol += 1 ;
			}
			mean_ashape_radius += vcur->ray ;

			// Estimating solvent accessibility of the sphere
			d = dist(vcur->x, vcur->y, vcur->z, vcur->bary[0], vcur->bary[1], vcur->bary[2]) ;
			masph_sacc += d/vcur->ray ;
		}

		if(nAlphaApol>0) desc->mean_loc_hyd_dens /= (float)nAlphaApol ;
		else desc->mean_loc_hyd_dens= 0.0;
		
		desc->apolar_asphere_prop = (float)nAlphaApol / (float)nvert ;
		desc->masph_sacc =  masph_sacc / nvert ;
		desc->mean_asph_ray = mean_ashape_radius / (float)nvert ;
		desc->nb_asph = nvert ;
	}

	desc->volume = get_verts_volume_ptr(tab_vert, nvert, 2500) ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void set_apolar_density(s_pocket *pocket, node_vertice *v)
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Set the apolar density for the given pocket.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_pocket *pocket: The pocket to handle
	@ s_fparams *par: Parameters
   -----------------------------------------------------------------------------
   ## RETURN:
	void
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
	void set_atom_based_descriptors(s_atm **atoms, int natoms, s_desc *desc)
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Update atomic descriptors of the pocket for the given atom. Here, we just
	update mean bfactor and electronegativity...
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_pocket *pocket: The pocket to handle
	@ s_atom *atom: The atom
	@ int *nb_polar_atm: Current Number of polar atoms.
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void set_atom_based_descriptors(s_atm **atoms, int natoms, s_desc *desc)
{
	s_atm *curatom = NULL ;

	int i,
		res_ids[natoms],			// Maximum nb_vert*4 residues contacting the pocket
		nb_res_ids = 0 ;				// Current number of residus

	int nb_polar_atm = 0 ;
 
	for(i = 0 ; i < natoms ; i++) {
		curatom = atoms[i] ;

	// Setting amino acid descriptor of the current atom
		if(in_tab(res_ids,  nb_res_ids, curatom->res_id) == 0) {
			set_aa_desc(desc, atoms[i]->res_name) ;
			res_ids[nb_res_ids] = curatom->res_id ;
			nb_res_ids ++ ;
		}

	// Setting atom descriptor
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
	void set_aa_desc(s_pocket *pocket, const char *aa_name)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Reset a descroptor structure
   -----------------------------------------------------------------------------
   ## PARAMETRES:
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

	// Ok, lets use the less comparisons possible... (may be used in the aa.c file later!)
	// Only A, C, G, H, I, L, M, P, S, T and V possibility for the first letter:
	switch(l1) {
		case 'A' : // Either ALA, ASP, ASN or ARG
			if(l2 == 'L') aa_idx = M_ALA_IDX ; 						// ALA amino acid!
			else if(l2 == 'R') aa_idx = M_ARG_IDX ; 				// ARG amino acid!
			else if(l2 == 'S' && l3 ==  'P') aa_idx = M_ASP_IDX ; 	// ASP amino acid!
			else aa_idx = M_ASN_IDX ; break ;						// ASN amino acid!
			
		case 'C' : aa_idx = M_CYS_IDX ; break ;						// CYS amino acid!
			
		case 'G' : // Either GLU, GLY, or GLN, so just check the 3rd letter
			if(l3 == 'U') aa_idx = M_GLU_IDX ; 						// GLU amino acid!
			else if(l3 == 'Y') aa_idx = M_GLY_IDX ; 				// GLY amino acid!
			else aa_idx = M_GLN_IDX ; break ;						// GLN amino acid!

		case 'H' : aa_idx = M_HIS_IDX ; break ;						// HIS amino acid!
		case 'I' : aa_idx = M_ILE_IDX ; break ;						// ILE amino acid!
			
		case 'L' : // Either ALA, ASP, ASN or ARG
			if(l2 == 'Y') aa_idx = M_LYS_IDX ; 						// LYS amino acid!
			else aa_idx = M_LEU_IDX ; break ;						// LEU amino acid!
			
		case 'M' : aa_idx = M_MET_IDX ; break ;						// MET amino acid!
		case 'P' : // Either ALA, ASP, ASN or ARG
			if(l2 == 'H') aa_idx = M_PHE_IDX ; 						// PHE amino acid!
			else aa_idx = M_PRO_IDX ; break ;						// PRO amino acid!

		case 'S' : aa_idx = M_SER_IDX ; break ;						// SER amino acid!

		case 'T' : // Either ALA, ASP, ASN or ARG
			if(l2 == 'H') aa_idx = M_THR_IDX ; 						// THR amino acid!
			else if(l2 == 'R') aa_idx = M_TRP_IDX ; 				// TRP amino acid!
			else aa_idx = M_TYR_IDX ; break ;						// TYR amino acid!

		case 'V' : aa_idx = M_VAL_IDX ; break ;							// VAL amino acid!

		default: /*fprintf(stderr, "! Amno acid %s does not exists!\n", aa_name) ;*/ break ;
	}

	// Ok now we have our amino acid, lets update statistics!

	if(aa_idx != -1) {
		desc->aa_compo[aa_idx] ++ ;

		desc->hydrophobicity_score += get_hydrophobicity_score_from_idx(aa_idx) ;
		desc->polarity_score += get_polarity_from_idx(aa_idx) ;
		desc->volume_score += get_volume_score_from_idx(aa_idx) ;
		desc->charge_score += get_charge_from_idx(aa_idx) ;
	}
}

