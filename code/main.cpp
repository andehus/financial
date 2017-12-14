/* Version 0.5 stasonær (this version doesn't have variance and equilibrium tests) */
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
  ofile.open(file);
  for(int i = 0; i<histogram.size(); i++) {
    ofile<<histogram[i]/n<<endl;
  }
  ofile.close();
  cout<<"OUTPUT: "<<file<<endl;
}
/* cycles = 10^3, transactions = 10^7 */
/* outfile N MC Transacions lambda alpha */
int main(int argc,char** argv) {
  int N,CYCLES,TRANSACTIONS;
  double lambda,alpha,gamma;
  clock_t start,stop;
  double m0 = 1000;
  double p = 0.0;
  char* outfile;
  /* reading arguments */
  if(argc == 8) {
    outfile = argv[1];
    N = atoi(argv[2]);
    CYCLES = atoi(argv[3]);
    TRANSACTIONS = atoi(argv[4]);
    lambda = atof(argv[5]);
    alpha = atof(argv[6]);
    gamma = atof(argv[7]);
    //cout<<argv[0]<<argv[1]<<argv[2]<<argv[3]<<argv[4]<<argv[5]<<argv[6]<<endl;
  }
  else{
    cout<<"Usage: ./main outfile N MC Transaction lambda alpha gamma"<<endl;
  }

  /* arrays */
  vector<double> histogram(1001,0.0);
    
  /* random number generator */
  random_device rd;
  mt19937_64 gen(rd());
  uniform_real_distribution<double>RandomNumberGenerator(0.0,1.0);

  /* show progression in console */
  int percent = 0;
  int total_percent = 0;
  cout<<"start"<<endl;
  (CYCLES>=100) ? total_percent=CYCLES/100 : total_percent=CYCLES/1;
  cout<<total_percent<<endl;
  start = clock();

  /*** loop mc ***/
  for(int t = 0; t<CYCLES; t++) {
    vector<double> agent(N,m0);
    if(t%total_percent == 0) {
      cout<<" [Processing "<<percent++<<"%]\r";
      cout.flush();
    }
    double p = 0.0;
    mat C(N,N,fill::zeros);
    double cmax = 0.0;
    /*** loop transactions ***/
    for(int e = 0; e<TRANSACTIONS; e++) {
      int i =  (int)(RandomNumberGenerator(gen)*N);
      int j =  (int)(RandomNumberGenerator(gen)*N);
      double epsilon = RandomNumberGenerator(gen);
      if(agent[i] - agent[j] == 0){p = 1.0;}
      else{p = 2*pow(fabs((agent[i]-agent[j])/double(m0)),-alpha)*(pow((C(i,j)+1)/(cmax+1),gamma));}
      if(RandomNumberGenerator(gen) <p && i!=j){
	double temp = agent[i]; //remember to use temp if not mortens way
	agent[i] = lambda*agent[i]+(1-lambda)*epsilon*(agent[i]+agent[j]); //saving for lambda>0
	agent[j] = lambda*agent[j]+(1-lambda)*(1-epsilon)*(temp+agent[j]); //saving for lambda>0
	C(i,j)++;
	C(j,i)++;
	double currentInteraction = C(i,j);
	if(currentInteraction>cmax){cmax = currentInteraction;}
      }
    }//end transactions
    for(int i = 0; i<agent.size(); i++) {
      int index = (int)agent[i]/100;
      if(index<1000)
	histogram[index]++;//= agent[i];
      else
	histogram[1000]++;//= agent[i]; //overflow
    }
  }//end monte carlo
  print(outfile,histogram,CYCLES);
  stop = clock();
  cout<<endl;
  cout<<"Time: "<< ((double)(stop-start)/CLOCKS_PER_SEC)<<endl;
}
















