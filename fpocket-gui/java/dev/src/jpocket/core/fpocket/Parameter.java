/**

## ----- GENERAL INFORMATION
##
## FILE 		Parameter.java
## AUTHORS		VL
## CREATED  	Oct 4, 2009
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

package jpocket.core.fpocket;

/**
 *
 * @author vince
 */
public class Parameter
{
    public static enum Mendatory {
        NOT_MENDATORY,
        MENDATORY,
        OR_MENDATORY
    }
    
    private String command ;
    private String name ;
    private String description ;
    private String type ;
    private String defaultValue ;
    private Mendatory mendatory;

    public Parameter(Mendatory mendatory, String command, String name, String description,
                     String type, String defaultValue)
    {
        this.mendatory = mendatory ;
        this.command = command;
        this.name = name;
        this.description = description;
        this.type = type;
        this.defaultValue = defaultValue ;
    }

    public String getDefaultValue() {
        return defaultValue;
    }

    public void setDefaultValue(String defaultValue) {
        this.defaultValue = defaultValue;
    }

    public Mendatory getMendatory() {
        return mendatory;
    }

    public void setMendatory(Mendatory mendatory) {
        this.mendatory = mendatory;
    }

    public String getCommand() {
        return command;
    }

    public void setCommand(String command) {
        this.command = command;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    @Override
    public boolean equals(Object p) {
        if(p == null || ! (p instanceof Parameter)) return false ;
        
        return command.equals(((Parameter)p).getCommand()) ;
    }
}
