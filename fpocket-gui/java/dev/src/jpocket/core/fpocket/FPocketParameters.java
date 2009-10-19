/**

## ----- GENERAL INFORMATION
##
## FILE 		FPocketParameters.java
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

import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import javax.swing.JOptionPane;
import jpocket.core.fpocket.Parameter.Mendatory;
import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;

/**
 *
 * @author vince
 */
public class FPocketParameters
{
    private static String XML_LOCATION = "/home/vince/NetBeansProjects/JPocket/settings/fpocket_params.xml";
    public static void setXmlLocation(String location) 
            throws Exception
    {
        XML_LOCATION = location ;
        instance.reload();
    }
    
    private static FPocketParameters instance ;

    public static final FPocketParameters instance()
    {
        if(instance == null) {
            try {
                instance = new FPocketParameters(XML_LOCATION);
            }
            catch (Exception ex) {
                ex.printStackTrace();
                JOptionPane.showMessageDialog(null,
                        "Error while loading fpocket available parameters"
                        +"\nMessage:\n\n"+ex.getLocalizedMessage());
            }
        }
        return instance ;
    }

    private List<Parameter> existingParameters ;
    private ParametersSet parametersValues ;
    private String xmlFile ;

    private FPocketParameters(String xmlFile)
            throws Exception
    {
        this.xmlFile = xmlFile ;
        reload() ;
    }


    public void reload()
            throws Exception 
    {
        existingParameters = new ArrayList<Parameter>();

        SAXBuilder sxb = new SAXBuilder();
        Document document = sxb.build(new File(xmlFile)) ;

        //On initialise un nouvel élément racine avec l'élément racine du document.
        Element root = document.getRootElement();
        Element cNode = null ;
        String  name = null,
                type = null,
                desc = null,
                command = null,
                mendatory = null,
                dval = null ;

        Iterator<Element> it = root.getChildren().iterator() ;
        while(it.hasNext()) {
            cNode = it.next() ;
            name = cNode.getName() ;

            // Ignore anything else that project
            if(! name.equals("PARAM")) continue ;

            mendatory = cNode.getAttributeValue("mendatory");
            command = cNode.getAttributeValue("command");
            name = cNode.getAttributeValue("name");
            desc = cNode.getAttributeValue("desc");
            type = cNode.getAttributeValue("type");
            dval = cNode.getAttributeValue("dval");
            System.out.println("Dbal: "+dval+" for "+name);

            Mendatory m = Parameter.Mendatory.NOT_MENDATORY ;
            if(mendatory.equals("1")) m = Parameter.Mendatory.MENDATORY ;
            else if(mendatory.equals("2")) m = Parameter.Mendatory.OR_MENDATORY ;

            existingParameters.add(new Parameter(m, command, name, desc, type, dval)) ;
        }
    }
    public ParametersSet getParametersValues() {
        if(parametersValues == null) {
            parametersValues = new ParametersSet("", "Default values", "Default values") ;
            for(Parameter p: existingParameters) 
                parametersValues.setParameter(p.getCommand(), p.getDefaultValue());
        }
        
        return parametersValues ;
    }

    public void setParametersValues(ParametersSet ps) {
        parametersValues = ps ;
    }

    public List<Parameter> getExistingParameters() {
        return existingParameters;
    }

}
