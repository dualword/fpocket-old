/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.gui.tree;

import java.awt.Component;
import javax.swing.ImageIcon;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import jpocket.core.odata.JLibrary;
import jpocket.core.odata.JPDBFile;
import jpocket.core.odata.JPDBLig;
import jpocket.core.odata.JProject;

/**
 *
 * @author Administrateur
 */
public class JProjectsTreeCellRenderer extends DefaultTreeCellRenderer
{
    private final ImageIcon PDBIcon =
        new ImageIcon(getClass().getResource("/jpocket/ressources/icons/prot.png"));

    private final ImageIcon fpocketPDBIcon =
        new ImageIcon(getClass().getResource("/jpocket/ressources/icons/prot_fpocket.png"));
    
    private final ImageIcon ligIcon =
        new ImageIcon(getClass().getResource("/jpocket/ressources/icons/benzene.png"));

    public JProjectsTreeCellRenderer() {}

    @Override
    public Component getTreeCellRendererComponent(JTree tree, Object value,
                                                  boolean sel, boolean expanded,
                                                  boolean leaf, int row,
                                                  boolean hasFocus)
    {
        super.getTreeCellRendererComponent( tree, value, sel,
                                            expanded, leaf, row,
                                            hasFocus);
        DefaultMutableTreeNode node = (DefaultMutableTreeNode) value ;
        value = node.getUserObject() ;

        if(value instanceof JProject) {
            JProject project = (JProject) value ;
            setText(project.getName());
            setToolTipText(project.getDescription());
        }
        else if(value instanceof JLibrary) {
            JLibrary lib = (JLibrary) value ;
            setText(lib.getName());
            setToolTipText(lib.getDescription());
        }
        else if(value instanceof JPDBFile) {
            JPDBFile pdb = (JPDBFile) value ;
            if(pdb.getFpocketOutputLocation() == null
               || pdb.getFpocketOutputLocation().exists())
                 setIcon(PDBIcon) ;
            else setIcon(fpocketPDBIcon);
            
            setText(pdb.getName()) ;
            setToolTipText(pdb.getAbsolutePath());
        }
        else if(value instanceof JPDBLig) {
            JPDBLig lig = (JPDBLig) value ;
            setIcon(ligIcon) ;
            setText(lig.getHetatmCode()) ;
            setToolTipText(lig.getType());
        }
        
        return this;
    }
}
