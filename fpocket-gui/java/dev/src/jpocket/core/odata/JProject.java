/**

## ----- GENERAL INFORMATION
##
## FILE 		JProject.java
## AUTHORS		VL
## CREATED  	Sep 27, 2009
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

package jpocket.core.odata;

import java.util.ArrayList;
import java.util.List;
import javax.swing.tree.DefaultMutableTreeNode;

/**
 *
 * @author vince
 */
public class JProject
{
    private String description ;
    private String xmlFile ;
    private String name ;

    List<JLibrary> libs ;

    public JProject(String name) {
        this(name, null, "No description yet", null) ;
    }

    public JProject(String name, String xmlFile, String description,
                    List<JLibrary> libs)
    {
        this.description = description;
        this.xmlFile = xmlFile;
        this.name = name;
        this.libs = libs;

        if(this.libs == null )this.libs = new ArrayList() ;
    }

    public List<JLibrary> getLibs() {
        return libs;
    }

    public void addLibrary(JLibrary lib) {
        libs.add(lib) ;
    }

    public void removeLibrary(JLibrary lib) {
        libs.remove(lib) ;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getName() {
        return name;
    }

    public void setLibraries(List<JLibrary> libs) {
        this.libs = libs ;
        if(this.libs == null) this.libs = new ArrayList() ;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getXmlFile() {
        return xmlFile;
    }

    public void setXmlFile(String xmlFile) {
        this.xmlFile = xmlFile;
    }

    public DefaultMutableTreeNode toTreeNode()
    {
        DefaultMutableTreeNode root = new DefaultMutableTreeNode(this) ;

        for(int i = 0 ; i < this.libs.size() ; i++) {
            DefaultMutableTreeNode libNode = libs.get(i).toTreeNode() ;
            root.add(libNode);
        }

        return root ;
        
    }

}
