#include "../headers/calc.h"

/**

## ----- GENERAL INFORMATIONS
##
## FILE 					calc.h
## AUTHORS					P. Schmidtke and V. Le Guilloux
## LAST MODIFIED			01-04-08
##
## ----- SPECIFICATIONS
##
##	Several function for calculations. CUrrently, only euclidian
##	distances are available.
##
## ----- MODIFICATIONS HISTORY
##
##	01-04-08	(v)  Added comments and creation of history
##	01-01-08	(vp) Created (random date...)
##	
## ----- TODO or SUGGESTIONS
##
##	(v) Check and update if necessary comments of each function!!
##

*/

/**-----------------------------------------------------------------------------
   ## FONCTION: 
	float dist(float x1, float y1, float z1, float x2, float y2, float z2) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Calculate euclidian distance between two points in space p1(x1, y1, z2) and 
	p2(x2, y2, z2)
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ float x1, y1, z1: The first point's coordinates.
	@ float x2, y2, z2: The second point's coordinates.
   -----------------------------------------------------------------------------
   ## RETURN: 
	float: the distance between p1(x1, y1, z2) and p2(x2, y2, z2)
   -----------------------------------------------------------------------------
*/
float dist(float x1, float y1, float z1, float x2, float y2, float z2) 
{
	float xdif = x1 - x2 ;
	float ydif = y1 - y2 ;
	float zdif = z1 - z2 ;

	return sqrt((xdif*xdif) + (ydif*ydif) + (zdif*zdif)) ;
}

/**-----------------------------------------------------------------------------
   ## FONCTION: 
	float ddist(float x1, float y1, float z1, float x2, float y2, float z2) 
   -----------------------------------------------------------------------------
   ## SPECIFICATION: 
	Calculate square of the euclidian distance between two points in space 
	p1(x1, y1, z2) and 	p2(x2, y2, z2)
   -----------------------------------------------------------------------------
   ## PARAMETRES:
	@ float x1, y1, z1: The first point's coordinates.
	@ float x2, y2, z2: The second point's coordinates.
   -----------------------------------------------------------------------------
   ## RETURN: 
	float
   -----------------------------------------------------------------------------
*/
float ddist(float x1, float y1, float z1, float x2, float y2, float z2) 
{
	float xdif = x1 - x2 ;
	float ydif = y1 - y2 ;
	float zdif = z1 - z2 ;

	return (xdif*xdif) + (ydif*ydif) + (zdif*zdif) ;
}

