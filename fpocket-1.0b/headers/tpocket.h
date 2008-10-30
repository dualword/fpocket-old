
#ifndef DH_TPOCKET
#define DH_TPOCKET

// -----------------------------------INCLUDES--------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tparams.h"
#include "neighbor.h"
#include "fpocket.h"

#include "memhandler.h"

// --------------------------------------MACROS---------------------------------------------

#define M_DST_CRIT 4.0

/* A set of index, giving the position of each value in the tab used to
 * store them. In the src file, those statistics values are stored in a tab
 * using indices given here.
 **/
#define M_NDDATA 8		/* Number of floating values */
#define M_MAXPCT1 0		/* Maximum observed overlap for the criteria 1 */
#define M_MAXPCT2 1		/* Maximum observed overlap for the criteria 2 */
#define M_MINDST 2		/* Minimum distance observed (barycenter/ligand), crit 3 */
#define M_OREL1 3		/* Relative overlap of the pocket found for criteria 1 */
#define M_OREL2 4		/* Relative overlap of the pocket found for criteria 2 */
#define M_OREL3 5		/* Relative overlap of the pocket found for criteria 3 */
#define M_LIGMASS 6		/* Mass of the ligand */
#define M_LIGVOL 7		/* Volume of the ligand */

#define M_NIDATA 4		/* Number of interger values */
#define M_NPOCKET 0		/* Total number of pocket found */
#define M_POS1 1		/* Rank of the right pocket for the first criteria */
#define M_POS2 2		/* Rank of the right pocket for the second criteria */
#define M_POS3 3		/* Rank of the right pocket for the third criteria */

/* Flags used for the pocket detection */
#define M_LIGNOTFOUND -2
#define M_PDBOPENFAILED -1
#define M_OK 0 
#define M_NOPOCKETFOUND 1

// --------------------------------------SRUCTURES------------------------------------------

// ------------------------------------PROTOTYPES-------------------------------------------

void test_fpocket(s_tparams *par) ;
int test_AC(s_tparams *par, int i, float ddata [][M_NDDATA], int idata [][M_NIDATA]) ;
void check_pockets(c_lst_pockets *pockets, s_atm **accpck, int naccpck, s_atm **lig, int nalig, float ligvol,
				   s_atm **alneigh, int nlneigh, float ddata [][M_NDDATA], int idata [][M_NIDATA], int i,
				   s_fparams *params) ;

s_atm** get_actual_pocket(s_pdb *com_pdb, s_pdb *com_pdb_nolig, int i, s_tparams *par, int *nb_atm) ;
s_atm** get_actual_pocket_DEPRECATED(s_pdb *com_pdb, float lig_dist_crit, int *nb_atm) ;

float set_overlap_volumes(s_pocket *pocket, s_atm **lig, int natoms, float lig_vol, s_fparams *params) ;
float set_mc_overlap_volume(s_atm **lig, int natoms, float lig_vol,s_pocket *pocket, int niter) ;
float set_basic_overlap_volume(s_atm **lig, int natoms, float lig_vol,s_pocket *pocket, int idiscret) ;

//void write_tpocket(c_lst_pockets *pockets, s_pdb *pdb, const char base_pdb_name[]) ;
void write_pockets_stats(c_lst_pockets *pockets, const char base_pdb_name[]);


#endif
