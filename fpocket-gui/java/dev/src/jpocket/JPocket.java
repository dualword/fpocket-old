/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket;

import javax.swing.SwingUtilities;
import jpocket.gui.JPocketGUI;

/**
 *
 * @author Administrateur
 */
public class JPocket {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new JPocketGUI().setVisible(true);
            }
        });
    }

}
