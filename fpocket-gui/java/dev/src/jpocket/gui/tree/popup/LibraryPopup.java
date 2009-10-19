/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.gui.tree.popup;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.security.InvalidParameterException;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JSeparator;
import javax.swing.SwingConstants;
import javax.swing.tree.DefaultMutableTreeNode;
import jpocket.gui.creators.JLibraryCreatorDialog;
import jpocket.gui.editors.JLibraryEditorDialog;
import jpocket.gui.tree.JProjectsTreeModel;
import jpocket.gui.workers.FpocketWorker;
import jpocket.gui.workers.FpocketWorkerParams;
import jpocket.gui.workers.WorkerProgressFrame;
import jpocket.core.odata.JLibrary;
import jpocket.core.odata.JPDBFile;
import jpocket.core.utils.Utils;

/**
 *
 * @author Noé
 */
public class LibraryPopup extends AbstractTreePopup implements ActionListener
{
    public static final String NEWLIB_ACTION    = "newLib" ;
    public static final String NEWPDB_ACTION    = "newPDB" ;
    public static final String FPOCKET_ACTION   = "launchFpocket" ;
    public static final String TPOCKET_ACTION   = "launchTpocket" ;
    public static final String EDIT_ACTION      = "editLib" ;
    public static final String REMOVE_ACTION    = "removeLib" ;

    public static final String [][] MENU = {
                    {"New library",     NEWLIB_ACTION },
                    {"New PDB",         NEWPDB_ACTION },
                    {"Separator",       ""},
                    {"Edit library",    EDIT_ACTION},
                    {"Remove library",  REMOVE_ACTION},
                    {"Separator",       ""},
                    {"Launch Fpocket",  FPOCKET_ACTION},
                    {"Launch Tpocket",  TPOCKET_ACTION}
                } ;
    
    private JLibrary lib ;

    public LibraryPopup(JProjectsTreeModel treeModel, DefaultMutableTreeNode node) {
        super(treeModel, node) ;

        initMenus() ;

        setOpaque(true) ;
        setLightWeightPopupEnabled(true);
    }

    private void initMenus()
    {
        JMenuItem menuItem = null ;
        for(int i = 0 ; i < MENU.length ; i++) {
            if(MENU[i][0].equals("Separator")) {
                add(new JSeparator(SwingConstants.HORIZONTAL));
            }
            else {
                menuItem = new JMenuItem(MENU[i][0]) ;
                menuItem.setActionCommand(MENU[i][1]);
                menuItem.addActionListener(this) ;
                add(menuItem) ;
            }
        }
    }

    @Override
    public void setNode(DefaultMutableTreeNode node) {
        this.node = node ;

        if(node.getUserObject() instanceof JLibrary)
            this.lib = (JLibrary) node.getUserObject() ;
        else 
            throw new InvalidParameterException("Node must be library!") ;
    }
    
    public JLibrary getLib() {
        return lib;
    }

    public void setLib(JLibrary lib) {
        this.lib = lib ;
    }

    public void actionPerformed(ActionEvent e) {
        if(e.getActionCommand().equals(NEWLIB_ACTION)) {
            JLibraryCreatorDialog dialog = new JLibraryCreatorDialog(null,
                                                                     true) ;
            dialog.setLocation(200, 200);
            dialog.setVisible(true);
            if(dialog.getReturnStatus() == JLibraryCreatorDialog.RET_OK) {
                JLibrary newLib = dialog.getLibrary() ;
                lib.addLibrary(newLib);
                node.add(newLib.toTreeNode());
                treeModel.nodeStructureChanged(node);
            }
        }
        else if(e.getActionCommand().equals(NEWPDB_ACTION)) {
            String [] pdbs = Utils.openFiles("Select XML project file(s)", "PDB", this) ;
            for(int i = 0 ; i < pdbs.length ; i++) {
                JPDBFile pdb = new JPDBFile(lib, pdbs[i], Utils.extractFourLetterCode(pdbs[i])) ;
                lib.addPDB(pdb);
                node.add(pdb.toTreeNode()) ;
            }
            treeModel.nodeStructureChanged(node);
        }
        else if(e.getActionCommand().equals(EDIT_ACTION)) {
            JLibraryEditorDialog dialog = new JLibraryEditorDialog(lib, null,
                                                                   true) ;
            dialog.setLocation(200, 200);
            dialog.setVisible(true);
            
            if(dialog.getReturnStatus() == JLibraryCreatorDialog.RET_OK) {
                treeModel.nodeChanged(node);
            }
        }
        else if(e.getActionCommand().equals(REMOVE_ACTION)) {
            if(lib.getPdbs().size() > 0) {
                int res = JOptionPane.showConfirmDialog(this,
                        "Also delete pdb files from hard drive?") ;
                if(res != JOptionPane.CANCEL_OPTION) {
                    if(res == JOptionPane.OK_OPTION) {
                        lib.removeAll(true) ;
                    }

                    if(lib.getParent() != null)
                        lib.getParent().removeLibrary(lib, false);

                    treeModel.removeNodeFromParent(node);
                }
            }
            else {
                if(lib.getParent() != null)
                    lib.getParent().removeLibrary(lib, false);

                treeModel.removeNodeFromParent(node);
                
            }
        }
        else if(e.getActionCommand().equals(FPOCKET_ACTION)) {
            FpocketWorkerParams params = new FpocketWorkerParams() ;
            params.setPdbs(lib.getPdbsAndSubLibPdbs()) ;

            FpocketWorker fw = new FpocketWorker(params);
            WorkerProgressFrame wpf = new WorkerProgressFrame(fw, "Running fpocket...",
                                                             this.getLocation()) ;
            fw.addPropertyChangeListener(wpf);
            wpf.startWorker();
        }
        else if(e.getActionCommand().equals(TPOCKET_ACTION)) {

        }
    }
    
}
