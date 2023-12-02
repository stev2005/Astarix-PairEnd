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


problem:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 50 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/problemquery.in >$(PairedEndStatsDIR)/problem.out

clean:
	-rm -f main
