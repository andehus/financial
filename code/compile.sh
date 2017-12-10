g++ -std=c++11 -larmadillo -o main main.cpp
# ./main file N MC Transactions lambda alpha
#./main clambda05_MC1 500 1 10000000 0.5 0.0
#./main clambda05_MC10 500 10 10000000 0.5 0.0
#./main clambda05_MC100 500 100 10000000 0.5 0.0
./main test 500 10 10000000 0.5 0.0
#python xyplot.py data.txt 0
#python3 histogram.py 1000transactions.txt 0
