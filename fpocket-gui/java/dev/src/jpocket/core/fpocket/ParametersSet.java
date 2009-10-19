/**

## ----- GENERAL INFORMATION
##
## FILE 		ParametersSet.java
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

import java.util.HashMap;

/**
 *
 * @author vince
 */
public class ParametersSet
{
    private HashMap<String, String> parameters ;

    private String xml ;
    private String name ;
    private String comments ;

    public ParametersSet(String xml, String name, String comments) {
        parameters = new HashMap() ;
        this.xml = xml ;
        this.name = name ;
        this.comments = comments ;
    }

    public String getParameterValue(String command) {
        String value = parameters.get(command) ;
        if(value == null) value = "" ;

        return value ;
    }

    public void setParameter(String param, String value) {
        parameters.put(param, value) ;
    }

    public HashMap<String, String> getParameters() {
        return parameters;
    }

    public void setParameters(HashMap<String, String> parameters) {
        this.parameters = parameters;
    }

    public String getXml() {
        return xml;
    }

    public void setXml(String xml) {
        this.xml = xml;
    }

    public String getComments() {
        return comments;
    }

    public void setComments(String comments) {
        this.comments = comments;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return parameters.toString() ;
    }
    
}
