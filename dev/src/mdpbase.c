#include "../headers/mdpbase.h"



/**
   ## FUNCTION:
	store_vertice_positions

   ## SPECIFICATION:
	Store vertice positions of current pockets in the mdconcat structure

   ## PARAMETRES:
	@ s_mdconcat *m: Pointer to the mdconcat structure (vertices),
        @ c_lst_pockets *pockets : Pointer to all pockets found by fpocket

   ## RETURN:
	void

*/
void store_vertice_positions(s_mdconcat *m,c_lst_pockets *pockets){
    if(pockets){
        int z;
        int n=pockets->vertices->nvert;
        size_t old_n=m->n_vertpos;
        /*if there a no vertices in m before, first allocate some space*/
        if(m->n_vertpos==0) alloc_first_md_concat(m,n);
        else realloc_md_concat(m,n);    /*else, reallocate the object size*/
        for(z=0;z<n;z++){               /*loop over all vertices*/
            /*store the positions and radius of the vertices*/
            m->vertpos[old_n+z][0]=pockets->vertices->vertices[z].x;
            m->vertpos[old_n+z][1]=pockets->vertices->vertices[z].y;
            m->vertpos[old_n+z][2]=pockets->vertices->vertices[z].z;
            m->vertpos[old_n+z][3]=pockets->vertices->vertices[z].ray;
        }
    }
}



/**
   ## FUNCTION:
	calculate_md_grid

   ## SPECIFICATION:
	Do the actual grid calculation (count number of vertices near a grid point)

   ## PARAMETRES:
	@ s_mdconcat *mdc: Pointer to the mdconcat structure (vertices),

   ## RETURN:
	void

*/
s_mdgrid *calculate_md_grid(s_mdconcat *mdc){
    s_mdgrid *g=init_md_grid(mdc);          /*initialize the md grid, memory allocation*/
    size_t vidx;
    int xidx,yidx,zidx;                     /*direct indices of the positions in the grid*/
    int sxidx,syidx,szidx;                  /*indices nearby xidx,yidx,zidx that are within M_MDP_CUBE_SIDE of this point*/
    int sxi,syi,szi;
    float vx,vy,vz;
    int s=(int)(((float)M_MDP_CUBE_SIDE/2.0)/g->resolution);    /*possible stepsize (resolution dependent) in the discrete grid*/
    /*loop over all known vertices and CALCULATE the grid positions and increment grid values by 1*/
    /*important : no distance calculations are done here, thus this routine is very fast*/
    for(vidx=0;vidx<mdc->n_vertpos;vidx++){
        vx=mdc->vertpos[vidx][0];                               /*tmp the vertice position*/
        vy=mdc->vertpos[vidx][1];
        vz=mdc->vertpos[vidx][2];
        xidx=(int)roundf((vx-g->origin[0])/g->resolution);      /*calculate the nearest grid point internal coordinates*/
        yidx=(int)roundf((vy-g->origin[1])/g->resolution);
        zidx=(int)roundf((vz-g->origin[2])/g->resolution);
        /*we use a cube of M_MDP_CUBE_SIDE**3 volume to check if there are grid points nearby,
         *thus a check in the neighbourhood is performed here...this looks a bit awful and should be changed later on*/
        for(sxi=-s;sxi<=s;sxi++){
            for(syi=-s;syi<=s;syi++){
                for(szi=-s;szi<=s;szi++){
                    /*check if we are not on the point xidx,yidx, zidx....the square sum is just a little trick*/
                    if((sxi*sxi+syi*syi+szi*szi)!=0){
                        /*next we have to check in a discrete manner if we can include a new grid point or not.*/
                        if(sxi<0) sxidx=(int)ceil(((float)sxi+vx-g->origin[0])/g->resolution);
                        else if(sxi>0) sxidx=(int)floor(((float)sxi+vx-g->origin[0])/g->resolution);
                        else if(sxi==0) sxidx=xidx;
                        if(syi<0) syidx=(int)ceil(((float)syi+vy-g->origin[1])/g->resolution);
                        else if(syi>0) syidx=(int)floor(((float)syi+vy-g->origin[1])/g->resolution);
                        else if(syi==0) syidx=yidx;
                        if(szi<0) szidx=(int)ceil(((float)szi+vz-g->origin[2])/g->resolution);
                        else if(szi>0) szidx=(int)floor(((float)szi+vz-g->origin[2])/g->resolution);
                        else if(szi==0) szidx=zidx;
                        /*double check if we are not on the already incremented grid point*/
                        if((sxidx!=xidx) || (syidx!=yidx) || (szidx!=zidx)) g->gridvalues[sxidx][syidx][szidx]+=1.0;
                    }
                }
            }
        }
        g->gridvalues[xidx][yidx][zidx]+=1.0;   /*increment the already known grid point value*/
    }
    return g;
}


/**
   ## FUNCTION:
	init_md_concat

   ## SPECIFICATION:
	Initialize the md concat (alloc)

   ## PARAMETRES:
	void

   ## RETURN:
	s_mdconcat * : the md concat structure

*/

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
    /*get max and min positions of vertices in the md concat structure*/
    float xmax=float_get_max_in_2D_array(mdc->vertpos,mdc->n_vertpos, 0);
    float ymax=float_get_max_in_2D_array(mdc->vertpos,mdc->n_vertpos, 1);
    float zmax=float_get_max_in_2D_array(mdc->vertpos,mdc->n_vertpos, 2);
    float xmin=float_get_min_in_2D_array(mdc->vertpos,mdc->n_vertpos, 0);
    float ymin=float_get_min_in_2D_array(mdc->vertpos,mdc->n_vertpos, 1);
    float zmin=float_get_min_in_2D_array(mdc->vertpos,mdc->n_vertpos, 2);
    int cx,cy,cz;
    float span=(M_MDP_CUBE_SIDE/2.0)/M_MDP_GRID_RESOLUTION; /*calculate a margin*/
    g->resolution=M_MDP_GRID_RESOLUTION;

    g->nx=1+(int)(xmax+2*span-xmin)/(g->resolution);        /*set the number of grid points in the x dimension*/
    g->ny=1+(int)(ymax+2*span-ymin)/(g->resolution);        /*set the number of grid points in the y dimension*/
    g->nz=1+(int)(zmax+2*span-zmin)/(g->resolution);        /*set the number of grid points in the z dimension*/

    /*alloc the grid in the x dimension*/
    g->gridvalues=(float ***)my_malloc(sizeof(float **)*g->nx);
    for(cx=0;cx<g->nx;cx++){
        /*alloc the grid in the y dimension*/
        g->gridvalues[cx]=(float **)my_malloc(sizeof(float *)*g->ny);
        for(cy=0;cy<g->ny;cy++){
            /*alloc the grid in the z dimension*/
            g->gridvalues[cx][cy]=(float *)my_malloc(sizeof(float *)*g->nz);
            for(cz=0;cz<g->nz;cz++){
                g->gridvalues[cx][cy][cz]=0.0;  /*init grid values to 0.0*/
            }
        }
    }

    g->origin=(float *)my_malloc(sizeof(float)*3);  /*alloc origin of the grid in cartesian space*/
    /*set grid origin to cartesian min - margin*/
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




