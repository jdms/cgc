all:
	clang++ -Wall -ggdb3 -std=c++11 readme.cpp -o main_tests

clean:
	rm -f main_tests valgrind-out.txt
