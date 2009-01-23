
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
int main(int argc, char** argv)
{
	fprintf(stdout, "\n*** TESTING FPOCKET PACKAGE ***\n") ;
	/* Setting parameters*/
	char args[20][100] = { "-f", "sample/1EED.txt",
						"-m", "3.1",
						"-M", "6.1",
						"-A", "2",
						"-D", "1.5",
						"-s", "4.0",
						"-n", "5",
						"-i", "1000",
						"-r", "10.0",
					    "-p", "0.0001"} ;
	/* Checking */

	fprintf(stdout, "\n*** END OF THE TEST ***\n") ;
	
	return (EXIT_SUCCESS) ;
}

