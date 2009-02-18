
/**

## ----- GENERAL INFORMATION
##
## FILE 					check.c
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			22-01-09
##
## ----- SPECIFICATIONS
##
##	Perform basic tests for fpocket.
##
## ----- MODIFICATIONS HISTORY
##
##	22-01-09	(v) Created (random date...)
##
## ----- TODO or SUGGESTIONS
##

*/

#include "../headers/check.h"

/**-----------------------------------------------------------------------------
   ## FUNCTION:
	int main(int argc, char *argv[])
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Main program for testing programm.
   -----------------------------------------------------------------------------
*/
int main()
{
	fprintf(stdout, "\n*** TESTING FPOCKET PACKAGE ***\n") ;
	int nfailure = check_fparams() ;
	if(check_qhull() == 0) {
		nfailure += check_fpocket () ;
	}

	fprintf(stdout, "\n*** TESTING ENDS WITH %d FAILURES ***\n", nfailure) ;
	
	return (EXIT_SUCCESS) ;
}

int check_qhull(void)
{
	fprintf(stdout, "\n--> TESTING QHULL INSTALLATION <--\n") ;
	fprintf(stdout, "    IS QVORONOI HERE ............... ") ;
	int status = system("qvoronoi > /dev/null") ;
	if (status != 0) {
		fprintf(stdout, "NO -> FPOCKET CANNOT BE TESTED\n") ;
		status = 1 ;
	}
	else{
		fprintf(stdout, "YES\n") ;
	}

	return status ;
}

int check_fpocket (void)
{
	fprintf(stdout, "\n--> TESTING FPOCKET ALGORITHM <--\n") ;
	/* Setting parameters*/
	int N = 3, i = 0, nfail = 0 ;
	char targs[][100] = {"fpocket", "-f", "sample/3LKF.pdb"} ;

	char **args = my_malloc(N*sizeof(char*)) ;
	for(i = 0 ; i < N ; i++) {
		args[i] = my_malloc(sizeof(char)*strlen(targs[i])) ;
		strcpy(args[i], targs[i]) ;
	}

	/* Checking */
	fprintf(stdout, "    PARSING COMMAND LINE ........... ") ;
	s_fparams *params = get_fpocket_args(N, args) ;
	if(!params) {
		fprintf(stdout, "FAILED \n") ;
		return 1 ;
	}
	fprintf(stdout, "OK \n") ;
	
	fprintf(stdout, "    OPENING PDB FILE................ ") ;
	s_pdb *pdb =  rpdb_open(params->pdb_path, NULL, M_DONT_KEEP_LIG) ;

	if(pdb) {
		/* Actual reading of pdb data and then calculation */
			fprintf(stdout, "OK \n") ;
			fprintf(stdout, "    READING PDB FILE ............... ") ;
			rpdb_read(pdb, NULL, M_DONT_KEEP_LIG) ;
			fprintf(stdout, "OK \n") ;

			fprintf(stdout, "    RUNNING FPOCKET ................ ") ;
			c_lst_pockets *pockets = search_pocket(pdb, params);
			if(pockets && pockets->n_pockets > 0) {
				fprintf(stdout, "OK \n") ;
				fprintf(stdout, "    WRITING FPOCKET OUTPUT ......... ") ;
				write_out_fpocket(pockets, pdb, params->pdb_path);
				fprintf(stdout, "OK \n") ;
				c_lst_pocket_free(pockets) ;
			}
			else {
				nfail++ ;
				fprintf(stdout, "NO POCKET FOUND\n") ;
			}
	}
	else {
		nfail++ ;
		fprintf(stdout, "FAILED (%s) \n", params->pdb_path) ;
	}

	for(i = 0 ; i < N ; i++) my_free(args[i]) ;
	my_free(args) ;
	free_fparams(params) ;

	return nfail ;
}

