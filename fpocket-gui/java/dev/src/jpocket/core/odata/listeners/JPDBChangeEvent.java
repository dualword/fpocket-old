/**

## ----- GENERAL INFORMATION
##
## FILE 		JPDBChangeEvent.java
## AUTHORS		VL
## CREATED  	Oct 19, 2009
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

package jpocket.core.odata.listeners;

import java.util.EventObject;
import jpocket.core.odata.JPDBFile;

/**
 *
 * @author vince
 */
public class JPDBChangeEvent extends EventObject
{
    public static enum JPDBEvent
    {
        PDB_CHANGED,
        FPOUT_CHANGED,
        LIGANDS_CHANGED,
        OWNER_CHANGED
    }

    private JPDBEvent event;
    private JPDBFile pdbSource ;

    public JPDBChangeEvent(JPDBFile source, JPDBEvent event) {
        super(source) ;
        pdbSource = source;
        this.event = event ;
    }

    @Override
    public JPDBFile getSource() {
        return pdbSource ;
    }

    public JPDBEvent getEvent() {
        return event ;
    }
}
