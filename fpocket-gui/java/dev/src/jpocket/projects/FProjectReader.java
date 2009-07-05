/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.projects;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
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

    private ArrayList<IFProject> projects ;

    public FProjectReader(String xml) {
        this.xml = xml ;
        projects = new ArrayList() ;
    }

    public ArrayList<IFProject> readProjects()
            throws JDOMException, IOException
    {
        projects = new ArrayList() ;

        SAXBuilder sxb = new SAXBuilder();
        Document document = sxb.build(new File(xml));

        //On initialise un nouvel élément racine avec l'élément racine du document.
        Element root = document.getRootElement();
        Element curProj = null ;
        String name = null, type = null;
        DefaultMutableTreeNode projectRoot = null ;
        
        Iterator it = root.getChildren().iterator() ;
        while(it.hasNext()) {
            curProj = (Element) it.next() ;
            name = curProj.getName() ;

            // Ignore anything else that project
            if(! name.equals("PROJECT")) continue ;

            name = curProj.getAttributeValue("name");
            type = curProj.getAttributeValue("type") ;

            if(type.equals("PDB")) {
                projectRoot = new DefaultMutableTreeNode(name) ;

                readPDB(curProj, projectRoot) ;
                projects.add(new FPDBProject(xml, name, projectRoot) ) ;
            }
        }

        return projects ;
    }

    private void readPDB(Element node, DefaultMutableTreeNode curTreeNode)
    {
        DefaultMutableTreeNode newTreeNode = null ;
        List childrens = node.getChildren() ;
        Element cNode = null ;
        
        Iterator it = childrens.iterator() ;
        while(it.hasNext()) {
            cNode = (Element) it.next() ;

            String nodeName = cNode.getName() ;
            if(nodeName.equals("CATEGORY")) {
                newTreeNode = new DefaultMutableTreeNode(cNode.getAttributeValue("name")) ;
                curTreeNode.add(newTreeNode) ;

                readPDB(cNode, newTreeNode) ;
            }
            else if(nodeName.equals("PDB")) {
                PDBFile pdb = new PDBFile(cNode.getAttributeValue("name"),
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
                        boolean drug = (ligand.getAttributeValue("isDrug").equals("1"))
                                       ?true:false ;
                        String hetatm = ligand.getAttributeValue("hetatm") ;
                        String type = ligand.getAttributeValue("type") ;

                        PDBLigand lig = new PDBLigand(hetatm, type, drug) ;
                        ligNode = new DefaultMutableTreeNode(lig) ;
                        
                        pdb.addLigand(lig) ;
                        newTreeNode.add(ligNode);
                    }
                }
            }
        }
    }
}