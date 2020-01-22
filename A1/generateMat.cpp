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
            cout << matrix.at(i).at(j) << ' ' ;
        }
        // printf("\n");
        cout <<endl;
    }
}

vector<vector<double>> generate_random_matrix(int n){
    vector<vector<double>> matrix;
    
    srand(1); // Send the same seed if you want to parallelize this part
    for(int i=0; i<n; i++){
        matrix.push_back( vector<double>());
        for(int j=0; j<n; j++){
            matrix.at(i).push_back(rand()%100);
        }
    }
    return matrix;
}

int main(int argc, char *argv[]){
    int n = stoi(argv[1]);
    cout << n << endl;
    vector<vector<double>> a = generate_random_matrix(n);
    print_matrix(a);

}

