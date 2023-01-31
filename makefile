default: main

main:
	g++ main.cpp -o main.exe

test1:
	./main.exe < testshand/input101.in > testshand/output101.txt

test2:
	./main.exe < tests/input2.txt > tests/output2.txt

test3:
	./main.exe < tests/input3.txt > tests/output3.txt

test4:
	./main.exe < tests/input4.txt > tests/output4.txt

test5:
	./main.exe < tests/input5.txt > tests/output5.txt

test6:
	./main.exe < tests/input6.txt > tests/output6.txt

printfilesnames:
	for file in $$(ls /root/serverexample/Astarix-PairEnd/tests/in); do \
		echo $$file; \
	done

alltests:
	for file in $$(ls /root/serverexample/Astarix-PairEnd/tests/in | grep ".txt"); do \
		#echo $$file; \
		./main.exe <tests/in/$$file >output.txt; \
		cat output.txt; \
	done

writeanswersofalltests:
	data="tests/in"
	answ="tests/out"
	#n=$${data[@]}
	#n= $ ls | wc -l
	n="$${#data[@]}"
	for((i=0;i<n;++i)); do \
		in="$${data[i]}"; \
		out="$${answ[i]}"; \
		./main.exe <in >out; \
	done

oldtests:
	for file in $$(ls testshand | grep ".in"); do \
		./main.exe <testshand/$$file >output.txt; \
		cat output.txt; \
	done

clean:
	-rm -f main
