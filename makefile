SingleReadDIR=data/single-reads/testsE.coli
SingleReadStatsDIR=data/single-reads/testsE.coli/stats
PE_DIR=data/pair-end-reads/testsE.coli
PE_StatsDIR=data/pair-end-reads/testsE.coli/stats
PairedEndCompareDijkstraIndDIR=data/pair-end-reads/Comparison/'Dijkstra independent'
PairedEndCompareDijkstraJointDIR=data/pair-end-reads/Comparison/'Dijkstra joint'
PairedEndCompareAstarIndDIR=data/pair-end-reads/Comparison/'Astarix independent'
EcoliDIR=data/EcoliRef/reference.in
EcoliREF="--reference $(EcoliDIR)"
PE=paired-end
SH=seed_heuristic

default: main

main:
	g++ main.cpp -o main.exe

testsingleread_seed_heuristic):
	time ./main.exe --alignment single-read --heuristic $(SH) --trie-depth 10 --seed-len 12 $(EcoliREF) --query $(SingleReadDIR)/1.in >$(SingleReadStatsDIR)/single-read.out

exptestsingleread_seed_heuristic:
	time ./main.exe --alignment single-read --heuristic $(SH) --trie-depth 10 --seed-len 12 $(EcoliREF) --query $(SingleReadDIR)/1.in >$(SingleReadStatsDIR)/exptestsingleread_statsAstar.out

testchep_dijkstra_heuristic:
	time ./main.exe --alignment $(PE) --heuristic dijkstra_heuristic --trie-depth 10 --seed-len 15 --insert-distance 5000 --filter-distance-difference 500 --punish-heuristic-cost 20 --reference $(EcoliDIR) --query $(PE_DIR)/1.in >$(PE_StatsDIR)/$(PE).out

testpairend_seed_heuristic:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 20 --reference $(EcoliDIR) --query $(PE_DIR)/1.in >$(PE_StatsDIR)/$(PE).out

testpairend_seed_heuristic_independent:
	time ./main.exe --alignment $(PE)_independent --independent-aligns 100 --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 20 --reference $(EcoliDIR) --query $(PE_DIR)/1.in >$(PE_StatsDIR)/$(PE).out



compare_dijkstra_independent:
	time ./main.exe --alignment single-read --heuristic dijkstra_heuristic --trie-depth 10 --insert-distance 5000 --filter-distance-difference 50 --reference $(EcoliDIR) --query $(PairedEndCompareDijkstraIndDIR)/DijTests.in >$(PairedEndCompareDijkstraIndDIR)/evaldijind.out

compare_dijkstra_joint:
	time ./main.exe --alignment $(PE) --heuristic dijkstra_heuristic --trie-depth 10 --insert-distance 500 --filter-distance-difference 50 --reference $(EcoliDIR) --query $(PairedEndCompareDijkstraJointDIR)/DijTests.in >$(PairedEndCompareDijkstraJointDIR)/evaldijjoint.out

compare_astar_ind:
	time ./main.exe --alignment single-read --heuristic $(SH)  --independent-aligns 1 --trie-depth 10 --seed-len 12 --insert-distance 500 --filter-distance-difference 50 --reference $(EcoliDIR) --query $(PairedEndCompareAstarIndDIR)/Group4.in >$(PairedEndCompareAstarIndDIR)/evalastarind.out



group1:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 20 --reference $(EcoliDIR) --query $(PE_DIR)/Group1.in >$(PE_StatsDIR)/Group1.out
group2:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group2.in >$(PE_StatsDIR)/Group2.out
group3:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group3.in >$(PE_StatsDIR)/Group3.out
group4:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group4.in >$(PE_StatsDIR)/Group4.out

