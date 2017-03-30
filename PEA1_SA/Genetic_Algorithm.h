#include "stdafx.h"
#include "Matrix.h"
using namespace std;
class GA{
public:
	__int64 counter;
	double frequency;
	void start();
	double stop();
	double evolve(double**& matrix, int size, int cities, vector<vector<int>>& population);
	vector<vector<int>> createPopulation(int size, int cities);
	void merge(double **&matrix, int cities, vector<vector<int>> &population, vector<vector<int>>& hlp, int beg, int mid, int end);
	void mergesort(double **&matrix, int cities, vector<vector<int>> &population, vector<vector<int>>& hlp, int beg, int end);
	std::vector<int> findBest(double **&matrix, int cities, int size, vector<vector<int>> routes);
	void permutation(vector<vector<int>>& routes, int size, int cities);
	std::vector<int>tournamentSelection(double **&matrix, int size, int cities, vector<vector<int>> routes, int tournament_size);
	double route(double **&matrix, int cities, vector<int> perm);
	std::vector<int> crossover(int cities, vector<int> parent1, vector<int> parent2);
	void mutate(int cities, vector<int> way);

};