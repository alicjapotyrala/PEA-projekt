#include"stdafx.h"
using namespace std;

class SA {

public:
	__int64 counter;
	double frequency; 
	const double e = 2.718281828459;
	double min_temperature = 0.001;
	double solution = 0;
	double stop();
	void start();
	double route(double **matrix, int cities,int *perm);
	void permutation(int *&perm,int cities);
	bool probability(int length1, int length2, double temperature);
	double sa(double **matrix1, int cities, double constant, int & solution);


};