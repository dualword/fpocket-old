#include "../headers/asa.h"



/**
   ## FUNCTION:
	atom_not_in_list

   ## SPECIFICATION:
	This function checks if the atom a is in the list of atoms "atoms"

   ## PARAMETRES:
	@ s_atm *a : The query atom,
	@ s_atm **atoms : The set of atoms to test
        @ int natoms : number of atoms in the set of atoms

   ## RETURN:
	1 if a is in atoms, 0 if not
 */
int atom_not_in_list(s_atm *a,s_atm **atoms,int natoms){
    int i;
    if(atoms){
        for(i=0;i<natoms;i++){
            if(atoms[i]==a) return 0;
        }
    }
    return 1;
}


int *get_surrounding_atoms_idx(s_vvertice **tvert,int nvert,s_pdb *pdb, int *n_sa){
    s_atm *a=NULL;
    int *sa=NULL;
    int i,z,flag=0;
    *n_sa=0;
    for(i=0;i<pdb->natoms;i++){
        a=pdb->latoms_p[i];
        //consider only heavy atoms for vdw incr.
        if(strncmp(a->symbol,"H",1)){
            flag=0;
            for(z=0;z<nvert && !flag;z++){
                //flag=atom_not_in_list(a,sa,*n_sa);
                flag=in_tab(sa,*n_sa,i);
                if(!flag && ddist(a->x,a->y,a->z,tvert[z]->x,tvert[z]->y,tvert[z]->z)<(tvert[z]->ray+M_PADDING)*(tvert[z]->ray+M_PADDING)){
                    *n_sa=*n_sa+1;
                    if(sa==NULL){
                        sa=(int *)malloc(sizeof(int));
                        sa[*n_sa-1]=i;
                    }
                    else {
                        sa=(int *)realloc(sa,sizeof(int)*(*n_sa));
                        sa[*n_sa-1]=i;
                    }
                }
            }
        }
    }
    return sa;
}


s_atm **get_unique_atoms_DEPRECATED(s_vvertice **tvert,int nvert, int *n_ua){
    s_atm *a=NULL;
    s_atm **ua=NULL;
    int z,j;
    *n_ua=0;

    s_vvertice *vcur = NULL ;
    s_atm **neighs = NULL ;
/*
    fprintf(stdout, "\nIn get unique atom\n") ;
*/
    for(z=0;z<nvert;z++){
        vcur = tvert[z] ;
/*
        fprintf(stdout, "%f %i\n", vcur->ray, vcur->id) ;
        fprintf(stdout, "%f %i\n", tvert[z]->ray, tvert[z]->id) ;
*/

        neighs = tvert[z]->neigh ;
        for(j=0;j<4;j++){
            a=neighs[j];
            if(atom_not_in_list(a,ua,*n_ua)){
                *n_ua=*n_ua+1;
                if(ua==NULL){
                    ua=(s_atm **)malloc(sizeof(s_atm*));
                    ua[*n_ua-1]=(s_atm *)malloc(sizeof(s_atm));
                    ua[*n_ua-1]=a;
                }
                else {
                    ua=(s_atm **)realloc(ua,sizeof(s_atm*)*(*n_ua));
                    ua[*n_ua-1]=(s_atm *)malloc(sizeof(s_atm));
                    ua[*n_ua-1]=a;
                }
            }
        }
    }
    return ua;
}


int *get_unique_atoms(s_vvertice **tvert,int nvert, int *n_ua,s_atm **atoms,int na){
    s_atm *a=NULL;
    int *ua=NULL;
    int z,j;
    int ca_idx;
    *n_ua=0;
    int flag=0;

    s_vvertice *vcur = NULL ;
    s_atm **neighs = NULL ;
/*
    fprintf(stdout, "\nIn get unique atom\n") ;
*/
    for(z=0;z<nvert;z++){
        vcur = tvert[z] ;
/*
        fprintf(stdout, "%f %i\n", vcur->ray, vcur->id) ;
        fprintf(stdout, "%f %i\n", tvert[z]->ray, tvert[z]->id) ;
*/

        neighs = tvert[z]->neigh ;
        for(j=0;j<4;j++){
            a=neighs[j];
            flag=in_tab(ua,*n_ua,a->id);
            if(!flag){
                *n_ua=*n_ua+1;
                if(ua==NULL){
                    ua=(int *)malloc(sizeof(int));
                    if(a->id-1 < na && a==atoms[a->id-1]) ua[*n_ua-1]=a->id-1;
                    else {
                        for(ca_idx=0;ca_idx<na;ca_idx++){
                            if(a==atoms[ca_idx]) {
                                ua[*n_ua-1]=ca_idx;
                                break;
                            }
                        }
                    }

                    //memcpy(ua[*n_ua-1],a,sizeof(a));
                    
                }
                else {
                    ua=(int *)realloc(ua,sizeof(int)*(*n_ua));
                    if(a->id-1 < na && a==atoms[a->id-1]) ua[*n_ua-1]=a->id-1;
                    else {
                        for(ca_idx=0;ca_idx<na;ca_idx++){
                            if(a==atoms[ca_idx]) {
                                ua[*n_ua-1]=ca_idx;
                                break;
                            }
                        }
                    }
                    //memcpy(ua[*n_ua-1],a,sizeof(a));
                }
            }
        }
    }

    return ua;
}

