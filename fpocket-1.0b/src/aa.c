#include "../headers/aa.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 				aa.h
## AUTHORS				P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## ----- SPECIFICATIONS
##
##	This file contains severals functions that allow one to
##	deal with amino-acids and there properties. Properties
##	should be stored in the static variable ST_aa, that
##	contains for each amino-acids, several properties
##	stored in a specific structure.
##
## ----- MODIFICATIONS HISTORY
##
##	01-04-08	(v)  Added comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
##
##	(v) Get more revelant descriptors, namely for the volume, 
##		polarity and charge
##	(v) Check and update if necessary comments of each function!!

*/

/**
	Several amino-acid properties, taken from:
	http://www.info.univ-angers.fr/~gh/Idas/proprietes.htm

	May be updated, because it seems to be a huge approximation... Moreover,
	no citations are given!
	Previous tab:

	{ "ALA", 'A', 2.0,  1.0,  0, 0, 2 },
	{ "CYS", 'C', 3.0,  1.0,  0, 0, 6 },
	{ "ASP", 'D', 3.0, -2.0, -1, 1, 3 },
	{ "GLU", 'E', 4.0, -2.0, -1, 1, 3 },
	{ "PHE", 'F', 6.0,  1.0,  0, 0, 1 },
	{ "GLY", 'G', 1.0,  0.0,  0, 0, 2 },
	{ "HIS", 'H', 4.0, -2.0,  1, 1, 1 },
	{ "ILE", 'I', 5.0,  3.0,  0, 0, 2 },
	{ "LYS", 'K', 6.0, -2.0,  1, 1, 5 },
	{ "LEU", 'L', 5.0,  2.0,  0, 0, 2 },
	{ "MET", 'M', 5.0,  1.0,  0, 0, 5 },
	{ "ASN", 'N', 3.0, -2.0,  0, 1, 3 },
	{ "PRO", 'P', 3.0, -1.0,  0, 0, 2 },
	{ "GLN", 'Q', 4.0, -2.0,  0, 1, 3 },
	{ "ARG", 'R', 7.0, -2.0,  1, 1, 5 },
	{ "SER", 'S', 2.0, -1.0,  0, 1, 4 },
	{ "THR", 'T', 3.0, -1.0,  0, 1, 4 },
	{ "VAL", 'V', 4.0,  3.0,  0, 0, 2 },
	{ "TRP", 'W', 8.0,  1.0,  0, 1, 1 },
	{ "TYR", 'Y', 7.0, -1.0,  0, 1, 1 }
	
	------ 

	Here is another one for the hydrophobicity:
	http://www.sigmaaldrich.com/Area_of_Interest/Biochemicals/PolyAmino_Acids/Reference_Chart.html

	This one seems to be better, and there is the reference:
		Monera & al. Journal of Protein Science 1, 319-329 (1995)

	This the current relative hydrophobicity index used.

 
*/
static const s_amino_a ST_aa[20] = 
{
	// Name Code Volume Hydrophobicity charge polatity func_grp
	{ "ALA", 'A', 2.0,  41.0,  0, 0, 2 },
	{ "CYS", 'C', 3.0,  49.0,  0, 0, 6 },
	{ "ASP", 'D', 3.0, -55.0, -1, 1, 3 },
	{ "GLU", 'E', 4.0, -31.0, -1, 1, 3 },
	{ "PHE", 'F', 6.0, 100.0,  0, 0, 1 },
	{ "GLY", 'G', 1.0,   0.0,  0, 0, 2 },
	{ "HIS", 'H', 4.0,   8.0,  1, 1, 1 },
	{ "ILE", 'I', 5.0,  99.0,  0, 0, 2 },
	{ "LYS", 'K', 6.0, -23.0,  1, 1, 5 },
	{ "LEU", 'L', 5.0,  97.0,  0, 0, 2 },
	{ "MET", 'M', 5.0,  74.0,  0, 0, 5 },
	{ "ASN", 'N', 3.0, -28.0,  0, 1, 3 },
	{ "PRO", 'P', 3.0, -46.0,  0, 0, 2 },
	{ "GLN", 'Q', 4.0, -10.0,  0, 1, 3 },
	{ "ARG", 'R', 7.0, -14.0,  1, 1, 5 },
	{ "SER", 'S', 2.0,  -5.0,  0, 1, 4 },
	{ "THR", 'T', 3.0,  13.0,  0, 1, 4 },
	{ "VAL", 'V', 4.0,  76.0,  0, 0, 2 },
	{ "TRP", 'W', 8.0,  97.0,  0, 1, 1 },
	{ "TYR", 'Y', 7.0,  63.0,  0, 1, 1 }
} ;

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	char* get_aa_name3(const int index) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Return the name of AA given in argument (index in the static table)
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ const int index: Index of the AA in the tab
   -----------------------------------------------------------------------------
   ## RETURN:
	Name if index is valid, NULL if not.
   -----------------------------------------------------------------------------
