#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <cstdlib>

using namespace std; 

int n; // Size of matrix
int t; // Number of threads

void print_matrix(double **a){
    for(int i =0 ; i < n ; i +=1){
        for(int j = 0 ; j < n ; j +=1){
            cout << a[i][j] << " " ;
        }
        cout << endl;
    }
}
double** multiply_matrix(double **a, double **b){
    double** mult = new double*[n];
    for(int i = 0; i < n; i++){
        mult[i] = new double[n];
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n; k++){
                mult[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return mult;
}

struct thread_args{
    int t; // number of threads
    int n;
    int thread_tid;
};

int main(int argc, char *argv[]){
    n = stoi(argv[1]);
    t = stoi(argv[2]);
    
    double** a = new double*[n];
    for(int i = 0; i < n; ++i)
        a[i] = new double[n];
    
    double** l = new double*[n];
    for(int i = 0; i < n; ++i)
        l[i] = new double[n];

    double** u = new double*[n];
    for(int i = 0; i < n; ++i)
        u[i] = new double[n];

    double** aCopy = new double*[n];
    for(int i = 0; i < n; ++i)
        aCopy[i] = new double[n];

    double** p = new double*[n];
    for(int i = 0; i < n; ++i)
        p[i] = new double[n];

    int* pi = new int[n];

    pthread_t tids[t];

    srand(1); // Send the same seed if you want to parallelize this part
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            a[i][j] = rand()%100;
            aCopy[i][j] = a[i][j] ;
            u[i][j] = 0.0;
            l[i][j] = 0.0; 
            p[i][j] = 0.0;
        }
        l[i][i] = 1.0;
        pi[i] = i;
    }

    // for(int k = 0 ; k < n ; k +=1){
    //     double max = 0.0;
    //     int kPrime ;
    //     for(int i = k; i < n ; i+= 1){
    //         if(max < abs(a[i][k])){
    //             max = a[i][k] ;
    //             kPrime = i ;
    //         }
    //     }
    //     if(max == 0){
    //         cout << "error : singular matrix" << endl ;
    //         exit(0);
    //     }
    //     int tempInt;
    //     tempInt = pi[k];
    //     pi[k] = pi[kPrime];
    //     pi[kPrime] = tempInt;
        
    //     double tempDouble;
    //     for(int co = 0 ; co < n ; co +=1){
    //         tempDouble = a[k][co];
    //         a[k][co] = a[kPrime][co];
    //         a[kPrime][co] = tempDouble;
    //     }
    //     for(int co = 0 ; co < k ; co+=1){
    //         tempDouble = l[k][co];
    //         l[k][co] = l[kPrime][co];
    //         l[kPrime][co] = tempDouble;
    //     }
    //     u[k][k] = a[k][k];
    //     for(int i = k+1 ; i < n ; i+=1){
    //         l[i][k] = a[i][k] / u[k][k];
    //         u[k][i] = a[k][i];
    //     }
    //     for(int i = k+1 ; i < n ; i+=1){
    //         for(int j = k+1 ; j < n ; j+=1){
    //             a[i][j] = a[i][j] - l[i][k]*u[k][j];
    //         }
    //     }
    // }

    // for(int i = 0; i < n; i++){
    //     int pos = pi[i];
    //     p[i][pos] = 1;
    // }


    // cout << endl << "a" << endl;
    // print_matrix(aCopy);
    // cout << endl << "l" << endl;
    // print_matrix(l);
    // cout << endl << "u" << endl;
    // print_matrix(u);
    // cout << endl << "pi" << endl;
    // for(int i = 0; i < n; i++){ cout << pi[i] << " "; }
    // cout << endl << "p" << endl;
    // print_matrix(p);

    // cout << endl << "p*a" << endl;
    // print_matrix(multiply_matrix(p,aCopy));
    // cout << endl << "l*u" << endl;
    // print_matrix(multiply_matrix(l,u));

}