SingleReadDIR=data/single-reads/testsE.coli
SingleReadStatsDIR=data/single-reads/testsE.coli/stats
PairedEndDIR=data/pair-end-reads/testsE.coli
PairedEndStatsDIR=data/pair-end-reads/testsE.coli/stats
PairedEndCompareDijkstraIndDIR=data/pair-end-reads/Comparison/'Dijkstra independent'
PairedEndCompareDijkstraJointDIR=data/pair-end-reads/Comparison/'Dijkstra joint'
PairedEndCompareAstarIndDIR=data/pair-end-reads/Comparison/'Astarix independent'
EcoliDIR=data/EcoliRef/reference.in
EcoliREF="--reference $(EcoliDIR)"


default: main

main:
	g++ main.cpp -o main.exe

testsingleread_seed_heuristic:
	time ./main.exe --alignment single-read --heuristic seed_heuristic --trie-depth 10 --seed-len 12 $(EcoliREF) --query $(SingleReadDIR)/1.in >$(SingleReadStatsDIR)/single-read.out

exptestsingleread_seed_heuristic:
	time ./main.exe --alignment single-read --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --reference $(EcoliDIR) --query $(SingleReadDIR)/1.in >$(SingleReadStatsDIR)/exptestsingleread_statsAstar.out

testchep_dijkstra_heuristic:
	time ./main.exe --alignment paired-end --heuristic dijkstra_heuristic --trie-depth 10 --seed-len 15 --insert-distance 5000 --filter-distance-difference 500 --punish-heuristic-cost 20 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/1.in >$(PairedEndStatsDIR)/paired-end.out

testpairend_seed_heuristic:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 20 --reference $(EcoliDIR) --query $(PairedEndDIR)/1.in >$(PairedEndStatsDIR)/paired-end.out

testpairend_seed_heuristic_independent:
	time ./main.exe --alignment paired-end_independent --independent-aligns 100 --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 20 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/1.in >$(PairedEndStatsDIR)/paired-end.out



compare_dijkstra_independent:
	time ./main.exe --alignment single-read --heuristic dijkstra_heuristic --trie-depth 10 --insert-distance 5000 --filter-distance-difference 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndCompareDijkstraIndDIR)/DijTests.in >$(PairedEndCompareDijkstraIndDIR)/evaldijind.out

compare_dijkstra_joint:
	time ./main.exe --alignment paired-end --heuristic dijkstra_heuristic --trie-depth 10 --insert-distance 500 --filter-distance-difference 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndCompareDijkstraJointDIR)/DijTests.in >$(PairedEndCompareDijkstraJointDIR)/evaldijjoint.out

compare_astar_ind:
	time ./main.exe --alignment single-read --heuristic seed_heuristic  --independent-aligns 1 --trie-depth 10 --seed-len 12 --insert-distance 500 --filter-distance-difference 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndCompareAstarIndDIR)/Group4.in >$(PairedEndCompareAstarIndDIR)/evalastarind.out



group1:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 20 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group1.in >$(PairedEndStatsDIR)/Group1.out
group2:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group2.in >$(PairedEndStatsDIR)/Group2.out
group3:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group3.in >$(PairedEndStatsDIR)/Group3.out
group4:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group4.in >$(PairedEndStatsDIR)/Group4.out

group5_1:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/1.in >$(PairedEndStatsDIR)/Group5/1.out
group5_2:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/2.in >$(PairedEndStatsDIR)/Group5/2.out
group5_3:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/3.in >$(PairedEndStatsDIR)/Group5/3.out
group5_4:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/4.in >$(PairedEndStatsDIR)/Group5/4.out
group5_5:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/5.in >$(PairedEndStatsDIR)/Group5/5.out
group5_6:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/6.in >$(PairedEndStatsDIR)/Group5/6.out
group5_7:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/7.in >$(PairedEndStatsDIR)/Group5/7.out
group5_8:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/8.in >$(PairedEndStatsDIR)/Group5/8.out
group5_9:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/9.in >$(PairedEndStatsDIR)/Group5/9.out
group5_10:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/10.in >$(PairedEndStatsDIR)/Group5/10.out
group5_11:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/11.in >$(PairedEndStatsDIR)/Group5/11.out
group5_12:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/12.in >$(PairedEndStatsDIR)/Group5/12.out
group5_13:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/13.in >$(PairedEndStatsDIR)/Group5/13.out
group5_14:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/14.in >$(PairedEndStatsDIR)/Group5/14.out
group5_15:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/15.in >$(PairedEndStatsDIR)/Group5/15.out
group5_16:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/16.in >$(PairedEndStatsDIR)/Group5/16.out
group5_17:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/17.in >$(PairedEndStatsDIR)/Group5/17.out
group5_18:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/18.in >$(PairedEndStatsDIR)/Group5/18.out
group5_19:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/19.in >$(PairedEndStatsDIR)/Group5/19.out
group5_20:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5/20.in >$(PairedEndStatsDIR)/Group5/20.out
group5all:
	make -j7 group5_1 group5_2 group5_3 group5_4 group5_5 group5_6 group5_7 group5_8 group5_9 group5_10 group5_11 group5_12 group5_13 group5_14 group5_15 group5_16 group5_17 group5_18 group5_19 group5_20


