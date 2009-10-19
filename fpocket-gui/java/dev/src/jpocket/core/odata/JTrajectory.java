/**

## ----- GENERAL INFORMATION
##
## FILE 		JTrajectory.java
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
import java.util.Collection;
import java.util.List;
import javax.swing.tree.DefaultMutableTreeNode;

/**
 *
 * @author vince
 */
public class JTrajectory
{
    private String description ;

    private List<JPDBFile> pdbs ;

    public JTrajectory(List<JPDBFile> pdbs, String description) {
        this.description = description ;
        this.pdbs = pdbs ;

        if(this.pdbs == null) this.pdbs = new ArrayList() ;
    }

    public void addPdb(JPDBFile pdb) {
        if(pdbs == null) pdbs = new ArrayList() ;
        pdbs.add(pdb);

    }

    void removePdbs(boolean deleteFromDisk) {
        for(JPDBFile pdb: pdbs) removePdb(pdb, deleteFromDisk) ;
    }

    public void removePdb(JPDBFile pdb, boolean deleteFromDisk) {
        pdbs.remove(pdb);
        if(deleteFromDisk) pdb.delete();
    }
    
    public Collection<JPDBFile> getPdbs() {
        return pdbs;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public DefaultMutableTreeNode toTreeNode() {
        DefaultMutableTreeNode node = new DefaultMutableTreeNode(this) ;

        for(int i = 0 ; i < pdbs.size() ; i++) {
            node.add(pdbs.get(i).toTreeNode()) ;
        }

        return node ;
    }

}
