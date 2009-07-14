/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.projects;

/**
 *
 * @author Administrateur
 */
public class PDBLigand implements IFMolecularObject
{
    private PDBFile pdb ;

    private boolean tpocket ;

    private String hetatmCode ;
    private String type ;

    public PDBLigand(String hetatmCode, String type, PDBFile parent)
    {
        this.hetatmCode = hetatmCode;
        this.type = type ;

        tpocket = false ;
        this.pdb = parent ;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getHetatmCode() {
        return hetatmCode;
    }

    public void setHetatmCode(String hetatmCode) {
        this.hetatmCode = hetatmCode;
    }

    public String getName() {
        return hetatmCode ;
    }

    public String getPDB() {
        return pdb.getName() ;
    }

    public String getPDBFile() {
        return pdb.getFile() ;
    }

    public String getPDBAtoms() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public PDBFile getPdb() {
        return pdb;
    }

    public void setPdb(PDBFile pdb) {
        this.pdb = pdb;
    }

    public boolean isTpocket() {
        return tpocket;
    }

    public void setTpocket(boolean tpocket) {
        this.tpocket = tpocket;
    }

}