group6_1:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/1.in >$(PairedEndStatsDIR)/Group6/1.out
group6_2:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/2.in >$(PairedEndStatsDIR)/Group6/2.out
group6_3:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/3.in >$(PairedEndStatsDIR)/Group6/3.out
group6_4:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/4.in >$(PairedEndStatsDIR)/Group6/4.out
group6_5:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/5.in >$(PairedEndStatsDIR)/Group6/5.out
group6_6:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/6.in >$(PairedEndStatsDIR)/Group6/6.out
group6_7:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/7.in >$(PairedEndStatsDIR)/Group6/7.out
group6_8:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/8.in >$(PairedEndStatsDIR)/Group6/8.out
group6_9:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/9.in >$(PairedEndStatsDIR)/Group6/9.out
group6_10:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/10.in >$(PairedEndStatsDIR)/Group6/10.out
group6_11:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/11.in >$(PairedEndStatsDIR)/Group6/11.out
group6_12:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/12.in >$(PairedEndStatsDIR)/Group6/12.out
group6_13:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/13.in >$(PairedEndStatsDIR)/Group6/13.out
group6_14:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/14.in >$(PairedEndStatsDIR)/Group6/14.out
group6_15:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/15.in >$(PairedEndStatsDIR)/Group6/15.out
group6_16:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/16.in >$(PairedEndStatsDIR)/Group6/16.out
group6_17:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/17.in >$(PairedEndStatsDIR)/Group6/17.out
group6_18:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/18.in >$(PairedEndStatsDIR)/Group6/18.out
group6_19:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/19.in >$(PairedEndStatsDIR)/Group6/19.out
group6_20:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group6/20.in >$(PairedEndStatsDIR)/Group6/20.out
group6all:
	make -j7 group6_1 group6_2 group6_3 group6_4 group6_5 group6_6 group6_7 group6_8 group6_9 group6_10 group6_11 group6_12 group6_13 group6_14 group6_15 group6_16 group6_17 group6_18 group6_19 group6_20


group7_1:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/1.in >$(PairedEndStatsDIR)/Group7/1.out
group7_2:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/2.in >$(PairedEndStatsDIR)/Group7/2.out
group7_3:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/3.in >$(PairedEndStatsDIR)/Group7/3.out
group7_4:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/4.in >$(PairedEndStatsDIR)/Group7/4.out
group7_5:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/5.in >$(PairedEndStatsDIR)/Group7/5.out
group7_6:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/6.in >$(PairedEndStatsDIR)/Group7/6.out
group7_7:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/7.in >$(PairedEndStatsDIR)/Group7/7.out
group7_8:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/8.in >$(PairedEndStatsDIR)/Group7/8.out
group7_9:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/9.in >$(PairedEndStatsDIR)/Group7/9.out
group7_10:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/10.in >$(PairedEndStatsDIR)/Group7/10.out
group7_11:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/11.in >$(PairedEndStatsDIR)/Group7/11.out
group7_12:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/12.in >$(PairedEndStatsDIR)/Group7/12.out
group7_13:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/13.in >$(PairedEndStatsDIR)/Group7/13.out
group7_14:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/14.in >$(PairedEndStatsDIR)/Group7/14.out
group7_15:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/15.in >$(PairedEndStatsDIR)/Group7/15.out
group7_16:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/16.in >$(PairedEndStatsDIR)/Group7/16.out
group7_17:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/17.in >$(PairedEndStatsDIR)/Group7/17.out
group7_18:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/18.in >$(PairedEndStatsDIR)/Group7/18.out
group7_19:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/19.in >$(PairedEndStatsDIR)/Group7/19.out
group7_20:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group7/20.in >$(PairedEndStatsDIR)/Group7/20.out
group7all:
	make -j7 group7_1 group7_2 group7_3 group7_4 group7_5 group7_6 group7_7 group7_8 group7_9 group7_10 group7_11 group7_12 group7_13 group7_14 group7_15 group7_16 group7_17 group7_18 group7_19 group7_20


