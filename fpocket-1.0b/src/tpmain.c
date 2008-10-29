 
#include "../headers/tpmain.h"
/**

## ----- GENERAL INFORMATIONS
##
## FILE 					tpmain.c
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
	int main(int argc, char *argv[])
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Main program!
   -----------------------------------------------------------------------------
*/
int main(int argc, char *argv[])
{
	s_tparams *par = get_tpocket_args(argc, argv) ;
	test_fpocket(par) ;
	free_tparams(par) ;
  	free_all() ;

	return 0 ;
}

