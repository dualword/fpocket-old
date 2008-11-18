 
#include "../headers/pocket.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					pocket.c
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## ----- SPECIFICATIONS
##	
##		All functions dealing with pockets: sorting, clustering...
##
## ----- MODIFICATIONS HISTORY
##
##  02-09-08	(p)  Added druggability score, curvature and surfaces for 
##					 each pocket structure.
##	01-04-08	(v)  Added template for comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
##

*/

/**
*	Private functions used for the quick sort
*/
static int pock_partition(c_lst_pockets *pockets, node_pocket **pocks, int start, int end, 
						  int (*fcmp)(const node_pocket*, const node_pocket*)) ;

static void pock_qsort_rec(c_lst_pockets *pockets, node_pocket **pocks, int start, int end,  
						   int (*fcmp)(const node_pocket*, const node_pocket*)) ;


/**
 ================================================================================
 ================================================================================

	ALLOCATIONS AND DESALLOCATIONS FOR POCKET AND CHAINED LIST OF POCKET 

 ================================================================================
 ================================================================================
*/

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	s_pocket* alloc_pocket(void) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Alloc memory for a pocket and reste values describing it.
   -----------------------------------------------------------------------------
   ## PARAMETRES:s
   -----------------------------------------------------------------------------
   ## RETURN:
	pointer to the pocket allocated. 
   -----------------------------------------------------------------------------
*/
s_pocket* alloc_pocket(void) 
{
	s_pocket *p = my_malloc(sizeof(s_pocket)) ;
	p->pdesc = my_malloc(sizeof(s_desc)) ;
	p->v_lst = NULL ;

	reset_pocket(p) ;

	return p ;
}

/**-----------------------------------------------------------------------------
   ## FONCTION: 
   c_lst_pockets* lst_pockets_alloc(void)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Allocate a list of pockets
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   -----------------------------------------------------------------------------
   ## RETURN:
	c_lst_pockets*
   -----------------------------------------------------------------------------
*/
c_lst_pockets *c_lst_pockets_alloc(void) 
{
	c_lst_pockets *lst = my_malloc(sizeof(c_lst_pockets)) ;

	lst->first = NULL ;
	lst->last = NULL ;
	lst->current = NULL ;
	lst->n_pockets = 0 ;
	lst->vertices = NULL ;

	return lst ;
}

/**-----------------------------------------------------------------------------
   ## FONCTION: 
	node_pocket_alloc (s_pocket *pocket)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Allocate memory for one pocket node
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_pocket *pocket : pointer to the pocket
   -----------------------------------------------------------------------------
   ## RETURN:
	node_pocket*
   -----------------------------------------------------------------------------
*/
node_pocket *node_pocket_alloc(s_pocket *pocket)
{
	node_pocket *n_pocket = my_malloc(sizeof(node_pocket)) ;

	n_pocket->next = NULL ;
	n_pocket->prev = NULL ;
	n_pocket->pocket = pocket ;
	
	return n_pocket ;
}


/**-----------------------------------------------------------------------------
   ## FONCTION: 
	void c_lst_pocket_free(c_lst_pockets *lst) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Free a pocket list.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_pockets *lst: The list to free.
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void c_lst_pocket_free(c_lst_pockets *lst) 
{
	node_pocket *cur = NULL,
				*next = NULL ;
	
	if(lst) {
		cur = lst->first ;
		while(cur) {
			next = cur->next ;
			dropPocket(lst, cur) ;
			cur = next ;
        }

		free_vert_lst(lst->vertices) ;

		lst->vertices = NULL ;
		lst->first = NULL ;	
		lst->last = NULL ;
		lst->current = NULL ;

		my_free(lst) ;
	}

}

/**
 ================================================================================
 ================================================================================

	FUNCTIONS FOR POCKET CHAINED LIST OPERATIONS

 ================================================================================
 ================================================================================
*/

/**-----------------------------------------------------------------------------
   ## FONCTION: 
  	node_pocket *c_lst_pockets_add_first(c_lst_pockets *lst, s_pocket *pocket)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Add a pocket on the first position of the list
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_pocket *lst: chained list of pockets
	@ s_pocket: pocket to add
   -----------------------------------------------------------------------------
   ## RETURN:
	pointer to the new node.
   -----------------------------------------------------------------------------
*/
node_pocket *c_lst_pockets_add_first(c_lst_pockets *lst, s_pocket *pocket)
{
	node_pocket *newn = NULL ;

	if(lst) {
		newn = node_pocket_alloc(pocket) ;
		lst->first->prev = newn ;
		newn->next = lst->first ;

		lst->first = newn ;
		lst->n_pockets += 1 ;
	}
	
	return newn ;
}

