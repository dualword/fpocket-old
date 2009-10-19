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
import jpocket.gui.creators.JLibraryCreatorDialog;
import jpocket.gui.editors.JProjectEditorDialog;
import jpocket.gui.tree.JProjectsTreeModel;
import jpocket.core.odata.JLibrary;
import jpocket.core.odata.JProject;

/**
 *
 * @author Noé
 */
public class ProjectPopup extends AbstractTreePopup implements ActionListener
{
    public static final String NEWLIB_ACTION    = "newLib" ;
    public static final String NEWTRAJ_ACTION   = "newTraj" ;
    public static final String EDIT_ACTION      = "editProject" ;
    public static final String SAVE_ACTION      = "saveProject" ;
    public static final String SAVEAS_ACTION    = "saveasProject" ;
    public static final String CLOSE_ACTION     = "closeProject" ;
    
    public static final String [][] MENU = {
                    {"New library",         NEWLIB_ACTION },
                    {"New trajectory",      NEWTRAJ_ACTION },
                    {"Separator",       ""},
                    {"Edit project",        EDIT_ACTION },
                    {"Save project",        SAVE_ACTION },
                    {"Save project as",     SAVEAS_ACTION },
                    {"Close project",       CLOSE_ACTION},
                } ;

    private JProject project ;

    public ProjectPopup(JProjectsTreeModel treeModel, DefaultMutableTreeNode node)
    {
        super(treeModel, node) ;

        initMenus() ;

        setOpaque(true) ;
        setLightWeightPopupEnabled(true);
    }

    @Override
    public void setNode(DefaultMutableTreeNode node) {
        this.node = node ;

        if(node.getUserObject() instanceof JProject)
            this.project = (JProject) node.getUserObject() ;
        else
            throw new InvalidParameterException("Node must be library!") ;
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

    public JProject getProject() {
        return project;
    }

    public void setProject(JProject project) {
        this.project = project;
    }

    public void actionPerformed(ActionEvent e) {
        if(e.getActionCommand().equals(NEWLIB_ACTION)) {
            JLibraryCreatorDialog dialog = new JLibraryCreatorDialog(null,
                                                                     true) ;
            dialog.setLocation(200, 200);
            dialog.setVisible(true);
            if(dialog.getReturnStatus() == JLibraryCreatorDialog.RET_OK) {
                JLibrary newLib = dialog.getLibrary() ;
                project.addLibrary(newLib);
                node.add(newLib.toTreeNode());
                treeModel.nodeStructureChanged(node);
            }
        }
        if(e.getActionCommand().equals(NEWTRAJ_ACTION)) {
            
        }
        else if(e.getActionCommand().equals(EDIT_ACTION)) {
            JProjectEditorDialog dialog = new JProjectEditorDialog(project, null,
                                                                     true) ;
            dialog.setLocation(200, 200);
            dialog.setVisible(true);
            if(dialog.getReturnStatus() == JLibraryCreatorDialog.RET_OK) {
                treeModel.nodeChanged(node);
            }
        }
        else if(e.getActionCommand().equals(SAVE_ACTION)) {
            
        }
        else if(e.getActionCommand().equals(SAVEAS_ACTION)) {

        }
        else if(e.getActionCommand().equals(CLOSE_ACTION)) {
            treeModel.removeNodeFromParent(node);
        }
    }
}
