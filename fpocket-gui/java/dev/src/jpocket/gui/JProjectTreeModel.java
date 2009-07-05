/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.gui;

import java.util.ArrayList;
import java.util.Iterator;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import jpocket.projects.FProjectReader;
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
    }

    public JProjectTreeModel(String xml) {
        this() ;
        
        try {
            updateModel(xml);
        }
        catch (Exception ex) {
            root = new DefaultMutableTreeNode("ERROR") ;
            model = new DefaultTreeModel(root);
            ex.printStackTrace();
        }
    }

    public DefaultTreeModel getModel() {
        return model ;
    }

    public ArrayList<IFProject> getProjects() {
        return projects ;
    }

    public void updateModel(String xml)
        throws Exception
    {
        root.removeAllChildren() ;
        
        FProjectReader reader = new FProjectReader(xml) ;
        
        projects = reader.readProjects();

        Iterator it = projects.iterator() ;
        while(it.hasNext()) {
            IFProject project = (IFProject) it.next();
            root.add(project.getArborescence());
        }
    }

}
