///**
//
//## ----- GENERAL INFORMATION
//##
//## FILE 		PocketsTableModel.java
//## AUTHORS		VL
//## CREATED  	Oct 18, 2009
//## GROUP        Institut de Chimie Organique et Analytique, UMR CNRS 6005
//##              Group of chemoinformatics and molecular modeling
//##              University of Orleans BP 6759, 45067 ORLEANS Cedex2, France
//##
//## CONTACT      vincent.le-guilloux@univ-orleans.fr
//##
//## ----- MODIFICATIONS HISTORY
//#
//## ----- SUGGESTIONS
//##
//##
//
//*/
//
///*
// * To change this template, choose Tools | Templates
// * and open the template in the editor.
// */
//
//package jpocket.gui.pockets;
//
//import java.util.List;
//import javax.swing.table.AbstractTableModel;
//import jpocket.project.JPDBFile;
//
///**
// *
// * @author vince
// */
//public class PocketsTableModel extends AbstractTableModel
//{
//    private static final String NULL_OBJ = "NULL" ;
//
//    private String[] columnNames ;
//    private Class [] columnClasses ;
//
//    private JPDBFile pdb ;
//
//    private int nbCols ;
//
//    /**
//     */
//    public PocketsTableModel() {
//        determineColumnTypes() ;
//    }
//
//    /**
//     * This method will store informations on columns displayed in the database
//     * viewer. In particular, it will identify colums supposed to store SD string
//     * representing molecules.
//     *
//     * This method will initialize all instance variable storing those informations.
//     *
//     * @param rs  The result set to deal with
//     * @throws SQLException A SQL exception if any problem with the database
//     *
//     */
//    private void determineColumnTypes()
//    {
//
//        if(pdb == null) {
//            columnNames = new String[]{"Pocket rank"};
//            columnClasses= new Class[]{Integer.class};
//
//            return ;
//        }
//
//        // PDB is not null -> check fpocket output
//
//    }
//
//    @Override
//    public int getRowCount() {
//        return this.nbMols ;
//    }
//
//    @Override
//    public int getColumnCount() {
//        return this.nbCols ;
//    }
//
//    @Override
//    public Object getValueAt(int rowIndex, int columnIndex)
//    {
//        if(rowIndex == -1 || columnIndex == -1) return "Invalid index!" ;
//
//        Object o = NULL_OBJ ;
//
//        ISAMol sam = mols.get(rowIndex) ;
//        if(sam != null) {
//            if(columnIndex == 0) {
//                String oids [] = sam.getOIDs() ; String val = "" ;
//                if(oids != null && oids.length > 0) {
//                    val = oids[0] ;
//                    for(int i = 1 ; i < oids.length ; i++) {
//                        val += " : "+oids[i] ;
//                    }
//                    o = val ;
//                }
//            }
//            else if (columnIndex >= this.dStart) {
//                DBDescField df = descriptors.get(columnIndex-dStart) ;
//                o = sam.getProperty(df) ;
//            }
//            else {
//                if(columnIndex == structureIndex) o = sam.getSDFString() ;
//                else if (columnIndex == scaffoldIndex) {
//                    if(sam.getScaffold() != null)
//                        o = sam.getScaffold().getSDFString() ;
//                    else
//                        o = "SCAFFOLD NULL" ;
//                }
//                else if (columnIndex == frameworkIndex) {
//                    if(sam.getFramework() != null)
//                        o = sam.getFramework().getSDFString() ;
//                    else
//                        o = "FRAMEWORK NULL" ;
//                }
//                else o = "ERROR_INDEX" ;
//            }
//        }
//        else o = "ERROR_MOL" ;
//
//        if(o == null) o = NULL_OBJ ;
//
//		return o ;
//    }
//
//    @Override
//    public String getColumnName(int col) {
//        //System.out.println("Column name: "+col+"/ "+nbcols);
//        return columnNames[col];
//    }
//
//    @Override
//    public Class getColumnClass(int column) {
//        if(column >= columnClasses.length) return java.lang.String.class ;
//        return columnClasses[column] ;
//    }
//}
