/**

## ----- GENERAL INFORMATION
##
## FILE 		RootPopup.java
## AUTHORS		VL
## CREATED  	Oct 15, 2009
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

package jpocket.gui.tree.popup;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.security.InvalidParameterException;
import javax.swing.JMenuItem;
import javax.swing.JSeparator;
import javax.swing.SwingConstants;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import jpocket.gui.creators.JProjectCreatorDialog;
import jpocket.gui.tree.JProjectsTreeModel;
import jpocket.core.odata.JProject;

/**
 *
 * @author vince
 */
public class RootPopup extends AbstractTreePopup implements ActionListener
{
    public static final String NEWPRO_ACTION    = "newPro" ;
    public static final String SAV_ACTION       = "savePro" ;

    public static final String [][] MENU = {
                    {"New project",     NEWPRO_ACTION },
                    {"Save all projects",  SAV_ACTION }
                } ;

    public RootPopup(JProjectsTreeModel treeModel, DefaultMutableTreeNode root)
    {
        super(treeModel, root) ;

        initMenus() ;

        setOpaque(true) ;
        setLightWeightPopupEnabled(true);
        System.out.println("TEST");
    }

    @Override
    public void setNode(DefaultMutableTreeNode node) {
        this.node = node ;
    }

    private void initMenus() {
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

    public void actionPerformed(ActionEvent e) {
        if(e.getActionCommand().equals(NEWPRO_ACTION)) {
            JProjectCreatorDialog dialog = new JProjectCreatorDialog(null,
                                                                     true) ;
            dialog.setLocation(200, 200);
            dialog.setVisible(true);
            
            if(dialog.getReturnStatus() == JProjectCreatorDialog.RET_OK) {
                JProject newP = dialog.getProject() ;
                node.add(newP.toTreeNode());
                treeModel.nodeStructureChanged(node);
            }

        }
        else if(e.getActionCommand().equals(SAV_ACTION)) {

        }
    }
}

