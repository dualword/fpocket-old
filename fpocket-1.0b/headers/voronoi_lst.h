
 
#ifndef DH_VORONOI_LST
#define DH_VORONOI_LST

// -----------------------------------INCLUDES--------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "voronoi.h"
#include "atom.h"
#include "memhandler.h"

// ------------------------------------MACROS-------------------------------------------


// ------------------------------------STRUCTURES-------------------------------------------

// Chained list stuff for vertices in a pocket (to enable dynamic modifications)
typedef struct node_vertice
{
	struct node_vertice *next;
	struct node_vertice *prev;
	s_vvertice *vertice ;

} node_vertice ;

typedef struct c_lst_vertices
{
	struct node_vertice *first ;
	struct node_vertice *last ;
	struct node_vertice *current ;
	size_t n_vertices ;

} c_lst_vertices ;

// ------------------------------------PROTOTYPES-------------------------------------------

c_lst_vertices *c_lst_vertices_alloc(void);
node_vertice *node_vertice_alloc(s_vvertice *vertice);
node_vertice *c_lst_vertices_add_first(c_lst_vertices *lst, s_vvertice *vertice);
node_vertice *c_lst_vertices_add_last(c_lst_vertices *lst,s_vvertice *vertice);
void c_lst_vertices_free(c_lst_vertices *lst);

s_atm** get_vert_contacted_atms(c_lst_vertices *v_lst, int *nneigh) ;

#endif


