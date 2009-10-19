/**

## ----- GENERAL INFORMATION
##
## FILE 		JPDBFile.java
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

import java.io.File;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import javax.swing.tree.DefaultMutableTreeNode;
import jpocket.core.odata.listeners.JPDBChangeEvent;
import jpocket.core.odata.listeners.JPDBChangeEvent.JPDBEvent;
import jpocket.core.odata.listeners.JPDBChangeListener;
import jpocket.core.utils.Utils;

/**
 *
 * @author vince
 */
public class JPDBFile extends File
{
    private Set<JPDBChangeListener> listeners ;

    private String fourLetterCode ;

    private File fpocketOutputLocation ;
    private List<JPDBLig> ligands ;

    private JLibrary owner ;
    
    public JPDBFile(JLibrary owner, String path, String fourLetterCode, String fpocketOutputLocation)
    {
        super(path) ;
        
        this.listeners = new HashSet() ;
        this.owner = owner ;
        this.fourLetterCode = fourLetterCode;

        if(fourLetterCode == null || fourLetterCode.equals("")) {
            fourLetterCode = Utils.extractFourLetterCode(path) ;
        }

        if(fpocketOutputLocation != null)
            this.fpocketOutputLocation = new File(fpocketOutputLocation);

        this.ligands = new ArrayList() ;
    }

    public JPDBFile(JLibrary parent, String path, String fourLetterCode)
    {
        this(parent, path, fourLetterCode, null) ;
    }

    public JLibrary getOwnerLib() {
        return owner;
    }

    public void setOwnerLib(JLibrary lib) {
        this.owner = lib;
        notifyChange(JPDBEvent.OWNER_CHANGED);
    }

    public void addLig(JPDBLig lig) {
        ligands.add(lig) ;
        notifyChange(JPDBEvent.LIGANDS_CHANGED);
    }

    public void removeLig(JPDBLig lig) {
        ligands.remove(lig) ;
        notifyChange(JPDBEvent.LIGANDS_CHANGED);
    }

    public Collection<JPDBLig> getLigands() {
        return ligands;
    }

    public String getFourLetterCode() {
        return fourLetterCode;
    }

    public void setFourLetterCode(String fourLetterCode) {
        this.fourLetterCode = fourLetterCode;
    }

    public File getFpocketOutputLocation() {
        return fpocketOutputLocation;
    }

    public void setFpocketOutputLocation(File fpocketOutputLocation) {
        this.fpocketOutputLocation = fpocketOutputLocation;
        notifyChange(JPDBEvent.FPOUT_CHANGED);
    }

    public DefaultMutableTreeNode toTreeNode()
    {
        DefaultMutableTreeNode node = new DefaultMutableTreeNode(this) ;

//        for(int i = 0 ; i < ligands.size() ; i++) {
//            node.add(new DefaultMutableTreeNode(ligands.get(i)));
//        }

        return node ;
    }

    public void addPDBChangeEventListener(JPDBChangeListener listener) {
        this.listeners.add(listener) ;
    }

    public void notifyChange(JPDBEvent event) {
        for(JPDBChangeListener l: listeners) {
            l.dataChange(new JPDBChangeEvent(this, event));
        }
    }
}
