 
#include "../headers/dparams.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					dparams.h
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## ----- SPECIFICATIONS
##
##	Handle parameters (parse the command line and sore values)
##	for the dpocket programm.
##
## ----- MODIFICATIONS HISTORY
##
##	01-04-08	(v)  Added comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
##
##	(v) Check and update if necessary comments of each function!!
##	(v) Review the main function and handle all possible crashes.
##

*/

/**-----------------------------------------------------------------------------
   ## FUNCTION:
	s_dparams* init_def_dparams(void)
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
s_dparams* init_def_dparams(void)
{
	s_dparams *par = my_malloc(sizeof(s_dparams)) ;

	par->f_exp = my_malloc(M_MAX_FILE_NAME_LENGTH*sizeof(char)) ;
	par->f_fpckp = my_malloc(M_MAX_FILE_NAME_LENGTH*sizeof(char)) ;
	par->f_fpcknp = my_malloc(M_MAX_FILE_NAME_LENGTH*sizeof(char)) ;

	strcpy(par->f_exp, M_OUTPUT_FILE1_DEFAULT) ;
	strcpy(par->f_fpckp, M_OUTPUT_FILE2_DEFAULT) ;
	strcpy(par->f_fpcknp, M_OUTPUT_FILE3_DEFAULT) ;

	par->fcomplex = NULL ;
	par->ligs = NULL ;
	par->nfiles = 0 ;
	par->interface_dist_crit = M_VERT_LIG_NEIG_DIST ;
	par->interface_method = M_INTERFACE_METHOD1 ;

	par->fpar = init_def_fparams() ;

	return par ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	s_dparams* get_dpocket_args(int nargs, char **args)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	This function analyse the user's command line and parse it to store parameters
	for the descriptor calculator programm.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ int nargs :  Number of arguments
	@ char **args: Arguments of main program
   -----------------------------------------------------------------------------
   ## RETURN:
	s_dparams*: Pointer to parameters
   -----------------------------------------------------------------------------
*/
s_dparams* get_dpocket_args(int nargs, char **args)
{
	int i,
		status = 0 ;

	int nstats = 0 ;
	
	char *str_list_file = NULL ;

	s_dparams *par = init_def_dparams() ;
	//read arguments by flags
	for (i = 1; i < nargs; i++) {
		if (strlen(args[i]) == 2 && args[i][0] == '-') {
			switch (args[i][1]) {
				case M_PAR_MAX_ASHAPE_SIZE	  : status += parse_asph_max_size(args[++i], par->fpar) ; 
											  break ;
				case M_PAR_MIN_ASHAPE_SIZE	  : status += parse_asph_min_size(args[++i], par->fpar) ; 
											  break ;
				case M_PAR_MIN_APOL_NEIGH	  : status += parse_min_apol_neigh(args[++i], par->fpar) ;
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
			
				case M_DPAR_DISTANCE_CRITERIA : 
						if(i < nargs-1) status += parse_dist_crit(args[++i], par) ;
						else {
							fprintf(stdout, "! Distance criteria defining the protein-ligand interface is missing.\n") ;
							status += 1 ;
						} 
						break ;

				case M_DPAR_INTERFACE_METHOD1 :
						par->interface_method = M_INTERFACE_METHOD1 ;
						par->interface_dist_crit = M_VERT_LIG_NEIG_DIST ;
						//if(i < nargs-1)  status += parse_interface_method(args[++i], par) ; 
						break ;

				case M_DPAR_INTERFACE_METHOD2 :
						par->interface_method = M_INTERFACE_METHOD2 ; 
						par->interface_dist_crit = M_LIG_NEIG_DIST ;
						//if(i < nargs-1) status += parse_interface_method(args[++i], par) ;
						break ;

				case M_DPAR_OUTPUT_FILE : 
						if(nstats >= 1) fprintf(stdout, "! More than one single file for the stats output file has been given. Ignoring this one.\n") ;
						else {
							if(i < nargs-1) {
								if(strlen(args[++i]) < M_MAX_FILE_NAME_LENGTH) {
									remove_ext(args[i]) ;
									sprintf(par->f_exp, "%s_exp.txt", args[i]) ;
									sprintf(par->f_fpckp, "%s_fp.txt", args[i]) ;
									sprintf(par->f_fpcknp, "%s_fpn.txt", args[i]) ;
								}
								else fprintf(stdout, "! Output file name is too long... Keeping default.") ;
							}
							else {
								fprintf(stdout, "! Invalid output file name argument missing.\n") ;
								status += 1 ;
							}
						}
						break ;

				case M_DPAR_INPUT_FILE : 
						if(i < nargs-1) str_list_file = args[++i] ;
						else {
							fprintf(stdout, "! Input file name argument missing.\n") ;
							status += 1 ; 
						}
						 break ;

				default: fprintf(stdout, "> Unknown option '%s'. Ignoring it.\n", args[i]) ; break ;
			}
		}
	}

	if(status > 0) {
		free_dparams(par) ;
		par = NULL ;
 		print_dpocket_usage(stdout);
	}
	else {
		if(str_list_file) {
			int res = add_list_complexes(str_list_file, par) ;
			if(res <= 0) {
				fprintf(stdout, "! No data has been read.\n") ;
				free_dparams(par) ;
				par = NULL ;
 				print_dpocket_usage(stdout);
			}
		}	
		else {
			fprintf(stdout, "! No input file given... Try again :).\n") ;
			free_dparams(par) ;
			par = NULL ;
	 		print_dpocket_usage(stdout);
		}
	}

	return par;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int add_list_complexes(char *str_list_file, s_dparams *par) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Load a list of protein-ligand pdb file path. This file should have the following
	format:

	complex_pdb_file	ligand_code
	complex_pdb_file2	ligand_code2
	complex_pdb_file3	ligand_code3
	(...)
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ char *str_list_file : Path of the file containing all data
	@ s_dparams *par: Structures that stores all thoses files
   -----------------------------------------------------------------------------
   ## RETURN: 
	int: Number of file read.
   -----------------------------------------------------------------------------
*/
int add_list_complexes(char *str_list_file, s_dparams *par) 
{
	FILE *f;
	int n,
		nread = 0,
		status ;

	char buf[M_MAX_PDB_NAME_LEN*2 + 6],
		 complexbuf[M_MAX_PDB_NAME_LEN],
		 ligbuf[5];

	// Loading data.
	f = fopen(str_list_file, "r") ;
	if(f) {
		while(fgets(buf, 210, f)) {
			n = par->nfiles ;
			status = sscanf(buf, "%s\t%s", complexbuf, ligbuf) ;

			if(status < 2) {
				fprintf(stderr, "! Skipping row '%s' with bad format (status %d).\n", buf, status) ;
			}
			else {
				nread += add_complexe(complexbuf, ligbuf, par) ;
			}
		}
	}
	else {
		fprintf(stderr, "! File %s doesn't exists\n", str_list_file) ;
	}

	return nread ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int add_complexe(char *apo, char *complex, char *ligan, s_dparams *par) 
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
	@ s_dparams *p: The structure than contains parameters.
   -----------------------------------------------------------------------------
   ## RETURN: 
	The file, NULL if the openning fails.
   -----------------------------------------------------------------------------
*/
int add_complexe(char *complex, char *ligand, s_dparams *par) 
{
	int nm1, i, l ;

	FILE *f = fopen_pdb_check_case(complex, "r") ;
	if(f) {
		l = strlen(ligand) ;
		if(strlen(ligand) >= 3) {
			for(i = 0 ; i < l ; i++) ligand[i] = toupper(ligand[i]) ;

			nm1 = par->nfiles ;
			par->nfiles += 1 ;

			par->ligs   = (char**) my_realloc(par->ligs, (par->nfiles)*sizeof(char*)) ;
			par->fcomplex = (char**) my_realloc(par->fcomplex, (par->nfiles)*sizeof(char*)) ;
	
			par->fcomplex[nm1] = my_malloc((strlen(complex)+1)*sizeof(char)) ;
			par->ligs[nm1]   = my_malloc((strlen(ligand)+1)*sizeof(char)) ;
	
			strcpy(par->fcomplex[nm1], complex) ;
			strcpy(par->ligs[nm1], ligand) ;
	
			fclose(f) ;
		}
		else {
			fprintf(stdout, "! The name given for the ligand is invalid or absent.\n") ;
			fclose(f) ;
			return 0 ;
		}
		
	}
	else {
		fprintf(stdout, "! The pdb file '%s' doesn't exists.\n", complex) ;
		return 0 ;
	}

	return 1 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int parse_interface_method(char *str, s_fparams *p, int method) 
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
int parse_interface_method(char *str, s_dparams *p) 
{
	if(str_is_float(str, M_NO_SIGN)) {
		p->interface_dist_crit = atof(str) ;
	}
	else {
		fprintf(stdout, "! Invalid value (%s) given for distance criteria to define interface atoms.\n", str) ;
		return 1 ;
	}

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int parse_dist_crit(char *str, s_fparams *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Parsing function for the distance criteria defining the protein-ligand interface
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *str: The string to parse
	@ s_fparams *p: The structure than will contain the parsed parameter
   -----------------------------------------------------------------------------
   ## RETURN: 
	0 if the parameter is valid (here a valid integer), 1 if not
   -----------------------------------------------------------------------------
*/
int parse_dist_crit(char *str, s_dparams *p) 
{
	if(str_is_float(str, M_NO_SIGN)) {
		p->interface_dist_crit = (float) atof(str) ;
	}
	else {
		fprintf(stdout, "! Invalid value (%s) given for the distance criteria defining the protein-ligand interface.\n", str) ;
		return 1 ;
	}

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void print_dparams(s_tparams *p, FILE *f)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Print function
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   -----------------------------------------------------------------------------
   ## RETURN: 
   -----------------------------------------------------------------------------
*/
void print_dparams(s_dparams *p, FILE *f)
{

	if(p) {
		fprintf(f, "==============\nParameters of the program: \n");
		int i ;
		for(i = 0 ; i < p->nfiles ; i++) {
			fprintf(f, "> Protein %d: '%s', '%s'\n", i+1, p->fcomplex[i], p->ligs[i]) ;
		}
		
		if(p->interface_method == M_INTERFACE_METHOD1) fprintf(f, "> Method used to define explicitely the interface atoms: contacted atom by alpha spheres.\n") ; 
		else fprintf(f, "> Method used to define explicitely the interface atoms: ligand's neighbors.\n") ;
		
		fprintf(f, "> Distance used to define explicitely the interface: %f.\n", p->interface_dist_crit) ;
		
		fprintf(f, "==============\n");
	}
	else fprintf(f, "> No parameters detected\n");
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void print_dparams_usage(FILE *f) 
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
void print_dpocket_usage(FILE *f)
{
	f = (f == NULL) ? stdout:f ;

	fprintf(f, "\n-------------------\nUSAGE (dpocket):\n") ;
	fprintf(f, "The program needs as input a file containing at each \n\
				line a pdb path and a ligand code.\n") ;
	fprintf(f, "The format of each line must be: PATH/2fej.pdb LIG.\n\n") ;
	fprintf(f, "The ligand code is the resname of the ligand atoms in \n\
				the pdb file.\n") ;
	fprintf(f, "See the manual for more informations.\n") ;
	fprintf(f, "Example of command using default parameters:\n") ;
	fprintf(f, "\t./bin/dpocket -f file_path\n\n") ;
	fprintf(f, "Options: \n") ;
	fprintf(f, "\t-o string  : Write statistics to this file. (stats.txt \n") ;
	fprintf(f, "\t             by default).\n") ;
	fprintf(f, "\t-e         : Use the first protein-ligand explicit \n") ;
	fprintf(f, "\t             interface definition (default). \n") ;
	fprintf(f, "\t-E         : Use the second protein-ligand explicit \n") ;
	fprintf(f, "\t             interface definition. \n") ;
	fprintf(f, "\t-d float   : Distance criteria for the choosen interface \n") ;
	fprintf(f, "               definition.\n") ;

	fprintf(f, "\nOptions specific to fpocket: (find standard parameters \n") ;
	fprintf(f, "in brackets)\n") ;
	fprintf(f, "\t-m (float)  : Minimum radius of an alpha-sphere. (2.8) \n") ;
	fprintf(f, "\t-M (float)  : Maximum radius of an alpha-sphere. (6.5)\n") ;
	fprintf(f, "\t-A (int)    : Minimum number of apolar neighbor for an \n") ;
	fprintf(f, "\t              a-sphere to be considered as apolar. (3) \n") ;
	fprintf(f, "\t-i (int)    : Minimum number of a-sphere per pocket. (3)\n") ;
	fprintf(f, "\t-D (float)  : Maximum distance for first clustering algorithm. (1.2)\n") ;
	fprintf(f, "\t-s (float)  : Maximum distance for single linkage clustering. (1.5)\n") ;
	fprintf(f, "\t-n (integer): Minimum number of neighbor close from each other \n") ;
	fprintf(f, "\t              for single linkage clustering. (4)\n") ;
	fprintf(f, "\t-r (float)  : Maximum distance between two pocket barycenter for \n") ;
	fprintf(f, "\t              refine algorithm. (4.0)\n") ;
	fprintf(f, "\t-p (float)  : Minimum proportion of apolar sphere in a pocket to \n") ;
	fprintf(f, "\t              keep it. (0.0)\n") ;
	fprintf(f, "\t-v (integer): Number of Monte-Carlo iteration for the calculation \n") ;
	fprintf(f, "\t              of each pocket volume. (2500)\n") ;
	fprintf(f, "\t-b (integer): Space approximation for the basic method of the volume \n") ;
	fprintf(f, "\t              calculation. (-1)\n") ;
	fprintf(f, "\t              If this option is used, the programm will use this method instead \n") ;


	fprintf(f, "\nSee the manual for mor informations on those parameters.\n") ;
	fprintf(f, "-------------------\n") ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void free_params(s_dparams *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Free parameters
   -----------------------------------------------------------------------------
   ## PARAMETRES: 
	@ s_dparams *p: Pointer to the structure to free
   -----------------------------------------------------------------------------
   ## RETURN: 
	void
   -----------------------------------------------------------------------------
*/
void free_dparams(s_dparams *p)
{
	if(p) {
		if(p->ligs) {
			my_free(p->ligs) ;
			p->ligs = NULL ;
		}

		if(p->fcomplex) {
			my_free(p->fcomplex) ;
			p->fcomplex = NULL ;
		}
		
		if(p->f_exp) {
			my_free(p->f_exp) ;
			p->f_exp = NULL ;
		}

		if(p->f_fpckp) {
			my_free(p->f_fpckp) ;
			p->f_fpckp = NULL ;
		}

		if(p->f_fpcknp) {
			my_free(p->f_fpcknp) ;
			p->f_fpcknp = NULL ;
		}

		free_fparams(p->fpar) ;

 		my_free(p) ;
	}
}