group5_1:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/1.in >$(PE_StatsDIR)/Group5/1.out
group5_2:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/2.in >$(PE_StatsDIR)/Group5/2.out
group5_3:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/3.in >$(PE_StatsDIR)/Group5/3.out
group5_4:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/4.in >$(PE_StatsDIR)/Group5/4.out
group5_5:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/5.in >$(PE_StatsDIR)/Group5/5.out
group5_6:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/6.in >$(PE_StatsDIR)/Group5/6.out
group5_7:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/7.in >$(PE_StatsDIR)/Group5/7.out
group5_8:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/8.in >$(PE_StatsDIR)/Group5/8.out
group5_9:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/9.in >$(PE_StatsDIR)/Group5/9.out
group5_10:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/10.in >$(PE_StatsDIR)/Group5/10.out
group5_11:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/11.in >$(PE_StatsDIR)/Group5/11.out
group5_12:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/12.in >$(PE_StatsDIR)/Group5/12.out
group5_13:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/13.in >$(PE_StatsDIR)/Group5/13.out
group5_14:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/14.in >$(PE_StatsDIR)/Group5/14.out
group5_15:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/15.in >$(PE_StatsDIR)/Group5/15.out
group5_16:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/16.in >$(PE_StatsDIR)/Group5/16.out
group5_17:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/17.in >$(PE_StatsDIR)/Group5/17.out
group5_18:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/18.in >$(PE_StatsDIR)/Group5/18.out
group5_19:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/19.in >$(PE_StatsDIR)/Group5/19.out
group5_20:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group5/20.in >$(PE_StatsDIR)/Group5/20.out
group5all:
	make -j7 group5_1 group5_2 group5_3 group5_4 group5_5 group5_6 group5_7 group5_8 group5_9 group5_10 group5_11 group5_12 group5_13 group5_14 group5_15 group5_16 group5_17 group5_18 group5_19 group5_20


group6_1:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/1.in >$(PE_StatsDIR)/Group6/1.out
group6_2:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/2.in >$(PE_StatsDIR)/Group6/2.out
group6_3:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/3.in >$(PE_StatsDIR)/Group6/3.out
group6_4:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/4.in >$(PE_StatsDIR)/Group6/4.out
group6_5:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/5.in >$(PE_StatsDIR)/Group6/5.out
group6_6:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/6.in >$(PE_StatsDIR)/Group6/6.out
group6_7:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/7.in >$(PE_StatsDIR)/Group6/7.out
group6_8:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/8.in >$(PE_StatsDIR)/Group6/8.out
group6_9:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/9.in >$(PE_StatsDIR)/Group6/9.out
group6_10:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/10.in >$(PE_StatsDIR)/Group6/10.out
group6_11:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/11.in >$(PE_StatsDIR)/Group6/11.out
group6_12:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/12.in >$(PE_StatsDIR)/Group6/12.out
group6_13:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/13.in >$(PE_StatsDIR)/Group6/13.out
group6_14:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/14.in >$(PE_StatsDIR)/Group6/14.out
group6_15:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/15.in >$(PE_StatsDIR)/Group6/15.out
group6_16:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/16.in >$(PE_StatsDIR)/Group6/16.out
group6_17:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/17.in >$(PE_StatsDIR)/Group6/17.out
group6_18:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/18.in >$(PE_StatsDIR)/Group6/18.out
group6_19:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/19.in >$(PE_StatsDIR)/Group6/19.out
group6_20:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group6/20.in >$(PE_StatsDIR)/Group6/20.out
group6all:
	make -j7 group6_1 group6_2 group6_3 group6_4 group6_5 group6_6 group6_7 group6_8 group6_9 group6_10 group6_11 group6_12 group6_13 group6_14 group6_15 group6_16 group6_17 group6_18 group6_19 group6_20


group7_1:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/1.in >$(PE_StatsDIR)/Group7/1.out
group7_2:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/2.in >$(PE_StatsDIR)/Group7/2.out
group7_3:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/3.in >$(PE_StatsDIR)/Group7/3.out
group7_4:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/4.in >$(PE_StatsDIR)/Group7/4.out
group7_5:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/5.in >$(PE_StatsDIR)/Group7/5.out
group7_6:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/6.in >$(PE_StatsDIR)/Group7/6.out
group7_7:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/7.in >$(PE_StatsDIR)/Group7/7.out
group7_8:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/8.in >$(PE_StatsDIR)/Group7/8.out
group7_9:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/9.in >$(PE_StatsDIR)/Group7/9.out
group7_10:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/10.in >$(PE_StatsDIR)/Group7/10.out
group7_11:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/11.in >$(PE_StatsDIR)/Group7/11.out
group7_12:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/12.in >$(PE_StatsDIR)/Group7/12.out
group7_13:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/13.in >$(PE_StatsDIR)/Group7/13.out
group7_14:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/14.in >$(PE_StatsDIR)/Group7/14.out
group7_15:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/15.in >$(PE_StatsDIR)/Group7/15.out
group7_16:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/16.in >$(PE_StatsDIR)/Group7/16.out
group7_17:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/17.in >$(PE_StatsDIR)/Group7/17.out
group7_18:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/18.in >$(PE_StatsDIR)/Group7/18.out
group7_19:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/19.in >$(PE_StatsDIR)/Group7/19.out
group7_20:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group7/20.in >$(PE_StatsDIR)/Group7/20.out
group7all:
	make -j7 group7_1 group7_2 group7_3 group7_4 group7_5 group7_6 group7_7 group7_8 group7_9 group7_10 group7_11 group7_12 group7_13 group7_14 group7_15 group7_16 group7_17 group7_18 group7_19 group7_20


