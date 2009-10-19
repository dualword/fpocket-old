/**

## ----- GENERAL INFORMATION
##
## FILE 		JProjectXMLReader.java
## AUTHORS		VL
## CREATED  	Sep 27, 2009
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

package jpocket.core.odata.io;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import jpocket.core.odata.JLibrary;
import jpocket.core.odata.JPDBFile;
import jpocket.core.odata.JPDBLig;
import jpocket.core.odata.JProject;
import org.jdom.Document;
import org.jdom.Element;
import org.jdom.JDOMException;
import org.jdom.input.SAXBuilder;

/**
 *
 * @author Administrateur
 */
public class JProjectXMLReader
{
    private String xml ;

    public JProjectXMLReader(String xml) {
        this.xml = xml ;
    }

    public List<JProject> readProjects()
            throws JDOMException, IOException
    {
        List<JProject> projects = new ArrayList() ;

        SAXBuilder sxb = new SAXBuilder();
        Document document = sxb.build(new File(xml));

        //On initialise un nouvel élément racine avec l'élément racine du document.
        Element root = document.getRootElement();
        Element cProj = null ;
        String name = null, type = null;

        JProject project = null ;
        
        Iterator it = root.getChildren().iterator() ;
        while(it.hasNext()) {
            cProj = (Element) it.next() ;
            name = cProj.getName() ;

            // Ignore anything else that project
            if(! name.equals("PROJECT")) continue ;

            name = cProj.getAttributeValue("name");
            type = cProj.getAttributeValue("type") ;

            if(type.equals("PDB")) {
                project = new JProject(name, xml, cProj.getAttributeValue("description"),
                                       null) ;
                
                project.setLibraries(readLibraries(cProj, null)) ;
                projects.add(project) ;
            }
        }

        return projects ;
    }


    private List<JLibrary> readLibraries(Element node, JLibrary lib)
    {
        // Declare variables
        List childrens = node.getChildren() ;
        Element cNode = null ;

        String nodeName = null ;

        // Declare objects
        JPDBFile pdb = null ;
        List<JLibrary> libs = (lib == null)?new ArrayList(): lib.getSubLibraries() ;

        // Iterate over each child, read recurcively.
        Iterator it = childrens.iterator() ;
        while(it.hasNext()) {
            cNode = (Element) it.next() ;

            nodeName = cNode.getName() ;
            if(nodeName.equals("LIBRARY")) {
                lib = new JLibrary(lib, cNode.getAttributeValue("name"),
                                   cNode.getAttributeValue("description")) ;

                libs.add(lib) ;
                lib.setSubLibraries(readLibraries(cNode, lib)) ;
            }
            else if(nodeName.equals("PDB")) {
                pdb = new JPDBFile(lib, cNode.getAttributeValue("path"),
                                   cNode.getAttributeValue("name"),
                                   cNode.getAttributeValue("fpocketOutput")) ;

                lib.addPDB(pdb) ;

                List ligands = cNode.getChildren() ;

                Iterator lit = ligands.iterator() ;
                while(lit.hasNext()) {
                    
                    Element ligand = (Element) lit.next() ;
                    if(ligand.getName().equals("LIG")) {
                        String hetatm = ligand.getAttributeValue("hetatm") ;
                        String type = ligand.getAttributeValue("type") ;
                        String tpocket = ligand.getAttributeValue("tpocket") ;

                        boolean isTp = false ;
                        if(tpocket != null && tpocket.equals("true")) isTp = true ;

                        JPDBLig lig = new JPDBLig(pdb, isTp, hetatm, type) ;

                        pdb.addLig(lig) ;
                    }
                }
            }
        }

        return libs ;
    }

}