 
#include "../headers/dpmain.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					dpmain.h
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## ----- SPECIFICATIONS
##
##	Top function to call dpocket routines. Get programm parameters,
##	call function and free memory.
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
	Main program for dpocket!
   -----------------------------------------------------------------------------
*/
int main(int argc, char *argv[])
{
	s_dparams *par = get_dpocket_args(argc, argv) ;
	print_dparams(par, stdout) ;

	dpocket(par) ;

	free_dparams(par) ;
  	free_all() ;

	return 0 ;
}

