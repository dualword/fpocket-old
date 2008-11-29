
#include "../headers/dpocket.h"

/**

## ----- GENERAL INFORMATION
##
## FILE 					dpocket.c
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## ----- SPECIFICATIONS
##
##	This file contains all main routines of the dpocket program.
##	Given a set of protein-ligand PDB file, those routines will
##	calculate several descriptors on the pocket using two
##	different way to define what is the pocket:
##
##	1 - EXPLICIT DEFINITION:
##		The pocket will be defined explicitely using a distance
##		criteria with respect to the ligand. Two way of defining
##		the pockets are available:
##
##		a - The pocket is defined as all atoms contacted by
##		alpha spheres situated a distance lower or equal that
##		D (defined by the user or 4.0 by default in dparams.h)
##		of each ligand's atoms. 
##
##		b - The pocket is defined as all atoms situated at
##		a distance D (defined by the user or 4.5 by default in 
##		dparams.h) of each ligand's atoms.
##
##		This way, one can define a more or less accurate zone 
##		of interaction between the ligand and the protein. 
##		Descriptors are then calculated using alpha spheres 
##		retained and contacted atoms.
##
##	2 - IMPLICIT DEFINITION
##		The pocket will be defined as the one having the greater
##		atomic overlap, using the fpocket algorithm.
##
##	Pockets described using the explicit and the implicit definition
##	will be stored in a different file. Additionnaly, pockets found by
##	fpocket and having an overlap < 50% will be stored in an additional 
##	file.
##
##	Default file name are given in dparams.h. Currently and respectively:
##	"dpout_explicitp.txt"
##	"dpout_fpocketp.txt"
##	"dpout_fpocketnp.txt"
##
## ----- MODIFICATIONS HISTORY
##
##	01-04-08	(v)  Comments UTD
##	01-04-08	(v)  Added comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
##
##	(v) Clean the structure of the code... It could be done in a much better way

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
	dpocket
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Dpocket main function. Simple loop is performed over all files.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_dparams *par: Parameters of the programm
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void dpocket(s_dparams *par)
{
	int i, j ;
	FILE *fout[3] ;

	if(par) {
	/* Opening output file file */

		fout[0] = fopen(par->f_exp,"w") ;
		fout[1] = fopen(par->f_fpckp,"w") ;
		fout[2] = fopen(par->f_fpcknp,"w") ;

		if(fout[0] && fout[1] && fout[2]) {

		/* Writing column names */
	
			for( i = 0 ; i < 3 ; i++ ) {
				fprintf(fout[i], M_DP_OUTP_HEADER) ;
				for( j = 0 ; j < 20 ; j++ ) fprintf(fout[i], " %s", get_aa_name3(j));
				fprintf(fout[i], "\n");
			}
	
		/* Begins dpocket */
			for(i = 0 ; i < par->nfiles ; i++) {
				fprintf(stdout, "=> Describing interface for protein %s...\n", 
						par->fcomplex[i]) ;

				desc_pocket(par->fcomplex[i], par->ligs[i], par, fout) ;
				
			}

			for( i = 0 ; i < 3 ; i++ ) fclose(fout[i]) ;
		}
		else {
			if(! fout[0]) {
				fprintf(stdout, "! Output file <%s> couldn't be opened.\n", 
						par->f_exp) ;
			}
			else if (! fout[1]) {
				fprintf(stdout, "! Output file <%s> couldn't be opened.\n", 
						par->f_fpckp) ;
			}
			else {
				fprintf(stdout, "! Output file <%s> couldn't be opened.\n", 
						par->f_fpcknp) ;
			}
		}
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
    desc_pocket
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	@ const char fcomplexe[] : File containing the PDB
	@ const char ligname[]   : Ligand resname identifier
	@ s_dparams *par         : Parameters
	@ FILE *f[3]             : The 3 FILE * to write output in
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
void desc_pocket(const char fcomplexe[], const char ligname[], s_dparams *par, 
				 FILE *f[3]) 
{
	c_lst_pockets *pockets = NULL ;
	s_lst_vvertice *verts = NULL ;	
	s_atm **interface = NULL ;
	s_desc *edesc ;
	s_atm **lig = NULL,
		  **patoms ;

	float vol, ovlp ;
	int nal = 0,
		nai = 0,	/* Number of atoms in the interface */
		nbpa ;

	fprintf(stdout, "dpocket: Loading pdb... ") ; fflush(stdout) ;
	s_pdb *pdb_cplx_l = rpdb_open(fcomplexe, ligname, M_KEEP_LIG) ;
	s_pdb *pdb_cplx_nl = rpdb_open(fcomplexe, ligname, M_DONT_KEEP_LIG) ;
	
	if(pdb_cplx_l && pdb_cplx_nl && pdb_cplx_l->natm_lig > 0) {
		rpdb_read(pdb_cplx_l, ligname, M_KEEP_LIG) ;	
		rpdb_read(pdb_cplx_nl, ligname, M_DONT_KEEP_LIG) ;
		fprintf(stdout, " OK\n") ;

		lig = pdb_cplx_l->latm_lig ;
		nal = pdb_cplx_l->natm_lig ;

		/* Getting explicit interface using the known ligand */
		fprintf(stdout, "dpocket: Explicit pocket definition... \n") ; 
		fflush(stdout) ;
		verts = load_vvertices(pdb_cplx_nl, 3, par->fpar->asph_min_size, 
							   par->fpar->asph_max_size) ;

		if(verts) {
			edesc = allocate_s_desc() ;
			interface = get_explicit_desc(pdb_cplx_l, verts, lig, nal, par,
										   &nai, edesc) ;
	
			/* Getting fpocket pockets */
 			pockets = clusterPockets(verts, par->fpar);
			reIndexPockets(pockets) ;
			refinePockets(pockets, par->fpar) ;
			reIndexPockets(pockets) ;
			pck_ml_clust(pockets, par->fpar);
			dropSmallNpolarPockets(pockets, par->fpar);
			set_pockets_descriptors(pockets);
	
			/* Writing output */
			vol = get_mol_volume_ptr(lig, nal, par->fpar->nb_mcv_iter) ; 
			write_pocket_desc(fcomplexe, ligname, edesc, vol, 100.0, f[0]) ;
			node_pocket *cur = pockets->first ;
			while(cur) {
			/* For each pocket, save descriptors and say if the pocket contains 
			 * the ligand */
				patoms = get_vert_contacted_atms(cur->pocket->v_lst, &nbpa) ;
				ovlp = atm_corsp(interface, nai, patoms, nbpa) ;

				if(ovlp > 40.0) {
					write_pocket_desc(fcomplexe, ligname, cur->pocket->pdesc, vol, 
									  ovlp, f[1]) ;
				}
				else {
					write_pocket_desc(fcomplexe, ligname, cur->pocket->pdesc, vol, 
									  ovlp, f[2]) ;
				}
				my_free(patoms) ;
				cur = cur->next ;
			}
			c_lst_pocket_free(pockets) ;
			my_free(interface) ;
		}

		/* Free memory */
		free_pdb_atoms(pdb_cplx_l) ;
		free_pdb_atoms(pdb_cplx_nl) ;
	}
	else {
		if(!pdb_cplx_l) 
			fprintf(stdout, "! Error occured while loading file %s...\n", 
							fcomplexe) ;
		else if(!pdb_cplx_nl) 
			fprintf(stdout, "! Error occured while loading file %s...\n", 
							fcomplexe) ;
		else fprintf(stdout, "! No ligand %s found in the pdb file...\n", 
							ligname) ;
	}
	
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	get_explicit_desc
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Determine the explicit pocket (see comments at the top of the file), and
	calculate descriptors (and fill the input structure) 
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_pdb *pdb_cplx_l     : The pdb structure
	@ s_lst_vvertice *verts : The vertices found
	@ s_atm **lig           : Atoms of the ligand
	@ int nal               : Number of atom in the ligand
	@ s_dparams *par        : Parameters
	@ int *nai              : OUTPUT Number of atom in the interface
	@ s_desc *desc          : OUTPUT Descriptors
   -----------------------------------------------------------------------------
   ## RETURN:
	s_atm **: List of pointer to atoms defining the explicit pocket.
	plus nai and desc are filled.
   -----------------------------------------------------------------------------
*/
s_atm** get_explicit_desc(s_pdb *pdb_cplx_l, s_lst_vvertice *verts, s_atm **lig, 
						  int nal, s_dparams *par, int *nai, s_desc *desc)
{
	int nvn = 0 ;	/* Number of vertices in the interface */

	s_atm **interface = NULL ;

	fprintf(stdout, "dpocket: Determning explicit interface... ") ; 
	fflush(stdout) ;
	
	if(par->interface_method == M_INTERFACE_METHOD2) {
	/* Use the distance-based method to define the interface */
		interface = get_mol_atm_neigh(lig, nal, pdb_cplx_l, 
									  par->interface_dist_crit, nai) ;
	}
	else {
	/* Use the voronoi vertices-based method to define the interface */
		interface = get_mol_ctd_atm_neigh(lig, nal, pdb_cplx_l, verts, 
										  par->interface_dist_crit, 
										  M_INTERFACE_SEARCH, nai) ; 
	}
	fprintf(stdout, " OK\n") ;

	/* Get a tab of pointer for interface's vertices to send a correct argument 
	 * type to set_descriptors */
	s_vvertice **tpverts = get_mol_vert_neigh(lig, nal, verts, pdb_cplx_l, 
											  par->interface_dist_crit, &nvn) ;
	
	/* Ok we have the interface and the correct vertices list, now calculate 
	 * descriptors and write it.
	 **/
	fprintf(stdout, "dpocket: Calculating descriptors... ") ; fflush(stdout) ;
	set_descriptors(interface, *nai, tpverts, nvn, desc) ;
	fprintf(stdout, " OK\n") ;

	/* Free memory */
	my_free(tpverts) ;

	return interface ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	write_pocket_desc
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
     Write pocket descriptors into a file.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
    @ const char fc[] : Name of the pdb file
	@ const char l[]  : Resname of the ligand
	@ s_desc *d       : Stucture of descriptors
	@ float lv        : Ligand volume
	@ float ovlp      : Overlap
	@ FILE *f         : Buffer to write in
   -----------------------------------------------------------------------------
   ## RETURN:
    void
   -----------------------------------------------------------------------------
*/
void write_pocket_desc(const char fc[], const char l[], s_desc *d, float lv, 
					   float ovlp, FILE *f) 
{
	fprintf(f, M_DP_OUTP_FORMAT, M_DP_OUTP_VAR(fc, l, ovlp, lv, d)) ;

	int i ;
	for(i = 0 ; i < 20 ; i++) fprintf(f, " %3d", d->aa_compo[i]) ;
	
	fprintf(f, "\n") ;
}
