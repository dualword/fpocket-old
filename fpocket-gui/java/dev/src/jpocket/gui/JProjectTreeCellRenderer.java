/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.gui;

import java.awt.Component;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import jpocket.projects.PDBFile;
import jpocket.projects.PDBLigand;

/**
 *
 * @author Administrateur
 */
public class JProjectTreeCellRenderer extends DefaultTreeCellRenderer
{
    private final ImageIcon PDBIcon = new ImageIcon(getClass().getResource("/jpocket/ressources/icons/prot.png"));
    private final ImageIcon fpocketPDBIcon = new ImageIcon(getClass().getResource("/jpocket/ressources/icons/prot_fpocket.png"));
    private final ImageIcon ligIcon = new ImageIcon(getClass().getResource("/jpocket/ressources/icons/benzene.png"));

    public JProjectTreeCellRenderer() {}

    @Override
    public Component getTreeCellRendererComponent(JTree tree, Object value,
                                                  boolean sel, boolean expanded,
                                                  boolean leaf, int row,
                                                  boolean hasFocus)
    {
        super.getTreeCellRendererComponent( tree, value, sel,
                                            expanded, leaf, row,
                                            hasFocus);
                                            System.out.println("    In Renderer?");
        DefaultMutableTreeNode node = (DefaultMutableTreeNode) value ;
        value = node.getUserObject() ;

        if(value instanceof String) {
            setText(value.toString());
        }
        else if(value instanceof PDBFile) {
            PDBFile pdb = (PDBFile) value ;
            System.out.println("PDB File to render: "+pdb.getName());
            if(pdb.getFpocketOutputPath() == null || pdb.getFpocketOutputPath().equals(""))
                 setIcon(PDBIcon) ;
            else setIcon(fpocketPDBIcon);
            
            setText(pdb.getName()) ;
            setToolTipText(pdb.getFile());
        }
        else if(value instanceof PDBLigand) {
            PDBLigand lig = (PDBLigand) value ;
            setIcon(ligIcon) ;
            setText(lig.getHetatmCode()) ;
            setToolTipText(lig.getType());
        }
        
        return this;
    }
}
