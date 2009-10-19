/**

## ----- GENERAL INFORMATION
##
## FILE 		JProjectsTreeModel.java
## AUTHORS		VL
## CREATED  	Oct 4, 2009
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

package jpocket.gui.tree;

import java.io.IOException;
import java.util.List;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import jpocket.core.odata.JProject;
import jpocket.core.odata.io.JProjectXMLReader;
import org.jdom.JDOMException;

/**
 *
 * @author vince
 */
public class JProjectsTreeModel extends DefaultTreeModel
{
    private DefaultMutableTreeNode treeRoot;

    public JProjectsTreeModel (DefaultMutableTreeNode root) {
        super(root) ;

        this.treeRoot = root ;
    }

    void addProject(String xml) 
            throws JDOMException, IOException
    {
        JProjectXMLReader reader = new JProjectXMLReader(xml) ;
        List<JProject> projects = reader.readProjects() ;

        for(JProject p: projects) 
            treeRoot.add(p.toTreeNode()) ;

        this.nodeStructureChanged(root);
    }

}
