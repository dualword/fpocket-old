#! /bin/sh

D=1.73
m=3.0
M=6.0
i=30
out="perf_all_D"$D"_m"$m"_M"$M"_i"$i

mkdir $out
echo "== Testing Pocketpicker HOLO"

./bin/tpocket -L data/pp-cplx-t.txt -D $D -m $m -M $M -i $i
mv stats_g.txt stats_g_pph.txt
mv stats_g_pph.txt $out

echo "== Testing Pocketpicker APO"
./bin/tpocket -L data/pp-apo-t.txt -D $D -m $m -M $M -i $i
mv stats_g.txt stats_g_ppa.txt
mv stats_g_ppa.txt $out

echo "== Testing training set"
./bin/tpocket -L data/train-t.txt -D $D -m $m -M $M -i $i
mv stats_g.txt stats_g_train.txt
mv stats_g_train.txt $out

echo "== Testing cheng set"
./bin/tpocket -L data/cheng-t.txt -D $D -m $m -M $M -i $i
mv stats_g.txt stats_g_ch.txt
mv stats_g_ch.txt $out

echo "== Testing gold set"
./bin/tpocket -L data/gold-t.txt -D $D -m $m -M $M -i $i
mv stats_g.txt stats_g_gold.txt
mv stats_g_gold.txt $out

