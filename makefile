SingleReadDIR=data/single-reads/testsE.coli
SingleReadStatsDIR=data/single-reads/testsE.coli/stats
PE_DIR=data/pair-end-reads/testsE.coli
PE_StatsDIR=data/pair-end-reads/testsE.coli/stats
PairedEndCompareDijkstraIndDIR=data/pair-end-reads/Comparison/'Dijkstra independent'
PairedEndCompareDijkstraJointDIR=data/pair-end-reads/Comparison/'Dijkstra joint'
PairedEndCompareAstarIndDIR=data/pair-end-reads/Comparison/'Astarix independent'
EcoliDIR=data/EcoliRef/reference.fa
EcoliREF="--reference $(EcoliDIR)"
PE=paired-end
SH=seed_heuristic

default: main

main:
	g++ main.cpp -o main.exe

run:
	./main.exe

test:
	time ./main.exe --alignment single-read --heuristic $(SH) --trie-depth 10 --seed-len 11 --reference $(EcoliDIR) --query $(SingleReadDIR)/1.in >$(SingleReadStatsDIR)/single-read.out

testsingleread_seed_heuristic:
	time ./main.exe --alignment single-read --heuristic $(SH) --trie-depth 10 --seed-len 12 --reference $(EcoliDIR) --query $(SingleReadDIR)/1.in >$(SingleReadStatsDIR)/single-read.out

exptestsingleread_seed_heuristic:
	time ./main.exe --alignment single-read --heuristic $(SH) --trie-depth 10 --seed-len 11 --reference $(EcoliDIR) --query $(SingleReadDIR)/1.in >$(SingleReadStatsDIR)/exptestsingleread_statsAstar.out

exptestsingleread_seed_heuristic_without_trie_opt:
	time ./main.exe --alignment single-read --heuristic $(SH) --trie-depth 10 --seed-len 11 --trie-crumbs-opt NO --reference $(EcoliDIR) --query $(SingleReadDIR)/1.in >$(SingleReadStatsDIR)/exptestsingleread_statsAstar.out

singleend_ART_trie_opt:
	time ./main.exe --alignment single-read --heuristic $(SH) --trie-depth 10 --seed-len 11 --reference $(EcoliDIR) --query $(SingleReadDIR)/single_testnew.in >$(SingleReadStatsDIR)/ART_trie_opt.out

singleend_ART_no_trie_opt:
	time ./main.exe --alignment single-read --heuristic $(SH) --trie-depth 10 --seed-len 11 --trie-crumbs-opt NO --reference $(EcoliDIR) --query $(SingleReadDIR)/single_test.in >$(SingleReadStatsDIR)/ART_no_trie_opt.out


test_crumbs_trie_opt_seed_heuristic:
	time ./main.exe --alignment single-read --heuristic $(SH) --trie-depth 10 --seed-len 11 --reference $(EcoliDIR) --query $(SingleReadDIR)/illumina250.in >$(SingleReadStatsDIR)/illumina250.out


testchep_dijkstra_heuristic:
	time ./main.exe --alignment $(PE) --heuristic dijkstra_heuristic --trie-depth 10 --seed-len 15 --insert-distance 5000 --filter-distance-difference 500 --punish-heuristic-cost 20 --reference $(EcoliDIR) --query $(PE_DIR)/1.in >$(PE_StatsDIR)/$(PE).out

testpairend_seed_heuristic:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50 --trie-positions-limit-checker 0 --reference $(EcoliDIR) --query1 $(PE_DIR)/pe_reads1.fq --query2 $(PE_DIR)/pe_reads2.fq  >$(PE_StatsDIR)/$(PE).out

testpairend_seed_heuristic_Artfq_problem:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 1000 --filter-distance-difference 20  --reference $(EcoliDIR) --query1 $(PE_DIR)/ART_problem_test1.fq --query2 $(PE_DIR)/ART_problem_test2.fq  >$(PE_StatsDIR)/$(PE).out

testpairend_seed_heuristic_Artfq:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 19 --insert-distance 1000 --filter-distance-difference 20  --reference $(EcoliDIR) --query1 $(PE_DIR)/paired_end_ART_tests1.fq --query2 $(PE_DIR)/paired_end_ART_tests2.fq  >$(PE_StatsDIR)/paired_end_ART_tests2.out

