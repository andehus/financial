/* Version 0.3 */
/*TODO
  make a matrix c_ij which contains the transaction count (in pairs)
  should be a symmetric matrix with zero on diagonals*/
#include <stdlib.h>
#include <iostream>
#include <random>
#include <fstream>
#include <armadillo>

using namespace std;
using namespace arma;
void print(vector<double> &agent,int n) {
  ofstream ofile;
  ofile.open("104transactions_with_savings.txt");//,ios::out | ios::binary);
  for(int i = 0; i<agent.size(); i++) { 
    ofile<<agent[i]<<" "<<agent[i]/n<<endl; // del på mc cycles
  }
  ofile.close();
}
int main(int argc,char**argv) {

  /* timing for non-paralized code */
  clock_t start,stop;
  start = clock();

  /* number of mc cycles */
  int n_cycles = 10000;//10^4
  int N = 500; //agents
  int ntrans = 10000;//10^7
  double m0 = 1000; //init money
  vector<int> transaction_count(N);
  vector<double> avg_money(N);
  vector<double> agent(N);
  /* lambda */
  double lambda = 0.25;

  /* matrix init */ 
  mat a(N,N,fill::zeros);
  
  /* money vector */
  for(int i  = 0; i<=N; i++) {
    agent[i] = m0;
    transaction_count[i] = 0;
    avg_money[i] = 0.0;
  }
  /* random number generator */
  random_device rd;
  mt19937_64 gen(rd());
  uniform_real_distribution<double>RandomNumberGenerator(0.0,1.0);

  /* progression in console */
  int percent = 0;
  int total_percent = n_cycles/100;

  /* loop mc cycles */
  for(int k = 0; k<=n_cycles; k++) {
        if(k%total_percent == 0) {
	  cout<<" [Processing "<<percent++<<"%]\r";
      cout.flush();
    }    
    for(int t = 0; t<=ntrans; t++){ 
      int i =  (int)(RandomNumberGenerator(gen)*N);
      int j =  (int)(RandomNumberGenerator(gen)*N);
      double epsilon = RandomNumberGenerator(gen);
      
      if(i!=j){
	double temp = agent[i]; //remember to use temp if not mortens way
	//agent[i] = epsilon*(agent[i]+agent[j]); //no saving
	//agent[j] = (1-epsilon)*(temp+agent[j]); //no saving
	agent[i] = lambda*agent[i]+(1-lambda)*epsilon*(agent[i]+agent[j]);
	agent[j] = lambda*agent[j]+(1-lambda)*(1-epsilon)*(temp+agent[j]);
	
	//transaction_count[i] += transaction_count[i];
	//transaction_count[j] += transaction_count[j];
      }

    }//end transactions
    //average_money += current_money;
    
  }//end mc
  cout<<endl;
  double sum = 0.0;
  for(int i = 0; i<agent.size(); i++)
    sum += agent[i];
  double avg_m = sum/agent.size();
  cout<<"Average money: "<<avg_m<<endl;
  print(agent,n_cycles);
  stop = clock();
  cout<<"Time: "<<((double)(stop-start)/CLOCKS_PER_SEC)<<endl;
  return 0;
}
