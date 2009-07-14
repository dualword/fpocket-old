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
 * @author Noé
 */
public class PDBLibrary implements IFLibrary
{
    public static final DateFormat DFORMAT = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss") ;

    private Date creationDate ;
    private Date lastModifiedDate ;

    private String description ;
    private String name ;

    private DefaultMutableTreeNode arborescence ;

    public PDBLibrary(String name, DefaultMutableTreeNode root)
    {
        this.name = name;

        if(root == null) arborescence = new DefaultMutableTreeNode(name) ;
        else arborescence = root ;

        this.creationDate = new java.util.Date();
        this.lastModifiedDate = new java.util.Date();

        description = "Project "+name+" created on "+DFORMAT.format(creationDate)+"." ;
    }

    public PDBLibrary(String name, String description, 
                      Date creationDate, Date lastModifiedDate,
                      DefaultMutableTreeNode arborescence)
    {
        this.lastModifiedDate = lastModifiedDate;
        this.creationDate = creationDate;

        this.description = description;
        this.name = name;
        this.arborescence = arborescence;
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
