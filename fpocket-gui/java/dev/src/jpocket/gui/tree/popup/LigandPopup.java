/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.gui.tree.popup;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.security.InvalidParameterException;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JSeparator;
import javax.swing.SwingConstants;
import javax.swing.tree.DefaultMutableTreeNode;
import jpocket.gui.tree.JProjectsTreeModel;
import jpocket.core.odata.JPDBLig;

/**
 *
 * @author Noé
 */
public class LigandPopup  extends AbstractTreePopup implements ActionListener
{
    public static final String REMOVE_ACTION    = "removeLig" ;

    public static final String [][] MENU = {
                    {"Remove Ligand",  REMOVE_ACTION }
                } ;
    
    private JPDBLig lig ;

    public LigandPopup(JProjectsTreeModel treeModel, DefaultMutableTreeNode node) {
        super(treeModel, node) ;

        initMenus() ;

        setOpaque(true) ;
        setLightWeightPopupEnabled(true);
    }

    private void initMenus()
    {
        JMenuItem menuItem = null ;
        for(int i = 0 ; i < MENU.length ; i++) {
            if(MENU[i][0].equals("Separator")) {
                add(new JSeparator(SwingConstants.HORIZONTAL));
            }
            else {
                menuItem = new JMenuItem(MENU[i][0]) ;
                menuItem.setActionCommand(MENU[i][1]);
                menuItem.addActionListener(this) ;
                add(menuItem) ;
            }
        }
    }

    @Override
    public void setNode(DefaultMutableTreeNode node) {
        this.node = node ;

        if(node.getUserObject() instanceof JPDBLig)
            setLigand((JPDBLig) node.getUserObject()) ;
        else
            throw new InvalidParameterException("Node must be library!") ;
    }

    public void setLigand(JPDBLig lig) {
        this.lig =lig ;
    }

    public void actionPerformed(ActionEvent e) {
        if(e.getActionCommand().equals(REMOVE_ACTION)) {
            lig.getPdb().removeLig(lig);
            node.removeFromParent();
        }
    }
    
}
