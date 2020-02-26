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

// Struct that stores arguments for threads
struct thread_args{
    int inputSize;
    int k;
    double** a;
    double** l;
    double** u;
    int startPos;
    int endPos;
};

// Parallel code for nested loop
void* parallelfn(void* argc){
    struct thread_args *t_args = (struct thread_args*) argc;
    
    for(int i = t_args->startPos+1 ; i < t_args->endPos ; i+=1){
        for(int j = t_args->k ; j < n ; j+=1){
            t_args->a[i][j] = t_args->a[i][j] - t_args->l[i][t_args->k]*t_args->u[t_args->k][j];
        }
    }
    
    pthread_exit(0);
}

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

    for(int k = 0 ; k < n ; k +=1){
        double max = 0.0;
        int kPrime ;
        for(int i = k; i < n ; i+= 1){
            if(max < abs(a[i][k])){
                max = a[i][k] ;
                kPrime = i ;
            }
        }
        if(max == 0){
            cout << "error : singular matrix" << endl ;
            exit(0);
        }
        int tempInt;
        tempInt = pi[k];
        pi[k] = pi[kPrime];
        pi[kPrime] = tempInt;
        
        double tempDouble;
        for(int co = 0 ; co < n ; co +=1){
            tempDouble = a[k][co];
            a[k][co] = a[kPrime][co];
            a[kPrime][co] = tempDouble;
        }
        for(int co = 0 ; co < k ; co+=1){
            tempDouble = l[k][co];
            l[k][co] = l[kPrime][co];
            l[kPrime][co] = tempDouble;
        }
        u[k][k] = a[k][k];
        for(int i = k+1 ; i < n ; i+=1){
            l[i][k] = a[i][k] / u[k][k];
            u[k][i] = a[k][i];
        }

        // Pthreads implementation
        int rowsPerThreads = (n-k)/t;
        pthread_t tids[t];
        for (int thread = 0; thread < t; thread++)
        {
            struct thread_args *targs = (struct thread_args *)malloc(sizeof(struct thread_args));
            targs->inputSize = n;
            targs->a = a;
            targs->l = l;
            targs->u = u;
            targs->k = k;
            targs->startPos = k + thread*rowsPerThreads;
            if(thread != t-1){ targs->endPos = k + (thread+1)*rowsPerThreads; }
            else{ targs->endPos = n; }

            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_create(&tids[thread], &attr, parallelfn, (void*)targs);
        }
        for(int thread=0; thread < t; thread++){
            pthread_join(tids[thread],NULL);
        }
    }

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