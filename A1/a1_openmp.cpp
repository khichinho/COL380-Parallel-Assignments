#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <cstdlib>

using namespace std;

int n; // Size of matrix
int t; // Number of threads

void print_matrix(vector<vector<double>> matrix){
    for(int i=0; i<matrix.size(); i++){
        for(int j =0; j<matrix.size(); j++){
            printf("%lf ",matrix.at(i).at(j));
        }
        printf("\n");
    }
}

vector<vector<double>> generate_identity_matrix(int n){
    vector<vector<double>> matrix;

    for(int i=0; i<n; i++){
        matrix.push_back(vector<double>(n,0));
    }
    for(int i=0; i<n; i++){
        matrix.at(i).at(i) = 1;
    }
    return matrix;
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

// vector<vector<double>> matrix_multiplication(vector<vector<double>> a, vector<vector<double>> b){
//     int r1 = a.size();
//     int c1 = a.at(0).size();
//     int c2 = b.at(0).size();
    
//     vector<vector<double>> mult;
//     for(int i=0; i<r1; i++){
//         mult.push_back(vector<double>(c2,0));
//     }
    
//     for(int i = 0; i < r1; ++i){
//         for(int j = 0; j < c2; ++j){
//             for(int k = 0; k < c1; ++k){
//                 mult.at(i).at(j) += a.at(i).at(k) * b.at(k).at(j);
//             }
//         }
//     }
//     return mult; 
// }

int main(int argc, char *argv[])
{    
    n = stoi(argv[1]);
    t = stoi(argv[2]);
    vector<vector<double>> a = generate_random_matrix(n);
    vector<int> pi(n,0);
    vector<vector<double>> u = generate_identity_matrix(n);
    vector<vector<double>> l = generate_identity_matrix(n);
    
    for(int i=0; i<n; i++){ pi.at(i) = i; }
    for(int k=0; k<n; k++){
        int max=0;
        int k_dash; // Check where to initialise
        for(int i=k; i<n; i++){
            if( max < abs( a.at(i).at(k) ) ){
                max = abs( a.at(i).at(k) );
                k_dash = i;
            }
        }
        try{ if( max == 0 ){ throw "Error: Singular Matrix";} }
        catch(string e){ printf("%s\n", e.c_str()); }
        swap(pi.at(k), pi.at(k_dash));
        swap(a.at(k), a.at(k_dash));
        for(int i=1; i<=k-1; i++){ swap(l.at(k).at(i), l.at(k_dash).at(i)); }
        u.at(k).at(k) = a.at(k).at(k);
        for(int i=k+1; i<n; i++){
            l.at(i).at(k) = a.at(i).at(k)/u.at(k).at(k);
            u.at(k).at(i) = a.at(k).at(i);
        }
        for(int i=k+1; i<n; i++){
            for(int j=k+1; j<n; j++){
                a.at(i).at(j) = a.at(i).at(j) - l.at(i).at(k)*u.at(k).at(j);
            }
        }

    }

    printf("A: \n");
    print_matrix(a);
    printf("\n L: \n");
    print_matrix(l);
    printf("\n U: \n");
    print_matrix(u);
    printf("\n Pi: \n");
    for(int i=0; i<n; i++){ printf("%d ", pi.at(i)); }
    printf("\n");
    // printf("\n L*U: \n");
    // print_matrix(matrix_multiplication(l,u));
    


    return(0);
}


