/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * JPocketGUI.java
 *
 * Created on 4 juil. 2009, 14:48:02
 */

package jpocket.gui;

import jpocket.gui.pockets.PocketsPanel;
import com.vlsolutions.swing.docking.DockingDesktop;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.tree.DefaultMutableTreeNode;
import jpocket.core.JPocket;
import jpocket.gui.editors.FpocketParamsEditors;
import jpocket.gui.jmol.JmolEvolvedPanel;
import jpocket.gui.jmol.JmolPanel;
import jpocket.gui.tree.JProjectsTree;
import jpocket.gui.tree.JProjectsTreeModel;
import jpocket.core.odata.JPDBFile;
import jpocket.core.utils.Utils;
import org.biojava.bio.structure.Structure;
import org.biojava.bio.structure.io.PDBFileReader;
import org.jdom.JDOMException;

/**
 *
 * @author Administrateur
 */
public class JPocketGUI extends javax.swing.JFrame 
    implements PropertyChangeListener
{
    private DockablePanel treePanel ;
    private DockablePanel rasmolPanel ;
    private DockablePanel pocketPanel ;
    private DockablePanel ligandsPanel ;
    private DockablePanel viewerPanel ;

    private JProjectsTree tree ;
    private JProjectsTreeModel treeModel ;

    private RasmolCommandPanel rasmolComPanel ;

    private DockingDesktop desk ;

    private JmolPanel jmolPanel ;
    private JmolEvolvedPanel jmolFullPanel;
    private PocketsPanel pocketsPanel;

    /** Creates new form JPocketGUI */
    public JPocketGUI() {
        initComponents();
        updateComponents() ;
    }

    private void updateComponents()
    {
        initDocks();
        
        // Create the tree model
        treeModel = new JProjectsTreeModel(new DefaultMutableTreeNode("Fpocket Projects")) ;
        tree = new JProjectsTree(treeModel) ;
        tree.setPreferredSize(new Dimension(200, 300)) ;
        tree.addPropertyChangeListener(this) ;
        
        JScrollPane jsp = new JScrollPane() ;
        jsp.setViewportView(tree) ;
        jsp.setPreferredSize(new Dimension(200, 300)) ;
        treePanel.add(jsp, BorderLayout.CENTER) ;

        // Set the viewer: simplest way
        jmolPanel = new JmolPanel();
        viewerPanel.add(jmolPanel, BorderLayout.CENTER) ;

        // Set the viewer: test way
//        jmolFullPanel = new JmolEvolvedPanel();
//        viewerPanel.add(jmolFullPanel, BorderLayout.CENTER) ;

        rasmolComPanel = new RasmolCommandPanel() ;
        rasmolComPanel.addPropertyChangeListener(this) ;
        rasmolPanel.add(rasmolComPanel, BorderLayout.CENTER) ;

        pocketsPanel = new PocketsPanel(this, null);
        pocketPanel.add(pocketsPanel, BorderLayout.CENTER) ;
    }


    public void initDocks()
    {
//        filtersPane = new DockedTabbedPane("Filtering - Other", "filterView") ;
//        filtersPane.getDockKey().setFloatEnabled(true);
//        filtersPane.getDockKey().setCloseEnabled(false);

        treePanel = new DockablePanel("Fpocket projects", "treeDock") ;
        treePanel.getDockKey().setFloatEnabled(true);
        treePanel.getDockKey().setCloseEnabled(false);
        treePanel.setLayout(new BorderLayout()) ;

        rasmolPanel = new DockablePanel("Rasmol command line", "rasmolDock") ;
        rasmolPanel.getDockKey().setFloatEnabled(true);
        rasmolPanel.getDockKey().setCloseEnabled(false);
        rasmolPanel.setLayout(new BorderLayout()) ;

        pocketPanel = new DockablePanel("(f)Pockets", "pocketDock") ;
        pocketPanel.getDockKey().setFloatEnabled(true);
        pocketPanel.getDockKey().setCloseEnabled(false);
        pocketPanel.setLayout(new BorderLayout()) ;

        ligandsPanel = new DockablePanel("Ligands", "ligandsDock") ;
        ligandsPanel.getDockKey().setFloatEnabled(true);
        ligandsPanel.getDockKey().setCloseEnabled(false);

        viewerPanel = new DockablePanel("Viewer", "jmolViewDock") ;
        viewerPanel.getDockKey().setFloatEnabled(true);
        viewerPanel.getDockKey().setCloseEnabled(false);
        viewerPanel.setLayout(new BorderLayout()) ;

        desk = new DockingDesktop() ;

        try {
//            desk.registerDockable(filtersPane);
            desk.registerDockable(treePanel);
            desk.registerDockable(rasmolPanel);
            desk.registerDockable(pocketPanel);
            desk.registerDockable(ligandsPanel);
            desk.registerDockable(viewerPanel);

            BufferedInputStream in = new BufferedInputStream(
                    new FileInputStream(JPocket.ENV_PATH+"settings/desktopLayout.xml"));

            // Load the workspace
            desk.readXML(in);

            in.close();
        }
        catch (Exception e){
            // Well... Do it manually
            e.printStackTrace();
        }

        getContentPane().add(desk, BorderLayout.CENTER);
    }

    
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        statusPanel = new javax.swing.JPanel();
        Status1 = new javax.swing.JLabel();
        jSeparator1 = new javax.swing.JSeparator();
        jSeparator2 = new javax.swing.JSeparator();
        jProgressBar1 = new javax.swing.JProgressBar();
        menuBar = new javax.swing.JMenuBar();
        fileMenu = new javax.swing.JMenu();
        openProjectMenu = new javax.swing.JMenuItem();
        saveMenuItem = new javax.swing.JMenuItem();
        saveAsMenuItem = new javax.swing.JMenuItem();
        exitMenuItem = new javax.swing.JMenuItem();
        editMenu = new javax.swing.JMenu();
        fpocketMenu = new javax.swing.JMenuItem();
        copyMenuItem = new javax.swing.JMenuItem();
        pasteMenuItem = new javax.swing.JMenuItem();
        deleteMenuItem = new javax.swing.JMenuItem();
        runMenu = new javax.swing.JMenu();
        helpMenu = new javax.swing.JMenu();
        contentsMenuItem = new javax.swing.JMenuItem();
        aboutMenuItem = new javax.swing.JMenuItem();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("JPocket - Fpocket GUI");

        statusPanel.setLayout(new java.awt.BorderLayout());

        Status1.setText("Status1");
        statusPanel.add(Status1, java.awt.BorderLayout.CENTER);
        statusPanel.add(jSeparator1, java.awt.BorderLayout.PAGE_START);
        statusPanel.add(jSeparator2, java.awt.BorderLayout.PAGE_END);
        statusPanel.add(jProgressBar1, java.awt.BorderLayout.LINE_END);

        getContentPane().add(statusPanel, java.awt.BorderLayout.PAGE_END);

        fileMenu.setText("File");

        openProjectMenu.setIcon(new javax.swing.ImageIcon(getClass().getResource("/jpocket/ressources/icons/benzene.png"))); // NOI18N
        openProjectMenu.setText("Open projects");
        openProjectMenu.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                openProjectMenuActionPerformed(evt);
            }
        });
        fileMenu.add(openProjectMenu);

        saveMenuItem.setText("Save projects");
        fileMenu.add(saveMenuItem);

        saveAsMenuItem.setText("Save projects As ...");
        fileMenu.add(saveAsMenuItem);

        exitMenuItem.setText("Exit");
        exitMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                exitMenuItemActionPerformed(evt);
            }
        });
        fileMenu.add(exitMenuItem);

        menuBar.add(fileMenu);

        editMenu.setText("Configure");

        fpocketMenu.setText("Fpocket");
        fpocketMenu.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                fpocketMenuActionPerformed(evt);
            }
        });
        editMenu.add(fpocketMenu);

        copyMenuItem.setText("Dpocket");
        editMenu.add(copyMenuItem);

        pasteMenuItem.setText("Tpocket");
        editMenu.add(pasteMenuItem);

        deleteMenuItem.setText("MDpocket");
        editMenu.add(deleteMenuItem);

        menuBar.add(editMenu);

        runMenu.setText("Run");
        menuBar.add(runMenu);

        helpMenu.setText("Help");

        contentsMenuItem.setText("Contents");
        helpMenu.add(contentsMenuItem);

        aboutMenuItem.setText("About");
        helpMenu.add(aboutMenuItem);

        menuBar.add(helpMenu);

        setJMenuBar(menuBar);

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void exitMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_exitMenuItemActionPerformed
        System.exit(0);
    }//GEN-LAST:event_exitMenuItemActionPerformed

    private void openProjectMenuActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_openProjectMenuActionPerformed
        String [] xml = Utils.openFiles("Select XML project file(s)", "XML", this) ;
        if(xml == null) return ;
        
        for(int i = 0 ; i < xml.length ; i++) {
            try {
                tree.addProjectXML(xml[i]);
            }
            catch (JDOMException ex) {
                ex.printStackTrace();
                JOptionPane.showMessageDialog(this, "XML Error while loading file "+xml[i]+
                        "\nMessage: \n\n"+ex.getLocalizedMessage());
            }
            catch (IOException ex) {
                ex.printStackTrace();
                JOptionPane.showMessageDialog(this, "IO Error while loading file "+xml[i]+
                        "\nMessage: \n\n"+ex.getLocalizedMessage());
            }
        }
    }//GEN-LAST:event_openProjectMenuActionPerformed

    private void fpocketMenuActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_fpocketMenuActionPerformed
        FpocketParamsEditors e = new FpocketParamsEditors(this,
                                                          true) ;
        e.setVisible(true);
        e.setLocation(200, 200);
}//GEN-LAST:event_fpocketMenuActionPerformed

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JLabel Status1;
    private javax.swing.JMenuItem aboutMenuItem;
    private javax.swing.JMenuItem contentsMenuItem;
    private javax.swing.JMenuItem copyMenuItem;
    private javax.swing.JMenuItem deleteMenuItem;
    private javax.swing.JMenu editMenu;
    private javax.swing.JMenuItem exitMenuItem;
    private javax.swing.JMenu fileMenu;
    private javax.swing.JMenuItem fpocketMenu;
    private javax.swing.JMenu helpMenu;
    private javax.swing.JProgressBar jProgressBar1;
    private javax.swing.JSeparator jSeparator1;
    private javax.swing.JSeparator jSeparator2;
    private javax.swing.JMenuBar menuBar;
    private javax.swing.JMenuItem openProjectMenu;
    private javax.swing.JMenuItem pasteMenuItem;
    private javax.swing.JMenu runMenu;
    private javax.swing.JMenuItem saveAsMenuItem;
    private javax.swing.JMenuItem saveMenuItem;
    private javax.swing.JPanel statusPanel;
    // End of variables declaration//GEN-END:variables

    public void propertyChange(PropertyChangeEvent evt) 
    {
        if(evt.getPropertyName().equals(JProjectsTree.PDB_PROP))
        {
            JPDBFile f = (JPDBFile) evt.getNewValue() ;

            PDBFileReader pdbr = new PDBFileReader();
            pdbr.setPath(f.getParent());

            String pdbCode = f.getFourLetterCode() ;
            Structure struc = null ;
            try {
                struc = pdbr.getStructureById(pdbCode);
            }
            catch (IOException ex) {
                JOptionPane.showMessageDialog(this, "Error while loading "
                        +f.getFourLetterCode()+
                        "\nMessage:\n\n"+ex.getLocalizedMessage()) ;
                ex.printStackTrace();
                return ;
            }

            if(jmolPanel != null) jmolPanel.setStructure(struc);
            if(jmolFullPanel != null) jmolFullPanel.setStructure(struc) ;

            // If fpocket output path is available, well load it.
            
        }
        else if(evt.getPropertyName().equals(RasmolCommandPanel.RASMOL_COM)) {
            String com = (String) evt.getNewValue() ;
            System.out.println("Executing command: "+com);
            if(jmolPanel != null) jmolPanel.executeCmd(com);
            if(jmolFullPanel != null) jmolFullPanel.executeCmd(com);
        }
    }
}
