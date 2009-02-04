#!/usr/bin/python

import sys
sys.path.append("/home/safraoui/Classes")
from PDB6_7 import *

x = PDB(sys.argv[1])
x.out(sys.argv[1]+".pdb")
