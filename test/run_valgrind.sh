#!/bin/bash

# This script uses valgrind to check whether cgc/ggc is working properly or
# not.  valgrind will perform a full leak analysis on the 'main' binary, and
# report any possible mismanagement of memory.

if [ $(command -v valgrind) ]; then
    valgrind \
        --leak-check=full \
        --show-leak-kinds=all \
        --track-origins=yes \
        --verbose \
        --log-file=valgrind-out.txt \
        --error-exitcode=1 \
        ./main_tests

    if [ $? -eq 0 ]
    then
        echo -e "\n" 
        echo -e "---> Test successful."
    else
        echo -e "\n" 
        echo -e "---> ERROR: valgrind output was saved to file 'valgrind-out.txt'"
        exit 1
    fi
else
    echo -e "*** Valgrind is required to run tests!\n"
    exit 2
fi
