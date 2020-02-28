#include "mpi.h"
#include <iostream>
#include <random>
#include <chrono>

using namespace std;

#define N 100
float A[N][32], B[32][N], C[N][N], C_Serial[N][N];

void initializeMatrices(){
    srand(0);
    for(int i = 0 ; i < N ; i +=1){
        for(int j = 0 ; j < 32 ; j +=1){
            A[i][j] = ((float)rand()/(float)RAND_MAX);
        }
    }
    for(int i = 0 ; i < 32 ; i +=1){
        for(int j = 0 ; j < N ; j +=1){
            B[i][j] = ((float)rand()/(float)RAND_MAX);
        }
    }
}

void Matrix_Multiply(float **A, float **B, float **C, int m, int n, int p){
	int i, j, k;
	for (i = 0; i < m; i++){
		for (j = 0; j < p; j++){
			C[i][j] = 0.f;
			for (k = 0; k < n; k++){
				C[i][j] += A[i][k] * B[k][j];
            }
		}
	}
}

int IsEqual(float **A, float **B, int m, int n)
{
    int flag = 1;
    for(int i = 0; i < m; i +=1){
        for(int j = 0 ; j < n ; j +=1){
            if(A[i][j] != B[i][j]){
                flag = 0;
                // cout << i << " " << j << " " << endl;
                // cout << A[i][j] << " " << B[i][j] <<endl;
                break;
            }
        }
    }
    return(flag);
}

int main(int argc, char *argv[]){
    int myRank, numThreads, rowsPerThread, rowsForLastThread, from, runtime;

    MPI_Request request1, request2, request3, request4, request5;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numThreads);
    
    auto start = chrono::steady_clock::now();

    /*---------------------------- master ----------------------------*/
    if (myRank == 0) {
        initializeMatrices();
        rowsPerThread = (N/numThreads);
        rowsForLastThread = rowsPerThread + (N%numThreads);
        from = rowsPerThread;

        /* send matrix data to the worker threads */
        for(int dest=1; dest<numThreads; dest++){
            if(dest != numThreads-1){
                MPI_Isend(&rowsPerThread, 1, MPI_INT, dest, 1, MPI_COMM_WORLD, &request1);
                MPI_Isend(&A[from][0], rowsPerThread*32, MPI_FLOAT, dest, 2, MPI_COMM_WORLD, &request2);
            }
            else{
                MPI_Isend(&rowsForLastThread, 1, MPI_INT, dest, 1, MPI_COMM_WORLD, &request1);
                MPI_Isend(&A[from][0], rowsForLastThread*32, MPI_FLOAT, dest, 2, MPI_COMM_WORLD, &request2);
            }
            MPI_Isend(&B, 32*N, MPI_FLOAT, dest, 3, MPI_COMM_WORLD, &request3);
            from += rowsPerThread;
        }

        /* Matrix multiplication for master thread */
        for (int i=0; i<rowsPerThread; i++){
            for (int j=0; j<N; j++) {
                C[i][j] = 0.0;
                for (int k=0; k<32; k++){
                    C[i][j] = C[i][j] + A[i][k] * B[k][j];
                }
            }
        }
        
        /* wait for results from all worker threads */
        from = rowsPerThread;
        for (int i=1; i<numThreads; i++){
            MPI_Recv(&rowsPerThread, 1, MPI_INT, i, 4, MPI_COMM_WORLD, &status);
            MPI_Recv(&C[from][0], rowsPerThread*N, MPI_FLOAT, i, 5, MPI_COMM_WORLD, &status);
            from += rowsPerThread;
        }


        auto end = chrono::steady_clock::now();
        auto diff = end - start;
        cout << "Parallel Program Runtime: " << chrono::duration <double, milli> (diff).count() << " ms" << endl;
        
        /* Print matrix A */
        // cout << "A:" << endl; 
        // for (int i=0; i<N; i++) {
        //     for (int j=0; j<32; j++){
        //         cout << A[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        /* Print matrix B */
        // cout << endl << "B:" << endl;
        // for (int i=0; i<32; i++) {
        //     for (int j=0; j<N; j++){
        //         cout << B[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        /* Print matrix C */
        // cout << endl << "C:" << endl;
        // for (int i=0; i<N; i++) {
        //     for (int j=0; j<N; j++){
        //         cout << C[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                C_Serial[i][j] = 0.f;
                for (int k = 0; k < 32; k++){
                    C_Serial[i][j] += A[i][k] * B[k][j];
                }
            }
        }

        /* Print matrix C_Serial */
        // cout << endl << "C_Serial:" << endl;
        // for (int i=0; i<N; i++) {
        //     for (int j=0; j<N; j++){
        //         cout << C_Serial[i][j] << " ";
        //     }
        //     cout << endl;
        // }

        int flag = 1;
        for(int i = 0; i < N; i +=1){
            for(int j = 0 ; j < N ; j +=1){
                if(C[i][j] != C_Serial[i][j]){
                    flag = 0;
                    break;
                }
            }
        }

        if(flag==1){ cout << "Serial And Parallel Computations are EQUAL " << endl; } 
        else if(flag==0){ cout << "Serial And Parallel Computations are NOT EQUAL " << endl; }         
    }
    
    /*---------------------------- worker----------------------------*/
    if (myRank > 0) {
        MPI_Recv(&rowsPerThread, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status, &request1);
        MPI_Wait(&request1, &status);
        MPI_Irecv(&A, rowsPerThread*32, MPI_FLOAT, 0, 2, MPI_COMM_WORLD, &status, &request2);
        MPI_Wait(&request2, &status);
        MPI_Irecv(&B, 32*N, MPI_FLOAT, 0, 3, MPI_COMM_WORLD, &status, &request3);
        MPI_Wait(&request3, &status);
        

        /* Matrix multiplication for worker threads */
        for (int i=0; i<rowsPerThread; i++){
            for (int j=0; j<N; j++) {
                C[i][j] = 0.0;
                for (int k=0; k<32; k++){
                    C[i][j] = C[i][j] + A[i][k] * B[k][j];
                }
            }
        }

        MPI_Send(&rowsPerThread, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&C, rowsPerThread*N, MPI_FLOAT, 0, 2, MPI_COMM_WORLD);
    
        // cout << rowsPerThread << " " << endl;
    }

    MPI_Finalize();

}