*/
char* get_aa_name3(const int index) 
{
	if(index < M_NB_AA && index >= 0) {
		return ST_aa[index].name3 ;
	}
	return NULL ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void get_aa_index(const char *name) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Return the index of AA given in argument (3letter code representation) in the
	static AA tab.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	const char *name: Amno acid name (3 letter code representation)
   -----------------------------------------------------------------------------
   ## RETURN:
	index of the given amino acid, -1 if not found in the tab
   -----------------------------------------------------------------------------
*/
int get_aa_index(const char *name) 
{
	int i,
		aa_index = -1 ;

	for(i = 0 ; i < M_NB_AA ; i++) {
		if(toupper(name[0]) == ST_aa[i].name3[0] && toupper(name[1]) == ST_aa[i].name3[1] 
		&& toupper(name[2]) == ST_aa[i].name3[2]   ) {
			aa_index = i ;
			break ;
		}
	}

	return aa_index ;
}


/************** Getting informations from an AA name in the static tab **************/


/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int get_aa_volume_score(const char *name) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Return the volume of given amino acid
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	const char *name: Amno acid name (3 letter code representation)
   -----------------------------------------------------------------------------
   ## RETURN:
	volume score, -1 if aa not found in the tab
   -----------------------------------------------------------------------------
*/
float get_aa_volume_score(const char *name) 
{
	int aa_index = get_aa_index(name) ;

	if(aa_index != -1) {
		return ST_aa[aa_index].volume ;
	}/*
	else {
		fprintf(stderr, "! Amino acid '%s' could not be found in property table...\n", name);
	}*/

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	float get_aa_hydrophobicity_score(const char *name) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Return the hydrophobicity score of given amino acid
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	const char *name: Amno acid name (3 letter code representation)
   -----------------------------------------------------------------------------
   ## RETURN:
	hydrophobicity score, -1 if aa not found in the tab
   -----------------------------------------------------------------------------
*/
float get_aa_hydrophobicity_score(const char *name) 
{
	int aa_index = get_aa_index(name) ;

	if(aa_index != -1) {
		return ST_aa[aa_index].hydrophobicity ;
	}/*
	else {
		fprintf(stderr, "! Amino acid '%s' could not be found in property table...\n", name);
	}*/

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int get_aa_charge(const char *name) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Return the charge score of given amino acid
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	const char *name: Amno acid name (3 letter code representation)
   -----------------------------------------------------------------------------
   ## RETURN:
	charge (positiv, negativ, neutral, see header for more details), -1 if aa 
	not found in the tab
   -----------------------------------------------------------------------------
*/
int get_aa_charge(const char *name) 
{
	int aa_index = get_aa_index(name) ;

	if(aa_index != -1) {
		return ST_aa[aa_index].charge ;
	}/*
	else {
		fprintf(stderr, "! Amino acid '%s' could not be found in property table...\n", name);
	}*/

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int get_aa_polarity(const char *name) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Return the polarity score of given amino acid
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	const char *name: Amno acid name (3 letter code representation)
   -----------------------------------------------------------------------------
   ## RETURN:
	polarity (polar, apolar), -1 if aa not found in the tab
   -----------------------------------------------------------------------------
*/
int get_aa_polarity(const char *name) 
{
	int aa_index = get_aa_index(name) ;

	if(aa_index != -1) {
		return ST_aa[aa_index].polarity ;
	}/*
	else {
		fprintf(stderr, "! Amino acid '%s' could not be found in property table...\n", name);
	}*/

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int get_func_grp_from_idx(const char *name) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Return the functional group type of the given amino acid
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	const char *name: Amno acid name (3 letter code representation)
   -----------------------------------------------------------------------------
   ## RETURN:
	int: functional group id
   -----------------------------------------------------------------------------
*/
int get_aa_func_grp(const char *name) 
{
	int aa_index = get_aa_index(name) ;

	if(aa_index != -1) {
		return ST_aa[aa_index].func_grp ;
	}/*
	else {
		fprintf(stderr, "! Amino acid '%s' could not be found in property table...\n", name);
	}*/

	return 0 ;
}


/************** Getting informations from an AA index in the static tab **************/


/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int get_volume_score_from_idx(int aa_index) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Return the volume score of given amino acid
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ int aa_index: Index of the amino acid in the tab
   -----------------------------------------------------------------------------
   ## RETURN:
	volume score, -1 if aa not found in the tab
   -----------------------------------------------------------------------------
*/
float get_volume_score_from_idx(int aa_index) 
{
	if(aa_index < M_NB_AA && aa_index >= 0){
		return ST_aa[aa_index].volume ;
	}/*
	else {
		fprintf(stderr, "! Amino acid %d could not be found in property table...\n", aa_index);
	}*/

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	float get_hydrophobicity_score_from_idx(int aa_index) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Return the hydrophobicity score of given amino acid
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ int aa_index: Index of the amino acid in the tab
   -----------------------------------------------------------------------------
   ## RETURN:
	hydrophobicity score, -1 if aa not found in the tab
   -----------------------------------------------------------------------------
*/
float get_hydrophobicity_score_from_idx(int aa_index) 
{
	if(aa_index < M_NB_AA && aa_index >= 0) {
		return ST_aa[aa_index].hydrophobicity ;
	}/*
	else {
		fprintf(stderr, "! Amino acid %d could not be found in property table...\n", aa_index);
	}*/

	return 0.0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int get_charge_from_idx(int aa_index) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Return the charge score of given amino acid
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ int aa_index: Index of the amino acid in the tab
   -----------------------------------------------------------------------------
   ## RETURN:
	charge (positiv, negativ, neutral, see header for more details), -1 if aa 
	not found in the tab
   -----------------------------------------------------------------------------
*/
int get_charge_from_idx(int aa_index) 
{
	if(aa_index < M_NB_AA && aa_index >= 0){
		return ST_aa[aa_index].charge ;
	}/*
	else {
		fprintf(stderr, "! Amino acid %d could not be found in property table...\n", aa_index);
	}*/

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int get_polarity_from_idx(int aa_index) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Return the polarity score of given amino acid
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ int aa_index: Index of the amino acid in the tab
   -----------------------------------------------------------------------------
   ## RETURN:
	int polarity (polar, apolar), -1 if aa not found in the tab
   -----------------------------------------------------------------------------
*/
int get_polarity_from_idx(int aa_index) 
{
	if(aa_index < M_NB_AA && aa_index >= 0) {
		return ST_aa[aa_index].polarity ;
	}/*
	else {
		fprintf(stderr, "! Amino acid %d could not be found in property table...\n", aa_index);
	}*/

	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int get_func_grp_from_idx(const char *name) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Return the functional group type of the given amino acid
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ int aa_index: Index of the amino acid in the tab
   -----------------------------------------------------------------------------
   ## RETURN:
	int: functional group id
   -----------------------------------------------------------------------------
*/
int get_func_grp_from_idx(int aa_index) 
{
	if(aa_index < M_NB_AA && aa_index >= 0) {
		return ST_aa[aa_index].func_grp ;
	}/*
	else {
		fprintf(stderr, "! Amino acid %d could not be found in property table...\n", aa_index);
	}*/

	return 0 ;
}