/**-----------------------------------------------------------------------------
   ## FONCTION: 
	node_pocket *c_lst_pockets_add_last(c_lst_pockets *lst,s_pocket *pocket)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Add a pocket at the end of the chained list
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_pocket *lst: chained list of pockets
	@ s_pocket: pocket to add
   -----------------------------------------------------------------------------
   ## RETURN:
	Pointer to the new pocket.
   -----------------------------------------------------------------------------
*/
node_pocket *c_lst_pockets_add_last(c_lst_pockets *lst,s_pocket *pocket,int cur_n_apol, int cur_n_pol)
{
	node_pocket *newn = NULL ;

	if(lst) {
		newn = node_pocket_alloc(pocket) ;
		newn->pocket->nAlphaApol = cur_n_apol;
 		newn->pocket->nAlphaPol = cur_n_pol;
		if(lst->last) {
			newn->prev = lst->last ;
			lst->last->next = newn ;
		}
		else {
			lst->first = newn ;

		}
		lst->last = newn ;
		lst->n_pockets += 1 ;
	}

	return newn ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void swap_pockets(c_lst_pockets *pockets, node_pocket *p1, node_pocket *p2) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Swap two pockets in the given list (not that easy !!! ;) )
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_pockets *pockets: The list of pockets
	@ const node_pocket *p1: Pocket 1
	@ const node_pocket *p2: Pocket 2
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void swap_pockets(c_lst_pockets *pockets, node_pocket *p1, node_pocket *p2) 
{
	node_pocket *p1prev = p1->prev,
				*p1next = p1->next,
				*p2prev = p2->prev,
				*p2next = p2->next ;
	
	if(p1->next == p2) {
	// If p1 is just before p2 (p1 can't be the last one and p2 can't be the first one)
		if(p1->prev) p1->prev->next = p2 ;
		else pockets->first = p2 ; // P1 is the first of the list

		if(p2->next) p2->next->prev = p1 ;
		else pockets->last = p2 ; // P2 is the last one

		p2->next = p1 ;
		p2->prev = p1prev ;
		p1->prev = p2 ;
		p1->next = p2next ;
	}
	else if(p1->prev == p2) {
	// If p1 is just after p2 (p2 can't be the last one and p1 can't be the first one)
		if(p1->next) p1->next->prev = p2 ;
		else pockets->last = p2 ; // p1 is the last one
		
		if(p2->prev) p2->prev->next = p1 ;
		else pockets->first = p1 ;

		p1->next = p2 ;
		p1->prev = p2prev ;
		p2->prev = p1 ;
		p2->next = p1next;
	}
	else {
	// If p1 and p2 are far away from each others in the list
	// Handle p1
		if(p1->next) {
		// If there is a pocket after p1, p1 is not the last one
			p1->next->prev = p2 ;	// p2 should be the new prev of p1's next
			if(p1->prev) p1->prev->next = p2 ;
			else pockets->first = p2 ;
		}
		else {
		// p1 is the last pocket of the list
			
			pockets->last = p2 ;
			if(p1->prev) {
				p1->prev->next = p2 ;
			}
		}

	// Handle P2
		if(p2->next) {
		// If there is a pocket after p1, p1 is not the last one
			p2->next->prev = p1 ;	// p2 should be the new prev of p1's next
			if(p2->prev) p2->prev->next = p1 ;
			else pockets->first = p1 ;
		}
		else {
		// p1 is the last pocket of the list
			if(p2->prev) {
				p2->prev->next = p1 ;
				pockets->last = p1 ;
			}
		}

		p1->next = p2next ;
		p1->prev = p2prev ;

		p2->next = p1next ;
		p2->prev = p1prev ;
		
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	node_pocket *searchPocket(int resid, c_lst_pockets *lst)
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Search a pocket
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ int resid: ID of the pocket
	@ c_lst_pockets *lst: The list of pockets
   -----------------------------------------------------------------------------
   ## RETURN:
	pointer to the pocket found. 
   -----------------------------------------------------------------------------
*/
node_pocket *searchPocket(int resid, c_lst_pockets *lst)
{
	node_pocket *cur = NULL ;
	
	if(lst) {
		cur = lst->first ;
		while(cur && cur->pocket->v_lst->first->vertice->resid != resid) {
			cur = cur->next ;
        }
	}

	return cur;
}

/**-----------------------------------------------------------------------------
   ## FONCTION: 
	void dropPocket(c_lst_pockets *pockets,node_pocket *pocket)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Drop a pocket from the list.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_pockets *lst: The list.
	@ node_pocket *pocket: The pocket to drop
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void dropPocket(c_lst_pockets *pockets,node_pocket *pocket)
{
	
	if(pocket->prev && pocket->next){
		pocket->prev->next = pocket->next;
		pocket->next->prev = pocket->prev;
	}
	else if(pocket->next){
		pocket->next->prev = NULL;
		pockets->first = pocket->next;
	}
	else if(pocket->prev){
		pocket->prev->next = NULL;
		pockets->last = pocket->prev;
	}

	pocket->next = NULL ;
	pocket->prev = NULL ;
	pockets->n_pockets -= 1;

	my_free(pocket->pocket->pdesc);
	pocket->pocket->pdesc = NULL ;
	my_free(pocket->pocket);
	pocket->pocket= NULL;

	my_free(pocket);

	if(pockets->n_pockets == 0) pockets->first = NULL ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void mergePockets(node_pocket *pocket,node_pocket *pocket2,c_lst_pockets *pockets)
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Merge two pockets.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ node_pocket *pocket: The first pocket
	@ node_pocket *pocket2: The second pocket
	@ c_lst_pockets *pockets: The list of pockets
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void mergePockets(node_pocket *pocket,node_pocket *pocket2,c_lst_pockets *pockets)
{
	s_pocket *pock =  pocket->pocket,
			 *pock2 = pocket2->pocket ;

	pock->nAlphaApol += pock2->nAlphaApol;
	pock->nAlphaPol += pock2->nAlphaPol;
	pock->v_lst->n_vertices += pock2->v_lst->n_vertices;
	pock->v_lst->last->next = pock2->v_lst->first;
	pock->v_lst->last = pock2->v_lst->last;
	pocket2->pocket->v_lst = NULL;

	my_free(pocket2->pocket);
	pocket2->pocket=NULL;
	
	if(pocket2->prev && pocket2->next){
		pocket2->prev->next = pocket2->next;
		pocket2->next->prev = pocket2->prev;
	}
	else if(pocket2->next){
		pockets->first=pocket2->next;
		pocket2->next->prev=NULL;
	}
	else if(pocket2->prev){

		pocket2->prev->next=NULL;
		pockets->last=pocket2->prev;
	}
	else if(!pocket2->prev && !pocket2->next) my_free(pocket2);

	pockets->n_pockets-=1;
 	my_free(pocket2);
}

/**
 ================================================================================
 ================================================================================

	FUNCTIONS ON POCKET (clusturing, volume calculation...)

 ================================================================================
 ================================================================================
*/

					// -------------------- //
					// CLUSTERING FUNCTIONS //
					// -------------------- //

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	c_lst_pockets *clusterPockets(s_lst_vvertice *lvvert, s_fparams *params)
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	This function takes in argument a list of vertice, and perform a first
	clusturing algorithm to merge vertices close from each others. The distance
	criteria is in the params struct.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_lst_vvertice *lvvert: The list of vertices.
	@ s_fparams *params: Parameters
   -----------------------------------------------------------------------------
   ## RETURN:
	list of pockets!
   -----------------------------------------------------------------------------
*/
c_lst_pockets *clusterPockets(s_lst_vvertice *lvvert, s_fparams *params)
{
	int i = -1,
		j = -1,
		curPocketId=1,
		vNb[4] = {0,0,0,0};

	
	s_vvertice *vertices = lvvert->vertices,
			   *vcur = NULL ;

	c_lst_pockets *pockets = c_lst_pockets_alloc();		//allocate memory for pockets chained list
		
	for(i=0;i<lvvert->nvert;i++) {
		vcur = vertices + i ;
		for(j=0;j<4;j++) vNb[j] = vcur->vneigh[j];	//read vertice neighbour indexes

		curPocketId=updateIds(lvvert,i,vNb,vcur->resid, curPocketId,pockets, params);
	}

	if(pockets->n_pockets > 0) return pockets ;
	else {
		my_free(pockets) ;
		return NULL ;
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int updateIds(s_lst_vvertice *lvvert, int i, int *vNb, int resid,int curPocket,c_lst_pockets *pockets, s_fparams *params)
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Update ids.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_lst_vvertice *lvvert: The list of vertices.
	@ int i:
	@ int *vNb:
	@ int resid:
	@ int curPocket:
	@ c_lst_pockets *pockets: The current list of pockets.
	@ s_fparams *params: Parameters
   -----------------------------------------------------------------------------
   int
   -----------------------------------------------------------------------------
*/
int updateIds(s_lst_vvertice *lvvert, int i, int *vNb, int resid, int curPocket,c_lst_pockets *pockets, s_fparams *params)
{
// 	s_pocket *curPocket;
	int filteredIdx,j,z;
	int groupCreatedFlag=0;
	int cur_n_apol=0;
	int cur_n_pol=0;

	s_vvertice *vertices = lvvert->vertices ;
	s_vvertice *vert = &(vertices[i]),
			   *fvert = NULL ;

	for(j=0;j<4;j++) {
		if(vNb[j] < lvvert->qhullSize && vNb[j] > 0) {
			filteredIdx = lvvert->tr[vNb[j]];
			if(filteredIdx!=-1 && filteredIdx < lvvert->nvert){
				fvert = &(vertices[filteredIdx]) ;
				
				if(dist(vert->x, vert->y, vert->z, fvert->x, fvert->y, fvert->z) <= params->clust_max_dist){
					groupCreatedFlag=1;
					if(resid == -1 && fvert->resid==-1){						//add a new pocket
						resid=++curPocket;
						vert->resid=resid;
						fvert->resid=curPocket;	
						s_pocket *pocket = alloc_pocket();				//create a new pocket
						pocket->v_lst=c_lst_vertices_alloc();
						c_lst_vertices_add_last(pocket->v_lst, vert);				//add vertices to the pocket
						c_lst_vertices_add_last(pocket->v_lst, fvert);

						if(vert->type==M_APOLAR_AS) cur_n_apol++;
						else cur_n_pol++;

						if(fvert->type==M_APOLAR_AS)cur_n_apol++;
						else cur_n_pol++;

						c_lst_pockets_add_last(pockets, pocket,cur_n_apol,cur_n_pol);
					}
					else if(resid!=-1 && fvert->resid==-1) 						//add new vertice to existing pocket
					{
						fvert->resid=resid;
						node_pocket *pocket=searchPocket(resid,pockets);
						if(pocket) c_lst_vertices_add_last(pocket->pocket->v_lst, fvert);

						if(fvert->type==M_APOLAR_AS) pocket->pocket->nAlphaApol+=1;
						else pocket->pocket->nAlphaPol+=1;
					}
					else if(resid==-1 && fvert->resid!=-1) {
						vert->resid = fvert->resid;
						resid=fvert->resid;
						node_pocket *pocket = searchPocket(resid,pockets);
						if(pocket) c_lst_vertices_add_last(pocket->pocket->v_lst, vert);
						if(vert->type==M_APOLAR_AS) pocket->pocket->nAlphaApol+=1;
						else pocket->pocket->nAlphaPol+=1;
					}
					else if((resid!=-1 && fvert->resid!=-1) && (resid!=fvert->resid)) {
						node_pocket *pocket=searchPocket(resid,pockets);
 						node_pocket *pocket2=searchPocket(fvert->resid,pockets);						
						mergePockets(pocket,pocket2,pockets);					//write content of pocket1 into pocket2

						for(z=0;z<lvvert->nvert;z++){
							if(vertices[z].resid==resid) vertices[z].resid=fvert->resid;	//merge two clusters -> to optimize, but we HAVE NO TIME !!!!!!!!!!!!!!!!!!!
						}
						resid=fvert->resid;

					}
				}
			}
			else {
				if(filteredIdx >= lvvert->nvert) {
					fprintf(stdout, "INDEX ERROR\n") ;
					//getchar() ;
				}
			}
		}
	}
	/* if a pocket was created return the pocket */
	if(groupCreatedFlag) return curPocket;
	
	/* if no vertice neighbours were found, still build a new pocket containing 
	 * only one vertice */
	
	cur_n_apol=0;
	cur_n_pol=0;
	resid=++curPocket;
	vert->resid=resid;
	s_pocket *pocket=alloc_pocket();				/* Create a new pocket */
	pocket->v_lst=c_lst_vertices_alloc();
	c_lst_vertices_add_last(pocket->v_lst, vert);	/* Add vertices to the pocket */
	if(vert->type==M_APOLAR_AS) cur_n_apol++;
	else cur_n_pol++;

	c_lst_pockets_add_last(pockets,pocket,cur_n_apol,cur_n_pol);

	return curPocket;
	
}


				// ------------------------------- //
				// VOLUME AND DESCRIPTOR FUNCTIONS //
				// ------------------------------- //


/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	float set_pocket_mtvolume(s_pocket *pocket, int niter) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Get an monte carlo approximation of the volume occupied by the pocket given
	in argument.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_pocket *pocket: Pockets to print
	@ int niter: Number of monte carlo iteration
   -----------------------------------------------------------------------------
   ## RETURN:
	float: volume.
   -----------------------------------------------------------------------------
*/
float set_pocket_mtvolume(s_pocket *pocket, int niter) 
{
	int i = 0,
		nb_in = 0,
		nit = 0 ;

	float xmin = 0.0, xmax = 0.0,
		  ymin = 0.0, ymax = 0.0,
		  zmin = 0.0, zmax = 0.0,
		  xtmp = 0.0, ytmp = 0.0, ztmp = 0.0,
		  xr = 0.0, yr = 0.0, zr = 0.0,
		  vbox = 0.0 ;

	c_lst_vertices *vertices = pocket->v_lst ;
	node_vertice *cur = vertices->first ;
	s_vvertice *vcur = NULL ;

	// First, search extrems coordinates
	while(cur) {
		vcur = cur->vertice ;
		
		// Update min:
		if(nit == 0) {
			xmin = vcur->x - vcur->ray ; xmax = vcur->x + vcur->ray ;
			ymin = vcur->y - vcur->ray ; ymax = vcur->y + vcur->ray ;
			zmin = vcur->z - vcur->ray ; zmax = vcur->z + vcur->ray ;
		}
		else {
			if(xmin > (xtmp = vcur->x - vcur->ray)) xmin = xtmp ;
			else if(xmax < (xtmp = vcur->x + vcur->ray)) xmax = xtmp ;
	
			if(ymin > (ytmp = vcur->y - vcur->ray)) ymin = ytmp ;
			else if(ymax < (ytmp = vcur->y + vcur->ray)) ymax = ytmp ;
	
			if(zmin > (ztmp = vcur->z - vcur->ray)) zmin = ztmp ;
			else if(zmax < (ztmp = vcur->z + vcur->ray)) zmax = ztmp ;
		}

		cur = cur->next ;
		nit++ ;
	}

	// Next calculate the box volume
	vbox = (xmax - xmin)*(ymax - ymin)*(zmax - zmin) ;
	
	// Then apply monte carlo approximation of the volume.	
	for(i = 0 ; i < niter ; i++) {
		xr = rand_uniform(xmin, xmax) ;
		yr = rand_uniform(ymin, ymax) ;
		zr = rand_uniform(zmin, zmax) ;
		cur = vertices->first ;
		
		while(cur) {
			vcur = cur->vertice ;
		// Distance between the center of curent vertice and the random point
			xtmp = vcur->x - xr ;
			ytmp = vcur->y - yr ;
			ztmp = vcur->z - zr ;

		// Compare r^2 and dist(center, random_point)^2  to avoid a call to sqrt() function
			if((vcur->ray*vcur->ray) > (xtmp*xtmp + ytmp*ytmp + ztmp*ztmp)) {
			// The point is inside one of the vertice!!
				nb_in ++ ; break ;
			}
			cur = cur->next ;
		}
	}

	pocket->pdesc->volume = ((float)nb_in)/((float)niter)*vbox ;

	// Ok lets just return the volume Vpok = Nb_in/Niter*Vbox
	return pocket->pdesc->volume ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	float set_pocket_volume(s_pocket *pocket, int niter) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Get an approximation of the volume occupied by the pocket given in argument,
	using a discretized space.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_pocket *pocket: Pockets to print
	@ int idiscret: Discretisation: the cube containing all vertices will be 
				 divided in idiscret*idiscret*idiscret.
   -----------------------------------------------------------------------------
   ## RETURN:
	float: volume.
   -----------------------------------------------------------------------------
*/
float set_pocket_volume(s_pocket *pocket, int idiscret) 
{
	int niter = 0,
		nb_in = 0,
		nit = 0 ;

	float discret = 1.0/(float)idiscret ;

	float x = 0.0, y = 0.0, z = 0.0,
		  xstep = 0.0, ystep = 0.0, zstep = 0.0 ;

	float xmin = 0.0, xmax = 0.0,
		  ymin = 0.0, ymax = 0.0,
		  zmin = 0.0, zmax = 0.0,
		  xtmp = 0.0, ytmp = 0.0, ztmp = 0.0,
		  vbox = 0.0 ;

	c_lst_vertices *vertices = pocket->v_lst ;
	node_vertice *cur = vertices->first ;
	s_vvertice *vcur = NULL ;

	// First, search extrems coordinates
	while(cur) {
		vcur = cur->vertice ;
		
		// Update min:
		if(nit == 0) {
			xmin = vcur->x - vcur->ray ; xmax = vcur->x + vcur->ray ;
			ymin = vcur->y - vcur->ray ; ymax = vcur->y + vcur->ray ;
			zmin = vcur->z - vcur->ray ; zmax = vcur->z + vcur->ray ;
		}
		else {
			if(xmin > (xtmp = vcur->x - vcur->ray)) xmin = xtmp ;
			else if(xmax < (xtmp = vcur->x + vcur->ray)) xmax = xtmp ;
	
			if(ymin > (ytmp = vcur->y - vcur->ray)) ymin = ytmp ;
			else if(ymax < (ytmp = vcur->y + vcur->ray)) ymax = ytmp ;
	
			if(zmin > (ztmp = vcur->z - vcur->ray)) zmin = ztmp ;
			else if(zmax < (ztmp = vcur->z + vcur->ray)) zmax = ztmp ;
		}

		cur = cur->next ;
		nit++ ;
	}

	// Next calculate the box volume
	vbox = (xmax - xmin)*(ymax - ymin)*(zmax - zmin) ;

	xstep = discret * (xmax - xmin) ;
	ystep = discret * (ymax - ymin) ;
	zstep = discret * (zmax - zmin) ;

	// Then apply monte carlo approximation of the volume.	
	for(x = xmin ; x < xmax ; x += xstep) {
		for(y = ymin ; y < ymax ; y += ystep) {	
			for(z = zmin ; z < zmax ; z += zstep) {
				cur = vertices->first ;
				while(cur) {
					vcur = cur->vertice ;
					xtmp = vcur->x - x ;
					ytmp = vcur->y - y ;
					ztmp = vcur->z - z ;
		
				// Compare r^2 and dist(center, random_point)^2
					if((vcur->ray*vcur->ray) > (xtmp*xtmp + ytmp*ytmp + ztmp*ztmp)) {
					//the point is inside one of the vertice!!
						nb_in ++ ; break ;
					}
					cur = cur->next ;
				}
				niter ++ ;
			}
		}
	}

	pocket->pdesc->volume = ((float)nb_in)/((float)niter)*vbox ;

	// Ok lets just return the volume Vpok = Nb_in/Niter*Vbox
	return pocket->pdesc->volume ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void set_descriptors(c_lst_pockets *pockets)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Set descriptors for a list of pockets.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_pockets *pockets: The list of pockets to handle
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void set_pockets_descriptors(c_lst_pockets *pockets)
{
	node_pocket *cur = NULL ;
	s_pocket *pcur = NULL ;

	float max_flex = 0.0 ;
	int natms = 0, i ;

	if(pockets && pockets->n_pockets > 0) {
		cur = pockets->first ;
		while(cur) {
			pcur = cur->pocket ;

		/* Getting a list of vertices in a tab of pointer */
			s_vvertice **tab_vert = my_malloc(pcur->v_lst->n_vertices*sizeof(s_vvertice*)) ;
			i = 0 ;
			node_vertice *nvcur = pcur->v_lst->first ;
			while(nvcur) {
				tab_vert[i] = nvcur->vertice ;
				nvcur = nvcur->next ;
				i++ ;
			}

		/* Getting atoms contacted by vertices */
			s_atm **pocket_atoms = get_pocket_contacted_atms(pcur, &natms) ;

			set_descriptors(pocket_atoms, natms, tab_vert, pcur->v_lst->n_vertices, pcur->pdesc) ;
			if(pcur->pdesc->flex > max_flex) max_flex = pcur->pdesc->flex ;

			my_free(pocket_atoms) ;
			my_free(tab_vert) ;

			cur = cur->next ;
		}

		// Normalisation of flexibility between pockets, with the most flexible pocket as reference...
		// Remember, flexibility is very abusive, as we just sum the b-factor of all atoms...
		cur = pockets->first ;
		while(cur) {
			pcur = cur->pocket ;
			pcur->pdesc->flex /= max_flex ;

			set_pocket_score2(pcur) ;

			cur = cur->next ;
		}
	}
}


/**-----------------------------------------------------------------------------
   ## FUNCTION: 
void set_pockets_bary(c_lst_pockets *pockets)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Set barycenter of each pockets.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_pockets *pockets: The list of pockets to handle
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void set_pockets_bary(c_lst_pockets *pockets)
{
	node_pocket *cur = NULL ;
	s_pocket *pcur = NULL ;

	float xsum, ysum, zsum ;
	int n ;

	if(pockets && pockets->n_pockets > 0) {
		cur = pockets->first ;
		while(cur) {
			pcur = cur->pocket ;
		// Reset values and calculate barycenter
			xsum = 0.0 ; ysum = 0.0 ; zsum = 0.0 ; 
			n = 0 ;

			node_vertice *nvcur = pcur->v_lst->first ;
			while(nvcur) {
				xsum += nvcur->vertice->x ;
				ysum += nvcur->vertice->y ;
				zsum += nvcur->vertice->z ;
				n ++ ;

				nvcur = nvcur->next ;
			}

			pcur->bary[0] = xsum / (float) n ;
			pcur->bary[1] = ysum / (float) n ;
			pcur->bary[2] = zsum / (float) n ;

			cur = cur->next ;
		}
	}
}


/**-----------------------------------------------------------------------------
   ## FUNCTION:
	s_atm** get_pocket_contacted_atms(s_pocket *pocket, int *natoms)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Get all atoms contacted by the alpha spheres of a given pocket.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_pocket *pocket: The pocket
	@ int *natoms: Number of atoms found (modified)
   -----------------------------------------------------------------------------
   ## RETURN:
	Modify the value of natoms, and return an array of pointer to atoms.
   -----------------------------------------------------------------------------
*/
s_atm** get_pocket_contacted_atms(s_pocket *pocket, int *natoms)
{
	int actual_size = 10,
		nb_atoms = 0,
		i = 0 ;
	
	node_vertice *nvcur = NULL ;
	s_vvertice *vcur = NULL ;

	s_atm **catoms = NULL ;
	
	if(pocket && pocket->v_lst && pocket->v_lst->n_vertices > 0) {
	// Remember atoms already stored.
		int atm_ids[pocket->v_lst->n_vertices * 4] ;	// Maximum nb_vert*4 atoms contacting the pocket ;

	// Do the search 
		catoms = my_malloc(actual_size*sizeof(s_atm*)) ;
		nvcur = pocket->v_lst->first ;
		while(nvcur) {
			vcur = nvcur->vertice ;
			for(i = 0 ; i < 4 ; i++) {
				if(in_tab(atm_ids,  nb_atoms, vcur->neigh[i]->id) == 0) {
					if(nb_atoms >= actual_size) {
						catoms = my_realloc(catoms, (actual_size+10)*sizeof(s_atm**)) ;
						actual_size += 10 ;
					}
	
					atm_ids[nb_atoms] = vcur->neigh[i]->id ;
					catoms[nb_atoms] = vcur->neigh[i] ;
					nb_atoms ++ ;
				}
			}
			nvcur = nvcur->next ;
		}
	}

	*natoms = nb_atoms ;
	
	return catoms ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void set_pocket_score(s_pocket *pok) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Set a score to a given pocket. The current scoring function has been determined
	using a logistic regression based on an analysis of pocket descriptors.

	However, several improvement have to be made to this analysis, and this logistic
	model is far from being predictive enough...
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_pocket *pocket: The pocket
   -----------------------------------------------------------------------------
   ## RETURN:
	The score
   -----------------------------------------------------------------------------
*/
float set_pocket_score(s_pocket *pok) 
{

	double e_tmp = -26.57 + 0.55*(float)pok->pdesc->nb_asph + 
					0.97*(float)pok->pdesc->mean_loc_hyd_dens + 
					8.42e-2*(float)pok->pdesc->mean_asph_ray - 
					1.58e-15*(float)pok->pdesc->masph_sacc - 
					5.85*(float)pok->pdesc->flex - 
					1.49e-16*(float)pok->pdesc->hydrophobicity_score + 
					1.41e-14*(float)pok->pdesc->prop_polar_atm - 
					2.0*(float)pok->pdesc->volume_score + 
					3.03e-16*(float)pok->pdesc->charge_score - 
					0.69*(float)pok->nAlphaApol;

	
	pok->score = e_tmp/(1.0 + e_tmp) ;	
	
	return pok->score ;
}


/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void set_pocket_score2(s_pocket *pok) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Set a score to a given pocket. The current scoring function has been determined
	using a logistic regression based on an analysis of pocket descriptors.

	However, several improvement have to be made to this analysis, and this 
	model is far from being predictive enough...
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_pocket *pocket: The pocket
   -----------------------------------------------------------------------------
   ## RETURN:
	The score
   -----------------------------------------------------------------------------
*/
float set_pocket_score2(s_pocket *pok) 
{
	//score 1
/*
 *	// 70/91
	double e_tmp = -14.835 + 0.29*(float)pok->pdesc->nb_asph +
							 0.52*(float)pok->pdesc->mean_loc_hyd_dens-
							 51.77*(float)pok->pdesc->masph_sacc+
							 10.678*(float)pok->pdesc->mean_asph_ray;
*/
/*	// 72/91
	double e_tmp = -14.835 + 0.56*(float)pok->pdesc->nb_asph +
							 0.72*(float)pok->pdesc->mean_loc_hyd_dens-
							 59.77*(float)pok->pdesc->masph_sacc+
							 6.678*(float)pok->pdesc->mean_asph_ray;
*/
	
	double e_tmp = -14.835 + 0.56*(float)pok->pdesc->nb_asph +
							 0.72*(float)pok->pdesc->mean_loc_hyd_dens-
							 59.77*(float)pok->pdesc->masph_sacc+
							 6.678*(float)pok->pdesc->mean_asph_ray;
	
	pok->score = e_tmp;//e_tmp/(1.0 + e_tmp) ;	//current scoring function
	
	return pok->score ;
}

				// ----------------------------------------------- //
				// SORTING FUNCTION (REARANGE CHAINED LIST TO HAVE //
				// POCKETS SORTED ACCORDING TO SEVERAL PROPERTIES) //
				// ----------------------------------------------- //

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void sort_pockets(c_lst_pockets *pockets, int (*fcmp)(const node_pocket*, const node_pocket*)) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Top function to use to sort pockets. First we copy the chained list of pockets
	in a tab to make the use of indices (and then the quick sort algorithm) possible,
	and then we will sort this tab, and update in the same time the chained list.

	Finally, the given chained list will be modified and sorted using the funvtion
	given in argument.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_pockets *pockets: The list of pockets that will be updated
	@ int (*fcmp)(const node_pocket*, const node_pocket*): Comparison function
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
void sort_pockets(c_lst_pockets *pockets, int (*fcmp)(const node_pocket*, const node_pocket*)) 
{
	size_t npock = 0 ;
	node_pocket **pocks = my_calloc(pockets->n_pockets, sizeof(node_pocket*)) ;
	node_pocket *cur = pockets->first ;

	while(cur && npock < pockets->n_pockets) {
		pocks[npock] = cur ;
		cur = cur->next ;
		npock++ ;
	}
	pock_qsort_rec(pockets, pocks, 0,  npock-1, fcmp) ;

	my_free(pocks) ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	static void pock_qsort_rec(c_lst_pockets *pockets, node_pocket **pocks, int start, int end,  
						  	   int (*fcmp)(const node_pocket*, const node_pocket*) )
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	This function will perform a recursive quick sort on the given tab, updating
	the corresponding chained list.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_pockets *pockets: The list of pockets that will be updated
	@ node_pocket **pocks: Tab that will be sorted
	@ int start: start index of the sort
	@ int end: end index of the sort
	@ int (*fcmp)(const node_pocket*, const node_pocket*): Comparison function
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
static void pock_qsort_rec(c_lst_pockets *pockets, node_pocket **pocks, int start, int end,  
						   int (*fcmp)(const node_pocket*, const node_pocket*) )
{
	if(start < end) {
		int piv = 0 ;
		piv = pock_partition(pockets, pocks, start, end, fcmp) ;
		
		pock_qsort_rec(pockets, pocks, start, piv-1, fcmp) ;
		pock_qsort_rec(pockets, pocks, piv+1, end, fcmp) ;
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	static int pock_partition(c_lst_pockets *pockets, node_pocket **pocks, int start, int end, 
							  int (*fcmp)(const node_pocket*, const node_pocket*))
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Partition fuction used for the quicksort. The comparison between two pockets
	is done with the function given in last argument. The pivot is chosen as the
	first element of the tab. A possible amelioration is to chose it randomly,
	to avoid low performance on already sorted tab...

	The function is sorting a tab of pointer to the pockets, and will update the
	corresponding chained list.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ c_lst_pockets *pockets: The list of pockets that will be updated
	@ node_pocket **pocks: Tab that will be sorted
	@ int start: start index of the sort
	@ int end: end index of the sort
	@ int (*fcmp)(const node_pocket*, const node_pocket*): Comparison function
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
static int pock_partition(c_lst_pockets *pockets, node_pocket **pocks, int start, int end, 
							int (*fcmp)(const node_pocket*, const node_pocket*))
{
	node_pocket *cpock = NULL,
				*tmp = NULL,
				*piv = NULL ;
	
	int c = start,
		i = 0 ;
		
	piv = pocks[start] ;	 // TODO: chose randomly the pivot.
	for(i = start+1 ; i <= end ; i++) {
		cpock = pocks[i] ;
		if( fcmp(cpock, piv) == -1) {
			c++ ;
			if(c != i) {
				swap_pockets(pockets, pocks[c], pocks[i]) ;
				tmp = pocks[c] ;
				pocks[c] = pocks[i] ; pocks[i] = tmp ;
			}
		}
	}
	
	if(c != start) {
		swap_pockets(pockets, pocks[c], piv) ;
		tmp = pocks[c] ;
		pocks[c] = pocks[start] ; pocks[start] = tmp ;
	
	}

	return(c) ;
}
/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int compare_pockets_nasph(const node_pocket *p1, const node_pocket *p2) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Function comparing two pocket on there number of alpha spheres. 
	Uses this for quicksort, or whatever you want...
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ const node_pocket *p1: Pocket 1
	@ const node_pocket *p2: Pocket 2
   -----------------------------------------------------------------------------
   ## RETURN:. 
   -----------------------------------------------------------------------------
*/
int compare_pockets_nasph(const node_pocket *p1, const node_pocket *p2) 
{
	if(p1->pocket->pdesc->nb_asph < p2->pocket->pdesc->nb_asph) return 1 ;
	else return -1 ;
}


/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int compare_pockets_volume(const node_pocket *p1, const node_pocket *p2) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Function comparing two pocket on there volume. Uses this for quicksort, or
	whatever you want...
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ const node_pocket *p1: Pocket 1
	@ const node_pocket *p2: Pocket 2
   -----------------------------------------------------------------------------
   ## RETURN:
	1 if the volume of p2 is greater than the volume of p1, -1 else. 
   -----------------------------------------------------------------------------
*/
int compare_pockets_volume(const node_pocket *p1, const node_pocket *p2) 
{
	if(p1->pocket->pdesc->volume < p2->pocket->pdesc->volume) return 1 ;
	else return -1 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int compare_pockets_score(const node_pocket *p1, const node_pocket *p2) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Function comparing two pocket on there score. Uses this for quicksort, or
	whatever you want...
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ const node_pocket *p1: Pocket 1
	@ const node_pocket *p2: Pocket 2
   -----------------------------------------------------------------------------
   ## RETURN:
	1 if the score of p2 is greater than the score of p1, -1 else. 
   -----------------------------------------------------------------------------
*/
int compare_pockets_score(const node_pocket *p1, const node_pocket *p2) 
{
	if(p1->pocket->score < p2->pocket->score) return 1 ;
	else return -1 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int compare_pockets_corresp(const node_pocket *p1, const node_pocket *p2)
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Function comparing two pocket on there correspondance with the ligan (for the
	test programm). Uses this for quicksort, orwhatever you want...
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ const node_pocket *p1: Pocket 1
	@ const node_pocket *p2: Pocket 2
   -----------------------------------------------------------------------------
   ## RETURN:
	1 if the correspondance of p2 is greater than the correspondance of p1, -1 else. 
   -----------------------------------------------------------------------------
*/
int compare_pockets_corresp(const node_pocket *p1, const node_pocket *p2) 
{
	if(p1->pocket->corresp < p2->pocket->corresp) return 1 ;
	else return -1 ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int compare_pockets_vol_corresp(const node_pocket *p1, const node_pocket *p2)
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Function comparing two pocket on there volume correspondance with the ligan 
	(for the test programm). Uses this for quicksort, orwhatever you want...
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ const node_pocket *p1: Pocket 1
	@ const node_pocket *p2: Pocket 2
   -----------------------------------------------------------------------------
   ## RETURN:
	1 if the volume correspondance of p2 is greater than the volume correspondance 
	of p1, -1 else. 
   -----------------------------------------------------------------------------
*/
int compare_pockets_vol_corresp(const node_pocket *p1, const node_pocket *p2) 
{
	if(p1->pocket->vol_corresp < p2->pocket->vol_corresp) return 1 ;
	else return -1 ;
}

/**
 ================================================================================
 ================================================================================

	OTHER FUNCTIONS (print...)

 ================================================================================
 ================================================================================
*/

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void reset_pocket(s_pocket *pocket)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Reset pocket descriptors
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_pocket pocket: The pocket.
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/

void reset_pocket(s_pocket *pocket)
{
	pocket->rank = -1 ;

	pocket->score = 0.0 ;
	pocket->corresp = -2.0 ;
	pocket->corresp2 = -2.0 ;
	pocket->vol_corresp = -2.0 ;
	pocket->nbv_nbl = -2.0 ;
	pocket->nbv_nbl2 = -2.0 ;
	pocket->apol_surface = 0.0;
	pocket->total_surface = 0.0;
	pocket->curvature = 0.0;
	pocket->drug_score = 0.0;

	pocket->nAlphaPol = 0 ;
	pocket->nAlphaApol = 0 ;

	reset_s_desc(pocket->pdesc) ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void print_pockets(FILE *f, c_lst_pockets *pockets) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Print pockets info in the given file
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ FILE *f: File to print in
	@ s_pocket *pocket: Pocket to print
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void print_pockets(FILE *f, c_lst_pockets *pockets) 
{
	node_pocket *cur = pockets->first ;
	if(pockets) {
		fprintf(f, "\n## FPOCKET RESULTS ##\n");
		cur = pockets->first;
		while(cur) {
			print_pocket(f, cur->pocket) ;
			cur = cur->next ;
		}
	}
	else {
		fprintf(f, "\n## NO POCKETS IN THE LIST ##\n");
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void print_pockets_inv(FILE *f, c_lst_pockets *pockets) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Print pockets info in the given file, starting with the last pocket of the
	chained list.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ FILE *f: File to print in
	@ c_lst_pockets *pockets: Pockets to print
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void print_pockets_inv(FILE *f, c_lst_pockets *pockets) 
{
	if(pockets) {
		fprintf(f, "\n## POCKET ##\n");
		node_pocket *cur = pockets->last ;
		while(cur) {
			print_pocket(f, cur->pocket) ;
			cur = cur->prev ;
		}
	}
	else {
		fprintf(f, "\n## NO POCKETS IN THE LIST ##\n");
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void print_pocket(FILE *f, c_lst_pockets *pockets) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Print one pocket info in the given file
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ FILE *f: File to print in
	@ s_pocket *pocket: Pocket to print
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
void print_pocket(FILE *f, s_pocket *pocket) 
{
	if(pocket) {
		fprintf(f, "\n## POCKET %d ##\n",pocket->v_lst->first->vertice->resid);
		
		if(pocket->corresp > -1.0) fprintf(f, "\t Correspondance: %f\n", pocket->corresp) ;
		if(pocket->vol_corresp > -1.0) fprintf(f, "\t Volume Correspondance: %f\n", pocket->vol_corresp) ;
		fprintf(f, "\t 0  - Pocket Score:                %.4f\n", pocket->score) ;
		fprintf(f, "\t 1  - Number of Voronoi vertices:  %d\n", pocket->pdesc->nb_asph) ;
		fprintf(f, "\t 2  - Mean alpha-sphere radius:    %f\n", pocket->pdesc->mean_asph_ray) ;
		fprintf(f, "\t 3  - Mean alpha-sphere solvent accessibility: %f\n", pocket->pdesc->masph_sacc) ;
		fprintf(f, "\t 4  - Flexibility:                 %f\n", pocket->pdesc->flex) ;
		fprintf(f, "\t 5  - Hydrophobicity Score:        %f\n", pocket->pdesc->hydrophobicity_score) ;
		fprintf(f, "\t 6  - Polarity Score:              %d\n", pocket->pdesc->polarity_score) ;
		fprintf(f, "\t 7  - Volume Score:                %f\n", pocket->pdesc->volume_score) ;
		fprintf(f, "\t 8  - Real volume (approximation): %f\n", pocket->pdesc->volume) ;
		fprintf(f, "\t 9  - Charge Score:                %d\n", pocket->pdesc->charge_score) ;
		fprintf(f, "\t 10 - Local hydrophobic density Score: %f\n", pocket->pdesc->mean_loc_hyd_dens) ;
		fprintf(f, "\t 11 - Number of apolar alpha sphere: %d\n", pocket->nAlphaApol) ;
		fprintf(f, "\t 12 - Amino acid composition:\n") ;
		int i ;
		fprintf(f, "\t 12 - ") ;
		for(i = 0 ; i < M_NB_AA ; i++) fprintf(f, "%3d ", pocket->pdesc->aa_compo[i]) ;
		fprintf(f, "\n") ;
	}
}

