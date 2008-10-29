
#include "../headers/sort.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					sort.c
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

static void merge_atom_vert(s_vsort *lsort, s_pdb *pdb, s_lst_vvertice *lvert)  ;
static void qsort_dim(s_vect_elem *lst, int len) ;
static void qsort_rec(s_vect_elem *lst, int start, int end) ;
static int partition_x(s_vect_elem *lst, int start, int end) ;
 
/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void get_sorted_list(s_lst_atoms *latoms, s_lst_vvertice *lvert) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	This function will return a lists which will contains all atoms and vertices 
	sorted on x axis.
	First, we merge atom and vertice lists into those a single list. Then they 
	will be sorted using a quickSort algorithm, using the x positions of vertices
	 and atoms as criteria for sorting.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_lst_atoms *latoms: List of atoms
	@ s_lst_vvertice *lvert: List of vertices (if NULL, only atoms will be sorted)
   -----------------------------------------------------------------------------
   ## RETURN:
	   pointer to the structure containing data
   -----------------------------------------------------------------------------
*/
s_vsort* get_sorted_list(s_pdb *pdb, s_lst_vvertice *lvert) 
{
	s_vsort *lsort = my_malloc(sizeof(s_vsort)) ;
	
	lsort->nelem = pdb->natoms ;
	if(lvert) lsort->nelem += lvert->nvert ;

	lsort->xsort = my_malloc((lsort->nelem)*sizeof(s_vect_elem)) ;
	merge_atom_vert(lsort, pdb, lvert) ;
	qsort_dim(lsort->xsort, lsort->nelem) ;
	
	return lsort ;

}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void merge_atom_vert(s_vsort *lsort, s_lst_atoms *latoms, s_lst_vvertice *lvert) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Merge atom and vertice lists into three single list that
	will be sorted next.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_vsort *lsort: Structure that should contains the 3 lists
	@ s_lst_atoms *latoms: List of atoms
	@ s_lst_vvertice *lvert: List of v ertices
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
static void merge_atom_vert(s_vsort *lsort, s_pdb *pdb, s_lst_vvertice *lvert) 
{
	s_vect_elem *cur = NULL ;
	int i,
		pos ;
	int natoms = pdb->natoms ;

	s_atm *atoms = pdb->latoms ;

	for(i = 0 ; i < natoms ; i++) {
		atoms[i].sort_x = i ;
		cur = &(lsort->xsort[i]) ;
		cur->data = &(atoms[i]) ;
		cur->type = M_ATOM_TYPE ;
	}

	if(lvert) {
		for(i = 0 ; i < lvert->nvert ; i++) {
			pos = natoms + i ;
			
			lvert->vertices[i].sort_x = pos ;
			cur = &(lsort->xsort[pos]) ;
			cur->data = &(lvert->vertices[i]) ;
			cur->type = M_VERTICE_TYPE ;
		}
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	static void qsort_dim(s_vect_elem *lst, int len) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_vect_elem *lst: List of vector to sort
	@ int *len: Length of the list
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
static void qsort_dim(s_vect_elem *lst, int len)
{
	qsort_rec(lst, 0,  len-1) ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	static int partition_x(s_vect_elem *lst, int start, int end) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_vect_elem *lst: List of vector to sort
	@ int start: Sort from start
	@ int end: to end
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
static void qsort_rec(s_vect_elem *lst, int start, int end)
{
	if(start < end) {
		int piv = 0 ;
		piv = partition_x(lst, start, end) ;
		
		qsort_rec(lst, start, piv-1) ;
		qsort_rec(lst, piv+1, end) ;
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	static int partition_x(s_vect_elem *lst, int start, int end)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Partition function for the qsort on atoms and vertices, using the X coordinate
	as criteria to sort the list.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_vect_elem *lst: List of vector to sort
	@ int start: Sort from start
	@ int end: to end
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
static int partition_x(s_vect_elem *lst, int start, int end)
{
	s_vect_elem tmp ;
	s_atm *acur      = NULL ;
	s_vvertice *vcur = NULL ;
	
	int c = start,
		i ;
		
	float piv ;	 // TODO: chose randomly the pivot.
	
	piv = (lst[start].type == M_ATOM_TYPE)? ((s_atm*)lst[start].data)->x : ((s_vvertice*)lst[start].data)->x ;
	
	for(i = start+1 ; i <= end ; i++) {
		if(lst[i].type == M_ATOM_TYPE) {
			acur = ((s_atm*) lst[i].data) ;
			if(acur->x < piv) {
				c++ ;
			
				// We have to swap, so change indices, and swap elements.
				if(lst[c].type == M_ATOM_TYPE) ((s_atm*)lst[c].data)->sort_x = i ;
				else ((s_vvertice*)lst[c].data)->sort_x = i ;
				
				acur->sort_x = c ;
				
				tmp = lst[c] ;
				lst[c] = lst[i] ; lst[i] = tmp ;
			}
		}
		else {
			vcur = ((s_vvertice*)lst[i].data) ;
			if(vcur->x < piv) {
				c++ ;
			
				// We have to swap, so change indices, and swap elements.
				if(lst[c].type == M_ATOM_TYPE) ((s_atm*)lst[c].data)->sort_x = i ;
				else ((s_vvertice*)lst[c].data)->sort_x = i ;
				
				vcur->sort_x = c ;
				
				tmp = lst[c] ;
				lst[c] = lst[i] ; lst[i] = tmp ;
			}
		}
	}
	
	if(lst[c].type == M_ATOM_TYPE) ((s_atm*)lst[c].data)->sort_x = start ;
	else ((s_vvertice*)lst[c].data)->sort_x = start ;
	
	if(lst[start].type == M_ATOM_TYPE) ((s_atm*)lst[start].data)->sort_x = c ;
	else ((s_vvertice*)lst[start].data)->sort_x = c ;
			
	tmp = lst[c] ;
	lst[c] = lst[start] ; lst[start] = tmp ;

	return(c);
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void print_sorted_lst(s_vsort *lsort, int which_dim, FILE *buf)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Print one of the sorted tab of a  s_vsort structure
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_vsort *lsort: Structure containing tab
	@ FILE *buf: Buffer to print in.
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
void print_sorted_lst(s_vsort *lsort, FILE *buf)
{
	s_vect_elem *lst = NULL ;
	lst = lsort->xsort ; fprintf(buf, "\n========== Printing list of vertices and atoms sorted on X axe:\n") ;
	
	float cval, prev = -1.0 ;
	int i ;
	
	for(i = 0 ; i < lsort->nelem ; i++) {
		fprintf(buf, "> Element at %d: ", i);
		if(lst[i].type == M_ATOM_TYPE) {
			s_atm *a = (s_atm*) lst[i].data ;
			cval = a->x ;
			fprintf(buf, " ATOM coord = %f, index stored: %d", cval, a->sort_x) ;
		}
		else {
			s_vvertice *v = (s_vvertice*) lst[i].data ;
			cval = v->x ;
			
			fprintf(buf, " VERTICE coord = %f, index stored: %d", cval, v->sort_x) ;
		}
		
		if(prev > cval) fprintf(buf, " !!!!!!! ") ;
		fprintf(buf, "\n") ;
		
		prev = cval ;	
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void free_s_vsort(s_vsort *lsort)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Free memory for s_vsort structure
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_vsort *lsort: Structure to free
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
void free_s_vsort(s_vsort *lsort)
{
	if(lsort) {
		if(lsort->xsort) my_free(lsort->xsort) ;
		
		my_free(lsort) ;
	}
}