group8_1:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/1.in >$(PairedEndStatsDIR)/Group8/1.out
group8_2:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/2.in >$(PairedEndStatsDIR)/Group8/2.out
group8_3:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/3.in >$(PairedEndStatsDIR)/Group8/3.out
group8_4:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/4.in >$(PairedEndStatsDIR)/Group8/4.out
group8_5:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/5.in >$(PairedEndStatsDIR)/Group8/5.out
group8_6:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/6.in >$(PairedEndStatsDIR)/Group8/6.out
group8_7:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/7.in >$(PairedEndStatsDIR)/Group8/7.out
group8_8:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/8.in >$(PairedEndStatsDIR)/Group8/8.out
group8_9:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/9.in >$(PairedEndStatsDIR)/Group8/9.out
group8_10:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/10.in >$(PairedEndStatsDIR)/Group8/10.out
group8_11:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/11.in >$(PairedEndStatsDIR)/Group8/11.out
group8_12:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/12.in >$(PairedEndStatsDIR)/Group8/12.out
group8_13:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/13.in >$(PairedEndStatsDIR)/Group8/13.out
group8_14:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/14.in >$(PairedEndStatsDIR)/Group8/14.out
group8_15:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/15.in >$(PairedEndStatsDIR)/Group8/15.out
group8_16:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/16.in >$(PairedEndStatsDIR)/Group8/16.out
group8_17:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/17.in >$(PairedEndStatsDIR)/Group8/17.out
group8_18:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/18.in >$(PairedEndStatsDIR)/Group8/18.out
group8_19:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/19.in >$(PairedEndStatsDIR)/Group8/19.out
group8_20:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group8/20.in >$(PairedEndStatsDIR)/Group8/20.out
group8all:
	make -j7 group8_1 group8_2 group8_3 group8_4 group8_5 group8_6 group8_7 group8_8 group8_9 group8_10 group8_11 group8_12 group8_13 group8_14 group8_15 group8_16 group8_17 group8_18 group8_19 group8_20



group9_1:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/1.in >$(PairedEndStatsDIR)/Group9/1.out
group9_2:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/2.in >$(PairedEndStatsDIR)/Group9/2.out
group9_3:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/3.in >$(PairedEndStatsDIR)/Group9/3.out
group9_4:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/4.in >$(PairedEndStatsDIR)/Group9/4.out
group9_5:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/5.in >$(PairedEndStatsDIR)/Group9/5.out
group9_6:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/6.in >$(PairedEndStatsDIR)/Group9/6.out
group9_7:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/7.in >$(PairedEndStatsDIR)/Group9/7.out
group9_8:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/8.in >$(PairedEndStatsDIR)/Group9/8.out
group9_9:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/9.in >$(PairedEndStatsDIR)/Group9/9.out
group9_10:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/10.in >$(PairedEndStatsDIR)/Group9/10.out
group9_11:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/11.in >$(PairedEndStatsDIR)/Group9/11.out
group9_12:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/12.in >$(PairedEndStatsDIR)/Group9/12.out
group9_13:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/13.in >$(PairedEndStatsDIR)/Group9/13.out
group9_14:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/14.in >$(PairedEndStatsDIR)/Group9/14.out
group9_15:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/15.in >$(PairedEndStatsDIR)/Group9/15.out
group9_16:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/16.in >$(PairedEndStatsDIR)/Group9/16.out
group9_17:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/17.in >$(PairedEndStatsDIR)/Group9/17.out
group9_18:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/18.in >$(PairedEndStatsDIR)/Group9/18.out
group9_19:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/19.in >$(PairedEndStatsDIR)/Group9/19.out
group9_20:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group9/20.in >$(PairedEndStatsDIR)/Group9/20.out
group9all:
	make -j7 group9_1 group9_2 group9_3 group9_4 group9_5 group9_6 group9_7 group9_8 group9_9 group9_10 group9_11 group9_12 group9_13 group9_14 group9_15 group9_16 group9_17 group9_18 group9_19 group9_20


problem:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/problemquery.in >$(PairedEndStatsDIR)/problem.out

clean:
	-rm -f main
