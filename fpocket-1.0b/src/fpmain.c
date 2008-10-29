
#include "../headers/fpmain.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					fpmain.h
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## ----- SPECIFICATIONS
##
##	Top function to call fpocket routines. Get and check programm parameters,
##	call functions, write output and free memory.
##
## ----- MODIFICATIONS HISTORY
##
##	01-04-08	(v)  Added comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
##

*/

/**-----------------------------------------------------------------------------
   ## FUNCTION:
	int main(int argc, char *argv[])
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Main program!
   -----------------------------------------------------------------------------
*/
int main(int argc, char *argv[])
{
	fprintf(stdout, "=========== Pocket hunting begins ========== \n") ;
	c_lst_pockets *pockets = NULL ;
	clock_t b, e ;

	s_fparams *params= get_fpocket_args(argc, argv) ;
	/*print_fparams(params, stdout) ;*/

	if(params) {
		/* Load PDB */
		fprintf(stdout,"> Loading pdb data in %s...\n", params->pdb_path);
		b = clock() ;
	 	s_pdb *pdb =  rpdb_open(params->pdb_path, NULL, M_DONT_KEEP_LIG) ;
		
		if(pdb) {
		/* Reading PDB */
			rpdb_read(pdb, NULL, M_DONT_KEEP_LIG) ;
			e = clock() ;
			fprintf(stdout, "> PDB successfully read in %f sec.\n", 
							((double)e - b) / CLOCKS_PER_SEC) ;
			pockets = search_pocket(pdb, params);
			write_out_fpocket(pockets, pdb, params);
		}
		else fprintf(stderr, "!! PDB reading failed!\n");
		
		fprintf(stdout, "> Freeing remainnig memory...\n");
		free_fparams(params) ;
	}
	fprintf(stdout, "=========== Pocket hunting ends ========== \n") ;
	free_all() ;

	return 0;
}
