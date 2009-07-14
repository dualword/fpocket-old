/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.utils;

import java.awt.Component;
import java.io.File;
import javax.swing.JFileChooser;

/**
 *
 * @author Administrateur
 */
public class Utils {
    public static String[] openFiles(String s, String extension, Component parent)
    {
        JFileChooser chooser = new JFileChooser();
        chooser.setDialogTitle(s);
        chooser.setMultiSelectionEnabled(true) ;

        ExampleFileFilter filter = new ExampleFileFilter();
        filter.addExtension(extension);
        filter.setDescription(extension);

        chooser.setFileFilter(filter);
        int returnVal = chooser.showOpenDialog(parent);

        String [] results = null ;
        if(returnVal == JFileChooser.APPROVE_OPTION) {
            File[] files = chooser.getSelectedFiles();
            results = new String[files.length] ;

            for(int i = 0 ; i < results.length ; i++) {
                results[i] = files[i].getAbsolutePath() ;
            }
        }

        return results ;
    }

}
