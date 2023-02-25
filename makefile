default: main

main:
	g++ main.cpp -o main.exe

test:
	for file in $$(ls tests); do \
		ext="$${file##*.}"; \
		if [ "$$ext" = "in" ]; then \
			./main.exe <tests/$$file >output.out; \
		else \
			diff -q output.out tests/$$file; \
		fi \
	done
	echo "Testing done"

testp:
	for file in $$(ls tests-paired-end); do \
		echo $$file; \
		ext="$${file##*.}"; \
		if [ "$$ext" = "in" ]; then \
			./main.exe <tests-paired-end/$$file >output.out; \
		else \
			diff -q output.out tests-paired-end/$$file; \
		fi \
	done
	echo "Testing done"

test1:
	./main.exe <tests-paired-end/1.in

writeanswers:
	for file in $$(ls tests); do\
		ext="$${file##*.}"; \
		#echo $$ext; \
		if [ "$$ext" = "in" ]; then \
			#echo "The value is in"; \
			./main.exe <tests/$$file >output.out; \
			#echo $$file; \
			#cat $$file; \
		else \
			#echo "The value is out"; \
			cp output.out tests/$$file; \
			#echo "this is an out file"; \
		fi \
	done

writeanswerspe:
	dir="tests-paired-end"
	for file in $$(ls tests-paired-end); do\
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
	for file in $$(ls tests-paired-end); do\
		ext="$${file##*.}"; \
		echo $$ext; \
		if [ "$$ext" = "in "]; then \
			echo $$ext; \
		else \
			rm tests-paired-end/$$file; \
		fi \
	done

printfilesnames:
	for file in $$(ls /root/serverexample/Astarix-PairEnd/tests/in); do \
		echo $$file; \
	done

changeext:
	for file in $$(ls tests | grep ".out$$"); do \
		mv -- tests/"$$file" tests/"$${file%.out}.sol"; \
	done

alltests:
	for file in $$(ls /root/serverexample/Astarix-PairEnd/tests/in | grep ".txt"); do \
		#echo $$file; \
		./main.exe <tests/in/$$file >output.txt; \
		cat output.txt; \
	done

clean:
	-rm -f main
