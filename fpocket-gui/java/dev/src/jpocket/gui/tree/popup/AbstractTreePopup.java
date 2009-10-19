/**

## ----- GENERAL INFORMATION
##
## FILE 		AbstractTreePopup.java
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

package jpocket.gui.tree.popup;

import javax.swing.JPopupMenu;
import javax.swing.tree.DefaultMutableTreeNode;
import jpocket.gui.tree.JProjectsTreeModel;

/**
 *
 * @author vince
 */
public abstract class AbstractTreePopup extends JPopupMenu implements ITreePopupMenu
{
    protected DefaultMutableTreeNode node;
    protected JProjectsTreeModel treeModel ;

    public AbstractTreePopup(JProjectsTreeModel treeModel, DefaultMutableTreeNode node) {
        this.treeModel = treeModel ;
        setNode(node) ;
        setOpaque(true) ;
        setLightWeightPopupEnabled(true);
    }

    public void setNode(DefaultMutableTreeNode node) {
        this.node = node ;
    }

    public DefaultMutableTreeNode getNode(){
        return node ;
    }
}
