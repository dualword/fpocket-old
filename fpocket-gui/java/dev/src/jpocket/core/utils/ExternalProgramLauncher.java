/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.core.utils;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 *
 * @author Noé
 */
public class ExternalProgramLauncher
{
    public String [] arguments ;

    public ExternalProgramLauncher(String [] arguments) {
        this.arguments = arguments ;
    }


    public ExternalProgramLauncher(String command) {
        arguments = command.split(" ") ;
    }

    public int launch(boolean verbose)
            throws Exception
    {
        System.out.println("Executing command: ");
        for(int i = 0 ; i < arguments.length ; i++) {
            System.out.print(arguments[i]+" ");
        }
        
        Runtime rt = Runtime.getRuntime();
        Process proc = rt.exec(arguments);
        // any error message?
        StreamGobbler errorGobbler = new
            StreamGobbler(proc.getErrorStream(), "ERROR", verbose);

        // any output?
        StreamGobbler outputGobbler = new
            StreamGobbler(proc.getInputStream(), "OUTPUT", verbose);

        // kick them off
        Exception e = errorGobbler.go();
        if(e != null) throw e ;

        e = outputGobbler.go();
        if(e != null) throw e ;

        // any error???
        int exitVal = proc.waitFor();
        if(verbose) System.out.println("ExitValue: " + exitVal);

        return exitVal ;
    }

    class StreamGobbler extends Thread
    {
        private InputStream is;
        private String type;
        private boolean verbose ;
        private Exception e ;

        StreamGobbler(InputStream is, String type, boolean verbose)
        {
            this.is = is;
            this.type = type;
            this.verbose = verbose ;
            this.e = null ;
        }

        public Exception go()
        {
            run() ;
            return e ;
        }

        @Override
        public void run()
        {
            try {
                InputStreamReader isr = new InputStreamReader(is);
                BufferedReader br = new BufferedReader(isr);
                String line=null;
                while ( (line = br.readLine()) != null) {
                    if(verbose) {
                        System.out.println(type + ">" + line);
                    }
                }
            } 
            catch (IOException ioe) {
                e = ioe ;
            }
        }
    }
}
