/**

## ----- GENERAL INFORMATION
##
## FILE 		FpocketWorker.java
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

import java.io.File;
import java.util.Iterator;
import java.util.List;
import jpocket.core.fpocket.FPocketParameters;
import jpocket.core.fpocket.Parameter;
import jpocket.core.fpocket.ParametersSet;
import jpocket.core.odata.JPDBFile;
import jpocket.core.utils.ExternalProgramLauncher;

/**
 *
 * @author vince
 */
public class FpocketWorker extends Worker
{
    private FpocketWorkerParams params;

    public FpocketWorker(FpocketWorkerParams params) {
        this.params = params ;
    }

    @Override
    protected Object doInBackground()
            throws Exception
    {
        List<Parameter> pars = FPocketParameters.instance().getExistingParameters() ;
        ParametersSet set = FPocketParameters.instance().getParametersValues() ;

        System.out.println("Set; "+set);
        List<JPDBFile> pdbs = params.getPdbs() ;
        int pdbCount = 0 ;
        int pdbSize = pdbs.size() ;

        sendMessage("\n"+pdbSize+" PDB(s) to process.",
                    WorkerProgressFrame.LOG_PROP, null) ;

        // Setting params commands:
        Iterator<Parameter> it = pars.iterator() ;
        String options = "" ;
        while(it.hasNext()) {
            Parameter p = it.next() ;
            String value = set.getParameterValue(p.getCommand()) ;

            if(value != null && ! value.equals("")) {
                sendMessage("\nParameter "+p.getName()+" set to "+value.trim(),
                            WorkerProgressFrame.LOG_PROP, null) ;
                options += " "+p.getCommand()+" "+value.trim() ;
            }
        }

        // Declare some variables
        String command = null ;

        JPDBFile pdb = null ;
        ExternalProgramLauncher epl = null ;

        for(int i = 0 ; i < pdbSize ; i++) {
            pdbCount++ ;

            pdb = pdbs.get(i) ;
            updateInfo("Processing pdb "+pdbCount+" / "+pdbSize+" : "+pdb.getFourLetterCode()) ;

            command = "fpocket -f "+pdb.getAbsolutePath()+options ;
            System.out.println("Executing command "+command);
            epl = new ExternalProgramLauncher(command) ;

            try {
                epl.launch(true);
                pdb.setFpocketOutputLocation(new File(pdb.getParent()+pdb.getFourLetterCode()+"_out"));
            }
            catch(Exception e) {
                e.printStackTrace() ;
                sendMessage("\nError with PDB file "+pdb.getAbsolutePath(),
                            WorkerProgressFrame.ERR_PROP, null) ;
            }

            setProgress(Math.round((float)pdbCount/(float)pdbSize));
        }

        return pdbCount ;
    }
}
