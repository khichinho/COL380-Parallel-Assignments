#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <cstdlib>

using namespace std; 


// void print_matrix(double **a){
//     int n = sizeof(a);
//     for(int i =0 ; i < n ; i +=1){
//         for(int j = 0 ; j < n ; j +=1){
//             cout << a[i][j] << " " ;
//         }
//         cout << endl;
//     }

// }

int main(int argc, char *argv[]){
    int n = stoi(argv[1]);
    // int n = 7;
    // int t = stoi(argv[2]);
    // double** a  =new double[n][n];
    double** a = new double*[n];
    for(int i = 0; i < n; ++i)
        a[i] = new double[n];
    // double aCopy[n][n];
    double** l = new double*[n];
    for(int i = 0; i < n; ++i)
        l[i] = new double[n];

    double** u = new double*[n];
    for(int i = 0; i < n; ++i)
        u[i] = new double[n];

    double** aCopy = new double*[n];
    for(int i = 0; i < n; ++i)
        aCopy[i] = new double[n];

    double** mult = new double*[n];
    for(int i = 0; i < n; ++i)
        mult[i] = new double[n];
    

    // double u[n][n];
    // double l[n][n];
    // cout << "here" << endl;
    int* pi = new int[n];
    srand(1); // Send the same seed if you want to parallelize this part
    for(int i=0; i<n; i++){
        // matrix.push_back( vector<double>());
        // l[i][i] = 1;
        for(int j=0; j<n; j++){
            // matrix.at(i).push_back(rand()%100);
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
        // cout << "*********" << endl;
        // for(int i =0 ; i < n ; i +=1){
        //     for(int j = 0 ; j < n ; j +=1){
        //         cout << a[i][j] << " " ;
        //     }
        //     cout << endl;
        // }
        // for(int i =0 ; i < n ; i +=1){
        //     for(int j = 0 ; j < n ; j +=1){
        //         cout << l[i][j] << " " ;
        //     }
        //     cout << endl;
        // }
        // for(int i =0 ; i < n ; i +=1){
        //     for(int j = 0 ; j < n ; j +=1){
        //         cout << u[i][j] << " " ;
        //     }
        //     cout << endl;
        // }

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
        double tempDouble;
        tempInt = pi[k];
        pi[k] = pi[kPrime];
        pi[kPrime] = tempInt;
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
            // l.at(i).at(k) = a.at(i).at(k) / u.at(k).at(k);
            // u.at(k).at(i) = a.at(k).at(i);
            l[i][k] = a[i][k] / u[k][k];
            u[k][i] = a[k][i];
        }

        // # pragma omp parallel for num_threads(4)
        // int i,j;
        // #pragma omp parallel num_threads(4)  default(none) private(i,j)  shared(n,k,a,u,l)
        # pragma omp parallel num_threads(4) 
        //default(none) shared(n,k,a,u,l)
        for(int i = k +1 ; i < n ; i +=1){
            for(int j = k + 1 ; j < n ; j +=1){
                // cout << "-------------" << endl;
                // cout << k << " " << i << " " <<j <<endl;
                // cout << "here "<<a[i][j] << " " << l[i][k] << " " << u[k][j] << endl;
                a[i][j] = a[i][j] - l[i][k]*u[k][j];
                // cout << "here "<<a[i][j] <<endl;
            }
        }        

    }

    // delete a;
    // delete u;
    // delete l;
    // delete aCopy;
    // delete pi;
    // print_matrix(aCopy);
    
    // for(int i = 0 ; i < n ; i +=1){
    //     for(int j = 0; j < n ; j +=1){
    //         for(int k = 0; k < n ; k +=1){
    //             mult[i][j] += l[i][k] * u[k][j];
    //         }
    //     }
    // }
    // cout << "a" <<endl;
    // for(int i =0 ; i < n ; i +=1){
    //     for(int j = 0 ; j < n ; j +=1){
    //         cout << aCopy[i][j] << " " ;
    //     }
    //     cout << endl;
    // }
    // cout << "lu" <<endl;
    // for(int i =0 ; i < n ; i +=1){
    //     for(int j = 0 ; j < n ; j +=1){
    //         cout << mult[i][j] << " " ;
    //     }
    //     cout << endl;
    // }
    // cout << "pi" <<endl;
    // for(int i =0 ; i < n ; i +=1){
    //     cout << pi[i] << " ";
    // }


    // cout << "done here" << endl;
    





}