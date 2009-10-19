/**

## ----- GENERAL INFORMATION
##
## FILE 		DockablePanel.java
## AUTHORS		VL
## CREATED  	Oct 4, 2009
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

package jpocket.gui;

import com.vlsolutions.swing.docking.DockKey;
import com.vlsolutions.swing.docking.Dockable;
import java.awt.Component;
import javax.swing.JPanel;

/**
 *
 * @author vince
 */
public class DockablePanel extends JPanel
    implements Dockable
{
    private DockKey docKey ;

    public DockablePanel(String title, String key) {
        super() ;

        docKey = new DockKey(key, title) ;
    }

    @Override
    public DockKey getDockKey() {
        return docKey ;
    }

    @Override
    public Component getComponent() {
        return this ;
    }
}
