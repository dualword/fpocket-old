
#include "../headers/utils.h"

/**

## ----- GENERAL INFORMATION
##
## FILE 					utils.c
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

static int ST_is_rand_init = 0 ;	// Says wether we have seeded the generator.

#ifdef MD_USE_GSL	/* GSL */
static gsl_rng *ST_r = NULL ;
#endif				/* /GSL */

/**-----------------------------------------------------------------------------
   ## FONCTION: 
	start_rand_generator()
   -----------------------------------------------------------------------------
   ## RETURN: 
	void
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Initialize generator. This initialisation depends on the library to use.
   -----------------------------------------------------------------------------
*/
void start_rand_generator(void) 
{
	if(ST_is_rand_init == 0) {
	// On vérifie si il y a eu un précédent générateur et qu'il a bien été arrété

	#ifdef MD_USE_GSL	/* GSL */
// 		fprintf(stdout, "> GSL generator used\n");
		if(ST_r != NULL) {
		// Si il n'a pas été arrété on libère la mémoire
			gsl_rng_free(ST_r);
		}

		// Initialisation de la graine
		gsl_rng_default_seed = time(NULL);

		// Initialisation des variables statiques
		const gsl_rng_type *T = M_GEN_MTWISTER ;
		ST_r = gsl_rng_alloc(T);
		gsl_rng_set (ST_r, gsl_rng_default_seed);

	#else				/* /GSL */

// 		fprintf(stdout, "> Standard C generator used\n");
		srand((int)time(NULL));

	#endif
		ST_is_rand_init = 1 ;
	}
}


