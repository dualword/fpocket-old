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
import jpocket.gui.tree.JProjectsTreeModel;
import jpocket.gui.workers.FpocketWorker;
import jpocket.gui.workers.FpocketWorkerParams;
import jpocket.gui.workers.WorkerProgressFrame;
import jpocket.core.odata.JPDBFile;

/**
 *
 * @author Noé
 */
public class PDBPopup extends AbstractTreePopup implements ActionListener
{
    public static final String EDIT_ACTION          = "editPDB" ;
    public static final String REMOVE_ACTION        = "removePDB" ;
    public static final String FPOCKET_ACTION       = "launchFpocket" ;

    public static final String [][] MENU = {
                    {"Edit PDB",        EDIT_ACTION },
                    {"Remove PDB",      REMOVE_ACTION},
                    {"Separator",       ""},
                    {"Launch Fpocket",  FPOCKET_ACTION},
                } ;
    
    private JPDBFile pdb ;

    public PDBPopup(JProjectsTreeModel treeModel, DefaultMutableTreeNode node) {
        super(treeModel, node) ;

        initMenus() ;

        setOpaque(true) ;
        setLightWeightPopupEnabled(true);
    }

    @Override
    public void setNode(DefaultMutableTreeNode node) {
        this.node = node ;

        if(node.getUserObject() instanceof JPDBFile)
            setPdb((JPDBFile) node.getUserObject()) ;
        else
            throw new InvalidParameterException("Node must be library!") ;
    }

    private void initMenus() {
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
    
    public JPDBFile getPdb() {
        return pdb;
    }

    public void setPdb(JPDBFile pdb) {
        this.pdb = pdb ;
    }

    public void actionPerformed(ActionEvent e) {
        if(e.getActionCommand().equals(EDIT_ACTION)) {
            
        }
        else if(e.getActionCommand().equals(REMOVE_ACTION)) {
            int res = JOptionPane.showConfirmDialog(this,
                    "Also delete pdb files from hard drive?") ;
            if(res != JOptionPane.CANCEL_OPTION) {
                boolean removeFromDisc = (res == JOptionPane.OK_OPTION) ? true:false ;
                pdb.getOwnerLib().removePDB(pdb, removeFromDisc);
                treeModel.removeNodeFromParent(node);
            }
        }
        else if(e.getActionCommand().equals(FPOCKET_ACTION)) {
            FpocketWorkerParams params = new FpocketWorkerParams() ;
            params.addPdb(pdb) ;

            FpocketWorker fw = new FpocketWorker(params);
            WorkerProgressFrame wpf = new WorkerProgressFrame(fw, "Running fpocket...",
                                                             this.getLocation()) ;
            fw.addPropertyChangeListener(wpf);
            wpf.startWorker();
        }
    }
}
