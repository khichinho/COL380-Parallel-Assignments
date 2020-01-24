rm a.out
g++ a1_sequential.cpp
time ./a.out 1500
rm a.out 
g++ -pthread a1_pthreads.cpp
time ./a.out 1500 4
rm a.out
g++ -fopenmp a1_openmp.cpp
time ./a.out 1500 4