rm mat.txt
g++ generateMat.cpp
./a.out 1000 > mat.txt
g++ a1_att2.cpp
./a.out mat.txt 