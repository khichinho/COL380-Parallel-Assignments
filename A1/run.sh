<<<<<<< HEAD
g++ a1_openmp.cpp
./a.out $1 $2
=======
rm mat.txt
g++ generateMat.cpp
./a.out 1000 > mat.txt
g++ a1_att2.cpp
./a.out mat.txt 
>>>>>>> a2aebee0cf314e038871bb86e61079383448e99b
