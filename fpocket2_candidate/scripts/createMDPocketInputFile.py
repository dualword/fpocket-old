import fnmatch
import os,re,sys

if len(sys.argv)>1 :
	d=sys.argv[1]
	if not os.path.exists(d):
		sys.exit(" ERROR : the directory you provided as data directory does not exist. Breaking up.")
else :
	sys.exit(" ERROR : please provide the name of the data directory (where you have all your snapshots) like : python createMDPocketInputFile.py data_dir\nThe snapshots must have the pdb extension.")

prefix=sys.argv[1]

#just a helder function to do some stupid concatenation
def getFname(s):
    return prefix+os.sep+s


snapshots=fnmatch.filter(os.listdir(prefix),"*.pdb") #get the fpocket output folders

snapshots=[getFname(sn) for sn in snapshots]

RE_DIGIT = re.compile(r'(\d+)')     #set a pattern to find digits
ALPHANUM_KEY = lambda s: [int(g) if g.isdigit() else g for g in RE_DIGIT.split(s)]      #create on the fly function (lambda) to return numbers in filename strings
snapshots.sort(key=ALPHANUM_KEY)      #sort by these numbers in filenames

fout=open("mdpocket_input.txt","w")
[fout.write(sn+"\n") for sn in snapshots]
fout.close()

