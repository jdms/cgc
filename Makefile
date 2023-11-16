CXX=c++

.NOTPARALLEL:
all: run clean

run:
	@echo "\n" 
	@echo "********************************************************************"
	@echo "*** Building test for cgc:"
	${CXX} -Wall -Wextra -ggdb3 -std=c++11 -I./ -I./test usage.cpp -o main_tests
	@echo "\n" 
	@echo "*** Running test for cgc:"
	@./test/run_valgrind.sh
	@echo "\n" 
	@echo "********************************************************************"
	@echo "*** Building test for ggc:"
	${CXX} -Wall -Wextra -ggdb3 -std=c++11 -I./ -I./test -D USE_GGC \
		usage.cpp -o main_tests
	@echo "\n" 
	@echo "*** Running test for ggc:"
	@./test/run_valgrind.sh
	@echo "\n" 

clean:
	@echo "********************************************************************"
	@echo "*** Cleaning up:" 
	rm -rf main_tests valgrind-out.txt main_tests.dSYM
	@echo "\n" 
