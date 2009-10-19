
/**

## ----- GENERAL INFORMATION
##
## FILE 		FpocketWorkerParams.java
## AUTHORS		VL
## CREATED  	Oct 18, 2009
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
package jpocket.gui.workers;

import java.util.ArrayList;
import java.util.List;
import jpocket.core.odata.JPDBFile;

/**
 *
 * @author vince
 */
public class FpocketWorkerParams
{

    List<JPDBFile> pdbs;

    public FpocketWorkerParams() {
        this.pdbs = new ArrayList();
    }

    public FpocketWorkerParams(List<JPDBFile> pdbs) {
        this.pdbs = pdbs;
    }

    public List<JPDBFile> getPdbs() {
        return pdbs;
    }

    public void setPdbs(List<JPDBFile> pdbs) {
        this.pdbs = pdbs;
    }

    public void addPdb(JPDBFile pdb) {
        if (this.pdbs == null) {
            pdbs = new ArrayList();
        }
        pdbs.add(pdb);
    }
}
