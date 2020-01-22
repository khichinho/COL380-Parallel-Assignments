#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <cstdlib>
using namespace std;

void print_matrix(vector<vector<double>> matrix){
    for(int i=0; i<matrix.size(); i++){
        for(int j =0; j<matrix.size(); j++){
            printf("%lf ",matrix.at(i).at(j));
        }
        printf("\n");
    }
}

double norm(vector<vector<double>> matrix){
    int n = matrix.size() ;
    double normVal = 0.0 ;
    for(int i = 0; i < n ; i +=1){
        for(int j = 0; j < n ; j +=1){
            normVal += matrix[i][j] * matrix[i][j] ;
        }        
    }
    normVal = sqrt(normVal) ;
    return normVal;
}

vector<vector<double>> generate_random_matrix(int n){
    vector<vector<double>> matrix;
    
    srand(1); // Send the same seed if you want to parallelize this part
    for(int i=0; i<n; i++){
        matrix.push_back( vector<double>());
        for(int j=0; j<n; j++){
            matrix.at(i).push_back(rand());
        }
    }
    return matrix;
}

vector<vector<double>> matrix_multiplication(vector<vector<double>> a, vector<vector<double>> b){
    int r1 = a.size();
    int c1 = a.at(0).size();
    int c2 = b.at(0).size();
    
    vector<vector<double>> mult;
    for(int i=0; i<r1; i++){
        mult.push_back(vector<double>(c2,0));
    }
    
    for(int i = 0; i < r1; ++i){
        for(int j = 0; j < c2; ++j){
            for(int k = 0; k < c1; ++k){
                mult.at(i).at(j) += a.at(i).at(k) * b.at(k).at(j);
            }
        }
    }
    return mult; 
}

vector<vector<double>> read_matrix(string matrix_filename){
    // FILE *fin = fopen(matrix_filename, "r");

    // *matrix = (double*) malloc( sizeof(double) * (*N) * (*N));

    // for(int i=0; i< (*N)*(*N); i++){
    //         fscanf(fin, "%d", (*matrix + i));
    //     }
    // }
    // fclose(fin)
    ifstream infile(matrix_filename);
    vector<vector<double>> matrix ;
    int n ;
    infile >> n ;
    double d ;
    for(int i = 0 ; i < n ; i +=1){
        matrix.push_back( vector<double>() ) ; 
        for(int j = 0 ; j < n ; j +=1){
            infile >> d ; 
            matrix.at(i).push_back(d) ;
        }
    }
    return matrix;
}


int main(int argc, char *argv[]){

    int n = stoi(argv[1]);
    // t = stoi(argv[2]);
    // vector<vector<double>> a = generate_random_matrix(n);
    vector<vector<double>> a = read_matrix("mat.txt");
    vector<vector<double>> aSaved = a;
    print_matrix(a);
    vector<int> pi;
    vector<vector<double>> u(n,vector<double>(n));
    // cout<<upper[0][1] <<endl;
    vector<vector<double>> l(n,vector<double>(n));
    for(int i =0 ; i < n ; i +=1){
        l[i][i] = 1.00;
    }
    for(int i = 0 ; i < n ; i +=1){
        pi.push_back(i);
    }


    // print_matrix(u);
    // print_matrix(l);
    for(int k = 0 ; k < n ; k +=1){
        // for(int i = 0 ; i < n ; i +=1){
        //     cout << pi[i] << " " ;
        // }
        // cout << endl ;
        double max = 0 ; 
        int kPrime = -1;
        for(int i = k ; i < n ; i +=1){
            if(max < abs(a[i][k])){
                max = abs(a[i][k]);
                kPrime = i ;
            }
        }
        if(max == 0){
            cout << "error singular matrix" << endl ;
            exit(0);
        }
        int tempInt ;
        double tempDouble;
        tempInt = pi[k];
        pi[k] = pi[kPrime];
        pi[kPrime] = tempInt;
        for(int j = 0 ; j < n ; j +=1){
            tempDouble = a[k][j] ;
            a[k][j] = a[kPrime][j];
            a[kPrime][j] = tempDouble;
        }
        for(int j = 0 ; j < k-1 ; j +=1){
            tempDouble = l[k][j] ;
            l[k][j] = l[kPrime][j] ;
            l[kPrime][j] = tempDouble ;
        }
        u[k][k] = a[k][k] ;
        for(int i = k + 1 ; i < n ; i +=1){
            l[i][k] = a[i][k] / u[k][k] ;
            u[k][i] = a[k][i] ;
        }
        for(int i = k +1 ; i < n ; i +=1){
            for(int j = k + 1 ; j < n ; j +=1){
                a[i][j] = a[i][j] - l[i][k] * u[k][j];
            }
        }
    }

    vector<vector<double>> lu = matrix_multiplication(l,u);
    // print_matrix(aSaved);
    for(int i = 0 ; i < n ; i +=1){
        cout << pi[i] << " " ;
    }
    cout << endl ;
    print_matrix(lu);
    // print()
}