testpairend_seed_heuristic_Artfq_without_trie_opt:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --trie-crumbs-opt NO --insert-distance 1000 --filter-distance-difference 20  --reference $(EcoliDIR) --query1 $(PE_DIR)/paired_end_ART_tests1.fq --query2 $(PE_DIR)/paired_end_ART_tests2.fq  >$(PE_StatsDIR)/$(PE).out

testpairend_seed_heuristic_Art:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 1000 --filter-distance-difference 20  --reference $(EcoliDIR) --query1 $(PE_DIR)/Art_reads1.in --query2 $(PE_DIR)/Art_reads2.in  >$(PE_StatsDIR)/$(PE).out

testpairend_seed_heuristic_without_trie_opt:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --trie-crumbs-opt NO --insert-distance 5000 --filter-distance-difference 50 --trie-positions-limit-checker 0 --reference $(EcoliDIR) --query $(PE_DIR)/1.in >$(PE_StatsDIR)/$(PE).out

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
		--trie-positions-limit-checker 0 \
		--reference $(EcoliDIR) \
		--query $(PE_DIR)/Group$(GROUP)/$(OUT_NUM).in \
		> $(PE_StatsDIR)/Group$(GROUP)/$(OUT_NUM).out

test_group5_1:
	make group_run GROUP=5 OUT_NUM=1
	echo "running group5_1"

test_simultaneous:
	#make -j 3 group_run GROUP=5 OUT_NUM=1 group_run GROUP=5 OUT_NUM=2 group_run GROUP=5 OUT_NUM=3 group_run GROUP=5 OUT_NUM=4 group_run GROUP=5 OUT_NUM=5
	make -j3 -l3 group_run GROUP=5 OUT_NUM=1 & group_run GROUP=5 OUT_NUM=2 & group_run GROUP=5 OUT_NUM=3 & group_run GROUP=5 OUT_NUM=4 & group_run GROUP=5 OUT_NUM=5


compare_dijkstra_independent:
	time ./main.exe --alignment single-read --heuristic dijkstra_heuristic --trie-depth 10 --insert-distance 5000 --filter-distance-difference 50 --reference $(EcoliDIR) --query $(PairedEndCompareDijkstraIndDIR)/DijTests.in >$(PairedEndCompareDijkstraIndDIR)/evaldijind.out

compare_dijkstra_joint:
	time ./main.exe --alignment $(PE) --heuristic dijkstra_heuristic --trie-depth 10 --insert-distance 500 --filter-distance-difference 50 --reference $(EcoliDIR) --query $(PairedEndCompareDijkstraJointDIR)/DijTests.in >$(PairedEndCompareDijkstraJointDIR)/evaldijjoint.out

compare_astar_ind:
	time ./main.exe --alignment single-read --heuristic $(SH)  --independent-aligns 1 --trie-depth 10 --seed-len 12 --insert-distance 500 --filter-distance-difference 50 --reference $(EcoliDIR) --query $(PairedEndCompareAstarIndDIR)/Group4.in >$(PairedEndCompareAstarIndDIR)/evalastarind.out

run_group5:
	parallel -j7 eval make ::: 'group_run GROUP=5 OUT_NUM=1' 'group_run GROUP=5 OUT_NUM=2' 'group_run GROUP=5 OUT_NUM=3' 'group_run GROUP=5 OUT_NUM=4' 'group_run GROUP=5 OUT_NUM=5' 'group_run GROUP=5 OUT_NUM=6'

group1:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group1.in >$(PE_StatsDIR)/Group1.out
group2:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group2.in >$(PE_StatsDIR)/Group2.out
group3:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group3.in >$(PE_StatsDIR)/Group3.out
group4:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/Group4.in >$(PE_StatsDIR)/Group4.out

problem:
	time ./main.exe --alignment $(PE) --heuristic $(SH) --trie-depth 10 --seed-len 12 --insert-distance 5000 --filter-distance-difference 50  --reference $(EcoliDIR) --query $(PE_DIR)/problemquery.in >$(PE_StatsDIR)/problem.out

clean:
	-rm -f main
