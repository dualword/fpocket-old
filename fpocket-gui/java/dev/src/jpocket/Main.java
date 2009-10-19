/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket;

import javax.swing.UIManager;
import jpocket.gui.JPocketGUI;

/**
 *
 * @author vince
 */
public class Main
{

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here


        try {
//			UIManager.setLookAndFeel(new com.jgoodies.looks.plastic.Plastic3DLookAndFeel());
//            UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        }
        catch (Exception e) {
            System.out.println(e.toString());
        }

        JPocketGUI gui = new JPocketGUI();
        gui.setSize(800, 600);
        gui.setVisible(true) ;
    }
}
