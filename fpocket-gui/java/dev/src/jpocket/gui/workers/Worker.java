/**

## ----- GENERAL INFORMATION
##
## FILE 		Worker.java
## AUTHORS		VL
## CREATED  	Oct 18, 2009
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

package jpocket.gui.workers;

import javax.swing.SwingWorker;

/**
 *
 * @author vince
 */
public abstract class Worker <U, V> extends SwingWorker<U, V>
{

    protected void sendMessage(String message, String property, Exception e)
    {
        if(e != null) {
            message += "\nERR: "+e.getMessage() ;
            e.printStackTrace();
        }

        firePropertyChange(property, "LOG", message);
    }

    protected void updateInfo(String message)
    {
        firePropertyChange(WorkerProgressFrame.INFO_LABEL_PROP, "LOG", message);
    }
}
