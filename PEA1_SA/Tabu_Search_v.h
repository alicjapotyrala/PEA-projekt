#include "stdafx.h"
#include "Matrix.h"


struct tList{
	int city1;
	int city2;
	int nCadence;

	tList(int x, int y, int cad);

};
class TS{
public:
	__int64 counter;
	double frequency;
	
	std::vector <tList> tabu;
	void start();
	double stop();
	void swap1(int *way, int a, int b);
	int* bestNeighbourhood(int *way, double **matrix, int cities, float asp);
	int* diversification(int cities, double **matrix);
	double route(double **matrix, int cities, int *perm);
	void permutation(int *&perm, int cities);
	void assign(int *arr1, int *arr2, int cities);
	void addToTabuList(int a, int b, int cities);
	bool isPresentOnTabuList(int a, int b);
	void decreaseCadence();
	void resetTabu();
	double ts(time_t runtime, double ** matrix1, int cities,int & solution);

};