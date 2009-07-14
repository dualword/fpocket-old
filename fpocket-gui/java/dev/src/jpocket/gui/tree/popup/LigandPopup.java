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
import jpocket.projects.PDBLigand;

/**
 *
 * @author Noé
 */
public class LigandPopup extends JPopupMenu implements ActionListener
{
    public static final String REMOVE_ACTION    = "removeLig" ;

    public static final String [][] MENU = {
                    {"Remove Ligand",  REMOVE_ACTION }
                } ;
    
    private PDBLigand lig ;

    public LigandPopup(PDBLigand lig) {
        super() ;

        this.lig = lig ;

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

    public void setLigand(PDBLigand lig) {
        this.lig =lig ;
    }

    public void actionPerformed(ActionEvent e) {
        if(e.getActionCommand().equals(REMOVE_ACTION)) {

        }
    }
    
}
