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


