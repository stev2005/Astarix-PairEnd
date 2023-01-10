default: main

main:
	g++ main.cpp -o main.exe

test:
	./main.exe < input.txt > output.txt

clean:
	-rm -f main