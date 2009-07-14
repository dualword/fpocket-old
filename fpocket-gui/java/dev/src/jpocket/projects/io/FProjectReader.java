/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.projects.io;

import java.text.ParseException;
import java.util.logging.Level;
import java.util.logging.Logger;
import jpocket.projects.*;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import javax.swing.tree.DefaultMutableTreeNode;
import org.jdom.Document;
import org.jdom.Element;
import org.jdom.JDOMException;
import org.jdom.input.SAXBuilder;

/**
 *
 * @author Administrateur
 */
public class FProjectReader
{
    private String xml ;

    public FProjectReader(String xml) {
        this.xml = xml ;
    }

    public ArrayList<IFProject> readProjects()
            throws JDOMException, IOException
    {
        ArrayList<IFProject> projects = new ArrayList() ;

        SAXBuilder sxb = new SAXBuilder();
        Document document = sxb.build(new File(xml));

        //On initialise un nouvel élément racine avec l'élément racine du document.
        Element root = document.getRootElement();
        Element cProj = null ;
        String name = null, type = null;
        DefaultMutableTreeNode projectRoot = null ;

        PDBProject project = null ;
        
        Iterator it = root.getChildren().iterator() ;
        while(it.hasNext()) {
            cProj = (Element) it.next() ;
            name = cProj.getName() ;

            // Ignore anything else that project
            if(! name.equals("PROJECT")) continue ;

            name = cProj.getAttributeValue("name");
            type = cProj.getAttributeValue("type") ;

            if(type.equals("PDB")) {
                Date creation = null, modif = null ;
                try {
                    creation = PDBLibrary.DFORMAT.parse(
                                        cProj.getAttributeValue("creaton_date"));
                    modif = PDBLibrary.DFORMAT.parse(
                                        cProj.getAttributeValue("creaton_date")) ;

                } catch (ParseException ex) {
                    Logger.getLogger(FProjectReader.class.getName()).log(Level.SEVERE, null, ex);
                }


                project = new PDBProject(xml, name,
                                         cProj.getAttributeValue("description"),
                                         creation, modif, null) ;

                projectRoot = new DefaultMutableTreeNode(name) ;

                project.setArborescence(projectRoot) ;

                readArborescence(cProj, projectRoot) ;
                projects.add(new PDBProject(xml, name, projectRoot) ) ;
            }
        }

        return projects ;
    }

    private void readArborescence(Element node, DefaultMutableTreeNode curTreeNode)
    {
        // Declare variables
        DefaultMutableTreeNode newTreeNode = null ;
        List childrens = node.getChildren() ;
        Element cNode = null ;

        String nodeName = null ;

        // Declare objects
        PDBFile pdb = null ;
        PDBLibrary lib = null ;

        // Iterate over each child, read recurcively.
        Iterator it = childrens.iterator() ;
        while(it.hasNext()) {
            cNode = (Element) it.next() ;

            nodeName = cNode.getName() ;
            if(nodeName.equals("LIBRARY")) {
                Date creation = null, modif = null ;
                try {
                    creation = PDBLibrary.DFORMAT.parse(
                                        cNode.getAttributeValue("creaton_date"));
                    modif = PDBLibrary.DFORMAT.parse(
                                        cNode.getAttributeValue("creaton_date")) ;
                    
                } catch (ParseException ex) {
                    Logger.getLogger(FProjectReader.class.getName()).log(Level.SEVERE, null, ex);
                }


                lib = new PDBLibrary(cNode.getAttributeValue("name"),
                                     cNode.getAttributeValue("description"),
                                     creation, modif, newTreeNode) ;

                newTreeNode = new DefaultMutableTreeNode(lib) ;
                lib.setArborescence(newTreeNode) ;
                
                curTreeNode.add(newTreeNode) ;
                readArborescence(cNode, newTreeNode) ;
            }
            else if(nodeName.equals("PDB")) {
                pdb = new PDBFile(cNode.getAttributeValue("name"),
                                  cNode.getAttributeValue("path"),
                                  cNode.getAttributeValue("fpocketOutput")) ;

                newTreeNode = new DefaultMutableTreeNode(pdb) ;
                curTreeNode.add(newTreeNode) ;
                
                List ligands = cNode.getChildren() ;
                DefaultMutableTreeNode ligNode = null ;
                
                Iterator lit = ligands.iterator() ;
                while(lit.hasNext()) {
                    Element ligand = (Element) lit.next() ;
                    if(ligand.getName().equals("LIG")) {
                        String hetatm = ligand.getAttributeValue("hetatm") ;
                        String type = ligand.getAttributeValue("type") ;

                        PDBLigand lig = new PDBLigand(hetatm, type, pdb) ;
                        ligNode = new DefaultMutableTreeNode(lig) ;
                        
                        pdb.addLigand(lig) ;
                        newTreeNode.add(ligNode);
                    }
                }
            }
        }
    }
}