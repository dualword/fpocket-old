/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.gui.tree.popup;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JSeparator;
import javax.swing.SwingConstants;
import jpocket.projects.IFProject;

/**
 *
 * @author Noé
 */
public class ProjectPopup extends JPopupMenu implements ActionListener
{
    public static final String NEWLIB_ACTION    = "newLib" ;
    public static final String NEWPDB_ACTION    = "newPDB" ;
    public static final String EDIT_ACTION      = "editProject" ;
    public static final String SAVE_ACTION      = "saveProject" ;
    public static final String CLOSE_ACTION     = "closeProject" ;
    public static final String REMOVE_ACTION    = "removeProject" ;
    
    public static final String [][] MENU = {
                    {"New library",     NEWLIB_ACTION },
                    {"New PDB",         NEWPDB_ACTION },
                    {"Separator",       ""},
                    {"Edit project",    EDIT_ACTION },
                    {"Save project",    SAVE_ACTION },
                    {"Close project",   CLOSE_ACTION},
                    {"Delete project",  REMOVE_ACTION}
                } ;

    private IFProject project ;

    public ProjectPopup(IFProject project)
    {
        super() ;
        
        this.project = project ;

        initMenus() ;

        setOpaque(true) ;
        setLightWeightPopupEnabled(true);
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

    public IFProject getProject() {
        return project;
    }

    public void setProject(IFProject project) {
        this.project = project;
    }

    public void actionPerformed(ActionEvent e) {
        if(e.getActionCommand().equals(NEWLIB_ACTION)) {

        }
        else if(e.getActionCommand().equals(NEWPDB_ACTION)) {

        }
        else if(e.getActionCommand().equals(EDIT_ACTION)) {

        }
        else if(e.getActionCommand().equals(SAVE_ACTION)) {

        }
        else if(e.getActionCommand().equals(CLOSE_ACTION)) {

        }
        else if(e.getActionCommand().equals(REMOVE_ACTION)) {

        }
    }
}
