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

testsingleread_seed_heuristic:
	time ./main.exe --alignment single-read --heuristic $(SH) --trie-depth 10 --seed-len 12 $(EcoliREF) --query $(SingleReadDIR)/1.in >$(SingleReadStatsDIR)/single-read.out

exptestsingleread_seed_heuristic:
	time ./main.exe --alignment single-read --heuristic $(SH) --trie-depth 10 --seed-len 12 $(EcoliREF) --query $(SingleReadDIR)/1.in >$(SingleReadStatsDIR)/exptestsingleread_statsAstar.out

testchep_dijkstra_heuristic:
	time ./main.exe --alignment $(PE) --heuristic dijkstra_heuristic --trie-depth 10 --seed-len 15 --insert-distance 5000 --filter-distance-difference 500 --punish-heuristic-cost 20 --reference $(EcoliDIR) --query $(PE_DIR)/1.in >$(PE_StatsDIR)/$(PE).out

testpairend_seed_heuristic:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --trie-positions-limit-checker 1100 --reference $(EcoliDIR) --query $(PE_DIR)/1.in >$(PE_StatsDIR)/$(PE).out

testpairend_seed_heuristic_independent:
	time ./main.exe --alignment $(PE)_independent --independent-aligns 100 --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --punish-heuristic-cost 20 --reference $(EcoliDIR) --query $(PE_DIR)/1.in >$(PE_StatsDIR)/$(PE).out


.PHONY group_run:
group_run:
	echo "Running group $(GROUP) for output $(OUT_NUM)"
	time ./main.exe \
		--alignment $(PE) \
		--heuristic $(SH) \
		--trie-depth 10 \
		--seed-len 12 \
		--insert-distance 5000 \
		--filter-distance-difference 50 \
		--trie-positions-limit-checker 1100
		--reference $(EcoliDIR) \
		--query $(PE_DIR)/Group$(GROUP)/$(OUT_NUM).in \
		> $(PE_StatsDIR)/Group$(GROUP)/$(OUT_NUM).out

test_group5_1:
	make group_run GROUP=5 OUT_NUM=1
	echo "running group5_1"

test_simultanious:
	#make -j 3 group_run GROUP=5 OUT_NUM=1 group_run GROUP=5 OUT_NUM=2 group_run GROUP=5 OUT_NUM=3 group_run GROUP=5 OUT_NUM=4 group_run GROUP=5 OUT_NUM=5
	make  group_run GROUP=5 OUT_NUM=1 & group_run GROUP=5 OUT_NUM=2 & group_run GROUP=5 OUT_NUM=3 & group_run GROUP=5 OUT_NUM=4 & group_run GROUP=5 OUT_NUM=5


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

group5all:
	make -j7 group5_1 group5_2 group5_3 group5_4 group5_5 group5_6 group5_7 group5_8 group5_9 group5_10 group5_11 group5_12 group5_13 group5_14 group5_15 group5_16 group5_17 group5_18 group5_19 group5_20

problem:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/problemquery.in >$(PE_StatsDIR)/problem.out

clean:
	-rm -f main
