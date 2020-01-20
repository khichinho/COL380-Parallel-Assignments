#include <bits/stdc++.h>
#include <fstream>
using namespace std ;
int n; // Matrix Size
// string matrix_file;
// double *matrix_a;

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


int main(int argc, char *argv[])
{

    // if (argc > 1){
    std::string matrix_filename(argv[1]);
    vector<vector<double>> matrix = read_matrix(matrix_filename) ;
    int n = matrix.size( ) ;
    
    // cout << "done" << endl ;
    // }
    // cout << matrix[0][0] << " " << matrix[0][1] << endl;
    // cout << matrix[1][0] << " " << matrix[1][1] << endl;



    // read_matrix(matrix_file, &n, &data)
    return(0);
}