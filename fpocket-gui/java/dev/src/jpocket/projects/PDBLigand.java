/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.projects;

/**
 *
 * @author Administrateur
 */
public class PDBLigand {
    
    private String hetatmCode ;
    private String type ;
    private boolean isDrug ;

    public PDBLigand(String hetatmCode, String type, boolean isDrug) {
        this.hetatmCode = hetatmCode;
        this.type = type ;
        this.isDrug = isDrug;
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

    public boolean isIsDrug() {
        return isDrug;
    }

    public void setIsDrug(boolean isDrug) {
        this.isDrug = isDrug;
    }
}
