#include "mpi.h"
#include <iostream>
using namespace std;
// #include <random>


#define N 100

float A[N][32], B[32][N], C[N][N];

void initializeMatrices(){

    for(int i = 0 ; i < N ; i +=1){
        for(int j = 0 ; j < 32 ; j +=1){
            A[i][j] = ((float)rand()/(float)RAND_MAX);
            // cout << i << " "<< j << " " << A[i][j] << endl;  
        }
    }
    for(int i = 0 ; i < 32 ; i +=1){
        for(int j = 0 ; j < N ; j +=1){
            B[i][j] = ((float)rand()/(float)RAND_MAX);
            // cout << i << " "<< j << " " << A[i][j] << endl;
        }
    }

}



int main(int argc, char *argv[]){
    int myRank, numProcessors, from, to;
    int tag = 3223;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcessors);
    // cout << numProcessors << endl;
    from  = myRank * (N/numProcessors);
    to = (myRank+1)*(N/numProcessors);

    if(myRank == 0){
        initializeMatrices();
    }
    MPI_Bcast(B,N*32,MPI_FLOAT,0,MPI_COMM_WORLD);
    MPI_Scatter(A,(N*32/numProcessors),MPI_FLOAT,A[from],(N*32/numProcessors),MPI_FLOAT,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    for(int i = from; i < to; i +=1){
        for(int j = 0 ; j < N ; j +=1){
            // cout << i << " " << j << " " << C[i][j] << " " << myRank << endl; 
            C[i][j] = 0.f;
            for(int k = 0; k < 32; k +=1){
                C[i][j] += A[i][k] + B[k][j];
            }
            // cout << i << " " << j << " " << C[i][j] << " " << myRank << endl; 
        }
    }
    // cout << "here " << myRank << endl;
    MPI_Gather(C[from],(N*N/numProcessors),MPI_FLOAT, C,(N*N/numProcessors),MPI_FLOAT,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);    
    // if(myRank == 0){
    //     cout << "here" << endl;
    // }
    // if(myRank == 0){

    //     for(int i = 0; i < N; i +=1){
    //         for(int j = 0 ; j < N ; j +=1){
    //             // cout << C[i][j] << " ";
    //             // if(C[i][j] != C2[i][j]){
    //             cout << C[i][j] <<" ";
    //             // }
    //         }
            
    //         cout << endl;
    //     }
    // }
    if(myRank == 0){
        float C2[N][N];
        for(int i = 0; i < N; i +=1){
            for(int j = 0 ; j < N ; j +=1){
                C2[i][j] = 0.f;
                for(int k = 0; k < 32; k +=1){
                    // cout <<"sone" << i << " " << j  <<endl;
                    C2[i][j] += A[i][k] + B[k][j];
                }
            }
        }
        bool flag = true;
        for(int i = 0; i < N; i +=1){
            for(int j = 0 ; j < N ; j +=1){
                // cout << C[i][j] << " ";
                if(C[i][j] != C2[i][j]){
                    flag = false;
                    break;
                    // cout << i << " " << j <<"  "<< C[i][j]  << " " <<  C2[i][j] <<endl;
                }
            }
            // cout << endl;
        }
        cout << "result = " << flag << endl;
            
    }
    MPI_Finalize();
    return 0;
}