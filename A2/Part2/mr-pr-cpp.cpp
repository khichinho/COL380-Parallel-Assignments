#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <boost/config.hpp>
#if defined(BOOST_MSVC)
#   pragma warning(disable: 4127)

// turn off checked iterators to avoid performance hit
#   if !defined(__SGI_STL_PORT)  &&  !defined(_DEBUG)
#       define _SECURE_SCL 0
#       define _HAS_ITERATOR_DEBUGGING 0
#   endif
#endif
#include "mapreduce.hpp"



using namespace std;

int web_size;
const double s = 0.85;
const double convergence = 0.00001;

vector<int> outgoing; // stores outgoing links from a page
double* pr;

namespace mr_pr {

    template<typename MapTask> class datasource : mapreduce::detail::noncopyable
    {
        public:
            datasource() : sequence_(0)
            {}
            
            bool const setup_key(typename MapTask::key_type &key)
            {
                key = sequence_ ++;
                return key < web_size;
            }
            bool const get_data(typename MapTask::key_type const &key, typename MapTask::value_type &value)
            {
                value = std::make_pair(outgoing[key], pr[key]);
                return true;
            }

        private:
            int sequence_;
    };

    
    struct map_task : public mapreduce::map_task<int, std::pair<int, double> >
    {
        template<typename Runtime>
        void operator()(Runtime &runtime, key_type const &key, value_type const &value) const
        {
            // std::cout << "MapTask Running \n";
            if(key != 0){
                runtime.emit_intermediate(value.first, value.second);
            }
        }
    };
    

    struct reduce_task : public mapreduce::reduce_task<int, double>
    {
        template<typename Runtime, typename It>
        void operator()(Runtime &runtime, key_type const &key, It it, It ite) const
        {

            value_type result_pr = 0;
            for (; it!=ite; ++it){
                result_pr += *it;
            }
            runtime.emit(key, result_pr);

            // std::cout << "Reduce fn \n";
        }
    };


    typedef mapreduce::job<mr_pr::map_task,
                mr_pr::reduce_task,
                mapreduce::null_combiner,
                mr_pr::datasource<mr_pr::map_task>
    >job;

}


void read_file(const string &filename) {
    istream *infile;
    infile = new ifstream(filename);

    outgoing.push_back(0); // to account for no outgoing links from webpage 0
    int count = 1;

    string line;
    while(getline(*infile, line)){
        int from, to;
        istringstream ss(line);
        ss >> from;
        ss >> to;

        outgoing.push_back(to);
        count += 1;
    }
    
    web_size = count;
}


double* step_mapreduce(double *pr){
    double* new_pr = new double[web_size];
    for(int i=0; i<web_size; i++){ new_pr[i] = 0;}

    double inner_product = s*pr[0]/web_size + (1-s)/web_size;
    
    /*
    * handling dangling pages
    */

    mapreduce::specification spec;
    mr_pr::job::datasource_type datasource;

    mr_pr::job job(datasource, spec);
    mapreduce::results result;

    #ifdef _DEBUG
        job.run<mapreduce::schedule_policy::sequential<mr_pr::job> >(results);
    #else
        job.run<mapreduce::schedule_policy::cpu_parallel<mr_pr::job> >(result);
    #endif
        for(auto i = job.begin_results(); i!=job.end_results(); ++i)
        {
            new_pr[i->first] += s*(i->second);
        }

    double new_pr_sum = 0;
    for(int i=0; i<web_size; i++){
        new_pr[i] += inner_product;
        new_pr_sum += new_pr[i];
    }

    for(int i=0; i<web_size; i++){
        new_pr[i] = new_pr[i]/new_pr_sum;
    }

    return(new_pr);

}

void pagerank_mapreduce(){
    int iteration = 1;
    double change = 1;
    double* new_pr;

    while(change > convergence){
        // if(iteration == 2){ break; }

        cout << "Iteration " << iteration << ": ";
        new_pr = step_mapreduce(pr);

        change = 0;
        for(int i=0; i<web_size; i++){ 
            change+= abs(new_pr[i] - pr[i]);
            pr[i] = new_pr[i];
        }
        cout << "Change in L1 Norm: " << change << endl;
        
        iteration += 1;
    }
}

int main(int argc, char *argv[]) {
    string filename = argv[1];
    read_file(filename);

    pr = new double[web_size];
    for(int i=0; i<web_size; i++){ pr[i] = ((double)1)/web_size; }

    pagerank_mapreduce();
    cout << endl << "Saving Pageranks to Output file." << endl;
    
    ofstream myfile;
    myfile.open(filename.substr(0, filename.length()-4) + "-pr-cpp.txt");
    
    for(int i=0; i<web_size; i++){
        myfile << i << " : " << pr[i] << endl;
    }

    myfile.close();
    
    return(0);
}