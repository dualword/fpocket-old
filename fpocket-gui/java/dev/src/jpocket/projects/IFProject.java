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
public interface IFProject {

    public String getName() ;
    public List getPDBs() ;

    public DefaultMutableTreeNode getArborescence() ;
    public void setArborescence(DefaultMutableTreeNode arborescence) ;
}
