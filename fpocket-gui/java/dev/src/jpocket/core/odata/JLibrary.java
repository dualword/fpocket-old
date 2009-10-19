/**

## ----- GENERAL INFORMATION
##
## FILE 		JLibrary.java
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
import java.util.Iterator;
import java.util.List;
import javax.swing.tree.DefaultMutableTreeNode;

/**
 *
 * @author vince
 */
public class JLibrary
{
    private String name ;
    private String description ;

    private List<JLibrary> subLibraries ;
    private List<JTrajectory> trajs ;
    private List<JPDBFile> pdbs ;

    private JLibrary parent ;

    public JLibrary(JLibrary parent, String name, String description) {
        this(parent, name, description, null, null, null) ;
    }

    public JLibrary(JLibrary parent, String name, String description,
                    List<JLibrary> subLibraries,
                    List<JTrajectory> trajs,
                    List<JPDBFile> pdbs)
    {
        this.parent = parent ;
        this.name = name;
        this.description = description;

        setPdbs(pdbs);
        setSubLibraries(subLibraries);
        setTrajs(trajs);
    }

    public List<JPDBFile> getPdbsAndSubLibPdbs() {
        List allPdbs = new ArrayList(pdbs) ;

        Iterator<JLibrary> it = subLibraries.iterator() ;
        while(it.hasNext()) {
            allPdbs.addAll(it.next().getPdbsAndSubLibPdbs());
        }
        
        return allPdbs ;
    }

    public void removeAll(boolean deleteFromDisk) {
        removePDBs(deleteFromDisk) ;
        removeLibraries(deleteFromDisk);
        removeTrajs(deleteFromDisk);
    }

    public void removePDBs(boolean deleteFromDisk) {
        for(JPDBFile pdb: pdbs) removePDB(pdb, deleteFromDisk) ;
    }
    
    public void removePDB(JPDBFile pdb, boolean deleteFromDisk) {
        pdbs.remove(pdb) ;

        if(deleteFromDisk) pdb.delete();
    }

    public void removeTrajs(boolean deleteFromDisk) {
        for(JTrajectory t: trajs) removeTraj(t, deleteFromDisk) ;
    }

    public void removeTraj(JTrajectory t, boolean deleteFromDisk) {
        trajs.remove(t) ;

        if(deleteFromDisk) t.removePdbs(deleteFromDisk) ;
        
    }

    public void removeLibraries(boolean deleteFromDisk) {
        for(JLibrary l: subLibraries) removeLibrary(l, deleteFromDisk) ;
    }

    public void removeLibrary(JLibrary lib, boolean deleteFromDisk) {
        subLibraries.remove(lib) ;

        if(deleteFromDisk) lib.removeAll(deleteFromDisk) ;
    }

    public void addPDB(JPDBFile pdb) {
        pdbs.add(pdb) ;
    }

    public void addTraj(JTrajectory t) {
        trajs.add(t) ;
    }

    public void addLibrary(JLibrary lib) {
        subLibraries.add(lib) ;
    }

    public void setTrajs(List<JTrajectory> trajs) {
        this.trajs = trajs;
        if(this.trajs == null) this.trajs = new ArrayList() ;
    }

    public void setPdbs(List<JPDBFile> pdbs) {
        this.pdbs = pdbs;
        if(this.pdbs == null) this.pdbs = new ArrayList() ;
    }

    public void setSubLibraries(List<JLibrary> subLibraries) {
        this.subLibraries = subLibraries;
        if(this.subLibraries == null) this.subLibraries = new ArrayList() ;
    }

    public List<JPDBFile> getPdbs() {
        return pdbs;
    }

    public List<JTrajectory> getTrajs() {
        return trajs;
    }

    public List<JLibrary> getSubLibraries() {
        return subLibraries;
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

    public void setName(String name) {
        this.name = name;
    }

    public JLibrary getParent() {
        return parent;
    }

    public void setParent(JLibrary parent) {
        this.parent = parent;
    }

    public DefaultMutableTreeNode toTreeNode() {
        DefaultMutableTreeNode node = new DefaultMutableTreeNode(this) ;

        for(int i = 0 ; i < subLibraries.size() ; i++) {
            node.add(subLibraries.get(i).toTreeNode());
        }

        for(int i = 0 ; i < trajs.size() ; i++) {
            node.add(trajs.get(i).toTreeNode()) ;
        }

        for(int i = 0 ; i < pdbs.size() ; i++) {
            node.add(pdbs.get(i).toTreeNode()) ;
        }
        
        return node ;
    }
}
