#include "../headers/tpocket.h" 

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					tpocket.c
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## ----- SPECIFICATIONS
## ----- MODIFICATIONS HISTORY
##
##	27-11-08	(v)  Write fpocket output if asked + relooking
##	15-10-08	(v)  Fixed bad count when PDB file not found.
##	01-04-08	(v)  Added template for comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
##

*/

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void test_fpocket(s_tparams *par)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Test fpocket for a set of data.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_tparams *par: Parameters, contain the fpocket parameters, and the list
	of data set to test. 
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void test_fpocket(s_tparams *par)
{
	int nranks = 10, novlp = 8, 
		i, j, k, nok = 0  ;
	int ranks [] = {1,2, 3, 4, 5, 6,7,8,9, 10} ;
	float ovlp[] = {20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0,90.0} ;
	float mean_ov1 = 0.0, mean_ov2 = 0.0, mean_dst = 0.0,
		  mean_ovr1 = 0.0, mean_ovr2 = 0.0, mean_ovr3 = 0.0 ;
	int n1, n2, n3, N = 0 ;
	int status[par->nfiles] ;

	if(par) {
	/* Store statistics for each files: */
		float ddata [par->nfiles][M_NDDATA];
		int idata [par->nfiles][M_NIDATA];

	/* Test all files */
		for(i = 0 ; i < par->nfiles ; i++) {
			strcpy(par->fpar->pdb_path, par->fapo[i]) ;
			status [i] = test_AC(par, i, ddata, idata) ;
			fprintf(stdout, "> %3d : %s output code %d\n", i+1, par->fapo[i],
														   status [i]) ;
			if(status[i] == M_OK) N++ ;
		}

	/* Printing each complexe statistics */
		FILE *fp = fopen(par->p_output, "w") ;
		if(fp) {	
			n1 = 0, n2 = 0, n3 = 0 ;
			fprintf(fp, "LIG | COMPLEXE | APO | NB_PCK | OVLP1 | OVLP2 | DIST_CM | POS1 | POS2 | POS3 | REL_OVLP1 | REL_OVLP2 | REL_OVLP3 | LIGMASS | LIGVOL\n") ;
			for(i = 0 ; i < par->nfiles ; i++) {
				remove_path(par->fcomplex[i]) ;
				remove_path(par->fapo[i]) ;
				if(status[i] == M_OK) {
					fprintf(fp, "%s %s %s %5d %7.2f %7.2f %7.2f %4d %4d %4d %8.2f %8.2f %8.2f %9.2f %9.2f\n", 
							par->fligan[i], par->fcomplex[i], par->fapo[i], idata[i][M_NPOCKET],
							ddata[i][M_MAXPCT1], ddata[i][M_MAXPCT2], ddata[i][M_MINDST],
							idata[i][M_POS1], idata[i][M_POS2], idata[i][M_POS3],
							ddata[i][M_OREL1], ddata[i][M_OREL2], ddata[i][M_OREL3],
							ddata[i][M_LIGMASS], ddata[i][M_LIGVOL]) ;
					
					if(idata[i][M_POS1] > 0) {
						mean_ov1 += ddata[i][M_MAXPCT1] ;
						mean_ovr1 += ddata[i][M_OREL1];
						n1++ ;
					}
					
					if(idata[i][M_POS2] > 0) {
						mean_ov2 += ddata[i][M_MAXPCT2] ; 
						mean_ovr2 += ddata[i][M_OREL2];
						n2++ ;
					}
					
					if(idata[i][M_POS3] > 0) {
						mean_dst += ddata[i][M_MINDST];   
						mean_ovr3 += ddata[i][M_OREL3];
						n3 ++ ;
					}
					
				}
				else {
					fprintf(fp, "%s %s %s %5d %7.2f %7.2f %7.2f %4d %4d %4d %8.2f %8.2f %8.2f %9.2f %9.2f\n", 
							par->fligan[i], par->fcomplex[i], par->fapo[i], -1, -1.0, -1.0,
							-1.0, -1, -1, -1, -1.0, -1.0, -1.0, -1.0, -1.0) ;
				}
			}
				
			mean_ov1 /= (float) n1 ; mean_ovr1 /= (float) n1 ;
			mean_ov2 /= (float) n2 ; mean_ovr2 /= (float) n2 ;
			mean_dst /= (float) n3 ; mean_ovr3 /= (float) n3 ;

			fclose(fp) ;
		}
		else fprintf(stdout, "The file %s could not be opened\n", par->p_output) ;

	/* Printing global statistics */
		FILE *fg = fopen(par->g_output, "w") ;
		if(fg) {
	// Write the first overlap statistics
			fprintf(fg, "===================== General statistics on all complexes =======================\n") ;
			fprintf(fg, "\n\t--------------------------------------------------------------------\n") ;
			fprintf(fg,   "\t- _ 1st overlap criteria (use of ligand's alpha sphere neighbors)_ -\n") ;
			fprintf(fg,   "\t--------------------------------------------------------------------\n\n") ;
			fprintf(fg,"           :");
                        for( j = 0 ; j < novlp ; j++) {
                            	fprintf(fg, "  >%5.2f  :", ovlp[j]) ;
                        }
                        fprintf(fg,"\n");
                	fprintf(fg,"------------");
                        for( j = 0 ; j < novlp ; j++) {
                            fprintf(fg, "-------------") ;
                        }
                        fprintf(fg,"\n");
			for(i = 0 ; i < nranks ; i++) {
				fprintf(fg, "Rank <= %2d :", ranks[i]) ;
				for( j = 0 ; j < novlp ; j++) {
					nok = 0 ;
					for(k = 0 ; k < par->nfiles ; k++) {
						if( status[k] == M_OK && 
							ddata[k][M_MAXPCT2] >= ovlp[j] &&
							idata[k][M_POS2] <= ranks[i] &&
							idata[k][M_POS2] > 0) {
							nok ++ ;
						}
					}
					fprintf(fg, "  %6.2f  :", ((float)nok) / ((float) N)) ;
				}
				fprintf(fg, "\n") ;
			}
			fprintf(fg, "-------------------------------------\n") ;
			fprintf(fg, "Mean overlap          : %f\n", mean_ov2) ;
			fprintf(fg, "Mean relative overlap : %f\n", mean_ovr2) ;

		/* Write the second overlap statistics */

			fprintf(fg, "\n\t--------------------------------------------------------------------\n") ;
			fprintf(fg,   "\t-        _ 2nd overlap criteria (simple distance criteria) _       -\n") ;
			fprintf(fg,   "\t--------------------------------------------------------------------\n\n") ;
			fprintf(fg,"           :");
                        for( j = 0 ; j < novlp ; j++) {
                            	fprintf(fg, "  >%5.2f  :", ovlp[j]) ;
                        }
                        fprintf(fg,"\n");
			fprintf(fg,"------------");
                        for( j = 0 ; j < novlp ; j++) {
                            fprintf(fg, "-------------") ;
                        }
                        fprintf(fg,"\n");
			for(i = 0 ; i < nranks ; i++) {
				fprintf(fg, "Rank <= %2d :", ranks[i]) ;
				for( j = 0 ; j < novlp ; j++) {
					nok = 0 ;
					for(k = 0 ; k < par->nfiles ; k++) {
						if( status[k] == M_OK 
							&& ddata[k][M_MAXPCT1] >= ovlp[j]
							&& idata[k][M_POS1] <= ranks[i]
							&& idata[k][M_POS1] > 0) {
							nok ++ ;
						}
					}
					fprintf(fg, "  %6.2f  :", ((float)nok) / ((float) N)) ;
				}
				fprintf(fg, "\n") ;
			}
			fprintf(fg, "-------------------------------------\n") ;
			fprintf(fg, "Mean overlap          : %f\n", mean_ov1) ;
			fprintf(fg, "Mean relative overlap : %f\n", mean_ovr1) ;

		/* Write the third overlap statistics */

			fprintf(fg, "\n\t--------------------------------------------------------------------\n") ;
			fprintf(fg,   "\t-                       _ Distance criteria _                      -\n") ;
			fprintf(fg,   "\t--------------------------------------------------------------------\n\n") ;
			fprintf(fg, "   Ratio of good predictions (dist = 4A) \n") ;
			fprintf(fg, "------------------------------------------\n") ;
				
			for(i = 0 ; i < nranks ; i++) {
				nok = 0 ;
				for(k = 0 ; k < par->nfiles ; k++) {
					if(status[k] == M_OK && idata[k][M_POS3] <= ranks[i]
					&& idata[k][M_POS3] > 0) {
						nok ++ ;
					}
				}
				fprintf(fg, "Rank <= %2d  :\t\t%6.2f\n", ranks[i], ((float)nok) / ((float) N)) ;
			}

			fprintf(fg, "-------------------------------------\n") ;
			fprintf(fg, "Mean distance          : %f\n", mean_dst) ;
			fprintf(fg, "Mean relative overlap : %f\n", mean_ovr3) ;


			fclose(fg) ;
		}
		else fprintf(stdout, "The file %s could not be opened\n", par->g_output) ;
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void test_complexe(s_tparams *par)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Test fpocket for a single set of apo/complexe .
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_tparams *par: Parameters, contain the fpocket parameters, and the list
	of data set to test. 
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
int test_AC(s_tparams *par, int i, float ddata [][M_NDDATA], int idata [][M_NIDATA]) 
{
	s_atm **accpck = NULL,
		  **accpck2 = NULL ;
	
	char *fa = par->fapo[i], 
		 *fc = par->fcomplex[i],
		 *lig = par->fligan[i] ;
	
	int naccpck = 0,
		naccpck2 = 0 ;

	/** Load the apo form, the complexe form with ligand, and the complexe form 
	  * without ligand. The complexe without ligand will allow us to get exact 
	  * atoms contacted by voronoi vertices detected by fpocket on the complexe 
	  * form and then detect which atoms are involved in the interaction.
	 **/
	s_pdb *apdb = rpdb_open(fa, lig, M_DONT_KEEP_LIG) ;	
	s_pdb *cpdb = rpdb_open(fc, lig, M_KEEP_LIG) ;
	s_pdb *cpdb_nolig = rpdb_open(fc, lig, M_DONT_KEEP_LIG) ;
	
	/* Check that both pdb files are opened */
	if(! apdb || !cpdb) {
		fprintf(stderr, "!! PDB loading failed for %s-%s ligand %s... %p %p\n", 
						fc, fa, lig, apdb, cpdb) ;
		if(cpdb) free_pdb_atoms(cpdb) ;
		if(apdb) free_pdb_atoms(apdb) ;
		
		return M_LIGNOTFOUND ;
	}
	
	/* Check now if the ligand have been found */
	if(cpdb->natm_lig <= 0) {
		
		fprintf(stderr, "! Ligand %s not found in the complex %s (apo %s)...\n", 
						lig, fc, fa) ;
		
		return M_LIGNOTFOUND ;
	}
	
	
	/* PDBs and ligand OK, now read PDB and launch fpocket */	
	rpdb_read(apdb, lig, M_DONT_KEEP_LIG) ;	//get rid of HETATM in the apo structure
	rpdb_read(cpdb, lig, M_KEEP_LIG) ;	
	rpdb_read(cpdb_nolig, lig, M_DONT_KEEP_LIG) ;	

	c_lst_pockets *pockets = search_pocket(apdb, par->fpar);
	set_pockets_bary(pockets) ;
	
	/* Check that at least one pocket have been found */
	if(!pockets || pockets->n_pockets <= 0) {
		if(pockets) c_lst_pocket_free(pockets) ;
		return M_NOPOCKETFOUND ;
	}
	
	/* Everything is OK, so now perform the evaluation*/
	
	/* We found pocket on the apo form get volume and mass of the ligand */
	idata[i][M_NPOCKET] = pockets->n_pockets ;
	ddata[i][M_LIGMASS] = get_mol_mass_ptr(cpdb->latm_lig, cpdb->natm_lig) ;
	ddata[i][M_LIGVOL]  = get_mol_volume_ptr(cpdb->latm_lig, cpdb->natm_lig, par->fpar->nb_mcv_iter);

	/* Get atoms involved in the actual pocket */
	accpck = get_actual_pocket_DEPRECATED(cpdb, M_DST_CRIT, &naccpck) ;
	accpck2 = get_actual_pocket(cpdb, cpdb_nolig, i, par, &naccpck2) ;
	if (naccpck <= 0 || naccpck2 <= 0) {
		fprintf(stdout, "! Warning: actual pocket has 0 atoms!! %d %d\n", naccpck, naccpck2) ; 
	}

	/* Calculate evaluation criterias */
	check_pockets(pockets, accpck2, naccpck2, cpdb->latm_lig, cpdb->natm_lig, ddata[i][M_LIGVOL],	
				  accpck, naccpck, ddata, idata, i, par->fpar) ;

	printf("%d", par->keep_fpout) ;
	if(par->keep_fpout != 0) {
		write_out_fpocket(pockets, apdb, par->fpar) ;
	}
	//write_pdb_com(cpdb, par->fcomplex[i]) ;
	
	/* Free memory */
	c_lst_pocket_free(pockets) ;
	my_free(accpck) ;
	free_pdb_atoms(apdb) ;
	free_pdb_atoms(cpdb) ;
	free_pdb_atoms(cpdb_nolig) ;
		
	return M_OK ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	s_atm** get_actual_pocket(s_pdb *cpdb, s_pdb *cpdb_nolig, int i, s_tparams *par, int *nb_atm) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Get atoms contacted by voronoi vertices present in the actual pocket.
	This way of finding exact atoms contacted by the ligand is more accurate,
	but need an extra search by fpocket to find voronoi vertices in the complexe
	form without the ligand...
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_tparams *par: Parameters, contain the fpocket parameters, and the list
	of data set to test. 
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
s_atm** get_actual_pocket(s_pdb *cpdb, s_pdb *cpdb_nolig, int i, s_tparams *par, int *nb_atm) 
{
	s_atm **neigh = NULL ;

	c_lst_pockets *pockets = search_pocket(cpdb_nolig, par->fpar);
	if(pockets && pockets->n_pockets > 0) {
	// Get the list of atoms contacted by the vertices near the ligand.
		neigh = get_mol_ctd_atm_neigh(cpdb->latm_lig, cpdb->natm_lig, 
				cpdb, pockets->vertices, M_DST_CRIT, M_INTERFACE_SEARCH, nb_atm) ;

		c_lst_pocket_free(pockets) ;
	}
	else {
		fprintf(stderr, "! No pocket found for apo %s and complex %s ligand %s...\n", 
						par->fapo[i], par->fcomplex[i], par->fligan[i]) ;

		if(pockets) c_lst_pocket_free(pockets) ;
	}

	return neigh ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	s_atm** get_actual_pocket_DEPRECATED(s_pdb *cpdb, float lig_dist_crit, int *nb_atm) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Get atoms near the ligand using a distance criteria. This can define the
	atoms involved in the interface between the ligand and the protein.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_tparams *par: Parameters, contain the fpocket parameters, and the list
					 of data set to test. 
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
s_atm** get_actual_pocket_DEPRECATED(s_pdb *cpdb, float lig_dist_crit, int *nb_atm) 
{	
	// Getting the ligan's neighbors.
	s_atm **alneigh = get_mol_atm_neigh(cpdb->latm_lig, cpdb->natm_lig, cpdb, lig_dist_crit, nb_atm) ;
	if(*nb_atm <= 0) {
		fprintf(stderr, "! No neighbor found for the ligand at %fA, trying with %fA\n", lig_dist_crit, lig_dist_crit+0.5) ;
		if(alneigh) my_free(alneigh) ;


		alneigh = get_mol_atm_neigh(cpdb->latm_lig, cpdb->natm_lig, cpdb, lig_dist_crit+0.5, nb_atm) ;

		if(*nb_atm <= 0) {
			fprintf(stderr, "! No neighbor found for the ligand at %fA, trying with %fA\n", lig_dist_crit+0.5, lig_dist_crit+1.0) ;
			if(alneigh) my_free(alneigh) ;

			alneigh = get_mol_atm_neigh(cpdb->latm_lig, cpdb->natm_lig, cpdb, lig_dist_crit+2.0, nb_atm) ;
			if(*nb_atm <= 0) {
				fprintf(stderr, "! No neighbor found for the ligand at %fA !!!?!\n", lig_dist_crit+2.0) ;
				if(alneigh) my_free(alneigh) ;
				alneigh = NULL ;
			}
		}
	}

	return alneigh ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void check_pockets(c_lst_pockets *pockets, int naccpck ) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Checking the pocket prediction for one data set. (apo+complex+ligand)
	This function will calculate atom several overlap for each pockets found.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_pockets *pockets: The pockets found for this set.
	@ s_pdb *cpdb: The complex corresponding pdb data
	@ float lig_dist_crit: The distance criteria for searching ligand's neighbours.
   -----------------------------------------------------------------------------
   ## RETURN:
	int: the real pocket has been found?
   -----------------------------------------------------------------------------
*/
void check_pockets(c_lst_pockets *pockets, s_atm **accpck, int naccpck, s_atm **lig, int nalig, float ligvol,
				   s_atm **alneigh, int nlneigh, float ddata [][M_NDDATA], int idata [][M_NIDATA], int i,
				   s_fparams *params)
{
	int found [] = {0, 0, 0} ;
	int npneigh = 0, pos, j ;
	float ov1, ov2, dst ;
	//float ovol ;
	
	s_atm **pneigh = NULL ;

	node_pocket *ncur = NULL ;
	s_pocket *pcur = NULL ;
	
	// Check the correspondance for each pocket found
	ncur = pockets->first ;

	pos = 0 ;
	//printf("=====\n") ;
	while(ncur) {
		pos ++ ;
		pcur = ncur->pocket ;
	
	// Getting the pocket's vertices neighbors, and then calculate the pocket properties
		//pneigh = get_vert_contacted_atms(pcur->v_lst, &npneigh) ;
		pneigh = get_pocket_contacted_atms(pcur, &npneigh) ;

		ov1 = atm_corsp(alneigh, nlneigh, pneigh, npneigh) ;
		ov2 = atm_corsp(accpck, naccpck, pneigh, npneigh) ;
		//ovol =  set_overlap_volumes(pcur, lig, nalig, ligvol, params) ;
		
		//printf ("%f\n", ovol) ;
		fflush(stdout) ;
		
		if(! found[0]) {
			for (j = 0 ; j < nalig ; j++) {
				dst = dist(lig[j]->x, lig[j]->y, lig[j]->z, 
							pcur->bary[0], pcur->bary[1], pcur->bary[2]) ;
				if(dst < 4.0) {
				// Criteria 3 OK
					ddata[i][M_MINDST] = dst ;
					ddata[i][M_OREL3] = (naccpck <= 0.0)?-1.0 :(float)npneigh/(float)naccpck*100.0 ;
					idata[i][M_POS3] = pos ;
					found[0] = 1 ; break ;
				}
			}
		}

		if(! found[1]) {
			if(ov1 > 50.0) {
				idata[i][M_POS1] = pos ;
				ddata[i][M_MAXPCT1] = ov1 ;
				ddata[i][M_OREL1] = (nlneigh <= 0.0)? -1.0 : (float)npneigh/(float)nlneigh*100.0 ;
				found[1] = 1 ;
			}
		}
	
		if(! found[2]) {
			if(ov2 > 50.0) {
				idata[i][M_POS2] = pos ;
				ddata[i][M_MAXPCT2] = ov2 ;
				ddata[i][M_OREL2] = (naccpck <= 0.0)?-1.0 : (float)npneigh/(float)naccpck*100.0 ;
				found[2] = 1 ;
			}
		}

		my_free(pneigh) ;
		ncur = ncur->next ;

		// Break the loop if all criteria are OK
		if(found[0] && found[1] && found[2]) break ;
		
	}

	if (! found[0]) {
		ddata[i][M_MINDST] = 0.0 ;
		ddata[i][M_OREL3] = 0.0 ;
		idata[i][M_POS3] = 0 ;
	}

	if (! found[1]) {
		ddata[i][M_MAXPCT1] = 0.0 ;
		ddata[i][M_OREL1] = 0.0 ;
		idata[i][M_POS1] = 0 ;
	}

	if (! found[2]) {
		ddata[i][M_MAXPCT2] = 0.0 ;
		ddata[i][M_OREL2] = 0.0 ;
		idata[i][M_POS2] = 0 ;
	}
}

/** -----------------------------------------------------------------------------
	-----------------------------------------------------------------------------
	-----------------------------------------------------------------------------

	ALL THE FOPLLOWING FUNCTIONS SHOULD BE USED TO CALCULATE AN OVERLAP VOLUME
	BETWEEN A GIVEN POPCKET AND THE REAL POCKET.

	THE FUNCTION set_overlap_volumes WILL SET AN OVERLAP VOLUME FOR EACH POCKET
	IN THE LIST GIVEN IN ARGUMENT, USING MONTE-CARLO OR BASIC DISCRETE ALGORITHM 
	DEPENDING ON THE PARAMETERS OF THE PROGRAM.

	BOTH ALGORITHM CAN BE USED INDEPENDENLTLY WITHOUT THE NEED OF A PARAMETER
	STRUCTURE (s_fparams)

	-----------------------------------------------------------------------------
	-----------------------------------------------------------------------------
    -----------------------------------------------------------------------------
*/

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void set_overlap_volumes(c_lst_pockets *pockets, s_atm **lig, int natoms, float lig_vol, s_fparams *params) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	This function will set an overlap volume between each pocket and the ligand.
	Function of the parameters, the method used will be either Monte Carlo 
	approximation, or discret basic approximation.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_pockets *pockets: Pockets
	@ s_atm **lig: The ligand atoms.
	@ int natoms natoms: Number of atoms of the ligand.
	@ float lig_vol: Volume of the ligand
	@ s_fparams *params: Parameters, giving the algorithm to use and the corresponding 
						 parameter.
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
float set_overlap_volumes(s_pocket *pocket, s_atm **lig, int natoms, float lig_vol, s_fparams *params) 
{
	float (*pf_set_vol)(s_atm**, int, float, s_pocket*, int ) = NULL ;	// The function to use for calculation
	int crit = 0,
		method = 0 ;

	if(params->basic_volume_div <= 0) {
		pf_set_vol = set_mc_overlap_volume ;
		crit = params->nb_mcv_iter ;
	}
	else {
		pf_set_vol = set_basic_overlap_volume ;
		crit = params->basic_volume_div ;
		method = 1 ;
	}

	if(pocket->v_lst->n_vertices > 150) {
		if(method == 0 && crit < 20000) crit = 20000 ;
	}
	
	return pf_set_vol(lig, natoms, lig_vol, pocket, crit) ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	float set_mc_overlap_volume(s_atm **lig, int natoms, float lig_vol,s_pocket *pocket, int niter)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	This function will set an overlap volume between each pocket and the ligand.
	Monte Carlo aproximation.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_pocket *pocket: The Pocket
	@ s_atm **lig: The ligand atoms.
	@ int natoms natoms: Number of atoms of the ligand.
	@ float lig_vol: Volume of the ligand
	@ int niter: Number of iterations to perform.
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
float set_mc_overlap_volume(s_atm **lig, int natoms, float lig_vol, s_pocket *pocket, int niter)
{
	c_lst_vertices *vertices = pocket->v_lst ;
	s_vvertice *vcur = NULL ;

	int i = 0, j = 0,
		nb_in = 0,
		found = 0 ;

	float xmin = 0.0, xmax = 0.0,
		  ymin = 0.0, ymax = 0.0,
		  zmin = 0.0, zmax = 0.0,
		  xtmp = 0.0, ytmp = 0.0, ztmp = 0.0,
		  xr   = 0.0, yr   = 0.0, zr   = 0.0,
		  vbox = 0.0, full_vol = 0.0;

	s_atm *acur = NULL ;

	// First, search extrems coordinates in the ligan
	for(i = 0 ; i < natoms ; i++) {
		acur = lig[i] ;
		if(i == 0) {
			xmin = acur->x - acur->radius ; xmax = acur->x + acur->radius ;
			ymin = acur->y - acur->radius ; ymax = acur->y + acur->radius ;
			zmin = acur->z - acur->radius ; zmax = acur->z + acur->radius ;
		}
		else {
		// Update the minimum and maximum extreme point
			if(xmin > (xtmp = acur->x - acur->radius)) xmin = xtmp ;
			else if(xmax < (xtmp = acur->x + acur->radius)) xmax = xtmp ;
	
			if(ymin > (ytmp = acur->y - acur->radius)) ymin = ytmp ;
			else if(ymax < (ytmp = acur->y + acur->radius)) ymax = ytmp ;
	
			if(zmin > (ztmp = acur->z - acur->radius)) zmin = ztmp ;
			else if(zmax < (ztmp = acur->z + acur->radius)) zmax = ztmp ;
		}
	}

	node_vertice *cur = vertices->first ;
	// Continue with the pocket
	while(cur) {
		vcur = cur->vertice ;
		if(xmin > (xtmp = vcur->x - vcur->ray)) xmin = xtmp ;
		else if(xmax < (xtmp = vcur->x + vcur->ray)) xmax = xtmp ;
	
		if(ymin > (ytmp = vcur->y - vcur->ray)) ymin = ytmp ;
		else if(ymax < (ytmp = vcur->y + vcur->ray)) ymax = ytmp ;
	
		if(zmin > (ztmp = vcur->z - vcur->ray)) zmin = ztmp ;
		else if(zmax < (ztmp = vcur->z + vcur->ray)) zmax = ztmp ;

		cur = cur->next ;
	}


	// Next calculate the box volume
	vbox = (xmax - xmin)*(ymax - ymin)*(zmax - zmin) ;

	// Then apply monte carlo approximation of the volume
	for(i = 0 ; i < niter ; i++) {
		found = 0 ;
		xr = rand_uniform(xmin, xmax) ;
		yr = rand_uniform(ymin, ymax) ;
		zr = rand_uniform(zmin, zmax) ;

		cur = vertices->first ;
		while(cur) {
			vcur = cur->vertice ;
			xtmp = vcur->x - xr ;
			ytmp = vcur->y - yr ;
			ztmp = vcur->z - zr ;

		// Compare r^2 and dist(center, random_point)^2
			if((vcur->ray*vcur->ray) > (xtmp*xtmp + ytmp*ytmp + ztmp*ztmp)) {
			//the point is inside one of the vertice!!
				nb_in ++ ; found = 1 ; break ;
			}
			cur = cur->next ;
		}

		if(!found) {
			for(j = 0 ; j < natoms ; j++) {
				acur = lig[j] ;
				xtmp = acur->x - xr ;
				ytmp = acur->y - yr ;
				ztmp = acur->z - zr ;
	
			// Compare r^2 and dist(center, random_point)^2
				if((acur->radius*acur->radius) > (xtmp*xtmp + ytmp*ytmp + ztmp*ztmp)) {
				//the point is inside one of the vertice!!
					nb_in ++ ; break ;
				}
			}
		}
	}

	full_vol = ((float)nb_in)/((float)niter)*vbox ;	
	pocket->vol_corresp = ((pocket->pdesc->volume + lig_vol) - full_vol)/lig_vol ;

	return pocket->vol_corresp ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	float set_basic_overlap_volume(s_atm **lig, int natoms, float lig_vol,s_pocket *pocket, int niter)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	This function will set an overlap volume between each pocket and the ligand.
	Discrete aproximation. (algo in idiscret*idiscret*idiscret minimum)
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_pocket *pocket: The Pocket
	@ s_atm **lig: The ligand atoms.
	@ int natoms natoms: Number of atoms of the ligand.
	@ float lig_vol: Volume of the ligand
	@ int niter: Number of iterations to perform.
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
float set_basic_overlap_volume(s_atm **lig, int natoms, float lig_vol,s_pocket *pocket, int idiscret)
{
	c_lst_vertices *vertices = pocket->v_lst ;
	s_vvertice *vcur = NULL ;

	int i = 0, j = 0,
		nb_in = 0,
		found = 0,
		niter = 0 ;

	float discret = 1.0/(float)idiscret ;

	float x = 0.0, y = 0.0, z = 0.0,
		  xstep = 0.0, ystep = 0.0, zstep = 0.0 ;

	float xmin = 0.0, xmax = 0.0,
		  ymin = 0.0, ymax = 0.0,
		  zmin = 0.0, zmax = 0.0,
		  xtmp = 0.0, ytmp = 0.0, ztmp = 0.0,
		  vbox = 0.0, full_vol = 0.0;

	s_atm *acur = NULL ;

	// First, search extrems coordinates in the ligan
	for(i = 0 ; i < natoms ; i++) {
		acur = lig[i] ;
		if(i == 0) {
			xmin = acur->x - acur->radius ; xmax = acur->x + acur->radius ;
			ymin = acur->y - acur->radius ; ymax = acur->y + acur->radius ;
			zmin = acur->z - acur->radius ; zmax = acur->z + acur->radius ;
		}
		else {
		// Update the minimum and maximum extreme point
			if(xmin > (xtmp = acur->x - acur->radius)) xmin = xtmp ;
			else if(xmax < (xtmp = acur->x + acur->radius)) xmax = xtmp ;
	
			if(ymin > (ytmp = acur->y - acur->radius)) ymin = ytmp ;
			else if(ymax < (ytmp = acur->y + acur->radius)) ymax = ytmp ;
	
			if(zmin > (ztmp = acur->z - acur->radius)) zmin = ztmp ;
			else if(zmax < (ztmp = acur->z + acur->radius)) zmax = ztmp ;
		}
	}

	node_vertice *cur = vertices->first ;
	// Continue with the pocket
	while(cur) {
		vcur = cur->vertice ;
		if(xmin > (xtmp = vcur->x - vcur->ray)) xmin = xtmp ;
		else if(xmax < (xtmp = vcur->x + vcur->ray)) xmax = xtmp ;
	
		if(ymin > (ytmp = vcur->y - vcur->ray)) ymin = ytmp ;
		else if(ymax < (ytmp = vcur->y + vcur->ray)) ymax = ytmp ;
	
		if(zmin > (ztmp = vcur->z - vcur->ray)) zmin = ztmp ;
		else if(zmax < (ztmp = vcur->z + vcur->ray)) zmax = ztmp ;

		cur = cur->next ;
	}


	// Next calculate the box volume
	vbox = (xmax - xmin)*(ymax - ymin)*(zmax - zmin) ;

	xstep = discret * (xmax - xmin) ;
	ystep = discret * (ymax - ymin) ;
	zstep = discret * (zmax - zmin) ;

	// Then apply monte carlo approximation of the volume
	for(x = xmin ; x < xmax ; x += xstep) {
		for(y = ymin ; y < ymax ; y += ystep) {	
			for(z = zmin ; z < zmax ; z += zstep) {
				found = 0 ;
				cur = vertices->first ;
				while(cur) {
					vcur = cur->vertice ;
					xtmp = vcur->x - x ;
					ytmp = vcur->y - y ;
					ztmp = vcur->z - z ;
		
				// Compare r^2 and dist(center, random_point)^2
					if((vcur->ray*vcur->ray) > (xtmp*xtmp + ytmp*ytmp + ztmp*ztmp)) {
					//the point is inside one of the vertice!!
						nb_in ++ ; found = 1 ; break ;
					}
					cur = cur->next ;
				}
		
				if(!found) {
					for(j = 0 ; j < natoms ; j++) {
						acur = lig[j] ;
						xtmp = acur->x - x ;
						ytmp = acur->y - y ;
						ztmp = acur->z - z ;
			
					// Compare r^2 and dist(center, random_point)^2
						if((acur->radius*acur->radius) > (xtmp*xtmp + ytmp*ytmp + ztmp*ztmp)) {
						//the point is inside one of the vertice!!
							nb_in ++ ; break ;
						}
					}
				}
				niter++ ;
			}
		}
	}

	full_vol = ((float)nb_in)/((float)niter)*vbox ;	
	pocket->vol_corresp = ((pocket->pdesc->volume + lig_vol) - full_vol)/lig_vol ;

	// Ok lets just return the volume Vpok = Nb_in/Niter*Vbox
	return pocket->vol_corresp ;
}
