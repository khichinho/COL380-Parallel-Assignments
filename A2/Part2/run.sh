# Basic Pagerank
g++ pr-cpp.cpp -o ./pr-cpp.o
./pr-cpp.o data/barabasi.txt

# Part 1
g++ mr-pr-cpp.cpp mapreduce.hpp /usr/lib/x86_64-linux-gnu/libboost_system.a /usr/lib/x86_64-linux-gnu/libboost_iostreams.a /usr/lib/x86_64-linux-gnu/libboost_filesystem.a -pthread -o ./mr-pr-cpp.o
./mr-pg-cpp.o data/barabasi-3000.txt