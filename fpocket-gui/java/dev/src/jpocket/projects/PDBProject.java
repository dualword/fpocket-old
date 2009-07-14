/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.projects;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import javax.swing.tree.DefaultMutableTreeNode;

/**
 *
 * @author Administrateur
 */
public class PDBProject implements IFProject
{
    private static final DateFormat dFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss") ;

    private Date creationDate ;
    private Date lastModifiedDate ;

    private String description ;
    private String xmlFile ;
    private String name ;

    private DefaultMutableTreeNode arborescence ;

    public PDBProject(String xmlFile, String name, DefaultMutableTreeNode root)
    {
        this.xmlFile = xmlFile;
        this.name = name;

        if(root == null) arborescence = new DefaultMutableTreeNode(this) ;
        else arborescence = root ;
        
        this.creationDate = new java.util.Date();
        this.lastModifiedDate = new java.util.Date();

        description = "Project "+name+" created on "+dFormat.format(creationDate)+"." ;
    }

    public PDBProject( String xmlFile, String name, String description,
                       Date creationDate, Date lastModifiedDate,
                       DefaultMutableTreeNode root)
    {
        this.lastModifiedDate = lastModifiedDate;
        this.creationDate = creationDate;
        
        this.description = description;
        this.xmlFile = xmlFile;
        this.name = name;
        this.arborescence = root;
        
        if(arborescence == null) arborescence = new DefaultMutableTreeNode(this) ;
    }

    public String getName() {
        return name ;
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

    public Date getCreationDate() {
        return creationDate ;
    }

    public Date getLastModifiedDate() {
        return lastModifiedDate ;
    }

    public String getDescription() {
        return description ;
    }

    public List getPDBs() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public int countPDBs() {
        throw new UnsupportedOperationException("Not supported yet.");
    }
}
