#include <iostream>
#include <string>
#include <vector>
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

int main(int argc, char *argv[]){
    n = stoi(argv[1]);
    // t = stoi(argv[2]);
    // declaring arryas and allocating memory

    double** a = new double*[n];
    for(int i = 0; i < n; ++i)
        a[i] = new double[n];

    double** l = new double*[n];
    for(int i = 0; i < n; ++i)
        l[i] = new double[n];

    double** u = new double*[n];
    for(int i = 0; i < n; ++i)
        u[i] = new double[n];
    // keeping a copy of initial array to check ||PA - LU||
    double** aCopy = new double*[n];
    for(int i = 0; i < n; ++i)
        aCopy[i] = new double[n];

    double** mult = new double*[n];
    for(int i = 0; i < n; ++i)
        mult[i] = new double[n];
    


    // assigning initial values
    int* pi = new int[n];
    srand(1); 
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            a[i][j] = rand()%100;
            aCopy[i][j] = a[i][j] ;
            u[i][j] = 0.0;
            l[i][j] = 0.0; 
            mult[i][j] = 0.0;
        }
        l[i][i] = 1.0;
        pi[i] = i;
    }

    for(int k = 0 ; k < n ; k +=1){

        double max = 0.0;
        int kPrime ;
        // finding max element in a column
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
        double tempDouble;
        tempInt = pi[k];
        pi[k] = pi[kPrime];
        pi[kPrime] = tempInt;
        // exchanging rows
        for(int co = 0 ; co < n ; co +=1){
            tempDouble = a[k][co];
            a[k][co] = a[kPrime][co];
            a[kPrime][co] = tempDouble;
        }
        // exchanging l matrix rows
        for(int co = 0 ; co < k ; co+=1){
            tempDouble = l[k][co];
            l[k][co] = l[kPrime][co];
            l[kPrime][co] = tempDouble;            
        }
        // assigning u[k][k] to a[k][k]
        u[k][k] = a[k][k];
        // adjusting l values and u values accordingly
        for(int i = k+1 ; i < n ; i+=1){
            l[i][k] = a[i][k] / u[k][k];
            u[k][i] = a[k][i];
        }

        // assigning a based on u and l
        int i,j;
        for(i = k +1 ; i < n ; i +=1){
            for(j = k + 1 ; j < n ; j +=1){
                a[i][j] = a[i][j] - l[i][k]*u[k][j];
            }
        }        
    }
    
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


    // clearing the allocated memory
    delete a;
    delete u;
    delete l;
    delete aCopy;
    delete pi;
    

}