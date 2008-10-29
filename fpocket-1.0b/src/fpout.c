
#include "../headers/fpout.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					fpout.h
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## ----- SPECIFICATIONS
##
##	Write output for fpocket.
##
## ----- MODIFICATIONS HISTORY
##
##	01-04-08	(v)  Added comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
##
##	(v) Clean!

*/


/**-----------------------------------------------------------------------------
   ## FUNCTION:
	void write_out(c_lst_pockets *pockets) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Output routine.
   -----------------------------------------------------------------------------
*/
void write_out_fpocket(c_lst_pockets *pockets, s_pdb *pdb, s_fparams *params) 
{
	char pdb_code[350] = "" ;
	char pdb_path[350] = "" ;
	char out_path[350] = "" ;
	char pdb_out_path[350] = "" ;
	char fout[350] = "" ;
	char command[370] = "" ;

	if(pockets) {
	// Extract path, pdb code...
		strcpy(pdb_code, params->pdb_path) ;
		extract_path(params->pdb_path, pdb_path) ;
		remove_ext(pdb_code) ;
		remove_path(pdb_code) ;
		sprintf(out_path, "%s/%s_out", pdb_path, pdb_code) ;
		sprintf(command, "mkdir %s", out_path) ;
		system(command) ;
		sprintf(out_path, "%s/%s_out/%s", pdb_path, pdb_code, pdb_code) ;
		sprintf(pdb_out_path, "%s_out.pdb", out_path) ;
	//Write vmd and pymol scripts
		sprintf(fout, "%s_out.pdb", pdb_code) ;
		write_visualization(out_path, fout);	
	// Print the whole pockets informations in a single file
		sprintf(fout, "%s_pockets.info", out_path) ;
		FILE *f = fopen(fout, "w") ;
		if(f) {
			print_pockets(f, pockets) ;
			fclose(f) ;
		}

	// Writing full pdb
		sprintf(pdb_out_path, "%s_out.pdb", out_path) ;

		write_pockets_single_pdb(pdb_out_path, pdb, pockets) ;
	
	// Writing pockets as a single pqr
		sprintf(fout, "%s_pockets.pqr", out_path) ;
		write_pockets_single_pqr(fout, pockets) ;

	// Writing individual pockets pqr

		sprintf(out_path, "%s/%s_out/pockets", pdb_path, pdb_code) ;
		sprintf(command, "mkdir %s", out_path) ;
		system(command) ;

		write_each_pocket(out_path, pockets) ;
	}
}
