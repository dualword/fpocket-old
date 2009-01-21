#! /bin/sh

D=1.73
m=3.0
M=6.0
i=30
out="desc_all_D"$D"_m"$m"_M"$M"_i"$i

mkdir $out

echo "== Describing Pocketpicker HOLO"
./bin/dpocket -f data/pp-cplx-d.txt -D $D -m $m -M $M -i $i
mv dpout_fpocketnp.txt dpout_fpocketnp_ppc.txt
mv dpout_fpocketp.txt dpout_fpocketp_ppc.txt
mv dpout_fpocket*_ppc.txt $out
cat $out/dpout_fpocket*_ppc.txt > $out/all_ppc.txt

echo "== Describing training set"
./bin/dpocket -f data/train-d.txt -D $D -m $m -M $M -i $i
mv dpout_fpocketnp.txt dpout_fpocketnp_train.txt
mv dpout_fpocketp.txt dpout_fpocketp_train.txt
mv dpout_fpocket*_train.txt $out
cat $out/dpout_fpocket*_train.txt > $out/all_train.txt

echo "== Describing cheng set"
./bin/dpocket -f data/cheng-d.txt -D $D -m $m -M $M -i $i
mv dpout_fpocketnp.txt dpout_fpocketnp_ch.txt
mv dpout_fpocketp.txt dpout_fpocketp_ch.txt
mv dpout_fpocket*_ch.txt $out
cat $out/dpout_fpocket*_ch.txt > $out/all_ch.txt

echo "== Describing gold set"
./bin/dpocket -f data/gold-d.txt -D $D -m $m -M $M -i $i
mv dpout_fpocketnp.txt dpout_fpocketnp_gold.txt
mv dpout_fpocketp.txt dpout_fpocketp_gold.txt
mv dpout_fpocket*_gold.txt $out
cat $out/dpout_fpocket*_gold.txt > $out/all_gold.txt

