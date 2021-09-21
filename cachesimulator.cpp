/*
Cache Simulator
We read the L1 and L2 cache params (set per cache, line per set, and block size) from the config file.
s = log2(#sets)   b = log2(block size)  t=32-s-b (tag bits)
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>
#include <bits/stdc++.h>

using namespace std;
//MACROS for access state:
#define NA 0 // no action
#define RH 1 // read hit
#define RM 2 // read miss
#define WH 3 // Write hit
#define WM 4 // write miss




struct configurationStruct{
       int L1bs;
       int L1Ss;
       int L1size;
       int L2bs;
       int L2Ss;
       int L2size;
       };

class cacheObj{
public: int s,b,t;
        
    cacheObj( int bs ,int ss ,int size){
        
        if (ss == 0){
            cout<<"fully associative\n"<<endl;        
            b = log2(bs);
            cout<<"offset  "<<b<<endl;
            s = log2(size) + 10 - log2(bs);    
            cout<<"index  "<<s<<endl;
            t = 32-s-b;
            cout<<"Tag  "<<t<<endl<<endl;
        }
        else if (ss == 1){
            
            cout<<"Direct mapped\n"<<endl;
            b = log2(bs);
            cout<<"offset  "<<b<<endl;
            s = log2(size) + 10 - log2(bs);    
            cout<<"index  "<<s<<endl;
            t = 32-s-b;
            cout<<"Tag  "<<t<<endl<<endl;
           
        }
        else if (log2(ss)==floor(log2(ss))){
            cout<<"Set Associativity\n"<<endl;
            b = log2(bs);
            cout<<"offset  "<<b<<endl;
            s = log2(size) + 10 - log2(bs);    
            cout<<"index  "<<s<<endl;
            t = 32-s-b;
            cout<<"Tag  "<<t<<endl<<endl;
        }
    }
};

int main(int argc, char* argv[]){


    
    configurationStruct confCache;
    ifstream cParams;
    string dummyLine;
    cout<<"argv0 "<<argv[0]<<endl;
    cout<<"argv1 "<<argv[1]<<endl;
    cout<<"argv2 "<<argv[2]<<endl;
    cout<<"\n";
    cParams.open(argv[1]);
    
    while(!cParams.eof()){
      cParams>>dummyLine;
      cParams>>confCache.L1bs;
      cParams>>confCache.L1Ss;              
      cParams>>confCache.L1size;
      cParams>>dummyLine;              
      cParams>>confCache.L2bs;           
      cParams>>confCache.L2Ss;        
      cParams>>confCache.L2size;
    }

    cacheObj Cache_L1( confCache.L1bs , confCache.L1Ss , confCache.L1size);
    cacheObj Cache_L2( confCache.L2bs , confCache.L2Ss , confCache.L2size);
    int pL1 = pow(2, Cache_L1.s);
    int qL2 = pow(2,Cache_L2.s);
    int tag_arrayL1[pL1 - 1][confCache.L1Ss];
    int tag_arrayL2[qL2-1][confCache.L2Ss];
    
    int l1_validity_arr[pL1 - 1][confCache.L1Ss];
    int l2_validity_arr[qL2-1][confCache.L2Ss];
   
    int L1_AS =0; 
    int L2_AS =0; 
   
    ifstream trace_ip;
    ofstream trace_op;
    string outname;
    outname = string(argv[2]) + ".out";
    
    trace_ip.open(argv[2]);
    trace_op.open(outname.c_str());

    string access_Type;  // the Read/Write access type, stored in the trace file
    string haddr;       // the hexd address from the memory trace
    string line;
    unsigned int addr;  // the integer value of the address from the memory trace
    bitset<32> accessaddr; // the address from the memory trace store in the bitset
    
    if (trace_ip.is_open()&&trace_op.is_open()){    
        while (getline (trace_ip,line)){
            
            istringstream iss(line); 
            if (!(iss >> access_Type >> haddr)) {
              break;
            }
            stringstream saddr(haddr);
            saddr >> std::hex >> addr;
            accessaddr = bitset<32> (addr);
                                                
            int countL1 = 0 ;
            int countL2 = 0;
            int ta1 = Cache_L1.t;
            int ind1 = Cache_L1.s;
            int off1 = Cache_L1.b; 
            cout<<"address in hex"<< haddr <<"\n"<<endl;
            cout<<"access address in hex"<< accessaddr <<"\n"<<endl;
            
            int tagL1 = (bitset<32>((accessaddr.to_string().substr(0,ta1))).to_ulong());
            cout<<tagL1<<"tag in main for L1"<<endl;
            
            int indexL1 = (bitset<32>((accessaddr.to_string().substr(ta1,ind1))).to_ulong());
            cout<<indexL1<<"index in main for L1"<<endl;
            
            int offL1 =(bitset<32>((accessaddr.to_string().substr(ta1+ind1,off1))).to_ulong());
            cout<<offL1<<"offset in main for L1"<<endl;
            
            
            int ta2 = Cache_L2.t;
            int ind2 = Cache_L2.s;
            int off2=Cache_L2.b; 
            
            cout<<"address in hex"<< haddr <<"\n"<<endl;
            cout<<"access address in hex"<< accessaddr <<"\n"<<endl;
            
            int tagL2 =  (bitset<32>((accessaddr.to_string().substr(0,ta2))).to_ulong());
            cout<<tagL2<<"tag in main for L2"<<endl;
            
            int indexL2 (bitset<32>((accessaddr.to_string().substr(ta2,ind2))).to_ulong());
            cout<<indexL2<<"index in main for L2"<<endl;
            
            int offL2 (bitset<32>((accessaddr.to_string().substr(ta2+ind2,off2))).to_ulong());
            cout<<offL2<<"offset in main for L2"<<endl;
        
            if (access_Type.compare("R")==0){
                 // read access to the L1 Cache, 
                 //  and then L2 (if required), 
                 //  update the L1 and L2 access state variable;
                 
                 for(int i3 = 0 ; i3<confCache.L1Ss;i3++){
                     if((tag_arrayL1[indexL1][i3] == tagL1) && (l1_validity_arr[indexL1][i3] == 1)){
                         L2_AS =0;
                         L1_AS =1;
                         cout<<" Read HIT\n";
                         break;
                     }
                 }
                  if(L1_AS == 0){
                      L1_AS = 2;
                      for(int i3 = 0;i3<confCache.L2Ss ; i3++){
                          if((tag_arrayL2[indexL2][i3]==tagL2) && (l2_validity_arr[indexL2][i3]==1)){
                              L2_AS = 1;
                               for(int i3 =0; i3 < confCache.L1Ss;i3++){
                                  if(l1_validity_arr[indexL1][i3] == 0){
                                        tag_arrayL1[indexL1][i3] = tagL1;
                                        l1_validity_arr[indexL1][i3] = 1;
                                        break;
                                  }
                                  else{
                                        i3 = countL1;
                                        tag_arrayL1[indexL1][i3] = tagL1;
                                        countL1 =countL1 + 1;              
                                        
                                        if(countL1 == confCache.L1Ss){
                                          countL1=0;
                                        }
                                  }
                               }
                               break;
                          }
                      }
                      if(L2_AS == 0){
                          L2_AS = 2;
                          for(int i3 = 0;i3 < confCache.L2Ss;i3++){
                            
                                if(l2_validity_arr[indexL2][i3]==0){
                                    tag_arrayL2[indexL2][i3] = tagL2;
                                    l2_validity_arr[indexL2][i3] = 1;
                                    break;
                                }
                                else{
                                    i3 = countL2;
                                    tag_arrayL2[indexL2][i3] = tagL2;
                                    countL2 = countL2 + 1;
                                    if(countL2 == confCache.L2Ss){
                                        countL2 = 0;
                                    }
                                }
                          }
                          for(int i3 = 0 ; i3 < confCache.L1Ss ; i3 ++){
                              if(l1_validity_arr[indexL1][i3] == 0){
                                    tag_arrayL1[indexL1][i3] = tagL1;
                                    l1_validity_arr[indexL1][i3] =1;
                                    break;              
                              }
                              else{
                                    i3 = countL1;
                                    tag_arrayL1[indexL1][i3] = tagL1;
                                    countL1 += 1;
                                    if(countL1== confCache.L1Ss){
                                        countL1 = 0;
                                    }
                              }
                          }
                      }
                                   
                  }
            }
                            
            else{    
                 for(int i3 = 0 ; i3 < confCache.L1Ss ; i3 ++){
                     if((tag_arrayL1[indexL1][i3] == tagL1) && (l1_validity_arr[indexL1][i3]==1)){
                         L1_AS = 3;
                         L2_AS = 0;
                         cout<<"";
                         break;
                     }
                 }
                 
                 if(L1_AS == 0){
                     L1_AS = 4;
                     for(int i3 =0 ; i3 < confCache.L2Ss; i3++){
                         if((tag_arrayL2[indexL2][i3] == tagL2)&& (l2_validity_arr[indexL2][i3]==1)){
                             L2_AS = 3;
                             break;
                         }
                     
                     }
                     if(L2_AS==0){
                        L2_AS = 4;
                        
                     }
                 }
            
            }
            trace_op<< L1_AS << " " << L2_AS << endl;  // Output hit/miss results for L1 and L2 to the output file;
            L1_AS = 0;
            L2_AS = 0;
             
        }
        trace_ip.close();
        trace_op.close(); 
    }
    else cout<< "Unable to open trace or traceout file ";
    return 0;
}
