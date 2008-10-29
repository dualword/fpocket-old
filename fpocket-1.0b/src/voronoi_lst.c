
#include "../headers/voronoi_lst.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					voronoi_lst.c
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
   ## FONCTION: 
   c_lst_vertices* lst_vertices_alloc(void)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Allocate a list of vertices
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   -----------------------------------------------------------------------------
   ## RETURN:
	c_lst_vertices*
   -----------------------------------------------------------------------------
*/
c_lst_vertices *c_lst_vertices_alloc(void) 
{
	c_lst_vertices *lst = my_malloc(sizeof(c_lst_vertices)) ;

	lst->first = NULL ;
	lst->last = NULL ;
	lst->current = NULL ;
	lst->n_vertices = 0 ;

	return lst ;
}

/**-----------------------------------------------------------------------------
   ## FONCTION: 
	node_vertice_alloc (s_vvertice *vertice)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Allocate memory for one vertice node.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_vvertice *vertice : pointer to the vertice
   -----------------------------------------------------------------------------
   ## RETURN:
	node_vertice*
   -----------------------------------------------------------------------------
*/
node_vertice *node_vertice_alloc(s_vvertice *vertice)
{
	node_vertice *n_vertice = my_malloc(sizeof(node_vertice)) ;

	n_vertice->next = NULL ;
	n_vertice->prev = NULL ;
	n_vertice->vertice = vertice ;
	
	return n_vertice ;
}

/**-----------------------------------------------------------------------------
   ## FONCTION: 
	node_vertice *c_vertice_lst_add_first(c_lst_vertices *lst, s_vvertice *vertice)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Add a vertice at the first position of the list.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_vertices *lst: chained list of vertices
	@ s_vvertice *vertice: vertice to add
   -----------------------------------------------------------------------------
   ## RETURN:
	pointer toi the new node.
   -----------------------------------------------------------------------------
*/
node_vertice *c_lst_vertices_add_first(c_lst_vertices *lst, s_vvertice *vertice)
{
	node_vertice *newn = NULL ;

	if(lst) {
		newn = node_vertice_alloc(vertice) ;
		lst->first->prev = newn ;
		newn->next = lst->first ;

		lst->first = newn ;
		lst->n_vertices += 1 ;
	}
	
	return newn ;
}

/**-----------------------------------------------------------------------------
   ## FONCTION: 
	node_vertice *c_vertice_lst_add_last(c_lst_vertices *lst,s_vvertice *vertice)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Add a vertice at the end of the chained list
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_pocket *lst: chained list of pockets
	@ s_vvertice *vertice: vertice to add
   -----------------------------------------------------------------------------
   ## RETURN:
	node_vertice *
   -----------------------------------------------------------------------------
*/
node_vertice *c_lst_vertices_add_last(c_lst_vertices *lst,s_vvertice *vertice)
{
	struct node_vertice *newn = NULL ;

	if(lst) {
		newn = node_vertice_alloc(vertice) ;
		if(lst->last) {
			newn->prev = lst->last ;
			lst->last->next = newn ;
		}
		else {
			lst->first = newn ;

		}
		lst->last = newn ;
		lst->n_vertices += 1 ;
	}

	return newn ;
}

/**-----------------------------------------------------------------------------
   ## FONCTION: 
   c_lst_vertices *lst_vertice_free(c_lst_vertices *lst) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Free memory of a chained list
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_vertices *lst: list of voronoi vertices
   -----------------------------------------------------------------------------
   ## RETURN:
	c_lst_vertice*
   -----------------------------------------------------------------------------
*/
void c_lst_vertices_free(c_lst_vertices *lst) 
{
	fprintf(stdout, "Freeing list of vertices\n") ;
	node_vertice *next = NULL ;
	
	if(lst) {
		lst->current = lst->first ;
		while(lst->current) {
			next = lst->current->next ;
			my_free(lst->current) ;
			lst->current = next ;
        }
	}

	lst->first = NULL ;
	lst->last = NULL ;
	lst->current = NULL ;

	my_free(lst) ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	s_atm** get_pocket_neigh(c_lst_vertices *v_lst, int *nneigh) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Get the list of atoms contacted by each vertice in the given list of vertices.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_vertices *v_lst: The list of vertices of the pocket.
	@ int *nneigh: A pointer to the number of neighbour found, will be modified
   -----------------------------------------------------------------------------
   ## RETURN:
	A tab of pointers to the pocket contacting atoms.
   -----------------------------------------------------------------------------
*/
s_atm** get_vert_contacted_atms(c_lst_vertices *v_lst, int *nneigh) 
{
	int i ;
	int nb_neigh = 0 ;
	int atm_seen[v_lst->n_vertices * 4] ;

	s_atm **neigh = my_malloc(sizeof(s_atm*)*v_lst->n_vertices * 4) ;
	
	node_vertice *cur = v_lst->first ;

	while(cur) {
		s_vvertice *vcur = cur->vertice ;
		
		for(i = 0 ; i < 4 ; i++) {
		// For each neighbor, if this atom has not been see yet, add it.
			if(!in_tab(atm_seen, nb_neigh, vcur->neigh[i]->id)) {
				neigh[nb_neigh] = vcur->neigh[i] ;
				atm_seen[nb_neigh] = neigh[nb_neigh]->id ;
				nb_neigh++ ;
			}
		}

		cur = cur->next ;
	}

	*nneigh = nb_neigh ;

	return neigh ;
}
