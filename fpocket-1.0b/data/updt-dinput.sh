#! /bin/sh

# File to update dpocket input files from tpocket input
# file for all 3 datasets.

# Update the new dataset inputs
# Training set
awk -F '\t' '{print $1, $3}' train-t.txt > train-d.txt
awk -F '\t' '{print $2}' train-t.txt > train-f.txt

# Pocket picker dataset
awk -F '\t' '{print $1, $3}' pp-cplx-t.txt > pp-cplx-d.txt
awk -F '\t' '{print $2}' pp-cplx-t.txt > pp-cplx-f.txt
awk -F '\t' '{print $1, $3}' pp-apo-t.txt > pp-apo-d.txt
awk -F '\t' '{print $2}' pp-apo-t.txt > pp-apo-f.txt

# Cheng & al dataset
awk -F '\t' '{print $1, $3}' cheng-t.txt > cheng-d.txt
awk -F '\t' '{print $2}' cheng-t.txt > cheng-f.txt

# Gold dataset
awk -F '\t' '{print $1, $3}' gold-t.txt > gold-d.txt
awk -F '\t' '{print $2}' gold-t.txt > gold-f.txt

# CCDC Clean dataset
awk -F '\t' '{print $1, $3}' ccdc-clean-t.txt > ccdc-clean-d.txt
awk -F '\t' '{print $2}' ccdc-clean-t.txt > ccdc-clean-f.txt

# Astex diverse set
awk -F '\t' '{print $1, $3}' astex-diverse-t.txt > astex-diverse-d.txt
awk -F '\t' '{print $2}' astex-diverse-t.txt > astex-diverse-f.txt