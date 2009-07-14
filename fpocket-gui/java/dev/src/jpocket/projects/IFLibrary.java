/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.projects;

import java.util.Date;
import java.util.List;
import javax.swing.tree.DefaultMutableTreeNode;

/**
 *
 * @author Noé
 */
public interface IFLibrary
{
    public String getName() ;
    public Date getCreationDate() ;
    public Date getLastModifiedDate() ;
    public String getDescription() ;

    public List getPDBs() ;
    public int countPDBs() ;

    public DefaultMutableTreeNode getArborescence() ;
    public void setArborescence(DefaultMutableTreeNode arborescence) ;
}
