
#include "../headers/tparams.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					tparams.c
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## ----- SPECIFICATIONS
## ----- MODIFICATIONS HISTORY
##
##	01-04-08	(v)  Added template for comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
##

*/

/**-----------------------------------------------------------------------------
   ## FUNCTION:
	s_tparams* init_def_tparams(void)
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Initialisation of default parameters
   -----------------------------------------------------------------------------
   ## PARAMETRES: void
   -----------------------------------------------------------------------------
   ## RETURN: 
	Pointer to allocated paramers.
   -----------------------------------------------------------------------------
*/
s_tparams* init_def_tparams(void)
{
	s_tparams *par = (s_tparams *)my_malloc(sizeof(s_tparams)) ;
	par->p_output = (char *)my_malloc(M_MAX_FILE_NAME_LENGTH*sizeof(char)) ;
	strcpy(par->p_output, M_STATS_OUTP) ;
	par->g_output = (char *)my_malloc(M_MAX_FILE_NAME_LENGTH*sizeof(char)) ;
	strcpy(par->g_output, M_STATS_OUTG) ;
	par->fapo = NULL ;
	par->fcomplex = NULL ;
	par->fligan = NULL ;
	par->nfiles = 0 ;
	par->lig_neigh_dist = M_LIG_NEIG_DIST ;
	par->fpar = init_def_fparams() ;
	return par ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	s_tparams* get_tpocket_args(int nargs, char **args)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	This function analyse the user's command line and parse it to store parameters
	for the test programm.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ int nargs :  Number of arguments
	@ char **args: Arguments of main program
   -----------------------------------------------------------------------------
   ## RETURN: 
	s_tparams*: Pointer to parameters
   -----------------------------------------------------------------------------
*/
s_tparams* get_tpocket_args(int nargs, char **args)
{
	int i,
		status = 0 ;

	int nlig = 0,
		ncomp = 0,
		napo = 0,
		nstats = 0;
	
	char *str_ligan_file = NULL,
		 *str_complex_file = NULL,
		 *str_apo_file = NULL,
		 *str_list_file = NULL ;

	s_tparams *par = init_def_tparams() ;
	//read arguments by flags
	for (i = 1; i < nargs; i++) {
		if (strlen(args[i]) == 2 && args[i][0] == '-' && i < (nargs-1)) {
			switch (args[i][1]) {
				case M_PAR_MAX_ASHAPE_SIZE	  : status += parse_asph_max_size(args[++i], par->fpar) ; 
											  break ;
				case M_PAR_MIN_ASHAPE_SIZE	  : status += parse_asph_min_size(args[++i], par->fpar) ; 
											  break ;
				case M_PAR_CLUST_MAX_DIST	  : status += parse_clust_max_dist(args[++i], par->fpar) ; 
											  break ;
				case M_PAR_SL_MAX_DIST		  : status += parse_sclust_max_dist(args[++i], par->fpar) ; 
											  break ;
				case M_PAR_SL_MIN_NUM_NEIGH   : status += parse_sclust_min_nneigh(args[++i], par->fpar) ; 
											  break ;
				case M_PAR_MC_ITER 			  : status += parse_mc_niter(args[++i], par->fpar) ; 
											  break ;
				case M_PAR_BASIC_VOL_DIVISION : status += parse_basic_vol_div(args[++i], par->fpar) ; 
											  break ;
				case M_PAR_MIN_POCK_NB_ASPH   : status += parse_min_pock_nb_asph(args[++i], par->fpar) ; 
											  break ;
				case M_PAR_REFINE_DIST		  : status += parse_refine_dist(args[++i], par->fpar) ; 
											  break ;
				case M_PAR_REFINE_MIN_NAPOL_AS: status += parse_refine_min_apolar_asphere_prop(args[++i], par->fpar) ; 
											  break ;
				case M_PAR_MIN_APOL_NEIGH	  : status += parse_min_apol_neigh(args[++i], par->fpar) ;
											  break ;
				case M_PAR_LIG_NEIG_DIST	  : status += parse_lig_neigh_dist(args[++i], par) ; 
											  break ;
				case M_PAR_VALID_COMPLEX_FILE	:
						if(ncomp >= 1) fprintf(stderr, "! More than one single file for complex protein has been given. Ignoring it.\n") ;
						else {
							str_complex_file = args[++i] ; ncomp++ ;
						}
						break ;
				case M_PAR_VALID_APO_FILE		: 
						if(napo >= 1) fprintf(stderr, "! More than one single file for apo protein has been given. Ignoring it.\n") ; 
						else {
							str_apo_file = args[++i] ; napo++ ; 
						}
						break ;

				case M_PAR_VALID_LIG_CODE		: 
						if(nlig >= 1) fprintf(stderr, "! More than one single file for ligand has been given. Ignoring it.\n") ;
						else {
							str_ligan_file = args[++i] ; nlig++ ; 
						}
						break ;
				case M_PAR_P_STATS_OUT : 
						if(nstats >= 1) fprintf(stdout, "! More than one single file for the stats output file has been given. Ignoring this one.\n") ;
						else {
							if(i < nargs-1) {
								if(strlen(args[++i]) < M_MAX_FILE_NAME_LENGTH) strcpy(par->p_output, args[i]) ;
								else fprintf(stdout, "! Output file name is too long... Keeping default (%s).", M_STATS_OUTP) ;
							}
							else {
								fprintf(stdout, "! Invalid output file name argument missing.\n") ;
								status += 1 ;
							}
						}
						break ;
				case M_PAR_G_STATS_OUT : 
						if(nstats >= 1) fprintf(stdout, "! More than one single file for the stats output file has been given. Ignoring this one.\n") ;
						else {
							if(i < nargs-1) {
								if(strlen(args[++i]) < M_MAX_FILE_NAME_LENGTH) strcpy(par->g_output, args[i]) ;
								else fprintf(stdout, "! Output file name is too long... Keeping default (%s).", M_STATS_OUTG) ;
							}
							else {
								fprintf(stdout, "! Invalid output file name argument missing.\n") ;
								status += 1 ;
							}
						}
						break ;
				case M_PAR_VALID_INPUT_FILE		: str_list_file = args[++i] ; break ;
				default: printf("> Unknown option '%s'. Ignoring it.\n", args[i]) ; break ;
			}
		}
	}

	if(status > 0) {
		free_tparams(par) ;
		par = NULL ;
		print_test_usage(stdout);
	}
	else {
		if(str_list_file) {
			int res = add_list_data(str_list_file, par) ;
			if(res <= 0) {
				fprintf(stderr, "! No data has been read.\n") ;
				free_tparams(par) ;
				par = NULL ;
				print_test_usage(stdout);
			}
			else {
				strcpy (par->stats_g, M_STATS_OUTG) ;
				strcpy (par->stats_p, M_STATS_OUTP) ;
				
			}
		}
		else {
			if(str_ligan_file && str_apo_file && str_complex_file) {
				if(add_prot(str_apo_file, str_complex_file, str_ligan_file, par) == 0){
					fprintf(stderr, "! No data has been read.\n") ;
					free_tparams(par) ;
					par = NULL ;
					print_test_usage(stdout);
				}
				else {
					strcpy (par->stats_g, M_STATS_OUTG) ;
					strcpy (par->stats_p, M_STATS_OUTP) ;
				
				}
			}
			else {
				fprintf(stdout, "! Argument is missing! \n");
				free_tparams(par) ;
				par = NULL ;
				print_test_usage(stdout) ;
			}
		}
	}

	return par;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int add_list_data(char *str_list_file, s_tparams *par) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Load a list of apo-complex-ligand file. This file should have the following
	format:
	apo_file complex_file ligan_file
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ char *str_list_file : Path of the file containing all data
	@ s_tparams *par: Structures that stores all thoses files
   -----------------------------------------------------------------------------
   ## RETURN: 
	int: Number of file read.
   -----------------------------------------------------------------------------
*/
int add_list_data(char *str_list_file, s_tparams *par) 
{
	FILE *f;
	int n,
		nread = 0,
		status ;

	char buf[M_MAX_PDB_NAME_LEN*2 + 6],
		 apobuf[M_MAX_PDB_NAME_LEN],
		 complexbuf[M_MAX_PDB_NAME_LEN],
		 ligbuf[5];

	// Loading data.
	f = fopen(str_list_file, "r") ;
	if(f) {
		while(fgets(buf, 210, f)) {
			n = par->nfiles ;
			status = sscanf(buf, "%s\t%s\t%s", apobuf, complexbuf, ligbuf) ;

			if(status < 3) {
				fprintf(stderr, "! Skipping row '%s' with bad format (status %d).\n", buf, status) ;
			}
			else {
				nread += add_prot(apobuf, complexbuf, ligbuf, par) ;
			}
		}
	}
	else {
		fprintf(stderr, "! File %s doesn't exists\n", str_list_file) ;
	}

// 	print_params(par, stdout) ;

	return nread ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int add_prot(char *apo, char *complex, char *ligan, s_tparams *par) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Add a set of data to the list of set of data in the parameters. this function
	is used for the tpocket program only.
	
	The function will try to open each file, and data will be stored only if the
	two files exists, and if the name of the ligand is valid.
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *apo: The apo path
	@ char *complex: The complex path
	@ char *ligan: The ligand resname: a 4 letter (max!) 
	@ s_tparams *p: The structure than contains parameters.
   -----------------------------------------------------------------------------
   ## RETURN: 
	The file, NULL if the openning fails.
   -----------------------------------------------------------------------------
*/
int add_prot(char *apo, char *complex, char *ligan, s_tparams *par) 
{
	FILE *f = fopen_pdb_check_case(apo, "r") ;
	int nm1, l, i ;
	if(f) {
		fclose(f) ;
		f = fopen_pdb_check_case(complex, "r") ;
		if(f) {
			l = strlen(ligan) ;
			if(ligan && l >= 3) {
				for(i = 0 ; i < l ; i++) ligan[i] = toupper(ligan[i]) ;

				nm1 = par->nfiles ;
				par->nfiles += 1 ;
				par->fapo     = (char**) my_realloc(par->fapo, (par->nfiles)*sizeof(char*)) ;
				par->fligan   = (char**) my_realloc(par->fligan, (par->nfiles)*sizeof(char*)) ;
				par->fcomplex = (char**) my_realloc(par->fcomplex, (par->nfiles)*sizeof(char*)) ;
	
				par->fapo[nm1]     = (char *)my_malloc((strlen(apo)+1)*sizeof(char)) ;
				par->fcomplex[nm1] = (char *)my_malloc((strlen(complex)+1)*sizeof(char)) ;
				par->fligan[nm1]   = (char *)my_malloc((strlen(ligan)+1)*sizeof(char)) ;
	
				strcpy(par->fapo[nm1], apo) ;
				strcpy(par->fcomplex[nm1], complex) ;
				strcpy(par->fligan[nm1], ligan) ;
	
				fclose(f) ;
			}
			else {
				fprintf(stdout, "! The name given for the ligand is invalid or absent.\n") ;
				return 0 ;
			}
			
		}
		else {
			fprintf(stdout, "! The pdb complexe file '%s' doesn't exists.\n", complex) ;
			return 0 ;
		}
	}
	else {
	// If the file does not exists, try with upper case
		fprintf(stdout, "! The pdb apo file '%s' doesn't exists.\n", apo) ;
		return 0 ;
	}

	return 1 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int parse_lig_neigh_dist(char *str, s_fparams *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Parsing function for the distance criteria to find ligand neighbours.
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *str: The string to parse
	@ s_fparams *p: The structure than will contain the parsed parameter
   -----------------------------------------------------------------------------
   ## RETURN: 
	0 if the parameter is valid (here a valid float), 1 if not
   -----------------------------------------------------------------------------
*/
int parse_lig_neigh_dist(char *str, s_tparams *p) 
{
	if(str_is_float(str, M_NO_SIGN)) {
		p->lig_neigh_dist = atof(str) ;
	}
	else {
		fprintf(stdout, "! Invalid value (%s) given for distance criteria to define interface atoms.\n", str) ;
		return 1 ;
	}

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void free_tparams(s_tparams *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Free parameters
   -----------------------------------------------------------------------------
   ## PARAMETRES: 
	@ s_tparams *p: Pointer to the structure to free
   -----------------------------------------------------------------------------
   ## RETURN: 
	void
   -----------------------------------------------------------------------------
*/
void free_tparams(s_tparams *p)
{
	if(p) {
		if(p->fapo) {
			my_free(p->fapo) ;
			p->fapo = NULL ;
		}

		if(p->fligan) {
			my_free(p->fligan) ;
			p->fligan = NULL ;
		}

		if(p->fcomplex) {
			my_free(p->fcomplex) ;
			p->fcomplex = NULL ;
		}

		if(p->fpar) {
			free_fparams(p->fpar) ;
			p->fpar = NULL ;
		}

 		my_free(p) ;
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void print_test_usage(FILE *f) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Displaying usage of the programm in the given buffer
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ FILE *f: buffer to print in
   -----------------------------------------------------------------------------
   ## RETURN: 
   -----------------------------------------------------------------------------
*/
void print_test_usage(FILE *f)
{
	f = (f == NULL) ? stdout:f ;

	fprintf(f, "\n-------------------\nUSAGE (tpocket):\n") ;
	fprintf(f, "1 - To launch a test with a list of proteins to test:\n") ;
	fprintf(f, "\t./bin/tpocket -L file_list\n\n") ;
	fprintf(f, "2 - To launch a test on one single protein:\n") ;
	fprintf(f, "\t./bin/tpocket -a apo_pdb -c complex_pdb -l ligan_pdb_name\n\n") ;
	fprintf(f, "Options: \n") ;
	fprintf(f, "\t-o string  : Write pocket detailed statistics to this file. (stats_p.txt \n") ;
	fprintf(f, "\t             by default).\n") ;
	fprintf(f, "\t-e string  : Write global pocket statistics to this file. (stats_g.txt \n") ;
	fprintf(f, "\t             by default).\n") ;
	fprintf(f, "\t-d (float)  : Radius to use for searching ligand's neighbours in the test. (3.5) \n") ;	

	fprintf(f, "\nOptions specific to fpocket: (find standard parameters in brackets)\n") ;
	fprintf(f, "\t-m (float)  : Minimum radius of an alpha-sphere. (2.8) \n") ;
	fprintf(f, "\t-M (float)  : Maximum radius of an alpha-sphere. (6.5)\n") ;
	fprintf(f, "\t-A (int)    : Minimum number of apolar neighbor for an a-sphere to be considered as apolar. (3) \n") ;
	fprintf(f, "\t-i (int)    : Minimum number of a-sphere per pocket. (3)\n") ;
	fprintf(f, "\t-D (float)  : Maximum distance for first clustering algorithm. (1.2)\n") ;
	fprintf(f, "\t-s (float)  : Maximum distance for single linkage clustering. (1.5)\n") ;
	fprintf(f, "\t-n (integer): Minimum number of neighbor close from each other for single linkage clustering. (4)\n") ;
	fprintf(f, "\t-r (float)  : Maximum distance between two pocket barycenter for refine algorithm. (4.0)\n") ;
	fprintf(f, "\t-p (float)  : Minimum proportion of apolar sphere in a pocket to keep it. (0.0)\n") ;
	fprintf(f, "\t-v (integer): Number of Monte-Carlo iteration for the calculation of each pocket volume. (2500)\n") ;
	fprintf(f, "\t-b (integer): Space approximation for the basic method of the volume calculation. (-1)\n") ;
	fprintf(f, "\t              If this option is used, the programm will use this method instead \n") ;
	fprintf(f, "\t              of Monte-Carlo. \n") ;
	fprintf(f, "\nSee the manual for mor informations on those parameters.\n") ;
	fprintf(f, "-------------------\n") ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void print_params(s_tparams *p, FILE *f)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Print function
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   -----------------------------------------------------------------------------
   ## RETURN: 
   -----------------------------------------------------------------------------
*/
void print_params(s_tparams *p, FILE *f)
{

	if(p) {
		fprintf(f, "==============\nParameters of the program: \n");
		int i ;
		for(i = 0 ; i < p->nfiles ; i++) {
			fprintf(f, "> Protein %d: '%s', '%s', '%s'\n", i+1, p->fapo[i], p->fcomplex[i], p->fligan[i]) ;
		}
		fprintf(f, "> Minimum alpha sphere radius: %f\n", p->fpar->asph_min_size);
		fprintf(f, "> Maximum alpha sphere radius: %f\n", p->fpar->asph_max_size);
		fprintf(f, "> Minimum number of apolar neighbor: %d\n", p->fpar->min_apol_neigh);
		fprintf(f, "> Maximum distance for first clustering algorithm: %f \n", p->fpar->clust_max_dist) ;
		fprintf(f, "> Single linkage clustering distance: %f\n", p->fpar->sl_clust_max_dist);
		fprintf(f, "> Single linkage clustering neighbor: %d\n", p->fpar->sl_clust_min_nneigh);
		fprintf(f, "> Refine clustering distance: %f\n", p->fpar->refine_clust_dist);
		fprintf(f, "> Min number of apolar sphere in refine to keep a pocket: %f\n", p->fpar->refine_min_apolar_asphere_prop) ;
		fprintf(f, "> Monte carlo iterations: %d\n", p->fpar->nb_mcv_iter);
		fprintf(f, "> Basic method for volume calculation: %d\n", p->fpar->basic_volume_div);
		fprintf(f, "==============\n");
	}
	else fprintf(f, "> No parameters detected\n");
}
