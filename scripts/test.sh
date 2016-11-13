#!/bin/bash
python3 scripts/searchDataGenerate.py

for i in {1..35}  
do	
  ./bin/cluclu -s A -t ./experiments/searchA/t$i.txt -p ./experiments/searchA/p$i.txt -o ./experiments/searchA/o$i.txt;
  ./bin/cluclu -s B -t ./experiments/searchB/t$i.txt -p ./experiments/searchB/p$i.txt -o ./experiments/searchB/o$i.txt;
  ./bin/cluclu -s C -t ./experiments/searchC/t$i.txt -p ./experiments/searchC/p$i.txt -o ./experiments/searchC/o$i.txt;
done

python3 scripts/dataAnalyse.py
