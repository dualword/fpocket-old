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
import jpocket.projects.IFLibrary;

/**
 *
 * @author Noé
 */
public class LibraryPopup extends JPopupMenu implements ActionListener
{
    public static final String NEWLIB_ACTION    = "newLib" ;
    public static final String NEWPDB_ACTION    = "newPDB" ;
    public static final String FPOCKET_ACTION   = "launchFpocket" ;
    public static final String TPOCKET_ACTION   = "launchTpocket" ;
    public static final String EDIT_ACTION      = "editLib" ;
    public static final String REMOVE_ACTION    = "removeLib" ;

    public static final String [][] MENU = {
                    {"New library",     NEWLIB_ACTION },
                    {"New PDB",         NEWPDB_ACTION },
                    {"Separator",       ""},
                    {"Edit library",    EDIT_ACTION},
                    {"Remove library",  REMOVE_ACTION},
                    {"Separator",       ""},
                    {"Launch Fpocket",  FPOCKET_ACTION},
                    {"Launch Tpocket",  TPOCKET_ACTION}
                } ;
    
    private IFLibrary lib ;

    public LibraryPopup(IFLibrary lib) {
        super() ;

        this.lib = lib ;
        
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
    
    public IFLibrary getLib() {
        return lib;
    }

    public void setLib(IFLibrary lib) {
        this.lib = lib ;
    }

    public void actionPerformed(ActionEvent e) {
        if(e.getActionCommand().equals(NEWLIB_ACTION)) {

        }
        else if(e.getActionCommand().equals(NEWPDB_ACTION)) {

        }
        else if(e.getActionCommand().equals(EDIT_ACTION)) {

        }
        else if(e.getActionCommand().equals(REMOVE_ACTION)) {

        }
        else if(e.getActionCommand().equals(FPOCKET_ACTION)) {

        }
        else if(e.getActionCommand().equals(TPOCKET_ACTION)) {

        }
    }
    
}
