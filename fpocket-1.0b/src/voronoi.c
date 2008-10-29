 
#include "../headers/voronoi.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					voronoi.c
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## ----- SPECIFICATIONS
## ----- MODIFICATIONS HISTORY
##
##	01-04-08	(v)  Added template for comments and creation of history
##	21-02-08	(p)	 Adding support for proteins with hydrogens
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
##

*/

/*
 
 */


static void fill_vvertices(s_lst_vvertice *lvvert, const char fpath[], s_atm *atoms, int natoms, 
						   int min_apol_neigh, float asph_min_size, float asph_max_size) ;

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	s_lst_vvertice* load_vvertices(s_lst_atoms *latoms) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Calculate voronoi vertices using an ensemble of atoms, and then load resulting
	vertices into a s_lst_vvertice structure. The function call an external 
	programm qvoronoi, part of qhull programme which can be download at:
		http://www.qhull.org/download/
	or installed with apt-get install qhull-bin
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_lst_atoms *latoms: List of atoms to use to calculate voronoi vertices
   -----------------------------------------------------------------------------
   ## RETURN:
	The structure containing list of vertices.
   -----------------------------------------------------------------------------
*/
s_lst_vvertice* load_vvertices(s_pdb *pdb, int min_apol_neigh, float asph_min_size, float asph_max_size)
{
	int i,nb_h=0;
	s_atm *ca = NULL ;
	s_lst_vvertice *lvvert = NULL ;

	FILE *fvoro = fopen("voro_tmp.dat", "w");
/* 	lvvert->h_tr=(int *)my_malloc(sizeof(int));*/
	
	
	
	if(fvoro != NULL) { 
		lvvert = my_malloc(sizeof(s_lst_vvertice)) ;
		lvvert->h_tr=NULL;
		//loop a first time to get out how many heavy atoms are in the file
		for(i = 0; i <  pdb->natoms ; i++){
			ca = (pdb->latoms)+i ;
			if(strcmp(ca->symbol,"H")) {
				lvvert->h_tr=my_realloc(lvvert->h_tr,sizeof(int)*(i-nb_h+1)) ;
				lvvert->h_tr[i-nb_h]=i ;
			}
			else nb_h++;
		}
		lvvert->n_h_tr=i-nb_h;
		
		//write the header for qvoronoi
		fprintf(fvoro,"3 rbox D3\n%d\n", lvvert->n_h_tr);
		//loop a second time for the qvoronoi input coordinates
		for(i = 0; i <  pdb->natoms ; i++){
			ca = (pdb->latoms)+i ;
			if(strcmp(ca->symbol,"H")) {
				fprintf(fvoro,"%f %f %f \n", ca->x, ca->y, ca->z);	//only if this is a heavy atom export it for voronoi tesselation, else discard it
			}
		}

		fflush(fvoro) ;
		fclose(fvoro) ;

		int status = system("cat voro_tmp.dat | qvoronoi p i Pp Fn > voro.tmp") ;

		if(status == M_VORONOI_SUCCESS) {
			
			fill_vvertices(lvvert, "voro.tmp", pdb->latoms, pdb->natoms, min_apol_neigh, asph_min_size, asph_max_size);
		}
		else {
			my_free(lvvert);
			lvvert = NULL ;
			fprintf(stderr, "! Voronoi command failed with status %d...\n", status) ;
		}
	}
	else {
		fprintf(stderr, "! File for Voronoi vertices calculation couldn't be opened...\n") ;
	}

	return lvvert ;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void fill_vertices(s_lst_vvertice *lvvert, const char fpath[],s_lst_atoms *latoms,int last_resid) 
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_lst_vvertice *lvvert: The structure to fill
	@ const char fpath[]: File containing vertices
	@ s_atm *atoms: List of atoms
	@ int natoms: Number of atoms
	@ s_fparams *params: Parameters
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Fill structure given in argument (must have been allocated) using a file
	containing vertice coordinates and neighbours using p i options of qhull.
   -----------------------------------------------------------------------------
   ## RETURN:
	void
   -----------------------------------------------------------------------------
*/
static void fill_vvertices(s_lst_vvertice *lvvert, const char fpath[], s_atm *atoms, int natoms, 
						   int min_apol_neigh, float asph_min_size, float asph_max_size) 
{
	FILE *f = NULL ;	//file handler for vertices coordinates
	FILE *fNb = NULL ;	//file handler for vertices atomic neighbours
	FILE *fvNb = NULL;	//file handler for vertices vertice neighbours
	
	s_vvertice *v = NULL ;
	
	float tmpRay;		//temporary Ray of voronoi vertice (ray of alpha sphere)
	float xyz[3] = {0,0,0};

	int i, j,nchar_max = 255,
		vInMem = 0,	//saved vertices counter
		curLineNb = 0,	//current line number
		trash = 0,
		tmpApolar=0,
		curVnbIdx[4],	//current vertice neighbours
		curNbIdx[4];	//current atomic neighbours

	char cline[nchar_max],
		 nbline[nchar_max],
		 vNbline[nchar_max],
		 *s_nvert = (char *) my_malloc(sizeof(char)*nchar_max) ;
	
// Once we have the number of lines, lets allocate memory and get the lines
	
	f = fopen(fpath,"r") ;
	fNb = fopen(fpath,"r") ;
	fvNb = fopen(fpath,"r") ;

	fgets(cline, nchar_max, f) ;				// Skip first line
	fgets(cline, nchar_max, f) ;				// Load second line containing number of coordinates.
	fgets(nbline, nchar_max, fNb) ;				// Skip first line
	fgets(nbline, nchar_max, fNb) ;				// Load second line containing number of coordinates.
	fgets(vNbline, nchar_max, fvNb) ;			// Skip first line
	fgets(vNbline, nchar_max, fvNb) ;			// Load second line containing number of coordinates.

 	sscanf(cline,"%d",&(lvvert->nvert)) ;
	lvvert->qhullSize = lvvert->nvert ;
	lvvert->tr = (int *) my_malloc(lvvert->nvert*sizeof(int));
	for(i = 0 ; i < lvvert->nvert ; i++) lvvert->tr[i] = -1;		// Initialize corres to -1
	
 	lvvert->vertices = my_calloc(lvvert->nvert, sizeof(s_vvertice)) ;
	
	// Get the string of number of vertices to read, to look up the neighbour list from qhull
	sprintf(s_nvert,"%d",lvvert->nvert);
	strcat(s_nvert,"\n");
	
	//Advance cursor to neighbour list
	while(fgets(nbline, nchar_max, fNb) != NULL && strcmp(s_nvert, nbline) != 0) ;	
	// Advance cursor to the vertice neighbour list
	while(fgets(vNbline,nchar_max,fvNb) != NULL && curLineNb++ < lvvert->nvert*2+1) ;

	i = 0 ;
	while(fgets(cline, nchar_max, f) != NULL) {	//read vertice positions
 		if(fgets(nbline, nchar_max,fNb)!=NULL){				//read neighbours
			if(fgets(vNbline, nchar_max,fvNb)!=NULL){				//read vertice neighbour vertices
				if(strcmp("\n", cline) != 0 && strcmp("\n", nbline) != 0 && strcmp("\n", vNbline) != 0) {
					sscanf(cline,"%f %f %f",&xyz[0], &xyz[1], &xyz[2]);
					sscanf(nbline,"%d %d %d %d",&curNbIdx[0],&curNbIdx[1],&curNbIdx[2],&curNbIdx[3]);
 					sscanf(vNbline,"%d %d %d %d %d", &trash, &curVnbIdx[0], &curVnbIdx[1], 
													 &curVnbIdx[2], &curVnbIdx[3]);
				// Test voro. vert. for alpha sphere cond. and returns ray if cond. are ok, -1 else
					tmpRay = testVvertice(xyz, curNbIdx, atoms, asph_min_size, asph_max_size,lvvert);	
					if(tmpRay > 0){
						v = (lvvert->vertices + vInMem) ;
						v->x = xyz[0]; v->y = xyz[1]; v->z = xyz[2];
						v->ray = tmpRay;
						v->sort_x = -1 ;
						tmpApolar=0;
						for(j = 0 ; j < 4 ; j++) {
							v->neigh[j] = &(atoms[lvvert->h_tr[curNbIdx[j]]]);
							if(atoms[lvvert->h_tr[curNbIdx[j]]].electroneg<2.8) tmpApolar++;	//if this is a C or S atom - use electronegativity instead !!
							if(curVnbIdx[j]>0) v->vneigh[j] = curVnbIdx[j];
						}
						v->apol_neighbours=0;
						lvvert->tr[i] = vInMem ;
						
						vInMem++ ;		//vertices actually read
						v->id = natoms+i+1-vInMem ;
						if(tmpApolar >= min_apol_neigh) v->type = M_APOLAR_AS;
						else v->type = M_POLAR_AS;
						v->qhullId = i;		//set index in the qhull file
						v->resid = -1;		//initialize internal index
						set_barycenter(v) ;	//set barycentre
					}
					i++ ;
				}
			}
 		}
	}

	lvvert->nvert=vInMem ;
	fclose(f) ;
	fclose(fNb) ;
	fclose(fvNb);
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void set_barycenter(s_vvertice *v) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Set barycenter of a vertice using it's 4 contacting atoms.
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ s_vvertice *v: The vertice
   -----------------------------------------------------------------------------
   ## RETURN: void
   -----------------------------------------------------------------------------
*/
void set_barycenter(s_vvertice *v) 
{
	int i ;
	float xsum = 0.0, 
		  ysum = 0.0,
		  zsum = 0.0 ;

	for(i = 0 ; i < 4 ; i++) {
		xsum += v->neigh[i]->x ;
		ysum += v->neigh[i]->y ;
		zsum += v->neigh[i]->z ;
	}

	v->bary[0] = xsum*0.25 ;
	v->bary[1] = ysum*0.25 ;
	v->bary[2] = zsum*0.25 ;
	
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	float testVvertice(float xyz[3],int curNbIdx[4], s_atm *atoms)
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Test if alpha sphere conditions are fulfilled for current vertice
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ float xyz[3]: Coordinates of current vertice
	@ int curNbIdx[4]: Indexes of atomic neighbours of the current vertice
	@ s_atm *atoms : List of all atoms
	@ float min_asph_size: Minimum size of alpha spheres.
	@ float max_asph_size: Maximum size of alpha spheres.
   -----------------------------------------------------------------------------
   ## RETURN: -1 if conditions are not fulfilled, else the alpha sphere radius
	is returned.
   -----------------------------------------------------------------------------
*/
float testVvertice(float xyz[3], int curNbIdx[4], s_atm *atoms, float min_asph_size, float max_asph_size, s_lst_vvertice *lvvert)
{
	float x = xyz[0],
		  y = xyz[1],
		  z = xyz[2] ;

	s_atm *cura = &(atoms[lvvert->h_tr[curNbIdx[0]]]) ;

	float distVatom1 = dist(x, y, z, cura->x, cura->y, cura->z) ;
	float distVatom2, 
		  distVatom3,
		  distVatom4;

	if(min_asph_size <= distVatom1  && distVatom1 <= max_asph_size){	
		cura = &(atoms[lvvert->h_tr[curNbIdx[1]]]) ;
		distVatom2 = dist(x, y, z, cura->x, cura->y, cura->z);

		cura = &(atoms[lvvert->h_tr[curNbIdx[2]]]) ;
 		distVatom3 = dist(x, y, z, cura->x, cura->y, cura->z);

		cura = &(atoms[lvvert->h_tr[curNbIdx[3]]]) ;
  		distVatom4=dist(x, y, z, cura->x, cura->y, cura->z);

		//test if all 4 neighbours are on the alpha sphere surface (approximate test)
		if(fabs(distVatom1-distVatom2) < M_PREC_TOLERANCE && fabs(distVatom1-distVatom3)< M_PREC_TOLERANCE && fabs(distVatom1-distVatom4) < M_PREC_TOLERANCE){
			return distVatom1;
		}
		
	}
	return -1.0;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void print_vvertices(s_lst_vvertice *lvvert) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Print function.
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ FILE *f: Buffer to print in
	@ s_lst_vvertice *lvvert: Atoms to print
   -----------------------------------------------------------------------------
   ## RETURN: void
   -----------------------------------------------------------------------------
*/
void print_vvertices(FILE *f, s_lst_vvertice *lvvert) 
{
	if(lvvert) {
		if(lvvert->vertices) {
			int i ;
			for(i = 0 ; i < lvvert->nvert ; i++) {
				s_vvertice *v = &(lvvert->vertices[i]) ;
				if( v->neigh[0] &&  v->neigh[1] && v->neigh[2] &&  v->neigh[3]) {
					fprintf(f, "====== Vertice %d: =====\n", i);
					fprintf(f, "- x = %f, y = %f, z = %f\n", v->x, v->y, v->z);
					fprintf(f, "- ix = %d\n",v->sort_x);

					float d1 = dist(v->x, v->y, v->z, v->neigh[0]->x, v->neigh[0]->y, v->neigh[0]->z) ;
					float d2 = dist(v->x, v->y, v->z, v->neigh[1]->x, v->neigh[1]->y, v->neigh[1]->z) ;
					float d3 = dist(v->x, v->y, v->z, v->neigh[2]->x, v->neigh[2]->y, v->neigh[2]->z) ;
					float d4 = dist(v->x, v->y, v->z, v->neigh[3]->x, v->neigh[3]->y, v->neigh[3]->z) ;

					fprintf(f, "- Neighbour: \n1 - %f (%f %f %f: %d) \n2 - %f (%f %f %f: %d)\n3 - %f (%f %f %f: %d)\n4 - %f (%f %f %f: %d)\n", d1, v->neigh[0]->x, v->neigh[0]->y, v->neigh[0]->z, v->neigh[0]->id, d2, v->neigh[1]->x, v->neigh[1]->y, v->neigh[1]->z, v->neigh[1]->id, d3, v->neigh[2]->x,  v->neigh[2]->y ,  v->neigh[2]->z, v->neigh[2]->id, d4, v->neigh[3]->x, v->neigh[3]->y, v->neigh[3]->z, v->neigh[3]->id);
				}
			}
		}
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	float get_verts_volume_ptr(s_vvertice **verts, int nvert, int niter) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Get an monte carlo approximation of the volume occupied by the alpha spheres
	given in argument (list of pointers)
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ s_vvertice **verts: List of pointer to alpha spheres
	@ int nvert: Number of spheres
	@ int niter: Number of monte carlo iteration to perform
   -----------------------------------------------------------------------------
   ## RETURN:
	float: volume.
   -----------------------------------------------------------------------------
*/
float get_verts_volume_ptr(s_vvertice **verts, int nvert, int niter)
{
	int i = 0, j = 0,
		nb_in = 0;

	float xmin = 0.0, xmax = 0.0,
		  ymin = 0.0, ymax = 0.0,
		  zmin = 0.0, zmax = 0.0,
		  xtmp = 0.0, ytmp = 0.0, ztmp = 0.0,
		  xr   = 0.0, yr   = 0.0, zr   = 0.0,
		  vbox = 0.0 ;

	s_vvertice *vcur = NULL ;

	// First, search extrems coordinates to get a contour box of the molecule
	for(i = 0 ; i < nvert ; i++) {
		vcur = verts[i] ;

		if(i == 0) {
			xmin = vcur->x - vcur->ray ; xmax = vcur->x + vcur->ray ;
			ymin = vcur->y - vcur->ray ; ymax = vcur->y + vcur->ray ;
			zmin = vcur->z - vcur->ray ; zmax = vcur->z + vcur->ray ;
		}
		else {
		// Update the minimum and maximum extreme point
			if(xmin > (xtmp = vcur->x - vcur->ray)) xmin = xtmp ;
			else if(xmax < (xtmp = vcur->x + vcur->ray)) xmax = xtmp ;
	
			if(ymin > (ytmp = vcur->y - vcur->ray)) ymin = ytmp ;
			else if(ymax < (ytmp = vcur->y + vcur->ray)) ymax = ytmp ;
	
			if(zmin > (ztmp = vcur->z - vcur->ray)) zmin = ztmp ;
			else if(zmax < (ztmp = vcur->z + vcur->ray)) zmax = ztmp ;
		}
	}

	// Next calculate the contour box volume
	vbox = (xmax - xmin)*(ymax - ymin)*(zmax - zmin) ;

	// Then apply monte carlo approximation of the volume.	
	for(i = 0 ; i < niter ; i++) {
		xr = rand_uniform(xmin, xmax) ;
		yr = rand_uniform(ymin, ymax) ;
		zr = rand_uniform(zmin, zmax) ;

		for(j = 0 ; j < nvert ; j++) {
			vcur = verts[j] ;
			xtmp = vcur->x - xr ;
			ytmp = vcur->y - yr ;
			ztmp = vcur->z - zr ;

		// Compare r^2 and dist(center, random_point)^2
			if((vcur->ray*vcur->ray) > (xtmp*xtmp + ytmp*ytmp + ztmp*ztmp)) {
			//the point is inside one of the vertice!!
				nb_in ++ ; break ;
			}
		}
	}

	// Ok lets just return the volume Vpok = Nb_in/Niter*Vbox
	return ((float)nb_in)/((float)niter)*vbox;
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void free_vert_lst(s_lst_vvertice *lvvert) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Free memory
   -----------------------------------------------------------------------------
   ## PARAMETERS:
	@ s_lst_vvertice *lvvert: Data to free
   -----------------------------------------------------------------------------
   ## RETURN: void
   -----------------------------------------------------------------------------
*/
void free_vert_lst(s_lst_vvertice *lvvert) 
{
	if(lvvert) {
		if(lvvert->vertices) {
			my_free(lvvert->vertices) ;
			lvvert->vertices = NULL ;		
		}
		if(lvvert->tr) {
			my_free(lvvert->tr) ;
			lvvert->tr = NULL ;
		}
		if(lvvert->h_tr) {
			my_free(lvvert->h_tr) ;
			lvvert->h_tr = NULL ;
		}
		my_free(lvvert) ;
	}
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	int is_in_lst_vert(s_atm lst_atm, int nb_atm, int atm_id) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Says wether a vertice of id v_id is in a list of vertices or not 
   -----------------------------------------------------------------------------
   ## PARAMETRES:
   -----------------------------------------------------------------------------
   ## RETURN:
	1 if the vertice is in the tab, 0 if not
   -----------------------------------------------------------------------------
*/
int is_in_lst_vert(s_vvertice **lst_vert, int nb_vert, int v_id) 
{
	int i ;
	for(i = 0 ; i < nb_vert ; i++) {
		if(v_id == lst_vert[i]->id) return 1 ;
	}

	return 0 ;
}

/** -----------------------------------------------------------------------------
	-----------------------------------------------------------------------------
	-----------------------------------------------------------------------------

	OUTPUT FUNCTIONS
	
	-----------------------------------------------------------------------------
	-----------------------------------------------------------------------------
	-----------------------------------------------------------------------------
*/

/**-----------------------------------------------------------------------------
   ## FUNCTION:
	void write_pdb_vertice(FILE *f, s_vvertice *v) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION:
	Write a voronoi vertice in pdb format.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ FILE *f: file to write in
	@ s_vvertice *v: The vertice
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
void write_pdb_vert(FILE *f, s_vvertice *v) 
{
	if(v->type==M_APOLAR_AS) write_pdb_atom_line(f, "HETATM", v->id, "APOL", ' ', "STP", "C", v->resid, ' ',v->x, v->y, v->z, 0.0, 0.0, "Ve", -1);
	else write_pdb_atom_line(f, "HETATM", v->id, " POL", ' ', "STP", "C", v->resid, ' ',v->x, v->y, v->z,0.0, 0.0, "Ve", -1);
}

/**-----------------------------------------------------------------------------
   ## FUNCTION: 
	void write_pqr_vertice(FILE *f, s_vvertice *v) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Write a voronoi vertice in pqr format.
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ FILE *f: file to write in
	@ s_vvertice *v: The vertice
   -----------------------------------------------------------------------------
   ## RETURN:
   -----------------------------------------------------------------------------
*/
void write_pqr_vert(FILE *f, s_vvertice *v) 
{
	if(v->type==M_APOLAR_AS) write_pqr_atom_line(f, "ATOM", v->id, "APOL", ' ', "STP", " ", v->resid, ' ',v->x, v->y, v->z, 0.0, v->ray);
	else write_pqr_atom_line(f, "ATOM", v->id, " POL", ' ', "STP", " ", v->resid, ' ',v->x, v->y, v->z,0.0, v->ray);
}

