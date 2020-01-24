# rm mat.txt
# g++ generateMat.cpp
# ./a.out 1000 > mat.txt
# g++ a1_pthreads.cpp
# time ./a.out 7000 2
rm ./a.out
g++ -lpthread a1_pthreads.cpp
time ./a.out 150 2
