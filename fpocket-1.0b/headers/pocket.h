
#ifndef DH_POCKET
#define DH_POCKET


//maximal distance between two vertices in order to belong to the same pocket (single linkage clustering)
#define MAX_CON_DIST 2.5

// -----------------------------------INCLUDES--------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "voronoi_lst.h"
#include "descriptors.h"
#include "fparams.h"
#include "aa.h"
#include "utils.h"
#include "memhandler.h"

// ------------------------------------ PUBLIC STRUCTURES -------------------------------------------

typedef struct s_pocket		//add other atoms here (not contacted by alpha spheres
{
	s_desc *pdesc ;

	c_lst_vertices *v_lst ;

	float score,				// Discretize different parts of the score
		  corresp,				// Atomic overlap	
		  corresp2,
		  vol_corresp,			// Volume overlap
		  nbv_nbl,				// Nb atom neighbours of the pocket / nb atoms neighbor of ligans
		  nbv_nbl2,			
		  apol_surface,
		  total_surface,
		  curvature,
		  drug_score,
		  bary[3] ;				// Barycenter of the pocket

	int rank,					//rank of the pocket
		size,
		nAlphaApol,				//number of apolar alpha spheres
		nAlphaPol ;				//number of polar alpha spheres

} s_pocket ; 

//chained list stuff for pockets
typedef struct node_pocket 
{
	struct node_pocket *next ;
	struct node_pocket *prev ;
	s_pocket *pocket ;

} node_pocket ;

typedef struct c_lst_pockets 
{
	struct node_pocket *first ;
	struct node_pocket *last ;
	struct node_pocket *current ;
	size_t n_pockets ;

	s_lst_vvertice *vertices ;

} c_lst_pockets ;


// ------------------------------------PROTOTYPES-------------------------------------------

c_lst_pockets *c_lst_pockets_alloc(void);
node_pocket *node_pocket_alloc(s_pocket *pocket);
void c_lst_pocket_free(c_lst_pockets *lst);

node_pocket *c_lst_pockets_add_first(c_lst_pockets *lst, s_pocket *pocket);
node_pocket *c_lst_pockets_add_last(c_lst_pockets *lst,s_pocket *pocket,int cur_n_apol, int cur_n_pol);
void swap_pockets(c_lst_pockets *pockets, node_pocket *p1, node_pocket *p2) ;
void dropPocket(c_lst_pockets *pockets,node_pocket *pocket);
void mergePockets(node_pocket *pocket,node_pocket *pocket2,c_lst_pockets *pockets);
node_pocket *searchPocket(int resid,c_lst_pockets *pockets);


c_lst_pockets *clusterPockets(s_lst_vvertice *lvvert, s_fparams *params);
int updateIds(s_lst_vvertice *lvvert, int i, int *vNb, int resid,int curPocket,c_lst_pockets *pockets, s_fparams *params);
void addStats(int resid, int size, int **stats,int *lenStats);

void set_pockets_descriptors(c_lst_pockets *pockets) ;
void set_pockets_bary(c_lst_pockets *pockets) ;
s_atm** get_pocket_contacted_atms(s_pocket *pocket, int *natoms) ;
float set_pocket_score(s_pocket *pok) ;
float set_pocket_score2(s_pocket *pok) ;

float set_pocket_mtvolume(s_pocket *pocket, int niter) ;
float set_pocket_volume(s_pocket *pocket, int discret) ;

void sort_pockets(c_lst_pockets *pockets, int (*fcmp)(const node_pocket*, const node_pocket*)) ;
int compare_pockets_corresp(const node_pocket *p1, const node_pocket *p2) ;
int compare_pockets_score(const node_pocket *p1, const node_pocket *p2) ;
int compare_pockets_volume(const node_pocket *p1, const node_pocket *p2) ;
int compare_pockets_vol_corresp(const node_pocket *p1, const node_pocket *p2) ;
int compare_pockets_nasph(const node_pocket *p1, const node_pocket *p2) ;

void reset_pocket(s_pocket *pocket) ;
void print_pocket(FILE *f, s_pocket *pocket);
void print_pockets(FILE *f, c_lst_pockets *pockets) ;
void print_pockets_inv(FILE *f, c_lst_pockets *pockets) ;

#define M_VOLUME_SORT_FUNCT &compare_pockets_volume
#define M_SCORE_SORT_FUNCT &compare_pockets_score
#define M_NASPH_SORT_FUNCT &compare_pockets_nasph

#endif
