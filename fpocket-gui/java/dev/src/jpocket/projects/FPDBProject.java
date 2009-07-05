/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.projects;

import java.util.List;
import javax.swing.tree.DefaultMutableTreeNode;

/**
 *
 * @author Administrateur
 */
public class FPDBProject implements IFProject
{
    private String xmlFile ;
    private String name ;
    private int id ;

    private DefaultMutableTreeNode arborescence ;

    public FPDBProject(String xmlFile, String name, DefaultMutableTreeNode root) {
        this.xmlFile = xmlFile;
        this.name = name;

        if(root == null) arborescence = new DefaultMutableTreeNode(name) ;
        else arborescence = root ;
    }

    public String getName() {
        return name ;
    }

    public int getID() {
        return id ;
    }

    public List getPDBs() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public DefaultMutableTreeNode getArborescence() {
        return arborescence;
    }

    public void setArborescence(DefaultMutableTreeNode arborescence) {
        this.arborescence = arborescence;
    }

    public String getXmlFile() {
        return xmlFile;
    }

    public void setXmlFile(String xmlFile) {
        this.xmlFile = xmlFile;
    }
}
