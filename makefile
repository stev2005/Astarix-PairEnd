SingleReadDIR=data/single-reads/testsE.coli
SingleReadStatsDIR=data/single-reads/testsE.coli/stats
PairedEndDIR=data/pair-end-reads/testsE.coli
PairedEndStatsDIR=data/pair-end-reads/testsE.coli/stats

default: main

main:
	g++ main.cpp -o main.exe

testsingleread_seed_heuristic:
	time ./main.exe --alignment single-read --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --reference $(SingleReadDIR)/reference.in --query $(SingleReadDIR)/1.in >$(SingleReadStatsDIR)single-read.out

exptestsingleread_seed_heuristic:
	time ./main.exe --alignment single-read --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --reference $(SingleReadDIR)/reference.in --query $(SingleReadDIR)/1.in >$(SingleReadStatsDIR)exptestsingleread_statsAstar.out

testchep_dijkstra_heuristic:
	time ./main.exe --alignment paired-end --heuristic dijkstra_heuristic --trie-depth 10 --seed-len 15 --insert-distance 5000 --filter-distance-difference 500 --punish-heuristic-cost 20 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/1.in >$(PairedEndStatsDIR)/paired-end.out

testpairend_seed_heuristic:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 20 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/1.in >$(PairedEndStatsDIR)/paired-end.out

group1:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 20 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group1.in >$(PairedEndStatsDIR)/Group1.out

group2:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group2.in >$(PairedEndStatsDIR)/Group2.out

group3:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group3.in >$(PairedEndStatsDIR)/Group3.out

group4:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group4.in >$(PairedEndStatsDIR)/Group4.out

group5:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group5.in >$(PairedEndStatsDIR)/Group5.out

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



problem:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/problemquery.in >$(PairedEndStatsDIR)/problem.out

clean:
	-rm -f main
