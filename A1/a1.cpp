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
    // return(0);


    vector<int> pi;
    vector<vector<double>> upper(n,vector<double>(n));
    // cout<<upper[0][1] <<endl;
    vector<vector<double>> lower(n,vector<double>(n));
    
    for(int i =0 ; i < n ; i +=1){
        lower[i][i] = 1.00;
    }
    for(int i = 0 ; i < n ; i +=1){
        pi.push_back(i);
    }
    cout << upper[0][0] << " " << upper[0][1] << endl ;
    cout << upper[1][0] << " " << upper[1][1] << endl ;
    cout << lower[0][0] << " " << lower[0][1] << endl ;
    cout << lower[1][0] << " " << lower[1][1] << endl ;
    

    for(int i =0 ; i < n ; i +=1){
        int iprime;
        double max= 0 ; 
        for(int j = i ; j < n ; j +=1){ 
            if(max < abs(matrix[j][i])){
                max = abs(matrix[j][i]);
                iprime = j ;
            }
            cout << "max = " << max << " " << matrix[j][i]<< endl ; 
        }
        if(max ==0 ){
            cout << i << endl ;
            cout << "error here" <<endl;
            exit(0);
        }
        int t = pi[i];
        double dtemp;
        pi[i] = pi[iprime];
        pi[iprime] = t ;
        for(int j =0 ; j < n; j +=1){
            // matrix(i,)
            dtemp = matrix[i][j] ;
            matrix[i][j] = matrix[iprime][j];
        }
        for(int j =0 ; j < i-1; j +=1){
            // matrix(i,)
            dtemp = matrix[i][j] ;
            lower[i][j] = lower[iprime][j];
        }
        upper[i][i] = matrix[i][i];
        for(int j = i+1 ; j < n ; j +=1){
            lower[j][i] = matrix[i][j] / upper[i][i] ;
            upper[i][j] = matrix[i][j];
        }

        for(int j = i +1 ; j < n ; j +=1){
            for(int k = i +1 ; k < n ; k +=1){
                matrix[j][k] -= lower[j][i] * upper[i][k] ;
            }
        }

    }

    cout << upper[0][0] << " " << upper[0][1] << endl ;
    cout << upper[1][0] << " " << upper[1][1] << endl ;
    cout << lower[0][0] << " " << lower[0][1] << endl ;
    cout << lower[1][0] << " " << lower[1][1] << endl ;
        

}