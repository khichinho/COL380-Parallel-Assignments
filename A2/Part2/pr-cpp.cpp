#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>

// #include "mapreduce.hpp"

using namespace std;

int web_size = 30000;
const double s = 0.85;
const double convergence = 0.00001;

vector<vector<int>> incoming; // stores incoming links to a page
vector<int> number_outgoing; // stores number of outgoing links from a page

void print_web(){
    for(int i=0; i<web_size; i++){
        cout << i << " -> Incoming: ";
        for(int j=0; j<incoming.at(i).size(); j++){
            cout << incoming.at(i).at(j) << ",";
        }
        cout << "\b" << endl;
        cout << "     Num_Outgoing: " << number_outgoing.at(i) << endl;
    }
}

void read_file(const string &filename) {
    istream *infile;
    infile = new ifstream(filename);
    
    // // TODO: Auto-initialize for diffenert input file type
    // for(int i=0; i<web_size; i++){ 
    //     vector<int> incoming_links;
    //     incoming.push_back(incoming_links); 
    //     number_outgoing.push_back(0);
    // }

    vector<int> incoming_links_to_0;
    incoming.push_back(incoming_links_to_0);
    number_outgoing.push_back(0);
    int count=1;

    string line;
    while (getline(*infile, line)){
        vector<int> incoming_links;
        incoming.push_back(incoming_links);
        number_outgoing.push_back(0);
        count += 1;

        int from, to;

        istringstream ss(line);
        ss >> from; 
        ss >> to;

        incoming.at(to).push_back(from);
        number_outgoing.at(from) += 1;

        // // cout << from << " " << to << endl;
    }
    web_size = count;
}

double* step(double **web, double *pr){
    double* v = new double[web_size];
    for(int i=0; i<web_size; i++){ v[i] = 0;}

    double inner_product = 0;
    for(int i=0; i<web_size; i++){
        // handling dangling pages
        if(number_outgoing[i] == 0){
            inner_product += pr[i];
        }
    }

    double v_sum = 0;
    for(int i=0; i<web_size; i++){
        double random_pick = 0;
        for(int j=0; j<incoming.at(i).size(); j++){ 
            int incoming_from = incoming.at(i).at(j);
            random_pick += pr[incoming_from]/number_outgoing[incoming_from];
        }
        double v_i = s*random_pick + s*inner_product/web_size + (1-s)/web_size;
        v[i] = v_i;
        v_sum += v_i;
    } 

    for(int i=0; i<web_size; i++){
        v[i] = v[i]/v_sum;
    }

    return(v);  
}

double* pagerank(double **web, double *pr){
    int iteration = 1;
    double change = 2;
    double* new_pr;

    while(change > convergence){
        cout << "Iteration " << iteration << " => ";
        new_pr = step(web, pr);
        
        change = 0;
        for(int i=0; i<web_size; i++){ change+= abs(new_pr[i] - pr[i]); }
        cout << "Change in L1 Norm: " << change << endl;

        pr = new_pr;
        iteration +=1;
    }

    return(new_pr);
}

int main(int argc, char *argv[]){
    string filename = argv[1];

    read_file(filename);
    // print_web();

    double* pr = new double[web_size];
    for(int i=0; i<web_size; i++){ pr[i] = ((double)1)/web_size; }
    double** web = new double*[web_size];
    for(int i=0; i<web_size; i++){ web[i] = new double[web_size]; }

    pr = pagerank(web, pr);

    cout << endl << "Page Ranks for our web are:" << endl;
    for(int i=0; i<web_size; i++){
        cout << i << " : " << pr[i] << endl;
    }

    return(0);
}