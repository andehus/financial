/* Version 0.5 stasonær */
/*TODO
  make a matrix c_ij which contains the transaction count (in pairs)
  should be a symmetric matrix with zero on diagonals*/
#include <stdlib.h>
#include <iostream>
#include <random>
#include <fstream>
#include <armadillo>
#include <string>
using namespace std;
using namespace arma;
void print(const char* file,vector<double> &histogram,int n) {
  ofstream ofile;
  ofile.open(file);//,ios::out | ios::binary);
  for(int i = 0; i<histogram.size(); i++) {
    ofile<<histogram[i]/n<<endl;
    //ofile<<agent[i]<<" "<<(double)agent[i]/n<<" "<<(int)agent[i]/100<<endl; // del på mc cycles
  }
  ofile.close();
  cout<<"OUTPUT: "<<file<<endl;
}
/* cycles = 10^3, transactions = 10^7 */
/* outfile N MC Transacions lambda alpha */
int main(int argc,char** argv) {
  int N,CYCLES,TRANSACTIONS;
  double lambda,alpha;
  clock_t start,stop;
  double m0 = 1000;
  double p = 0.0;
  char* outfile;
  /* reading arguments */
  if(argc == 7) {
    outfile = argv[1];
    N = atoi(argv[2]);
    CYCLES = atoi(argv[3]);
    TRANSACTIONS = atoi(argv[4]);
    lambda = atof(argv[5]);
    alpha = atof(argv[6]);
    //cout<<argv[0]<<argv[1]<<argv[2]<<argv[3]<<argv[4]<<argv[5]<<argv[6]<<endl;
  }
  else{
    cout<<"Usage: ./main outfile N MC Transaction lambda alpha"<<endl;
  }

  /* arrays */
  vector<double> histogram(51,0.0);
  mat c(N,N,fill::zeros);
  
  /* random number generator */
  random_device rd;
  mt19937_64 gen(rd());
  uniform_real_distribution<double>RandomNumberGenerator(0.0,1.0);

  /* progression in console */
  int percent = 0;
  int total_percent = 0;
  cout<<"start"<<endl;
  (CYCLES>=100) ? total_percent=CYCLES/100 : total_percent=CYCLES/1;//should divide by 100
  cout<<total_percent<<endl;
  start = clock();

  /*** loop mc ***/
  for(int t = 0; t<CYCLES; t++) {
    vector<double> agent(N,m0);
    if(t%total_percent == 0) {
      cout<<" [Processing "<<percent++<<"%]\r";
      cout.flush();
    }
    /*** loop transactions ***/
    for(int e = 0; e<TRANSACTIONS; e++) {
      int i =  (int)(RandomNumberGenerator(gen)*N);
      int j =  (int)(RandomNumberGenerator(gen)*N);
      double epsilon = RandomNumberGenerator(gen);
      /*if(agent[i] - agent[j] == 0){p = 1.0;}
      else{p = 2*pow(fabs((agent[i]-agent[j])/double(m0)),-alpha);}*/
      if(i!=j){
	double temp = agent[i]; //remember to use temp if not mortens way
	agent[i] = lambda*agent[i]+(1-lambda)*epsilon*(agent[i]+agent[j]); //saving for lambda>0
	agent[j] = lambda*agent[j]+(1-lambda)*(1-epsilon)*(temp+agent[j]); //saving for lambda>0
      }
    }//end transactions
    for(int i = 0; i<agent.size(); i++) {
      int index = (int)agent[i]/100;
      cout<<"Agent: "<<agent[i]<<" "<<index<<endl;
      if(index<50)
	histogram[index]++;//= agent[i];
      else
	histogram[50]++;//= agent[i]; //overflow
    }

    /* insert into histogram */
    //for(int i = 0; i<agent.size(); i++) {
      
    //}
    //    agent.clear();
    
  }//end monte carlo
  print(outfile,histogram,CYCLES);
  stop = clock();
  cout<<endl;
  cout<<"Time: "<< ((double)(stop-start)/CLOCKS_PER_SEC)<<endl;
}















  
  /* timing for non-paralized code 
  clock_t start,stop;
  start = clock();

  /* number of mc cycles 
  int n_cycles = 2;//10^4
  int N = 500; //agents
  int ntrans = 1;//10000000;//10^7
  double m0 = 1000; //init money
  vector<int> transaction_count(N);
  vector<double> avg_money(N);
  //vector<double> agent(N);
  vector<double> expectation_value(N);
  /* lambda 
  double lambda = 0.0;//0.0 0.25 0.5 0.9;
  double alpha = 0.5;
  /* probability 
  double p = 0.0;
  /* matrix init  
  mat a(N,N,fill::zeros);
  
  /* money vector
  for(int i  = 0; i<=N; i++) {
    //agent[i] = m0;
    transaction_count[i] = 0;
    avg_money[i] = 0.0;
  }
















  /*
  /* random number generator 
  random_device rd;
  mt19937_64 gen(rd());
  uniform_real_distribution<double>RandomNumberGenerator(0.0,1.0);
  /* progression in console 
  int percent = 0;
  int total_percent = n_cycles/1;//should divide by 100


  /* loop mc cycles 
  for(int k = 0; k<n_cycles; k++) {
    vector<double> agent(N);
    for(int i = 0; i<N; i++) {
      agent[i] = m0;
    }
    if(k%total_percent == 0) {
	  cout<<" [Processing "<<percent++<<"%]\r";
      cout.flush();
    }    
    for(int t = 0; t<ntrans; t++){ 
      int i =  (int)(RandomNumberGenerator(gen)*N);
      int j =  (int)(RandomNumberGenerator(gen)*N);
      double epsilon = RandomNumberGenerator(gen);
      if(agent[i] - agent[j] == 0){
	p = 1.0;
      }
      else {
	p = 2*pow(fabs((agent[i]-agent[j])/double(m0)),-alpha);
      }
      if(i!=j){
	double temp = agent[i]; //remember to use temp if not mortens way
	//agent[i] = epsilon*(agent[i]+agent[j]); //no saving
	//agent[j] = (1-epsilon)*(temp+agent[j]); //no saving
	agent[i] = lambda*agent[i]+(1-lambda)*epsilon*(agent[i]+agent[j]); //saving
	agent[j] = lambda*agent[j]+(1-lambda)*(1-epsilon)*(temp+agent[j]); //saving
      }
    }//end transactions
    for(int i = 0; i<N; i++) {
      expectation_value[i] += agent[i];
    }
  }//end mc
  cout<<endl;
  /*double sum = 0.0;
  for(int i = 0; i<agent.size(); i++)
    sum += agent[i];
  double avg_m = sum/agent.size();
  cout<<"Average money: "<<avg_m<<endl;
  print(agent,n_cycles);
  *//*
  print(expectation_value,n_cycles);
  stop = clock();
  cout<<"Time: "<<((double)(stop-start)/CLOCKS_PER_SEC)<<endl;
  return 0;
}
*/
