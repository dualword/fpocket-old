/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.jmol;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Rectangle;
import javax.swing.JPanel;
import org.jmol.adapter.smarter.SmarterJmolAdapter;
import org.jmol.api.JmolAdapter;
import org.jmol.api.JmolSimpleViewer;

/**
 *
 * @author vince
 */
public class JMolPanelSA extends JPanel
{
    private JmolSimpleViewer viewer;
    private JmolAdapter adapter;
    
    public JMolPanelSA() {
        adapter = new SmarterJmolAdapter();
        viewer = JmolSimpleViewer.allocateSimpleViewer(this, adapter);
    }

    public JmolSimpleViewer getViewer() {
        return viewer;
    }

    public void executeCmd(String rasmolScript){
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
