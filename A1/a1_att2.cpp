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
            // printf("%lf ",matrix.at(i).at(j));
            cout << matrix.at(i).at(j) << " " ;
        }
        // printf("\n");
        cout << endl ;
    }
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

vector<vector<double>> matrix_multiplication(vector<vector<double>> a, vector<vector<double>> b){
    int r1 = a.size();
    int c1 = a.at(0).size();
    int c2 = b.at(0).size();
    
    vector<vector<double>> mult;
    for(int i=0; i<r1; i++){
        mult.push_back(vector<double>(c2,0));
    }
    
    for(int i = 0; i < r1; i++){
        for(int j = 0; j < c2; j++){
            for(int k = 0; k < c1; k++){
                mult.at(i).at(j) += a.at(i).at(k) * b.at(k).at(j);
            }
        }
    }
    return mult; 
}

vector<vector<double>> generateP(vector<int > pi){
    int n = pi.size();
    vector<vector<double>> p(n,vector<double>(n));
    for(int i = 0; i < n ; i+=1){
        p.at(i).at(pi[i]) = 1.0;
    }
    return p;
}

vector<vector<double>> difference(vector<vector<double>> a, vector<vector<double>> b){
    int n = a.size();
    vector<vector<double>> diff(n,vector<double>(n));
    for(int i = 0 ; i < n ; i+=1){
        for(int j = 0 ; j < n ; j+=1){
            diff.at(i).at(j) = a.at(i).at(j) - b.at(i).at(j);
        }
    }
    return diff;
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

int main(int argc, char *argv[]){
    std::string matrix_filename(argv[1]);
    vector<vector<double>> a = read_matrix(matrix_filename) ;
    vector<vector<double>> aSave = a;
    int n = a.size( ) ;
    // print_matrix(a);
    vector<int> pi;
    for(int i = 0 ; i < n ; i +=1){
        pi.push_back(i) ;
    }
    vector<vector<double>> u(n,vector<double>(n));
    vector<vector<double>> l(n,vector<double>(n));
    // print_matrix(u) ;
    for(int i = 0 ; i < n ; i +=1){
        l.at(i).at(i) = 1.0; 
    }
    // print_matrix(l) ;
    for(int k = 0 ; k < n ; k +=1){
        double max = 0.0;
        int kPrime ;
        for(int i = k ; i < n ; i +=1){
            if(max < abs(a.at(i).at(k))){
                max = a.at(i).at(k);
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
            tempDouble = a.at(k).at(co);
            a.at(k).at(co) = a.at(kPrime).at(co);
            a.at(kPrime).at(co) = tempDouble;
        }
        for(int co = 0 ; co < k ; co+=1){
            tempDouble = l.at(k).at(co);
            l.at(k).at(co) = l.at(kPrime).at(co);
            l.at(kPrime).at(co) = tempDouble;            
        }
        u.at(k).at(k) = a.at(k).at(k);
        for(int i = k+1 ; i < n ; i+=1){
            l.at(i).at(k) = a.at(i).at(k) / u.at(k).at(k);
            u.at(k).at(i) = a.at(k).at(i);
        }
        for(int i = k+1 ; i < n ; i+=1){
            for(int j = k+1 ; j < n ; j+=1){
                a.at(i).at(j) = a.at(i).at(j) - l.at(i).at(k)*u.at(k).at(j); 
            }
        }
    }
    // cout << "A" << endl;
    // print_matrix(aSave);
    // for(int i = 0 ; i < n ; i +=1){
    //     cout << pi.at(i) << " " ;
    // }
    // cout << endl;
    // cout << "U" <<endl;
    // print_matrix(u);
    // cout << "L" <<endl;
    // print_matrix(l);
    // vector<vector<double>> lu = matrix_multiplication(l,u);
    // cout << "LU" << endl ;
    // print_matrix(lu); 
    vector<vector<double>> p = generateP(pi);
    // cout <<"PA"<<endl;
    // vector<vector<double>> pa = ;
    // print_matrix(matrix_multiplication(p,aSave));
// 
    // vector<vector<double>> diff = ;
    // cout << norm(difference(lu,matrix_multiplication(p,aSave))) << endl;
}