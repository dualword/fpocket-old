 
#include "../headers/fparams.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					fparams.h
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## ----- SPECIFICATIONS
##
##	Handle parameters (parse the command line and sore values)
##	for the fpocket programm.
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
	s_fparams* init_def_fparams(void)
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
s_fparams* init_def_fparams(void)
{
	s_fparams *par = my_malloc(sizeof(s_fparams)) ;

	par->min_apol_neigh = M__MIN_APOL_NEIGH_DEFAULT ;
	par->asph_min_size = M_MIN_ASHAPE_SIZE_DEFAULT ;
	par->asph_max_size = M_MAX_ASHAPE_SIZE_DEFAULT ;
	par->sl_clust_max_dist = M_SLCLUST_MAX_DIST ;
	par->sl_clust_min_nneigh = M_SLCLUST_MIN_NUM_NEIGH ;
	par->pdb_path[0] = 0 ;
	par->basic_volume_div = M_BASIC_VOL_DIVISION ;
	par->nb_mcv_iter = M_MC_ITER ;
	par->min_pock_nb_asph = M_MIN_POCK_NB_ASPH ;
	par->refine_clust_dist = M_REFINE_DIST ;
	par->refine_min_apolar_asphere_prop = M_REFINE_MIN_PROP_APOL_AS ;
	par->clust_max_dist = M_CLUST_MAX_DIST ;

	return par ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	s_params* get_fpocket_args(int nargs, char **args)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	This function analyse the user's command line and parse it to store parameters
	for the pocket finder programm.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ int nargs :  Number of arguments
	@ char **args: Arguments of main program
   -----------------------------------------------------------------------------
   ## RETURN: 
	s_params*: Pointer to parameters
   -----------------------------------------------------------------------------
*/
s_fparams* get_fpocket_args(int nargs, char **args)
{
	int i,
		status = 0 ;

	int npdb = 0;
	
	char *str_pdb_file = NULL;

	s_fparams *par = init_def_fparams() ;

	//read arguments by flags
	for (i = 1; i < nargs; i++) {
		if (strlen(args[i]) == 2 && args[i][0] == '-' && i < (nargs-1)) {
			switch (args[i][1]) {
				case M_PAR_MAX_ASHAPE_SIZE	  : status += parse_asph_max_size(args[++i], par) ; 
											  break ;
				case M_PAR_MIN_ASHAPE_SIZE	  : status += parse_asph_min_size(args[++i], par) ; 
											  break ;
				case M_PAR_MIN_APOL_NEIGH	  : status += parse_min_apol_neigh(args[++i], par) ;
											  break ;
				case M_PAR_CLUST_MAX_DIST	  : status += parse_clust_max_dist(args[++i], par) ; 
											  break ;
				case M_PAR_SL_MAX_DIST		  : status += parse_sclust_max_dist(args[++i], par) ; 
											  break ;
				case M_PAR_SL_MIN_NUM_NEIGH   : status += parse_sclust_min_nneigh(args[++i], par) ; 
											  break ;
				case M_PAR_MC_ITER 			  : status += parse_mc_niter(args[++i], par) ; 
											  break ;
				case M_PAR_BASIC_VOL_DIVISION : status += parse_basic_vol_div(args[++i], par) ; 
											  break ;
				case M_PAR_MIN_POCK_NB_ASPH   : status += parse_min_pock_nb_asph(args[++i], par) ; 
											  break ;
				case M_PAR_REFINE_DIST		  : status += parse_refine_dist(args[++i], par) ; 
											  break ;
				case M_PAR_REFINE_MIN_NAPOL_AS: status += parse_refine_min_apolar_asphere_prop(args[++i], par) ; 
											  break ;
				case M_PAR_PDB_FILE			  : 
						if(npdb >= 1) fprintf(stderr, "! More than one single pdb has been given. Ignoring the rest.\n") ;
						else {
							str_pdb_file = args[++i] ; npdb++ ;
						}
						break ;
				default: printf("> Unknown option '%s'. Ignoring it.\n", args[i]) ; break ;
			}
		}
	}
	if(status > 0) {
 		free_fparams(par) ;
		par = NULL ;
		print_pocket_usage(stdout);
	}
	else {
		if(str_pdb_file) {
			int len = strlen(str_pdb_file) ;
			if(len < M_MAX_PDB_NAME_LEN) {
				strcpy(par->pdb_path, str_pdb_file)	;
			}
			else {
				fprintf(stderr, "! Maximum number of caracters (%d) for the pdb names has been reached...\n", M_MAX_PDB_NAME_LEN) ;
				free_fparams(par) ;
				par = NULL ;
			}
		}
		else {
			fprintf(stderr, "! No pdb file given!\n") ;
			free_fparams(par) ;
			par = NULL ;
			print_pocket_usage(stdout);
		}
	}

	return par;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int parse_clust_max_dist(char *str, s_fparams *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Parsing function for the distance criteria first clustering algorithm.
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *str: The string to parse
	@ s_fparams *p: The structure than will contain the parsed parameter
   -----------------------------------------------------------------------------
   ## RETURN: 
	0 if the parameter is valid (here a valid float), 1 if not
   -----------------------------------------------------------------------------
*/
int parse_clust_max_dist(char *str, s_fparams *p) 
{
	if(str_is_float(str, M_NO_SIGN)) {
		p->clust_max_dist = atof(str) ;
	}
	else {
		fprintf(stdout, "! Invalid value (%s) given for the single linkage max dist.\n", str) ;
		return 1 ;
	}

	return 0 ;
}


/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int parse_sclust_max_dist(char *str, s_fparams *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Parsing function for the distance criteria in the single linkage clustering.
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *str: The string to parse
	@ s_fparams *p: The structure than will contain the parsed parameter
   -----------------------------------------------------------------------------
   ## RETURN: 
	0 if the parameter is valid (here a valid float), 1 if not
   -----------------------------------------------------------------------------
*/
int parse_sclust_max_dist(char *str, s_fparams *p) 
{
	if(str_is_float(str, M_NO_SIGN)) {
		p->sl_clust_max_dist = atof(str) ;
	}
	else {
		fprintf(stdout, "! Invalid value (%s) given for the single linkage max dist.\n", str) ;
		return 1 ;
	}

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int parse_sclust_min_nneigh(char *str, s_fparams *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Parsing function for the number of neighbours in the single linkage clustering.
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *str: The string to parse
	@ s_fparams *p: The structure than will contain the parsed parameter
   -----------------------------------------------------------------------------
   ## RETURN: 
	0 if the parameter is valid (here a valid int), 1 if not
   -----------------------------------------------------------------------------
*/
int parse_sclust_min_nneigh(char *str, s_fparams *p) 
{
	if(str_is_number(str, M_NO_SIGN)) {
		p->sl_clust_min_nneigh = atoi(str) ;
	}
	else {
		fprintf(stdout, "! Invalid value (%s) given for the single linkage max dist.\n", str) ;
		return 1 ;
	}

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int parse_min_apol_neigh(char *str, s_fparams *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Parsing function for the minimum number of apolar atom neighbour for an alpha
	sphere to be considered as apolar.
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *str: The string to parse
	@ s_fparams *p: The structure than will contain the parsed parameter
   -----------------------------------------------------------------------------
   ## RETURN: 
	0 if the parameter is valid (here a valid int), 1 if not
   -----------------------------------------------------------------------------
*/
int parse_min_apol_neigh(char *str, s_fparams *p)
{
	if(str_is_number(str, M_NO_SIGN)) {
		p->min_apol_neigh = (int) atoi(str) ;
		if(p->min_apol_neigh < 0) p->min_apol_neigh = 0 ;
		if(p->min_apol_neigh > 4) p->min_apol_neigh = 4 ;
	}
	else {
		fprintf(stdout, "! Invalid value (%s) given for the min radius of alpha shperes.\n", str) ;
		return 1 ;
	}

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int parse_asph_min_size(char *str, s_fparams *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Parsing function for the minimum radius of each alpha shpere
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *str: The string to parse
	@ s_fparams *p: The structure than will contain the parsed parameter
   -----------------------------------------------------------------------------
   ## RETURN: 
	0 if the parameter is valid (here a valid float), 1 if not
   -----------------------------------------------------------------------------
*/
int parse_asph_min_size(char *str, s_fparams *p) 
{
	if(str_is_float(str, M_NO_SIGN)) {
		p->asph_min_size = (float) atof(str) ;
	}
	else {
		fprintf(stdout, "! Invalid value (%s) given for the min radius of alpha shperes.\n", str) ;
		return 1 ;
	}

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int parse_asph_max_size(char *str, s_fparams *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Parsing function for the maximum radius of each alpha shpere
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *str: The string to parse
	@ s_fparams *p: The structure than will contain the parsed parameter
   -----------------------------------------------------------------------------
   ## RETURN: 
	0 if the parameter is valid (here a valid float), 1 if not
   -----------------------------------------------------------------------------
*/
int parse_asph_max_size(char *str, s_fparams *p) 
{
	if(str_is_float(str, M_NO_SIGN)) {
		p->asph_max_size = (float) atof(str) ;
	}
	else {
		fprintf(stdout, "! Invalid value (%s) given for the max radius of alpha shperes.\n", str) ;
		return 1 ;
	}

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int parse_mc_niter(char *str, s_fparams *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Parsing function for the number of iteration for the Monte Carlo volume
	calculation.
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *str: The string to parse
	@ s_fparams *p: The structure than will contain the parsed parameter
   -----------------------------------------------------------------------------
   ## RETURN: 
	0 if the parameter is valid (here a valid float), 1 if not
   -----------------------------------------------------------------------------
*/
int parse_mc_niter(char *str, s_fparams *p)
{
	if(str_is_float(str, M_NO_SIGN)) {
		p->nb_mcv_iter = (int) atoi(str) ;
	}
	else {
		fprintf(stdout, "! Invalid value (%s) given for the number of monte-carlo iteration for the volume.\n", str) ;
		return 1 ;
	}

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int parse_basic_vol_div(char *str, s_fparams *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Parsing function for the number of iteration for the basic volume calculation.
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *str: The string to parse
	@ s_fparams *p: The structure than will contain the parsed parameter
   -----------------------------------------------------------------------------
   ## RETURN: 
	0 if the parameter is valid (here a valid integer), 1 if not
   -----------------------------------------------------------------------------
*/
int parse_basic_vol_div(char *str, s_fparams *p) 
{
	if(str_is_number(str, M_NO_SIGN)) {
		p->basic_volume_div = (int) atoi(str) ;
	}
	else {
		fprintf(stdout, "! Invalid value (%s) given for the precision of the basic volume calculation.\n", str) ;
		return 1 ;
	}

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int parse_refine_dist(char *str, s_fparams *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Parsing function for the distance in the refine algorithm
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *str: The string to parse
	@ s_fparams *p: The structure than will contain the parsed parameter
   -----------------------------------------------------------------------------
   ## RETURN: 
	0 if the parameter is valid (here a valid float), 1 if not
   -----------------------------------------------------------------------------
*/
int parse_refine_dist(char *str, s_fparams *p) 
{
	if(str_is_float(str, M_NO_SIGN)) {
		p->refine_clust_dist = (float) atof(str) ;
	}
	else {
		fprintf(stdout, "! Invalid value (%s) given for the refine distance.\n", str) ;
		return 1 ;
	}

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int parse_refine_min_apol(char *str, s_fparams *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Parsing function for the minimum number of apolar sphere per pocket.
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *str: The string to parse
	@ s_fparams *p: The structure than will contain the parsed parameter
   -----------------------------------------------------------------------------
   ## RETURN: 
	0 if the parameter is valid (here a valid integer), 1 if not
   -----------------------------------------------------------------------------
*/
int parse_refine_min_apolar_asphere_prop(char *str, s_fparams *p) 
{
	if(str_is_float(str, M_NO_SIGN)) {
		p->refine_min_apolar_asphere_prop = (float) atof(str) ;
	}
	else {
		fprintf(stdout, "! Invalid value (%s) given for the refine distance.\n", str) ;
		return 1 ;
	}

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int parse_min_pock_nb_asph(char *str, s_fparams *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Parsing function for the minimum number of alpha sphere per pocket.
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *str: The string to parse
	@ s_fparams *p: The structure than will contain the parsed parameter
   -----------------------------------------------------------------------------
   ## RETURN: 
	0 if the parameter is valid (here a valid integer), 1 if not
   -----------------------------------------------------------------------------
*/
int parse_min_pock_nb_asph(char *str, s_fparams *p) 
{
	if(str_is_number(str, M_NO_SIGN)) {
		p->min_pock_nb_asph = (int) atoi(str) ;
	}
	else {
		fprintf(stdout, "! Invalid value (%s) given for the refine distance.\n", str) ;
		return 1 ;
	}

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void free_fparams(s_params *p) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Free parameters
   -----------------------------------------------------------------------------
   ## PARAMETRES: 
	@ s_params *p: Pointer to the structure to free
   -----------------------------------------------------------------------------
   ## RETURN: 
	void
   -----------------------------------------------------------------------------
*/
void free_fparams(s_fparams *p)
{
	if(p) {
		my_free(p) ;
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void print_pocket_usage(FILE *f) 
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
void print_pocket_usage(FILE *f)
{
	f = (f == NULL) ? stdout:f ;

	fprintf(f, "\n\n-------------------\nUSAGE: \n") ;
	fprintf(f, "Pocket finding on a pdb file: \n") ;
	fprintf(f, "\t./bin/fpocket -f pdb\n\n") ;
	fprintf(f, "Options: (find standard parameters in brackets)\n") ;
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
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void print_fparams(s_params *p, FILE *f)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Print function
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   -----------------------------------------------------------------------------
   ## RETURN: 
   -----------------------------------------------------------------------------
*/
void print_fparams(s_fparams *p, FILE *f)
{
	if(p) {
		fprintf(f, "==============\nParameters of the program: \n");
		fprintf(f, "> Minimum alpha sphere radius: %f\n", p->asph_min_size);
		fprintf(f, "> Maximum alpha sphere radius: %f\n", p->asph_max_size);
		fprintf(f, "> Minimum number of apolar neighbor: %d\n", p->min_apol_neigh);
		fprintf(f, "> Maximum distance for first clustering algorithm: %f \n", p->clust_max_dist) ;
		fprintf(f, "> Single linkage clustering distance: %f\n", p->sl_clust_max_dist);
		fprintf(f, "> Single linkage clustering neighbor: %d\n", p->sl_clust_min_nneigh);
		fprintf(f, "> Refine clustering distance: %f\n", p->refine_clust_dist);
		fprintf(f, "> Min number of apolar sphere in refine to keep a pocket: %f\n", p->refine_min_apolar_asphere_prop) ;
		fprintf(f, "> Monte carlo iterations: %d\n", p->nb_mcv_iter);
		fprintf(f, "> Basic method for volume calculation: %d\n", p->basic_volume_div);
		fprintf(f, "> PDB file: %s\n", p->pdb_path);
		fprintf(f, "==============\n");
	}
	else fprintf(f, "> No parameters detected\n");
}

