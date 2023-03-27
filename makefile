default: main

main:
	g++ main.cpp -o main.exe

testtrie:
	for file in $$(ls tests-single-read_small_length); do \
		echo $$file; \
		ext="$${file##*.}"; \
		if [ "$$ext" = "in" ]; then \
			time ./main.exe single-read 4 seed_heuristic No Yes <tests-single-read_small_length/$$file >output.out; \
		else \
			diff -q output.out tests/$$file; \
		fi \
	done
	echo "Testing done"

test:
	for file in $$(ls tests-single-read_small_length); do \
		echo $$file; \
		ext="$${file##*.}"; \
		if [ "$$ext" = "in" ]; then \
			time ./main.exe single-read 4 seed_heuristic No No <tests-single-read_small_length/$$file >output.out; \
		else \
			diff -q output.out tests/$$file; \
		fi \
	done
	echo "Testing done"

testdijkstra:
	for file in $$(ls tests-single-read_small_length); do \
		echo $$file; \
		ext="$${file##*.}"; \
		if [ "$$ext" = "in" ]; then \
			time ./main.exe single-read 4 dijkstra_heuristic No <tests-single-read_small_length/$$file >output.out; \
		else \
			diff -q output.out tests/$$file; \
		fi \
	done
	echo "Testing done"

testp:
	for file in $$(ls tests-paired-end_small_length); do \
		echo $$file; \
		ext="$${file##*.}"; \
		if [ "$$ext" = "in" ]; then \
			./main.exe single-read 4 <tests-paired-end_small_length/$$file >output.out; \
		else \
			diff -q output.out tests-paired-end/$$file; \
		fi \
	done
	echo "Testing done"

test1dijkstra:
	./main.exe single-read 10 dijkstra_heuristic Yes No <tests/1.in

testche_dijkstra_heuristic:
	time ./main.exe single-read 10 dijkstra_heuristic Yes Yes <testche.in

testche_seed_heuristic:
	time ./main.exe single-read 10 seed_heuristic Yes Yes <testche.in

testchep_dijkstra_heuristic:
	time ./main.exe pairend-read 10 dijkstra_heuristic Yes Yes <testchep.in

testchep_seed_heuristic:
	time ./main.exe pairend-read 10 seed_heuristic Yes Yes <testchep.in

helptest:
	time ./main.exe single-read 10 seed_heuristic Yes Yes <helptest.in

evaltest:
	time ./main.exe single-read 11 seed_heuristic Yes Yes <Evaltest.in

testp1:
	./main.exe <tests-paired-end/1.in

printexplstatesastartrie:
	for file in $$(ls tests-single-read_small_length); do \
		ext="$${file##*.}"; \
		if [ "$$ext" = "in" ]; then \
			./main.exe single-read 4 seed_heuristic Yes Yes <tests-single-read_small_length/$$file >output.out; \
		else \
			filename="$${file%%.*}"; \
			echo "Test $$filename:" >> stats.out; \
			cat output.out >> stats.out; \
		fi \
	done

printexplstatesastar:
	for file in $$(ls test-single-read_small_length); do \
		ext="$${file##*.}"; \
		if [ "$$ext" = "in" ]; then \
			./main.exe single-read 4 seed_heuristic Yes No <tests-single-read_small_length/$$file >output.out; \
		else \
			filename="$${file%%.*}"; \
			echo "Test $$filename:" >> stats.out; \
			cat output.out >> stats.out; \
		fi \
	done

printexplstatesdijktratrie:
	for file in $$(ls tests-single-read_small_length); do \
		ext="$${file##*.}"; \
		if [ "$$ext" = "in" ]; then \
			./main.exe single-read 4 dijkstra_heuristic Yes <tests-single-read_small_length/$$file >output.out; \
		else \
			filename="$${file%%.*}"; \
			echo "Test $$filename:" >> stats.out; \
			cat output.out >> stats.out; \
		fi \
	done

writeanswers:
	for file in $$(ls tests-single-read_small_length); do\
		ext="$${file##*.}"; \
		#echo $$ext; \
		if [ "$$ext" = "in" ]; then \
			#echo "The value is in"; \
			./main.exe <tests-single-read_small_length/$$file >output.out; \
			#echo $$file; \
			#cat $$file; \
		else \
			#echo "The value is out"; \
			cp output.out tests-single-read_small_length/$$file; \
			#echo "this is an out file"; \
		fi \
	done

writeanswerspe:
	dir="tests-paired-end"
	for file in $$(ls tests-paired_small_length); do\
		ext="$${file##*.}"; \
		echo tests-paired-end/$$file; \
		echo $$ext; \
		if [ "$$ext" = "in" ]; then \
			./main.exe < tests-paired-end/$$file >output.out; \
		else \
			cp output.out tests-paired-end/$$file; \
		fi \
	done

 deletion:
	for file in $$(ls tests-paired-end_small_length); do\
		ext="$${file##*.}"; \
		echo $$ext; \
		if [ "$$ext" = "in "]; then \
			echo $$ext; \
		else \
			rm tests-paired-end_small_length/$$file; \
		fi \
	done

clean:
	-rm -f main
