/**

## ----- GENERAL INFORMATION
##
## FILE 		ParametersSetReader.java
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

package jpocket.core.fpocket;

import java.io.File;
import java.io.IOException;
import java.util.Iterator;
import org.jdom.Document;
import org.jdom.Element;
import org.jdom.JDOMException;
import org.jdom.input.SAXBuilder;

/**
 *
 * @author vince
 */
public class ParametersSetReader
{
    public ParametersSet readParameters(String xml)
            throws JDOMException, IOException
    {
        ParametersSet ps = new ParametersSet(xml, "Unamed", "Undescribed") ;
        SAXBuilder sxb = new SAXBuilder();
        Document document = sxb.build(new File(xml));

        //On initialise un nouvel élément racine avec l'élément racine du document.
        Element root = document.getRootElement();
        Element cProj = null ;
        String command = null, value = null;

        Iterator it = root.getChildren().iterator() ;
        while(it.hasNext()) {
            cProj = (Element) it.next() ;
            value = cProj.getName() ;
            // Ignore anything else that project

            if(value.equals("NAME"))
                ps.setName(cProj.getValue());
            else if(value.equals("DESC"))
                ps.setComments(cProj.getValue());
            else {
                if(! value.equals("PVALUE")) continue ;

                command = cProj.getAttributeValue("name");
                value = cProj.getAttributeValue("desc") ;

                ps.setParameter(command, value) ;
            }
        }

        return ps ;
    }
}