void set_ASA(s_desc *desc,s_pdb *pdb, s_vvertice **tvert,int nvert){
    desc->surf_pol_vdw14=0.0;
    desc->surf_apol_vdw14=0.0;
    desc->surf_vdw14=0.0;
    int *sa=NULL;    /*surrounding atoms container*/
    int *ua=NULL;    /*unique atoms contacting vvertices*/
    int n_sa;
    int n_ua;
    int i;
    sa=get_surrounding_atoms_idx(tvert,nvert,pdb, &n_sa);
    ua=get_unique_atoms(tvert,nvert, &n_ua,pdb->latoms_p,pdb->natoms);
    s_atm *a,*cura;
    float *curpts=NULL,tz,tx,ty,dsq,area;
    int j=0,iv,k,burried,nnburried,vidx,vrefburried;
    curpts=get_points_on_sphere(M_NSPIRAL);
    for(i=0;i<n_ua;i++){
        cura=pdb->latoms_p[ua[i]];
        nnburried=0;
        
        for(k=0;k<M_NSPIRAL;k++){
            burried=0;
            j=0;
            tx=cura->x+curpts[3*k]*(cura->radius+M_PROBE_SIZE);
            ty=cura->y+curpts[3*k+1]*(cura->radius+M_PROBE_SIZE);
            tz=cura->z+curpts[3*k+2]*(cura->radius+M_PROBE_SIZE);
            vrefburried=1;
            for(iv=0;iv<nvert;iv++){
                for(vidx=0;vidx<4;vidx++){
                    if(cura==tvert[iv]->neigh[vidx]){
                        if(ddist(tx,ty,tz,tvert[iv]->x,tvert[iv]->y,tvert[iv]->z)<=tvert[iv]->ray*tvert[iv]->ray) vrefburried=0;
                    }
                }
            }
            while(!burried && j< n_sa && !vrefburried){
                a=pdb->latoms_p[sa[j]];
                if(a!=cura){
                    dsq=ddist(tx,ty,tz,a->x,a->y,a->z);
                    if(dsq<(a->radius+M_PROBE_SIZE)*(a->radius+M_PROBE_SIZE)) burried=1;
                }
                j++;
            }
            if(!burried && !vrefburried) {
                nnburried++;
            }
        }
        area=(4*PI*(cura->radius+M_PROBE_SIZE)*(cura->radius+M_PROBE_SIZE)/(float)M_NSPIRAL)*nnburried;
        if(cura->electroneg<2.8) desc->surf_apol_vdw14=desc->surf_apol_vdw14+area;
        else desc->surf_pol_vdw14=desc->surf_pol_vdw14+area;
        desc->surf_vdw14=desc->surf_vdw14+area;
        
    }
    free(curpts);
    curpts=NULL;
   /* for(i=0;i<n_ua-1;i++){
        free(ua[i]);
    }*/
    free(ua);

    free(sa);

    sa=NULL;
    ua=NULL;
}

/**
   ## FUNCTION:
	get_points_on_sphere

   ## SPECIFICATION:
	This function takes an integer as argument and returns a unit sphere
        with distributed points on its surface according to the Golden Section
        Spiral Distribution

   ## PARAMETRES:
	@ int nop : number of points to create on the unit sphere (resolution of the surface)

   ## RETURN:
	float* : list of points on the unit sphere

*/

float *get_points_on_sphere(int nop) {
    float *pts =NULL;
    pts=(float *) malloc(sizeof (float) * nop * 3);
    float inc = PI * (3.0 - sqrt(5.0));
    float off = 2.0 / (float) nop;
    float y, r, phi;
    int k;
    for (k = 0; k < nop; k++) {
        y = ((float) k) * off - 1.0 + (off / 2.0);
        r = sqrt(1.0 - y * y);
        phi = k * inc;
        pts[3 * k] = cos(phi) * r;
        pts[3 * k + 1] = y;
        pts[3 * k + 2] = sin(phi) * r;
    }
    return pts;
}