int check_fparams(void)
{
	fprintf(stdout, "\n--> TESTING FPOCKET PARAMETERS <--\n") ;
	/* Setting parameters*/
	int N = 21 ;
	int i ;
	char targs[][100] = { "fpocket",
						"-f", "sample/1EED.txt",
						"-m", "3.1",
						"-M", "6.1",
						"-A", "2",
						"-D", "1.5",
						"-s", "4.0",
						"-n", "5",
						"-i", "1000",
						"-r", "10.0",
					    "-p", "0.0001"} ;

	char **args = my_malloc(N*sizeof(char*)) ;
	for(i = 0 ; i < N ; i++) {
		args[i] = my_malloc(sizeof(char)*strlen(targs[i])) ;
		strcpy(args[i], targs[i]) ;
	}
	/* Checking */

	fprintf(stdout, "    PARSING COMMAND LINE ........... ") ;
	s_fparams *params = get_fpocket_args(N, (char**)args) ;
	if(!params) {
		fprintf(stdout, "FAILED \n") ;
		return 1 ;
	}
	fprintf(stdout, "OK \n") ;
	
	int nfails = 0 ;
	if(strcmp(args[2], params->pdb_path) == 0)
		fprintf(stdout, "    TEST FPARAM f .................. OK \n") ;
	else {
		nfails ++ ;
		fprintf(stdout, "    TEST FPARAM f .................. FAILED \n") ;
	}
	
	if( params->asph_min_size <= atof(args[4])+0.001
		&& params->asph_min_size >= atof(args[4])-0.001)
		fprintf(stdout, "    TEST FPARAM m .................. OK \n") ;
	else{
		nfails ++ ;
		fprintf(stdout, "    TEST FPARAM m .................. FAILED \n") ;
	}

	if( params->asph_max_size <= atof(args[6])+0.001
		&& params->asph_max_size >= atof(args[6])-0.001)
		fprintf(stdout, "    TEST FPARAM M .................. OK \n") ;
	else{
		nfails ++ ;
		fprintf(stdout, "    TEST FPARAM M .................. FAILED \n") ;
	}

	if(atoi(args[8]) == params->min_apol_neigh)
		fprintf(stdout, "    TEST FPARAM A .................. OK \n") ;
	else{
		nfails ++ ;
		fprintf(stdout, "    TEST FPARAM A .................. FAILED \n") ;
	}

	if( atof(args[10]) <= params->clust_max_dist + 0.001 &&
		atof(args[10]) >= params->clust_max_dist - 0.001)
		fprintf(stdout, "    TEST FPARAM D .................. OK \n") ;
	else{
		nfails ++ ;
		fprintf(stdout, "    TEST FPARAM D .................. FAILED \n") ;
	}

	if( atof(args[12]) <= params->sl_clust_max_dist + 0.001 &&
		atof(args[12]) >= params->sl_clust_max_dist - 0.001)
		fprintf(stdout, "    TEST FPARAM S .................. OK \n") ;
	else{
		nfails ++ ;
		fprintf(stdout, "    TEST FPARAM S .................. FAILED \n") ;
	}

	if(atoi(args[14]) == params->sl_clust_min_nneigh)
		fprintf(stdout, "    TEST FPARAM n .................. OK \n") ;
	else{
		nfails ++ ;
		fprintf(stdout, "    TEST FPARAM n .................. FAILED \n") ;
	}

	if(atoi(args[16]) == params->min_pock_nb_asph)
		fprintf(stdout, "    TEST FPARAM i .................. OK \n") ;
	else{
		nfails ++ ;
		fprintf(stdout, "    TEST FPARAM i .................. FAILED \n") ;
	}

	if( atof(args[18]) <= params->refine_clust_dist + 0.001 &&
		atof(args[18]) >= params->refine_clust_dist - 0.001)
		fprintf(stdout, "    TEST FPARAM r .................. OK \n") ;
	else{
		nfails ++ ;
		fprintf(stdout, "    TEST FPARAM r .................. FAILED \n") ;
	}

	if( atof(args[20]) <= params->refine_min_apolar_asphere_prop + 0.001 &&
		atof(args[20]) >= params->refine_min_apolar_asphere_prop - 0.001)
		fprintf(stdout, "    TEST FPARAM p .................. OK \n") ;
	else{
		nfails ++ ;
		fprintf(stdout, "    TEST FPARAM p .................. FAILED \n") ;
	}
	
	for(i = 0 ; i < N ; i++) my_free(args[i]) ;
	my_free(args) ;

	return nfails ;
}

