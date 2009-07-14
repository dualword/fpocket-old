/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.gui.tree;

import java.util.ArrayList;
import java.util.Iterator;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import jpocket.projects.io.FProjectReader;
import jpocket.projects.IFProject;

/**
 *
 * @author Administrateur
 */
public class JProjectTreeModel
{
    private ArrayList<IFProject> projects ;
    
    private DefaultTreeModel model ;
    private DefaultMutableTreeNode root ;

    public JProjectTreeModel() {
        root = new DefaultMutableTreeNode("Fpocket projects") ;
        model = new DefaultTreeModel(root) ;
        projects = new ArrayList<IFProject>() ;
    }

    public JProjectTreeModel(String [] xml) {
        this() ;
        
        for(int i = 0 ; i < xml.length ; i++) {
            try {
                addProjects(xml[i]);
            }
            catch (Exception ex) {
//                root = new DefaultMutableTreeNode("ERROR") ;
//                model = new DefaultTreeModel(root);
//                ex.printStackTrace();
            }
        }
    }

    public DefaultTreeModel getModel() {
        return model ;
    }

    public ArrayList<IFProject> getProjects() {
        return projects ;
    }

    public void addProjects(String xml)
        throws Exception
    {
        FProjectReader reader = new FProjectReader(xml) ;
        ArrayList<IFProject> newProjects = reader.readProjects();

        Iterator it = newProjects.iterator() ;
        while(it.hasNext()) {
            IFProject project = (IFProject) it.next();
            
            root.add(project.getArborescence());
            projects.add(project) ;
            
            model.nodeStructureChanged(root) ;
        }
    }

}
