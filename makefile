SingleReadDIR=data/single-reads/tests

default: main

main:
	g++ main.cpp -o main.exe

testsingleread_seed_heuristic:
	time ./main.exe single-read 10 12 seed_heuristic Yes Yes <testsingleread.in >testsingleread_statsAstar.out

exptestsingleread_seed_heuristic:
	#time ./main.exe single-read 10 12 seed_heuristic Yes Yes <testsingleread.in >exptestsingleread_statsAstar.out
	#time ./main.exe --alignment single-read --heuristic seed_heuristic --trie-depth 10 --seed-len 12 <testsingleread.in >exptestsingleread_statsAstar.out
	time ./main.exe --alignment single-read --heuristic seed_heuristic --trie-depth 10 --seed-len 12 --reference data/single-reads/reference.in --query data/single-reads/tests/1.in >exptestsingleread_statsAstar.out

problemtest:
	time ./main.exe single-read 10 20 seed_heuristic Yes Yes <problemtest.in >problemtest.out

testchep_dijkstra_heuristic:
	time ./main.exe pairend-read 10 dijkstra_heuristic Yes Yes <testchep.in

testpairend_seed_heuristic:
	time ./main.exe pairend-read 10 15 seed_heuristic Yes Yes 5001 <testpairend.in >testpairend_statsAstar.out 

test10pairend_seed_heuristic:
	time ./main.exe pairend-read 10 25 seed_heuristic Yes Yes 10000 <onlytest10.in >onlytest10.out

test_try_pe1024:
	time ./main.exe pairend-read 11 seed_heuristic Yes Yes <tests_try_pair-end/test1024.in >tests_try_pair-end/rezult1024.out

helptest:
	time ./main.exe single-read 10 seed_heuristic Yes Yes <helptest.in

evaltest:
	time ./main.exe single-read 11 seed_heuristic Yes Yes <Evaltest.in

clean:
	-rm -f main
