#!/usr/bin/python


"""a script to compare fpocket results on translated structures (works also for rotated structures)"""

import Biskit as B
import Biskit.BiskitFF as bff
import Biskit.geometry as geo
import Biskit.cudm as cu
import numpy as npy

import sys
import os

if len(sys.argv)<3 :
    sys.exit("USAGE : python compareResults.py pdb1_out.pdb pdb2_out.pdb")


if not os.path.exists(sys.argv[1]):
    sys.exit("%s not found, breaking up"%(sys.argv[1]))
if not os.path.exists(sys.argv[2]):
    sys.exit("%s not found, breaking up"%(sys.argv[2]))


pdb1=B.PDBModel(sys.argv[1])
pdb2=B.PDBModel(sys.argv[2])

protein1=pdb1.compress(pdb1.maskProtein())
pocket1=pdb1.compress(pdb1.maskFrom("residue_name","STP"))
protein2=pdb2.compress(pdb2.maskProtein())
pocket2=pdb2.compress(pdb2.maskFrom("residue_name","STP"))

#superimpose protein 1 to 2 automatically and retrieve rotation matrix
(newCoords1,rmsd,rotMat)=geo.superpose3D(protein1.xyz, protein2.xyz, None,None,None,returnRotMat=True)

protein1Centroid=npy.mean(protein1.xyz,axis=0)
protein2Centroid=npy.mean(protein2.xyz,axis=0)

centeredPocketCoords1=pocket1.xyz-protein1Centroid
#centeredPocketCoords2=pocket2.xyz-protein2Centroid

#now translate and rotate the pocket1 to pocket2 according to the protein rotation matrix and protein centroids (for translation).
pocket1.xyz = npy.dot((centeredPocketCoords1), rotMat)+ protein2Centroid  #translate & rotate


"""VALIDATION STARTS HERE"""
#next check if each pocket vertice has a second pocket vertice on its translated counterpart
#first lets calculate a complete distance matrix

dm=cu.getDistanceMatrix3D(pocket1.xyz,pocket2.xyz)

nPocket1= pocket1.xyz.shape[0]
nPocket2= pocket2.xyz.shape[0]

print "Test 1 (Number of vertices) :\t\t",
if nPocket1==nPocket2:
    print "Passed, Number of vertices are equal : %d"%(nPocket1)
else :
    print "Failed, Number of vertices are unequal: %d vs %d"%(nPocket1,nPocket2)





print "Test 2 (Positions of vertices) :\t",
nGood1=0
for v1 in range(nPocket1):
    if (npy.sum(dm[:,v1]<1e-3)>0):
        nGood1+=1

nGood2=0
for v2 in range(nPocket2):
    if (npy.sum(dm[v2,:]<1e-3)>0):
        nGood2+=1
nRat1=nGood1/float(nPocket1)
nRat2=nGood2/float(nPocket2)
if (nRat1<1.0) or nRat2<1.0:
    print "Failed: %.6f, %.6f"%(nRat1,nRat2)
else :
    print "Passed"


print "Test 3 (Positions and ranking) :\t",

goodRanking1=0
goodRanking2=0
for v1 in range(nPocket1):
    w=npy.where(dm[:,v1]<1e-3)
    if len(w[0])>0:
#        print w[0][0]
        resid1=pocket1.atoms[v1]["residue_number"]
        resid2=pocket2.atoms[w[0][0]]["residue_number"]
        if resid1==resid2:
            goodRanking1+=1

#print goodRanking1/float(nPocket1)
            
for v2 in range(nPocket2):
    w=npy.where(dm[v2,:]<1e-3)
    if len(w[0])>0:
#        print w[0][0]
        resid1=pocket1.atoms[w[0][0]]["residue_number"]
        resid2=pocket2.atoms[v2]["residue_number"]
        if resid1==resid2:
            goodRanking2+=1

rankingRat1=goodRanking1/float(nPocket1)
rankingRat2=goodRanking2/float(nPocket2)

if rankingRat1<1.0 or rankingRat2<1.0 :
    print "Failed : %.6f vs %.6f"%(rankingRat1,rankingRat2)
else :
    print "Passed"


#print nGood1/float(nPocket1)
#print nGood2/float(nPocket2)

#    print numMatch
#print dm

#pocket1.writePdb("1.pdb")
#pocket2.writePdb("2.pdb")
