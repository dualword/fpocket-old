/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.projects;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Administrateur
 */
public class PDBFile implements IFProjectObject
{
    private String name ;
    private String file ;
    private String fpocketOutputPath ;
    private List<PDBLigand> ligands ;

    public PDBFile(String name, String file) {
        this.name = name;
        this.file = file;
    }

    public PDBFile(String name, String file, String fpocketOutputPath) {
        this.name = name;
        this.file = file;
        this.fpocketOutputPath = fpocketOutputPath;
    }

    public PDBFile(String name, String file, String fpocketOutputPath, List<PDBLigand> ligands) {
        this.name = name;
        this.file = file;
        this.fpocketOutputPath = fpocketOutputPath;
        this.ligands = ligands;
    }

    public String getFile() {
        return file;
    }

    public void setFile(String file) {
        this.file = file;
    }

    public String getFpocketOutputPath() {
        return fpocketOutputPath;
    }

    public void setFpocketOutputPath(String fpocketOutputPath) {
        this.fpocketOutputPath = fpocketOutputPath;
    }

    public List<PDBLigand> getLigands() {
        return ligands;
    }

    public void setLigands(List<PDBLigand> ligands) {
        this.ligands = ligands;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
    
    public PDBLigand addLigand(PDBLigand ligand) {
        if(this.ligands == null) {
            ligands = new ArrayList() ;
        }

        ligands.add(ligand) ;
    
        return ligand ;
    }

    public String getType() {
        return "PDB" ;
    }

    public String getPDB() {
        return this.name ;
    }

    public String getPDBFile() {
        return this.file ;
    }
}
