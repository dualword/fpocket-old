/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.utils;

import java.awt.Component;
import javax.swing.JFileChooser;

/**
 *
 * @author Administrateur
 */
public class Utils {
    public static String openFile(String s, String extension, Component parent)
    {
        JFileChooser chooser = new JFileChooser();
        chooser.setDialogTitle(s);
        ExampleFileFilter filter = new ExampleFileFilter();
        filter.addExtension(extension);
        filter.setDescription(extension);

        chooser.setFileFilter(filter);
        int returnVal = chooser.showOpenDialog(parent);

        String result = null ;
        if(returnVal == JFileChooser.APPROVE_OPTION)
                result = chooser.getSelectedFile().getAbsolutePath();

        return result;
    }

}
