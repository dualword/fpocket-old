
#include "../headers/tparams.h"

/**

## ----- GENERAL INFORMATION
##
## FILE 					tparams.c
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			28-11-08
##
## ----- SPECIFICATIONS
##
##  Handle and parse parameters for the tpocket program.
##
## ----- MODIFICATIONS HISTORY
##
##	28-11-08	(v)  Comments UTD
##	27-11-08	(v)  Added option to keep fpocket output + minor relooking
##					 Also, fpocket option parsing is now performed exclusively
##					 using fparams.c and the function get_fpocket_args
##	01-04-08	(v)  Added template for comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
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
	par->keep_fpout = 0 ;
	par->lig_neigh_dist = M_LIG_NEIG_DIST ;
	
	return par ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	get_tpocket_args
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	This function analyse the user's command line and parse it to store parameters
	for the test programm.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ int nargs   :  Number of arguments
	@ char **args : Arguments of main program
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
	
	char *str_lig = NULL,
		 *str_complex_file = NULL,
		 *str_apo_file = NULL,
		 *str_list_file = NULL ;

	s_tparams *par = init_def_tparams() ;
	par->fpar = get_fpocket_args(nargs, args) ;
	
	if(!par->fpar) {
		free_tparams(par) ;
		print_test_usage(stdout);
		
		return NULL ;
	}
	
	/* Read arguments by flags */
	for (i = 1; i < nargs; i++) {
		if (strlen(args[i]) == 2 && args[i][0] == '-' && 
			(i < (nargs-1) || args[i][1] != M_PAR_VALID_COMPLEX_FILE) ) {
			
			switch (args[i][1]) {
				case M_PAR_LIG_NEIG_DIST	  : 
					status += parse_lig_neigh_dist(args[++i], par) ; 
					break ;
				case M_PAR_KEEP_FP_OUTPUT     :
					par->keep_fpout = 1 ;
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
						if(nlig >= 1) fprintf(stderr, "! More than one PDB ligand code has been given. Ignoring it.\n") ;
						else {
							str_lig = args[++i] ; nlig++ ; 
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
			if(str_lig && str_apo_file && str_complex_file) {
				if(add_prot(str_apo_file, str_complex_file, str_lig, par) == 0){
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
	add_list_data
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Load a list of apo-complex-ligand file. This file should have the following
	format:
	apo_file complex_file ligan_file
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ char *str_list_file : Path of the file containing all data
	@ s_tparams *par      : Structures that stores all thoses files
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

	/* Loading data. */
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

/* 	print_params(par, stdout) ; */

	return nread ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	add_prot
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Add a set of data to the list of set of data in the parameters. this function
	is used for the tpocket program only.
	
	The function will try to open each file, and data will be stored only if the
	two files exists, and if the name of the ligand is valid.
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *apo     : The apo path
	@ char *complex : The complex path
	@ char *ligan   : The ligand resname: a 4 letter (max!) 
	@ s_tparams *p  : The structure than contains parameters.
   -----------------------------------------------------------------------------
   ## RETURN: 
	int: Flag: 1 = ok, 0 : ko
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
			if(ligan && l >= 2) {
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
	/* If the file does not exists, try with upper case */
		fprintf(stdout, "! The pdb apo file '%s' doesn't exists.\n", apo) ;
		return 0 ;
	}

	return 1 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	parse_lig_neigh_dist
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Parsing function for the distance criteria to find ligand neighbours.
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *str    : The string to parse
	@ s_fparams *p : The structure than will contain the parsed parameter
   -----------------------------------------------------------------------------
   ## RETURN: 
	int: 0 if the parameter is valid (here a valid float), 1 if not
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
	free_tparams
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
	print_test_usage
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Displaying usage of the programm in the given buffer
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ FILE *f: buffer to print in
   -----------------------------------------------------------------------------
   ## RETURN: 
    void
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
	fprintf(f, "\nSee the manual for mor information on those parameters.\n") ;
	fprintf(f, "-------------------\n") ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	print_params
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Print function
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   @ s_tparams *p : Pointer to parameters structure
   @ FILE *f      : Buffer to write in
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
