
/**

## ----- GENERAL INFORMATION
##
## FILE 		JmolEvolvedPanel.java
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
package jpocket.gui.jmol;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Rectangle;
import javax.swing.JPanel;
import org.biojava.bio.structure.Structure;
import org.jmol.adapter.smarter.SmarterJmolAdapter;
import org.jmol.api.JmolAdapter;
import org.jmol.api.JmolViewer;
import org.jmol.popup.JmolPopup;
import org.jmol.viewer.Viewer;

/**
 *
 * @author vince
 */
public class JmolEvolvedPanel extends JPanel
{
    private JmolViewer viewer;
    private JmolPopup popup;
    private JmolAdapter adapter;

    public JmolEvolvedPanel() {
        adapter = new SmarterJmolAdapter();
        viewer = new Viewer(this, adapter);
        popup = JmolPopup.newJmolPopup(viewer, true, null, true);
    }

    public void setStructure(Structure struct) {
        if (viewer.isScriptExecuting()) {
            System.out.println(
                    "StructurePanel.setStructure(): jmol is still executing - seems to be crashed!");
            //resetJmolDisplay();
        }

        String pdbstr = struct.toPDB();
        viewer.openStringInline(pdbstr);
        
        String strError = viewer.getErrorMessage();
        if (strError != null) {
            System.out.println("Error  in jmol: "+strError);
        }

        popup.updateComputedMenus();
    }

    public void executeCmd(String rasmolScript) {
        viewer.evalString(rasmolScript);
    }

    final Dimension currentSize = new Dimension();
    final Rectangle rectClip = new Rectangle();
    
    @Override
    public void paint(Graphics g) {
        getSize(currentSize);
        g.getClipBounds(rectClip);
        viewer.renderScreenImage(g, currentSize, rectClip);
    }
}
