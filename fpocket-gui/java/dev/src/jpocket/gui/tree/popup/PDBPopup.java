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
import jpocket.projects.PDBFile;

/**
 *
 * @author Noé
 */
public class PDBPopup  extends JPopupMenu implements ActionListener
{
    public static final String EDIT_ACTION          = "editPDB" ;
    public static final String REMOVE_ACTION        = "removePDB" ;
    public static final String FPOCKET_ACTION       = "launchFpocket" ;

    public static final String [][] MENU = {
                    {"Edit PDB",        EDIT_ACTION },
                    {"Remove PDB",      REMOVE_ACTION},
                    {"Separator",       ""},
                    {"Launch Fpocket",  FPOCKET_ACTION},
                } ;
    
    private PDBFile pdb ;

    public PDBPopup(PDBFile pdb) {
        super() ;

        this.pdb = pdb ;

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
    
    public PDBFile getPdb() {
        return pdb;
    }

    public void setPDB(PDBFile pdb) {
        this.pdb = pdb ;
    }

    public void actionPerformed(ActionEvent e) {
        if(e.getActionCommand().equals(EDIT_ACTION)) {

        }
        else if(e.getActionCommand().equals(REMOVE_ACTION)) {

        }
        else if(e.getActionCommand().equals(FPOCKET_ACTION)) {

        }
    }
}