group8_1:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/1.in >$(PE_StatsDIR)/Group8/1.out
group8_2:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/2.in >$(PE_StatsDIR)/Group8/2.out
group8_3:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/3.in >$(PE_StatsDIR)/Group8/3.out
group8_4:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/4.in >$(PE_StatsDIR)/Group8/4.out
group8_5:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/5.in >$(PE_StatsDIR)/Group8/5.out
group8_6:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/6.in >$(PE_StatsDIR)/Group8/6.out
group8_7:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/7.in >$(PE_StatsDIR)/Group8/7.out
group8_8:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/8.in >$(PE_StatsDIR)/Group8/8.out
group8_9:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/9.in >$(PE_StatsDIR)/Group8/9.out
group8_10:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/10.in >$(PE_StatsDIR)/Group8/10.out
group8_11:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/11.in >$(PE_StatsDIR)/Group8/11.out
group8_12:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/12.in >$(PE_StatsDIR)/Group8/12.out
group8_13:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/13.in >$(PE_StatsDIR)/Group8/13.out
group8_14:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/14.in >$(PE_StatsDIR)/Group8/14.out
group8_15:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/15.in >$(PE_StatsDIR)/Group8/15.out
group8_16:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/16.in >$(PE_StatsDIR)/Group8/16.out
group8_17:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/17.in >$(PE_StatsDIR)/Group8/17.out
group8_18:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/18.in >$(PE_StatsDIR)/Group8/18.out
group8_19:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/19.in >$(PE_StatsDIR)/Group8/19.out
group8_20:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group8/20.in >$(PE_StatsDIR)/Group8/20.out
group8all:
	make -j7 group8_1 group8_2 group8_3 group8_4 group8_5 group8_6 group8_7 group8_8 group8_9 group8_10 group8_11 group8_12 group8_13 group8_14 group8_15 group8_16 group8_17 group8_18 group8_19 group8_20



group9_1:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/1.in >$(PE_StatsDIR)/Group9/1.out
group9_2:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/2.in >$(PE_StatsDIR)/Group9/2.out
group9_3:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/3.in >$(PE_StatsDIR)/Group9/3.out
group9_4:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/4.in >$(PE_StatsDIR)/Group9/4.out
group9_5:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/5.in >$(PE_StatsDIR)/Group9/5.out
group9_6:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/6.in >$(PE_StatsDIR)/Group9/6.out
group9_7:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/7.in >$(PE_StatsDIR)/Group9/7.out
group9_8:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/8.in >$(PE_StatsDIR)/Group9/8.out
group9_9:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/9.in >$(PE_StatsDIR)/Group9/9.out
group9_10:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/10.in >$(PE_StatsDIR)/Group9/10.out
group9_11:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/11.in >$(PE_StatsDIR)/Group9/11.out
group9_12:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/12.in >$(PE_StatsDIR)/Group9/12.out
group9_13:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/13.in >$(PE_StatsDIR)/Group9/13.out
group9_14:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/14.in >$(PE_StatsDIR)/Group9/14.out
group9_15:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/15.in >$(PE_StatsDIR)/Group9/15.out
group9_16:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/16.in >$(PE_StatsDIR)/Group9/16.out
group9_17:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/17.in >$(PE_StatsDIR)/Group9/17.out
group9_18:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/18.in >$(PE_StatsDIR)/Group9/18.out
group9_19:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/19.in >$(PE_StatsDIR)/Group9/19.out
group9_20:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group9/20.in >$(PE_StatsDIR)/Group9/20.out
group9all:
	make -j7 group9_1 group9_2 group9_3 group9_4 group9_5 group9_6 group9_7 group9_8 group9_9 group9_10 group9_11 group9_12 group9_13 group9_14 group9_15 group9_16 group9_17 group9_18 group9_19 group9_20


problem:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/problemquery.in >$(PE_StatsDIR)/problem.out

clean:
	-rm -f main