/**-----------------------------------------------------------------------------
   ## FONCTION: 
	rand_uniform()
   -----------------------------------------------------------------------------
   ## RETURN: 
	double
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Generate a random number between 0 and 1 using a uniform distribution.
   -----------------------------------------------------------------------------
*/
float rand_uniform(float min, float max)
{
	if(ST_is_rand_init == 0){
		start_rand_generator() ;
	}

	// Génération du nb aléatoire
	float rnd = 0.0 ;	

	#ifdef MD_USE_GSL	/* GSL */
		rnd = gsl_rng_uniform(ST_r) * (max-min) ;
	#else				/* /GSL */
   		rnd = ((float)rand()/(float)RAND_MAX) * (max-min) ;
	#endif

	return min+rnd ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	my_fopen(char *fpath, char *mode)
   -----------------------------------------------------------------------------
   ## RETURN: 
	FILE* : Un pointeur sur le descripteur de fichier.	
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ '*fpath' : Un pointeur sur le chemin du fichier a ouvrir
	@ '*mode'  : Le mode d'ouverture du fichier
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Test the existance of a file, open it if it does, and print an error of it
	doesn't, asking the user to exit or not.
   -----------------------------------------------------------------------------
*/

FILE* my_fopen(const char *fpath, const char *mode)
{
	FILE *f = fopen(fpath, mode) ;

	if(!f) {
		fprintf(stderr, "! The file <%s> couldn't be opened.\n", fpath);
		exit(1) ;
	}
  
	return f ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	 tab_str* f_readl(const char fpath[], int nchar_max) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Read file line and store them in a tab_str structure. The function skip empty
	lines.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ const char *fpath: Full path of the file.
	@ int nchar_max: A number giving the max number of caractere in each line.
   -----------------------------------------------------------------------------
   ## RETURN: 
	tab_str* : Pointer to the sab_str structure containing lines of the file.
   -----------------------------------------------------------------------------
*/
tab_str* f_readl(const char *fpath, int nchar_max) 
{
// --- Variable declaration

	FILE *f ;
	int i, n,
		nb_string ;
	char *cline,
		 **f_lines ;
	tab_str *lines ;

// --- Variable initialisation

	i = nb_string = 0 ;
	cline = (char *) my_malloc(nchar_max*sizeof(char)) ;

// --- How many lines is there in the file?

 	f = my_fopen(fpath, "r") ;
	while(fgets(cline, nchar_max, f) != NULL) {
		if(strcmp("\n", cline) != 0) {
			nb_string ++ ;
		}
	}
	fclose(f) ;

// --- Once we have the number of lines, lets allocate memory and get the lines

	f = my_fopen(fpath, "r") ;

	lines = (tab_str *)my_malloc(sizeof(tab_str)) ;
	f_lines = (char **)my_malloc(nb_string*sizeof(char*)) ;
	
// --- Getting lines.

	while(fgets(cline, nchar_max, f) != NULL) {
		if(strcmp("\n", cline) != 0) {
			n = strlen(cline) ;
			if(cline[n-1] == '\n') {
				n -- ;
				cline[n] = '\0' ;
			}
			
			char *line =(char *) my_malloc((n+1)*sizeof(char)) ;
			memcpy (line, cline, n+1);
	
			f_lines[i] = line ;
			i++ ;
		}
	}

	lines->nb_str  = nb_string ;
	lines->t_str = f_lines ;

// --- Free memory and close file

	fclose(f) ;
	my_free(cline);
 
	return lines ;
}
 
/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void free_tab_str(tab_str *tstr)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Free the given structure
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ tab_str* strings : Pointer to the tab_str to print
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void free_tab_str(tab_str *tstr) 
{
	if(tstr) {
		int i ;
		if(tstr->t_str) {
			for(i = 0 ; i < tstr->nb_str ; i++) {
				if(tstr->t_str[i]) {
					my_free(tstr->t_str[i]) ;
					tstr->t_str[i] = NULL ;
				}
				else {
					fprintf(stderr, "! string %d is NULL and canot be freed in free_tab_str().\n", i);
				}
			}
			my_free(tstr->t_str) ;
		}
		else {
			fprintf(stderr, "! t_str is NULL and cannot be freed in free_tab_str().\n");
		}

		my_free(tstr) ;
	}
	else {
		fprintf(stderr, "! Argument NULL in free_tab_str().\n");
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void print_tab_str(tab_str* strings)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Print strings contained in the given tab_str.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ tab_str* strings : Pointer to the tab_str to print
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void print_tab_str(tab_str* strings)
{
	if(strings) {
		int i ;
		char **strs = strings->t_str ;
		
		printf("\n----- String tab: \n");
		for (i = 0 ; i < strings->nb_str ; i++) {
			fprintf(stdout, "<%s>\n", strs[i]) ;
		}
		printf("-----\n") ;
	}
	else {
		fprintf(stderr, "! Argument NULL in print_tab_str().\n");
	}
}


/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int str_is_number(const char *str, const int sign)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Check if the string given in argument is a number.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ char *str: The string to deal with
	@ const int sign: The first caractere is the sign?
   -----------------------------------------------------------------------------
   ## RETURN:
	int: 1 if its a valid number, 0 else
   -----------------------------------------------------------------------------
*/
int str_is_number(const char *str, const int sign)
{
	int ok = 0 ;

	if (str != NULL) {
		const char *p = str ;
		int c = *p ;

		if (sign) {	// Checking the first caractere if the sign has to be taken in account

			if(isdigit (c) || ((c == '+' || c == '-') && str[1] != 0)) {
				ok = 1 ;
			}
			p++;
		}
		else {
			ok = 1 ;
		}

		if (ok) {
			while (*p != 0) {
 				if (!isdigit (*p)) {
					ok = 0;
					break ;
				}
				p++;
			}
		}
	}
	else {
		fprintf(stderr, "! Argument NULL in str_is_number().\n");
	}

	return ok;
}


/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int str_is_float(const char *str, const int sign)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Check if the string given in argument is a valid float.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ char *str: The string to deal with
	@ const int sign: The first caractere is the sign?
   -----------------------------------------------------------------------------
   ## RETURN:
	int:  1 if its a valid float, 0 else
   -----------------------------------------------------------------------------
*/
int str_is_float(const char *str, const int sign)
{
	int ok = 0 ;
	int nb_dot = 0 ;

	if (str != NULL) {
		const char *p = str ;
		int c = *p ;

		if (sign) {	// Checking the first caractere if the sign has to be taken in account

			if(isdigit (c) || ((c == '+' || c == '-') && str[1] != 0)) {
				ok = 1 ;
			}
			p++;
		}
		else {
			ok = 1 ;
		}

		if (ok) {
			while (*p != 0) {
 				if (!isdigit (*p)) {
					if((*p) == '.') {
						nb_dot++ ;
						if(nb_dot > 1) {
							ok = 0;
							break ;
						}
					}
					else {
						ok = 0;
						break ;
					}
				}
				p++;
			}
		}
	}
	else {
		fprintf(stderr, "! Argument NULL in str_is_number().\n");
	}

	return ok;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int in_tab(int *tab, int size, int val)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Check if val is present in tab.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ int *tab: Tab
	@ int size: Size of the tab
	@ int val: Value to check
   -----------------------------------------------------------------------------
   ## RETURN:
	int: 1 if val is in tab, 0 if not
   -----------------------------------------------------------------------------
*/
int in_tab(int *tab, int size, int val)
{
	if(tab) {
		int i ;
		for(i = 0 ; i < size ; i++) {
			if(tab[i] == val) return 1 ;
		}
	}
	
	return 0 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int index_of(int *tab, int size, int val) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Check if val is present in tab and return its index if so
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ int *tab: Tab
	@ int size: Size of the tab
	@ int val: Value to check
   -----------------------------------------------------------------------------
   ## RETURN:
	int: index if val is in tab, -1 if not
   -----------------------------------------------------------------------------
*/
int index_of(int *tab, int size, int val) 
{
	if(tab) {
		int i;
		for(i = 0 ; i < size ; i++) {
			if(val == tab[i]) return i ;
		}
	}
	
	return -1 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void str_trim(char *str) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Remove spaces from given string
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ char *str: String to deal with
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
void str_trim(char *str) 
{
	int i, len;
	
	len = strlen(str);
	while (len > 0 && str[len-1] == ' ') {
		str[len-1] = '\0';
		len--;
	}
	
	while (len > 0 && str[0] == ' ') {
		for (i=0; i < len; i++) str[i] = str[i+1];
		len--;
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: extract_path(char *str, char *dest)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: extract path from a string
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ char *str: String to deal with
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
void extract_path(char *str, char *dest) 
{
	char sav ;	
	char *pstr = str,
		 *last_backsl = NULL ;
	
	while(*pstr) {
	// Advance in the path name while it is possible
		if(*pstr == '/') {
		// If we encounter a '/', save its position
			last_backsl = pstr ;
		}
		pstr ++ ;
	}

	if(last_backsl) {
	// If we have found one '/' at least, copy the path
		sav = *(last_backsl) ;
		(*last_backsl) = '\0' ;

		strcpy(dest, str) ;
		(*last_backsl) = sav ;	
	}
	else {
	// If no '/' has been found, just return a dot as current folder 
		dest[0] = '\0' ;
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: extract_ext(char *str, char *dest) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: get rid of the extension of a string (.pdb eg.)
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ char *str: String to deal with
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
void extract_ext(char *str, char *dest) 
{
	char *pstr = str,
		 *last_dot = NULL ;
	
	while(*pstr) {
	// Advance in the path name while it is possible
		if(*pstr == '.') {
		// If we encounter a '/', save its position
			last_dot = pstr ;
		}
		pstr ++ ;
	}

	if(last_dot) {
		strcpy(dest, last_dot+1) ;
	}
	else {
	// If no '/' has been found, just return a dot as current folder 
		dest[0] = '\0' ;
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: remove_path(char *str)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: remove the path from a string
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ char *str: String to deal with
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
void remove_path(char *str) 
{
	int i, filelen ;
	char *pstr = str,
		 *last_backsl = NULL ;
	
	while(*pstr) {
	// Advance in the path name while it is possible
		if(*pstr == '/') {
		// If we encounter a '/', save its position
			last_backsl = pstr ;
		}
		pstr = pstr + 1 ;
	}

	if(last_backsl) {
	// If we have found one '/' at least, copy the path, else dont do anything
		last_backsl = last_backsl + 1 ;
		filelen = strlen(last_backsl) ;
		for(i = 0 ; i < filelen ; i++) {
			str[i] = *(last_backsl+i) ;
		}
		str[i] = '\0' ;
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ char *str: String to deal with
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
void remove_ext(char *str)
{
	char *pstr = str,
		 *last_dot = NULL ;
	
	while(*pstr) {
	// Advance in the path name while it is possible
		if(*pstr == '.') {
		// If we encounter a '/', save its position
			last_dot = pstr ;
		}
		pstr ++ ;
	}

	if(last_dot) {
		*last_dot = '\0' ;
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	FILE* fopen_pdb_check_case(char *str, const char *mode) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 	
	Try to open a pdb file. If the open failed, put the 4 letter before extention
	at the lower case and try again.
	This function assume that the file name has the format path/file.pdb !
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ char *name: The string to parse
	@ s_fparams *p: The structure than will contain the parsed parameter
   -----------------------------------------------------------------------------
   ## RETURN: 
	The file, NULL if the openning fails.
   -----------------------------------------------------------------------------
*/
FILE* fopen_pdb_check_case(char *name, const char *mode) 
{
	FILE *f = fopen(name, mode) ;
	if(!f) {
		int len = strlen(name) ;
		name[len-5] = toupper(name[len-5]);
		name[len-6] = toupper(name[len-6]);
		name[len-7] = toupper(name[len-7]);
		name[len-8] = toupper(name[len-8]);

		f = fopen(name, mode) ;
	}

	return f ;
	
}
