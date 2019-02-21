CXX=clang++

.NOTPARALLEL:
all: run clean

build:
	@echo "\n" 
	@echo "********************************************************************"
	@echo "Building test:"
	@echo "********************************************************************"
	@echo "\n" 
	${CXX} -Wall -ggdb3 -std=c++11 -I./ -I./debug readme.cpp -o main_tests
	@echo "\n" 
	@echo "\n" 

run: build
	@echo "********************************************************************"
	@echo "Running test:"
	@echo "********************************************************************"
	@echo "\n" 
	@./debug/run_valgrind.sh
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
