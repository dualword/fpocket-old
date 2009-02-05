#PATH_PyMOL='/home/tintin/tuffery/softs/pymol/
#load 1STN-1SNC.pdb
log_open FRAME.ppicker_log
load FRAME-noHet.pdb
run /home/tintin/tuffery/softs/pymol/modules/pmg_tk/startup/PocketPickerScript/LigandKicker_1.0.py
run /home/tintin/tuffery/softs/pymol/modules/pmg_tk/startup/PocketPickerScript/PocketPicker_1.0.py
log_close
quit
