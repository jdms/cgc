.NOTPARALLEL:
all: run clean

build:
	@echo "\n" 
	@echo "********************************************************************"
	@echo "Building test:"
	@echo "********************************************************************"
	@echo "\n" 
	clang++ -Wall -ggdb3 -std=c++11 readme.cpp -o main_tests
	@echo "\n" 
	@echo "\n" 

run: build
	@echo "********************************************************************"
	@echo "Running test:"
	@echo "********************************************************************"
	@echo "\n" 
	@./run_valgrind.sh
	@echo "\n" 
	@echo "\n" 

clean:
	@echo "********************************************************************"
	@echo "Cleaning up:" 
	@echo "********************************************************************"
	@echo "\n" 
	rm -rf main_tests valgrind-out.txt main_tests.dSYM
	@echo "\n" 
	@echo "\n" 
