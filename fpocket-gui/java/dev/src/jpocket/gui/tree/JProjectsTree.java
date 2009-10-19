/**

## ----- GENERAL INFORMATION
##
## FILE 		JProjectsTree.java
## AUTHORS		VL
## CREATED  	Oct 4, 2009
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

package jpocket.gui.tree;

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.IOException;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreePath;
import jpocket.gui.tree.popup.LibraryPopup;
import jpocket.gui.tree.popup.LigandPopup;
import jpocket.gui.tree.popup.PDBPopup;
import jpocket.gui.tree.popup.ProjectPopup;
import jpocket.gui.tree.popup.RootPopup;
import jpocket.core.odata.JLibrary;
import jpocket.core.odata.JPDBFile;
import jpocket.core.odata.JPDBLig;
import jpocket.core.odata.JProject;
import org.jdom.JDOMException;

/**
 *
 * @author vince
 */
public class JProjectsTree extends JTree
{
    public static final String LIB_PROP = "LIB_P" ;
    public static final String PDB_PROP = "PDB_P" ;

    private RootPopup rootPopup ;
    private ProjectPopup projectPopup ;
    private LigandPopup ligandPopup ;
    private PDBPopup pdbPopup ;
    private LibraryPopup libraryPopup ;

    private JProjectsTreeModel model ;

    private JTree instance ;

    private JPDBFile selectedPdb ;
    private JLibrary selectedLib;

    public JProjectsTree(JProjectsTreeModel model)
    {
        super(model) ;

        this.model = model ;
        this.selectedPdb = null ;
        this.selectedLib = null ;
        
        instance = this ;

        setCellRenderer(new JProjectsTreeCellRenderer()) ;
        repaint();
        revalidate();

        // Add listener for popup menus
        addMouseListener(new PopupMouseListener()) ;
    }
    public JProjectsTree()
    {
        this(null) ;
    }

    public void addProjectXML(String xml)
            throws JDOMException, IOException
    {
        System.out.println("Adding "+xml);
        model.addProject(xml);

        setModel(model) ;
        repaint();
        revalidate();
    }

    class PopupMouseListener extends MouseAdapter
    {
        @Override
        public void mouseReleased(MouseEvent e)
        {
            if(e.getButton()==MouseEvent.BUTTON2 || e.getButton() == MouseEvent.BUTTON3) {
                popupButtonAction(e);
            }
            else if(e.getButton() == MouseEvent.BUTTON1) {
                leftButtonAction(e);
            }
        }
    }

    private void leftButtonAction(MouseEvent e) 
    {
        // Get the user object selected
        TreePath path = getPathForLocation(e.getX(), e.getY()) ;
        if(path == null) return ;

        DefaultMutableTreeNode tn = (DefaultMutableTreeNode) path.getLastPathComponent() ;
        Object userObj = tn.getUserObject() ;

        // Trigger the right popup, given which tree node have been
        // selected
        System.out.println("User object: "+userObj);
        if(userObj instanceof JPDBFile) {
            System.out.println("Firering property: "+userObj);
            firePropertyChange(PDB_PROP, selectedPdb, userObj);
            this.selectedPdb = (JPDBFile) userObj ;
        }
        else if(userObj instanceof JLibrary) {
            firePropertyChange(LIB_PROP, selectedLib, userObj);
            this.selectedLib = (JLibrary) userObj ;
        }
    }

    private void popupButtonAction(MouseEvent e)
    {
        // Get the user object selected
        TreePath path = getPathForLocation(e.getX(), e.getY()) ;
        if(path == null) return ;

        DefaultMutableTreeNode tn = (DefaultMutableTreeNode) path.getLastPathComponent() ;
        Object userObj = tn.getUserObject() ;

        System.out.println("User object: "+userObj);
        if(tn.getLevel() == 0) {
            if(rootPopup == null)
                rootPopup = new RootPopup(model, tn) ;

            rootPopup.setNode(tn) ;
            rootPopup.show(instance, e.getX(), e.getY());
        }
        else if(userObj instanceof JProject) {
            if(projectPopup == null)
                projectPopup = new ProjectPopup(model, tn) ;

            projectPopup.setNode(tn) ;
            projectPopup.show(instance, e.getX(), e.getY());
        }
        else if(userObj instanceof JLibrary) {
            if(libraryPopup == null)
                libraryPopup = new LibraryPopup(model, tn) ;

            libraryPopup.setNode(tn) ;
            libraryPopup.show(instance, e.getX(), e.getY());
        }
        else if(userObj instanceof JPDBFile) {
            if(pdbPopup == null)
                pdbPopup = new PDBPopup(model, tn) ;

            pdbPopup.setNode(tn) ;
            pdbPopup.show(instance, e.getX(), e.getY());
        }
        else if(userObj instanceof JPDBLig) {
            if(ligandPopup == null)
                ligandPopup = new LigandPopup(model, tn) ;

            ligandPopup.setNode(tn) ;
            ligandPopup.show(instance, e.getX(), e.getY());
        }
    }
}
