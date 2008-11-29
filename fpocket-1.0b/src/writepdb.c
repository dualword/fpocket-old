
#include "../headers/writepdb.h"
/**

## ----- GENERAL INFORMATION
##
## FILE 					writepdb.c
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
	void write_pdb_atom_line(FILE *f, s_atm *a, int aid) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Write an atom in the following pdb format 2.3.
	
	COLUMNS      DATA TYPE        FIELD      DEFINITION
	------------------------------------------------------
	1 -  6      Record name      "ATOM    "
	7 - 11      Integer          serial     Atom serial number.
	13 - 16      Atom             name       Atom name.
	17           Character        altLoc     Alternate location indicator.
	18 - 20      Residue name     resName    Residue name.
	22           Character        chainID    Chain identifier.
	23 - 26      Integer          resSeq     Residue sequence number.
	27           AChar            iCode      Code for insertion of residues.
	31 - 38      Real(8.3)        x          Orthogonal coordinates for X in 
											 Angstroms
	39 - 46      Real(8.3)        y          Orthogonal coordinates for Y in 
											 Angstroms
	47 - 54      Real(8.3)        z          Orthogonal coordinates for Z in 
											 Angstroms
	55 - 60      Real(6.2)        occupancy  Occupancy.
	61 - 66      Real(6.2)        tempFactor Temperature factor.
	77 - 78      LString(2)       element    Element symbol, right-justified.
	79 - 80      LString(2)       charge     Charge on the atom.

   -----------------------------------------------------------------------------
   ## PARAMETRES:
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
void write_pdb_atom_line(FILE *f, const char rec_name[], int id, const char atom_name[], 
						 char alt_loc, const char res_name[], const char chain[], 
						 int res_id, const char insert, float x, float y, float z, float occ, 
						 float bfactor,	const char *symbol, int charge) 
{
	// Example of pdb record:
	// Position:          1         2         3         4         5         6         7         8
	// Position: 12345678901234567890123456789012345678901234567890123456789012345678901234567890
	// Record:   ATOM    145  N   VAL A  25      32.433  16.336  57.540  1.00 11.92           N

	int status = 0 ;
	char id_buf[6] = "*****",
		 res_id_buf[5] = "****",
		 charge_buf[3] = "  ";
	
	if (id < 100000) sprintf(id_buf, "%5d", id);
	else sprintf(id_buf, "%05x", id);

	if (res_id < 10000) sprintf(res_id_buf, "%4d", res_id);
	else if (res_id < 65536) sprintf(res_id_buf, "%04x", res_id);
	else sprintf(res_id_buf, "****");

	alt_loc = (alt_loc == '\0')? ' ': alt_loc;

	if(charge == -1) sprintf(charge_buf, "  ") ;
	else sprintf(charge_buf, "%2d", charge) ;

 	status = fprintf(f, "%-6s%5s %4s%c%-4s%c%4s%c   %8.3f%8.3f%8.3f%6.2f%6.2f          %2s%2s\n",
 						 rec_name, id_buf, atom_name, alt_loc, res_name, chain[0], 
 						 res_id_buf, insert, x, y, z, occ, bfactor, symbol, charge_buf);
	
}


/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void write_pqr_atom_line(FILE *f, const char *rec_name, int id, const char *atom_name, 
						 char alt_loc, const char *res_name, const char *chain, 
						 int res_id, const char insert, float x, float y, float z, float charge, 
						 float radius)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Write an atom in pqr format.
	
	COLUMNS      DATA TYPE        FIELD      DEFINITION
	------------------------------------------------------
	1 -  6      Record name      "ATOM    "
	7 - 11      Integer          serial     Atom serial number.
	13 - 16      Atom             name       Atom name.
	17           Character        altLoc     Alternate location indicator.
	18 - 20      Residue name     resName    Residue name.
	22           Character        chainID    Chain identifier.
	23 - 26      Integer          resSeq     Residue sequence number.
	27           AChar            iCode      Code for insertion of residues.
	31 - 38      Real(8.3)        x          Orthogonal coordinates for X in 
											 Angstroms
	39 - 46      Real(8.3)        y          Orthogonal coordinates for Y in 
											 Angstroms
	47 - 54      Real(8.3)        z          Orthogonal coordinates for Z in 
											 Angstroms
						 charge
						 vdw radius
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
void write_pqr_atom_line(FILE *f, const char *rec_name, int id, const char *atom_name, 
						 char alt_loc, const char *res_name, const char *chain, 
						 int res_id, const char insert, float x, float y, float z, float charge, 
						 float radius) 
{
	// Example of pdb record:
	// Position:          1         2         3         4         5         6         7         8
	// Position: 12345678901234567890123456789012345678901234567890123456789012345678901234567890
	// Record:   ATOM    145  N   VAL A  25      32.433  16.336  57.540  1.00 11.92           N

	int status ;
 	char id_buf[7],
 		 res_id_buf[6];
// 		 charge_buf[3] ;
 	
 	if (id < 100000) sprintf(id_buf, "%5d", id);
 	else sprintf(id_buf, "%05x", id);

 	if (res_id < 10000) sprintf(res_id_buf, "%4d", res_id);
 	else if (res_id < 65536) sprintf(res_id_buf, "%04x", res_id);
 	
 	alt_loc = (alt_loc == '\0')? ' ': alt_loc;
 	
// 	if(charge == -1) {
// 		charge_buf[0] = charge_buf[1] = ' ' ;
// 		charge_buf[2] = '\0' ;
// 	}
// 	else sprintf(charge_buf, "%2d", charge) ;
 	
 	status = fprintf(f, "%-6s%5s %4s%c%-4s%c%4s%c   %8.3f%8.3f%8.3f  %6.2f   %6.2f\n",
 						 rec_name, id_buf, atom_name, alt_loc, res_name, chain[0], 
 						 res_id_buf, insert, x, y, z, charge,radius) ;
}

