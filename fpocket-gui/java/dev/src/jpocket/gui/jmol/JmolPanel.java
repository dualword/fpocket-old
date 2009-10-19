
/**

## ----- GENERAL INFORMATION
##
## FILE 		JMolPanel.java
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
import java.io.File;
import javax.swing.JPanel;
import jpocket.core.odata.JPDBFile;
import org.biojava.bio.structure.Structure;
import org.jmol.adapter.smarter.SmarterJmolAdapter;
import org.jmol.api.JmolAdapter;
import org.jmol.api.JmolSimpleViewer;

/**
 *
 * @author vince
 */
public class JmolPanel extends JPanel
{
    private static final long serialVersionUID = -3661941083797644242L;

    private JmolSimpleViewer viewer;
    private JmolAdapter adapter;

    public JmolPanel() {
        adapter = new SmarterJmolAdapter();
        viewer = JmolSimpleViewer.allocateSimpleViewer(this, adapter);
    }

    public JmolSimpleViewer getViewer() {
        return viewer;
    }

    public void executeCmd(String rasmolScript) {
        viewer.evalString(rasmolScript);
    }

    public void setStructure(Structure s)
    {
        String pdb = s.toPDB();
        viewer.openStringInline(pdb);
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

