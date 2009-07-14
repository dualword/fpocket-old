/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.gui.tree;

import jpocket.gui.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JOptionPane;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreePath;
import jpocket.gui.tree.popup.LibraryPopup;
import jpocket.gui.tree.popup.LigandPopup;
import jpocket.gui.tree.popup.PDBPopup;
import jpocket.gui.tree.popup.ProjectPopup;
import jpocket.projects.IFLibrary;
import jpocket.projects.IFProject;
import jpocket.projects.PDBFile;
import jpocket.projects.PDBLigand;

/**
 *
 * @author Noé
 */
public class JPocketProjectTree extends JTree
{
    private ProjectPopup projectPopup ;
    private LigandPopup ligandPopup ;
    private PDBPopup pdbPopup ;
    private LibraryPopup libraryPopup ;

    private JProjectTreeModel model ;

    private JTree instance ;

    public JPocketProjectTree()
    {
        super() ;

        instance = this ;

        projectPopup = new ProjectPopup(null) ;
        ligandPopup = new LigandPopup(null) ;
        pdbPopup = new PDBPopup(null) ;
        libraryPopup = new LibraryPopup(null) ;
        
        model = new JProjectTreeModel();
        setCellRenderer(new JProjectTreeCellRenderer()) ;
        setModel(model.getModel());
        repaint();
        revalidate();

        // Add listener for popup menus
        addMouseListener(new PopupMouseListener()) ;
    }

    public void addProjectsXML(String xml)
    {
        try {
            model.addProjects(xml);
        }
        catch (Exception ex) {
            Logger.getLogger(JPocketGUI.class.getName()).log(Level.SEVERE, null, ex);
            JOptionPane.showMessageDialog(this, "Error while loading XML file.\n"
                                            +"Message:\n\n"+ex.getMessage());
        }
        
        repaint();
        revalidate();
    }

    class PopupMouseListener extends MouseAdapter {
        @Override
        public void mouseReleased(MouseEvent e) {
            if(e.isPopupTrigger()) {
                //System.out.println("Source: "+e.getSource());
                // Get the user object selected
                TreePath path = getPathForLocation(e.getX(), e.getY()) ;
                if(path == null) return ;
                
                DefaultMutableTreeNode tn = (DefaultMutableTreeNode) path.getLastPathComponent() ;

                Object userObj = tn.getUserObject() ;

                // Trigger the right popup, given which tree node have been
                // selected
                if(userObj instanceof IFProject) {
                    IFProject project = (IFProject) userObj ;
                    projectPopup.setProject(project) ;
                    projectPopup.show(instance, e.getX(), e.getY());
                }
                else if(userObj instanceof IFLibrary) {
                    IFLibrary lib = (IFLibrary) userObj ;
                    libraryPopup.setLib(lib) ;
                    libraryPopup.show(instance, e.getX(), e.getY());
                }
                else if(userObj instanceof PDBFile) {
                    PDBFile pdb = (PDBFile) userObj ;
                    pdbPopup.setPDB(pdb) ;
                    pdbPopup.show(instance, e.getX(), e.getY());
                }
                else if(userObj instanceof PDBLigand) {
                    PDBLigand lig = (PDBLigand) userObj ;
                    ligandPopup.setLigand(lig) ;
                    ligandPopup.show(instance, e.getX(), e.getY());
                }
            }
        }
    }
}
