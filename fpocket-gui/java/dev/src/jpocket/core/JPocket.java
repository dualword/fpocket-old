/**

## ----- GENERAL INFORMATION
##
## FILE 		JPocket.java
## AUTHORS		VL
## CREATED  	Oct 18, 2009
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

package jpocket.core;

import java.io.File;
import java.io.UnsupportedEncodingException;
import java.net.URL;
import java.net.URLDecoder;
import javax.swing.JOptionPane;

/**
 *
 * @author vince
 */
public class JPocket
{
    public static String ENV_PATH = findPath() ;

    private static String findPath()
    {
        // Used to check the path
        boolean ok = false ;

        // First, try to guess the path.
        String path = null ;
        try {
            path = guessSAPath();
            ok = testPath(path) ;
        }
        catch (UnsupportedEncodingException ex) {
            ok = false ;
        }

        if(ok) return path ;

        // If not OK, check the presence of an environment variable
        path = System.getenv("JPOCKET") ;

        if(testPath(path)) return path ;

        // Well... Try the current directory. If not ok again, exit the program,
        // SA won't work properly.
        path = System.getProperty("user.dir") ;
        if(!path.endsWith(File.separator)) {
            path = path+File.separator ;
        }

        if(testPath(path)) return path ;

        // Display a message and quit the program.
        JOptionPane.showMessageDialog(null,
                " $ FATAL ERROR $ \n"+
                " JPOCKET directory cannot be found.\n\n"+
                " You may solve this problem by creating a new\n" +
                " environment variable named JPOCKET that\n" +
                " must store the JPOCKET full path. See the \n" +
                " README file included in the distribution for \n" +
                " all instructions. ");

        System.exit(-1);

        return null ;
    }

    public static String guessSAPath ()
            throws UnsupportedEncodingException
    {
        String os = System.getProperties().getProperty("os.name") ;
        String path = "/"+ JPocket.class.getName().replace('.', '/')+ ".class";

        URL url = JPocket.class.getResource(path);
        path = URLDecoder.decode(url.toString(), "UTF-8");

        // suppression de  la classe ou du jar du path de l'url
        int index = path.lastIndexOf("/");
        path = path.substring(0, index);

        if (path.startsWith("jar:file:")) {
            // Remove jar:file: substring and class path into the jar
            index = path.indexOf("!");
            path = path.substring(9, index);

            // Remove the actual name of the jar...
            if(path.substring(path.length()-3, path.length()).equals("jar")) {
                index = path.lastIndexOf("/");
                path = path.substring(0, index);
            }
        }
        else {
            // Remove file: from url if its a class outside the jar, and remove
            // package path if present.
            path = path.substring(5, path.length());
            Package pack = JPocket.class.getPackage();
            if (null != pack) {
                String packPath = pack.toString().replace('.', '/');
                if (path.endsWith(packPath)) {
                    path = path.substring(0, (path.length() - packPath.length()));
                }
            }
        }

        if(path.charAt(path.length()-1) != '/') path = path + '/' ;

        path = path.replace('/', File.separatorChar) ;

        if(os.contains("Windows") && path.charAt(0) == File.separatorChar) {
            path = path.substring(1) ;
        }
        System.out.println("PATH Guessed: "+path);

        return path ;
    }

    public static boolean testPath (String path)
    {
        boolean ok = true ;

        if(path == null) return false ;

        File f = new File(path+"settings") ;

        if(!f.exists() || !f.isDirectory()) ok = false ;

        return ok ;
    }


}
