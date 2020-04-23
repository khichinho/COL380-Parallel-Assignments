#include "mpi.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;
#define THRESHOLD 0.001

int main(int argc, char **argv){
    MPI_Init(&argc,&argv);
    int rank, size ;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    string fileName = argv[1];
    std::ifstream infile(fileName);
    infile.ignore( std::numeric_limits<std::streamsize>::max() );
    int fileSize = infile.gcount();
    int blockSize = fileSize/size;
    int start, end , overlap = 100;
    start = rank * blockSize;
    end = start + blockSize -1 ;
    end += overlap;
    if (rank == size-1) end = fileSize;
    blockSize =  end - start + 1;
    char *chunk =(char*) malloc( (blockSize + 1)*sizeof(char));
    MPI_File in;
    int ierr = MPI_File_open(MPI_COMM_WORLD, fileName.c_str(), MPI_MODE_RDONLY, MPI_INFO_NULL, &in);
    MPI_File_read_at_all(in, start, chunk, blockSize, MPI_CHAR, MPI_STATUS_IGNORE);
    chunk[blockSize] = '\0'; 
    int locstart=0, locend=blockSize;
    if (rank != 0) {
        while(chunk[locstart] != '\n') locstart++;
        locstart++;
    }
    if (rank != size-1) {
        locend-=overlap;
        while(chunk[locend] != '\n') locend++;
    }
    blockSize = locend-locstart+1;
    char *data = (char *)malloc((blockSize+1)*sizeof(char));
    memcpy(data, &(chunk[locstart]), blockSize);
    data[blockSize] = '\0';
    free(chunk);
    int nlines = 0;
    for (int i=0; i<blockSize; i++)
        if (data[i] == '\n') (nlines)++;
    
    string lines(data);
    // vector<pair<int,int>> links;
    stringstream linkStream(lines);
    int from,to;
    // int linksCount = 0;
    map<int,vector<int>> links;
    ofstream logFile ;
    logFile.open("process" + to_string(rank) + ".log");
    int maxVal = -1 ;
    int startIndex[size];
    int endIndex[size];
    int totalLinks = 0;
    while(linkStream >> from >> to ){
        if (max(from,to) > maxVal) {
            maxVal = max(from,to);
        }
        if(links.find(from)==links.end()){
            links[from] = vector<int>();
        }
        links[from].push_back(to);
        totalLinks += 1;
    }
    int numNodes = maxVal + 1 ;
    MPI_Allreduce(MPI_IN_PLACE,&numNodes,1,MPI_INT,MPI_MAX,MPI_COMM_WORLD);
    MPI_Allreduce(MPI_IN_PLACE,&totalLinks,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);

    logFile <<"numNodes " << numNodes << endl;
    int numOutgoing[numNodes];
    int numIncoming[numNodes];
    // int numIncoming2[numNodes];
    for(int i = 0 ; i < numNodes ; i += 1){
        numOutgoing[i] = 0;
        numIncoming[i] =0 ;
    }
    for (auto const& x : links){
        numOutgoing[x.first] += x.second.size();
        for(auto y:x.second){
            numIncoming[y]++ ;
        }
    }
    // cout << numIncoming[0] << endl;
    MPI_Allreduce(MPI_IN_PLACE,numOutgoing,numNodes,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    if(rank == 0){
        MPI_Reduce(MPI_IN_PLACE,numIncoming,numNodes,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    }
    else{
        MPI_Reduce(numIncoming,numIncoming,numNodes,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    }
    // int total
    int nodesRemaining = totalLinks; // gotta change it to total links
    int procsRemaining = size;
    int done ;
    int doneArr[size];
    if(rank == 0){
        // int done =0;
        for(int i = 0 ; i < numNodes ; i += 1){
            if(procsRemaining == 1){
                startIndex[size-1]=i;
                endIndex[size-1] = numNodes;
                doneArr[size-procsRemaining] = nodesRemaining; 
                
                break;
            }
            done = 0 ;
            startIndex[size - procsRemaining] = i;
            while(done < nodesRemaining/procsRemaining ){
                done += numIncoming[i];
                i += 1;
            }
            endIndex[size - procsRemaining] = i + 1;
            doneArr[size-procsRemaining] = done; 
            nodesRemaining -= done;
            procsRemaining -= 1;
        }
        // cout << "totaincoming " << totalIncoming << endl;
    }
    int myStart, myEnd;
    MPI_Bcast(startIndex,size,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(endIndex,size,MPI_INT,0,MPI_COMM_WORLD);
    int numKVToSend[size];
    int numKVToReceive[size];

    // for(int i = 0 ; i < size ; i += 1){
    //     cout<< "rank = " << rank  << " start = " << startIndex[i] << " end " << endIndex[i] << endl; // << " done " << doneArr[i]<< endl;
    // }
    // int nodesInOne = numNodes/size;

    for(int i = 0 ; i < size ; i += 1){
        numKVToSend[i] = 0;
        numKVToReceive[i] = 0;
    }
    vector<int> myDangling;
    for(auto x:links){
        for(auto y:x.second){
            for(int i = 0 ; i < size ; i += 1){
                if(y >= startIndex[i] && y < endIndex[i]){
                    numKVToSend[i]++;
                    break;
                }
            }            
        }
    }
    // for(int i = startIndex[rank] ; i < endIndex[rank] ; i += 1){
    //     if(numOutgoing[i]==0){
    //         myDangling.push_back(i);
    //     }
    // }
    for(int i = rank*numNodes/size ; i < ((rank + 1)*numNodes)/size ; i += 1){
            
        // innerProduct += I[i];
        if(numOutgoing[i] == 0){
            myDangling.push_back(i);
        }
        if(i == numNodes-1){
            break;
        }
    }
    int myOwn = numKVToSend[rank];
    numKVToSend[rank] = 0;
    int toRecieve=0;
    int toSend = 0;
    MPI_Alltoall(numKVToSend,1,MPI_INT,numKVToReceive,1,MPI_INT,MPI_COMM_WORLD);

    for(int i = 0; i < size ; i+= 1){
        toRecieve+=numKVToReceive[i];
        toSend += numKVToSend[i];
    }
    // cout << "in process "<< rank << " toRecieve " << toRecieve << " tosend " << toSend << endl; // " total " << myOwn + toRecieve << endl;
    int sendKeys[toSend];
    int receiveKeys[toRecieve];
    float sendValues[toSend];
    float receiveValues[toSend];
    int sendDisps[size];
    int recvDisps[size];
    int myKeys[myOwn];
    float myValues[myOwn];
    sendDisps[0] = 0;
    recvDisps[0] = 0;
    for(int i = 1; i < size ; i += 1){
        sendDisps[i] = sendDisps[i-1] + numKVToSend[i-1];
        recvDisps[i] = recvDisps[i-1] + numKVToReceive[i-1];
    }

    float I[numNodes];
    float ICopy[numNodes];
    for(int i = 0 ; i < numNodes ; i += 1){
        I[i] = 1.f/((float) numNodes);
        ICopy[i] = 0.f;
    }
    int currProcIdx[size];
    for(int i = 0 ; i < size ; i += 1){
        currProcIdx[i] = 0;
    }
    for(auto x:links){
        for(auto y:x.second){
            for(int i = 0 ; i < size ; i += 1){
                if(y >= startIndex[i] && y < endIndex[i]){
                    if(i == rank){
                        myKeys[currProcIdx[i]] = y;
                        currProcIdx[i] += 1;
                    }
                    else{
                        sendKeys[sendDisps[i] + currProcIdx[i]] = y;
                        currProcIdx[i] += 1;
                    }
                    break;
                }
            }
        }
    }
    MPI_Alltoallv(sendKeys,numKVToSend,sendDisps,MPI_INT,receiveKeys,numKVToReceive,recvDisps,MPI_INT,MPI_COMM_WORLD);
    logFile << startIndex[rank] << " " << endIndex[rank] << endl;
    logFile << "after recv keys" << endl;
    logFile << "min " <<*min_element(receiveKeys,receiveKeys+toRecieve) << ", max " << *max_element(receiveKeys,receiveKeys+toRecieve) << endl; 
    // for(int i = 0 ; i < toRecieve; i ++){
    //     logFile << receiveKeys[i] << endl;
    // }

    int converged = 0 ;
    float innerProduct ;
    int currNumOutgoing;
    float myI[endIndex[rank] - startIndex[rank]];
    float s = 0.85;
    float diff;
    int t = 0;
    while (!converged){
        // if(rank == 0){
        //     float sum=0.0f;
        //     for(int i = 0 ; i < numNodes ; i ++ ){
        //         sum += I[i];
        //         // logFile << "sum " << sum << endl;
        //     }
        //     // logFile << numNodes << endl;
        //     // logFile << I[0] << endl;

        // }
        t += 1;
        innerProduct = 0.f;
        diff = 0.f;
        // for(int i = rank*numNodes/size ; i < (rank + 1)*numNodes/size ; i += 1){
            
        //     innerProduct += I[i];
        //     if(i == numNodes-1){
        //         break;
        //     }
        // }
        for(auto x:myDangling){
            innerProduct += I[x];
            // i += 1;
        }
        for(int i = 0 ; i < size ; i += 1){
           currProcIdx[i] = 0;
        }
        MPI_Allreduce(MPI_IN_PLACE,&innerProduct,1,MPI_FLOAT,MPI_SUM,MPI_COMM_WORLD);
        for(int i = startIndex[rank] ; i < endIndex[rank]   ; i += 1){
            ICopy[i] = (s*innerProduct + 1.f - s) / (float) numNodes;
        }
        // logFile << "see inner product " << innerProduct << endl;
        // logFile << "see fixed value " << ICopy[startIndex[rank]] << endl;
        for(auto x:links){
            currNumOutgoing = numOutgoing[x.first];
            for(auto y:x.second){
                for(int i = 0 ; i < size ; i += 1){
                    if(y >= startIndex[i] && y < endIndex[i]){
                        if(i == rank){
                            myValues[currProcIdx[i]] = I[x.first]/((float) currNumOutgoing);
                            currProcIdx[i]++;
                        }
                        else{
                            sendValues[sendDisps[i] + currProcIdx[i]] = I[x.first]/((float) currNumOutgoing);
                            currProcIdx[i]++;
                        }
                        break;
                    }
                }
            }
        }
        MPI_Alltoallv(sendValues,numKVToSend,sendDisps,MPI_FLOAT,receiveValues,numKVToReceive,recvDisps,MPI_FLOAT,MPI_COMM_WORLD);
        logFile << "++++++++" << endl;
        for(int i = 0 ; i < myOwn ; i += 1){
            logFile << myKeys[i] << " " <<myValues[i] << endl;
            ICopy[myKeys[i]] += s*myValues[i];
        }
        // logFile << "++++++++" << endl;
        for(int i = 0 ; i < toRecieve ; i += 1){
            logFile << receiveKeys[i] << " " << receiveValues[i] << endl;
            ICopy[receiveKeys[i]] += s*receiveValues[i];
        }
        logFile << "-------" << endl;
        for(int i = startIndex[rank] ; i < endIndex[rank] ; i += 1){
            logFile << i << " " << ICopy[i] << endl;
        }
        // logFile << "*******" << endl;
        for(int i = startIndex[rank] ; i < endIndex[rank] ; i += 1){
            diff += abs(ICopy[i] - I[i]);
        }
        MPI_Allreduce(MPI_IN_PLACE,&diff,1,MPI_FLOAT,MPI_SUM,MPI_COMM_WORLD);
        MPI_Allreduce(ICopy,I,numNodes,MPI_FLOAT,MPI_SUM,MPI_COMM_WORLD);
        // for(int i = startIndex[rank] ; i < endIndex[rank] ; i += 1){
        //     logFile << ICopy[i] << " " << I[i] << endl; 
        // }

        if(diff < THRESHOLD){
            converged = true;
        }
        if(rank == 0){
            cout << "diff = "<< diff << endl;
        }
        if(diff < THRESHOLD){
            converged = true;
        }
    }
    
    // if(rank == 0){
    //     for(int i = 0 ; i < numNodes ; i += 1){
    //         cout << I[i] << endl;
    //     }
    // }
    
    
    // for(auto x:links){
    //     for(auto y:x.second){
    //         if(y/nodesInOne < size){
    //             numKVToSend[y/nodesInOne]++;
    //         }
    //         else{
    //             numKVToSend[size-1]++;
    //         }
    //     }
    // }
    // int myOwn = numKVToSend[rank];
    // numKVToSend[rank]=0;
    // numKVToReceive[rank]=0;
    // int one[size];
    // for(int i = 0 ; i < size ; i += 1){
    //     one[i] = 1;
    // }
    // // MPI_Reduce_scatter(numKVToSend,&toRecieve,one,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    // MPI_Alltoall(numKVToSend,1,MPI_INT,numKVToReceive,1,MPI_INT,MPI_COMM_WORLD);

    // // for(int i = )
    MPI_Finalize();

}