#include "../headers/mdpout.h"



void write_md_grid(s_mdgrid *g, FILE *f, FILE *fiso,s_mdparams *par,float isovalue)
{
    int cx,cy,cz;
    float cv;
    float rx,ry,rz;
    size_t cnt=0;
    /*write the header of the dx file*/
    fprintf(f,"# Data calculated by mdpocket, part of the fpocket package\n");
    fprintf(f,"# This is a standard DX file of occurences of cavities within MD trajectories.\n");
    fprintf(f,"# The file can be visualised using the freely available VMD software\n");
    fprintf(f,"# fpocket parameters used to create this dx file : \n");
    fprintf(f,"# \t-m %2.f (min alpha sphere size) -M %.2f (max alpha sphere size)\n",par->fpar->asph_min_size, par->fpar->asph_max_size);
    fprintf(f,"# \t-i %d (min number of alpha spheres per pocket)\n",par->fpar->min_pock_nb_asph);
    fprintf(f,"# \t-D %.2f (Max distance for 1st clustering algo)\n#\t-r %.2f (Max dist for 2nd clustering algo)\n#\t-s %.2f (Max dist for third clustering algo)\n",par->fpar->clust_max_dist, par->fpar->refine_clust_dist, par->fpar->sl_clust_max_dist);
    fprintf(f,"# \t-n %d (Min neighbour atoms for multi linkage clustering)\n",par->fpar->sl_clust_min_nneigh);
    if(par->flag_scoring) fprintf(f,"# \t-S (Map drug score to density map!)\n");
    fprintf(f,"object 1 class gridpositions counts %d %d %d\n",g->nx,g->ny,g->nz);
    fprintf(f,"origin %.2f %.2f %.2f\n",g->origin[0],g->origin[1],g->origin[2]);
    fprintf(f,"delta %.2f 0 0\n",g->resolution);
    fprintf(f,"delta 0 %.2f 0\n",g->resolution);
    fprintf(f,"delta 0 0 %.2f\n",g->resolution);
    fprintf(f,"object 2 class gridconnections counts %d %d %d\n",g->nx,g->ny,g->nz);
    fprintf(f,"object 3 class array type double rank 0 items %d data follows\n",g->nx*g->ny*g->nz);
    int i=0;
    for(cx=0;cx<g->nx;cx++){
        for(cy=0;cy<g->ny;cy++){
            for(cz=0;cz<g->nz;cz++){
                if(i==3) {
                    i=0;
                    fprintf(f,"\n");
                }
                cv=g->gridvalues[cx][cy][cz];
                fprintf(f,"%.3f ",cv);
                if(cv>=isovalue){
                    cnt++;
                    rx=g->origin[0]+cx*g->resolution;
                    ry=g->origin[1]+cy*g->resolution;
                    rz=g->origin[2]+cz*g->resolution;
                    fprintf(fiso,"ATOM  %5d  C   PTH     1    %8.3f%8.3f%8.3f%6.2f%6.2f\n",(int)cnt,rx,ry,rz,0.0,0.0);
                }
                i++;
            }
        }
    }
}

void write_md_pocket_atoms(FILE *f,int *ids,s_pdb *prot, int nids, int sn){
    s_atm *cura;
    int i,j,flag;
    i=0;
    j=0;
    fprintf(f,"MODEL        %d\n",sn);
    for(i=0;i<nids;i++){
        flag=0;
        while(flag==0 && j<prot->natoms){
            cura=prot->latoms_p[j];
            if(cura->id==ids[i]){
                flag=1;
                write_pdb_atom_line(f, "ATOM", cura->id, cura->name,
						 cura->pdb_aloc, cura->res_name, cura->chain,
						 cura->res_id, cura->pdb_insert, cura->x, cura->y, cura->z, cura->occupancy,
						 cura->bfactor,	cura->symbol, cura->charge);
            }
            j++;
        }
    }
    fprintf(f,"ENDMDL\n");
    
}



void write_first_bfactor_density(FILE *f,s_pdb *prot){
    s_atm *cura;
    int i;
    //fprintf(f,"MODEL        %d\n",sn);
    for(i=0;i<prot->natoms;i++){
        cura=prot->latoms_p[i];
        write_pdb_atom_line(f, "ATOM", cura->id, cura->name,
						 cura->pdb_aloc, cura->res_name, cura->chain,
						 cura->res_id, cura->pdb_insert, cura->x, cura->y, cura->z, cura->occupancy,
						 cura->bfactor,	cura->symbol, cura->charge);
    }
    fprintf(f,"TER\n");
    fprintf(f,"END\n");

}
