# Locality-sensitive hashing of curves

This is a naive implementation of the paper [Locality-sensitive hashing of curves](https://arxiv.org/pdf/1703.04040.pdf) which explores data structures for storing a set of polygonal curves such that given a query curve, we can efficiently retrieve "similar" curves from the set. 

## Compilation:
* make
* make src (source files only)
* make tests (tests only)

## Execution:
* ./bin/conformations -i "dataset path" -o "output file path" -c "cluster configuration file" -d "distance metric: {DFT, cRMSD}"
* ./bin_tests/test
