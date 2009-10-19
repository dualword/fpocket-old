/**

## ----- GENERAL INFORMATION
##
## FILE 		JPDBLig.java
## AUTHORS		VL
## CREATED  	Sep 27, 2009
## GROUP        Institut de Chimie Organique et Analytique, UMR CNRS 6005
##              Group of chemoinformatics and molecular modeling
##              University of Orleans BP 6759, 45067 ORLEANS Cedex2, France
##
## CONTACT      vincent.le-guilloux@univ-orleans.fr
##
## ----- MODIFICATIONS HISTORY
#
## ----- SUGGESTIONS
##
##

*/

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.core.odata;

/**
 *
 * @author vince
 */
public class JPDBLig
{
    private JPDBFile pdb ;

    private boolean tpocket ;

    private String hetatmCode ;
    private String type ;

    public JPDBLig(JPDBFile pdb, boolean tpocket, String hetatmCode, String type)
    {
        this.pdb = pdb;
        this.tpocket = tpocket;
        this.hetatmCode = hetatmCode;
        this.type = type;
    }

    public String getHetatmCode() {
        return hetatmCode;
    }

    public void setHetatmCode(String hetatmCode) {
        this.hetatmCode = hetatmCode;
    }

    public JPDBFile getPdb() {
        return pdb;
    }

    public void setPdb(JPDBFile pdb) {
        this.pdb = pdb;
    }

    public boolean isTpocket() {
        return tpocket;
    }

    public void setTpocket(boolean tpocket) {
        this.tpocket = tpocket;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    
}
