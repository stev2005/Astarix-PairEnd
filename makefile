default: main

main:
	g++ main.cpp -o main.exe

test1:
	./main.exe < tests/input1.txt > tests/output1.txt

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

clean:
	-rm -f main