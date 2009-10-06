#include "../headers/mdpmem.h"


s_mdconcat *init_md_concat(void){
    s_mdconcat *m=(s_mdconcat *) my_malloc(sizeof(s_mdconcat));
    m->n_vertpos=0;
    m->vertpos=NULL;
    m->n_snapshots=0;
    return m;
}


/**
   ## FUNCTION:
	init_md_grid

   ## SPECIFICATION:
	Initialize the md grid (allocate + 0)

   ## PARAMETRES:
	@ s_mdconcat *mdc: Pointer to the mdconcat structure (vertices),

   ## RETURN:
	s_mdgrid * : the grid

*/
s_mdgrid *init_md_grid(s_mdconcat *mdc){
    s_mdgrid *g=(s_mdgrid *)my_malloc(sizeof(s_mdgrid));
    float xmax=float_get_max_in_2D_array(mdc->vertpos,mdc->n_vertpos, 0);
    float ymax=float_get_max_in_2D_array(mdc->vertpos,mdc->n_vertpos, 1);
    float zmax=float_get_max_in_2D_array(mdc->vertpos,mdc->n_vertpos, 2);
    float xmin=float_get_min_in_2D_array(mdc->vertpos,mdc->n_vertpos, 0);
    float ymin=float_get_min_in_2D_array(mdc->vertpos,mdc->n_vertpos, 1);
    float zmin=float_get_min_in_2D_array(mdc->vertpos,mdc->n_vertpos, 2);
    int cx,cy,cz;
    float span=(M_MDP_CUBE_SIDE/2.0)/M_MDP_GRID_RESOLUTION;
    g->resolution=M_MDP_GRID_RESOLUTION;

    g->nx=1+(int)(xmax+2*span-xmin)/(g->resolution);
    g->ny=1+(int)(ymax+2*span-ymin)/(g->resolution);
    g->nz=1+(int)(zmax+2*span-zmin)/(g->resolution);

    g->gridvalues=(float ***)my_malloc(sizeof(float **)*g->nx);
    for(cx=0;cx<g->nx;cx++){
        g->gridvalues[cx]=(float **)my_malloc(sizeof(float *)*g->ny);
        for(cy=0;cy<g->ny;cy++){
            g->gridvalues[cx][cy]=(float *)my_malloc(sizeof(float *)*g->nz);
            for(cz=0;cz<g->nz;cz++){
                g->gridvalues[cx][cy][cz]=0.0;
            }
        }
    }

    g->origin=(float *)my_malloc(sizeof(float)*3);
    g->origin[0]=xmin-span;
    g->origin[1]=ymin-span;
    g->origin[2]=zmin-span;
    g->n_snapshots=mdc->n_snapshots;
    return g;
}


/**
   ## FUNCTION:
	alloc_first_md_concat

   ## SPECIFICATION:
	Allocate memory for the md concat structure (first snapshot)

   ## PARAMETRES:
	@ s_mdconcat *m: Pointer to the structure to free,
        @ size_t n: Number of vertices in the snapshot to add to m

   ## RETURN:
	void

*/
void alloc_first_md_concat(s_mdconcat *m,size_t n){
    size_t z;
    m->vertpos=(float **) my_malloc(sizeof(float *)*n);
    for(z=0;z<n;z++){
        m->vertpos[z]=(float *)my_malloc(sizeof(float)*4);
        m->vertpos[z][0]=0.0;
        m->vertpos[z][1]=0.0;
        m->vertpos[z][2]=0.0;
        m->vertpos[z][3]=0.0;
    }
    m->n_vertpos=n;
}

/**
   ## FUNCTION:
	realloc_md_concat

   ## SPECIFICATION:
	Reallocate memory for the md concat structure (to add a new snapshot)

   ## PARAMETRES:
	@ s_mdconcat *m: Pointer to the structure to free,
        @ size_t n: Number of vertices in the snapshot to add to m

   ## RETURN:
	void

*/
void realloc_md_concat(s_mdconcat *m,size_t n){
    size_t z;
    m->vertpos=(float **) my_realloc(m->vertpos,sizeof(float *)*(m->n_vertpos+n));
    for(z=0;z<n;z++){
        m->vertpos[m->n_vertpos+z]=(float *)my_malloc(sizeof(float)*4);
        m->vertpos[m->n_vertpos+z][0]=0.0;
        m->vertpos[m->n_vertpos+z][1]=0.0;
        m->vertpos[m->n_vertpos+z][2]=0.0;
        m->vertpos[m->n_vertpos+z][3]=0.0;
    }
    m->n_vertpos+=n;
}

/**
   ## FUNCTION:
	free_mdconcat

   ## SPECIFICATION:
	Free the mdconcat structure

   ## PARAMETRES:
	@ s_mdconcat *m: Pointer to the structure to free

   ## RETURN:
	void

*/
void free_mdconcat(s_mdconcat *m){
    size_t i;
    for(i=0;i<m->n_vertpos;i++){
        my_free(m->vertpos[i]);
    }
    my_free(m->vertpos);
    my_free(m);
}


