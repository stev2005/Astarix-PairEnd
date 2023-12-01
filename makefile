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
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 15 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 20 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/1.in >$(PairedEndStatsDIR)/paired-end.out

group1:
	time ./main.exe --alignment paired-end --heuristic seed_heuristic --trie-depth 10 --seed-len 15 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 20 --reference $(PairedEndDIR)/reference.in --query $(PairedEndDIR)/Group1.in >$(PairedEndStatsDIR)/Group1.out


clean:
	-rm -f main
