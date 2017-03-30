#include "stdafx.h"
#include "Genetic_Algorithm.h"
#include "Matrix.h"

using namespace std;

void GA::start()
{
	LARGE_INTEGER l_int;

	QueryPerformanceFrequency(&l_int);
	frequency = double(l_int.QuadPart);

	QueryPerformanceCounter(&l_int);
	counter = l_int.QuadPart;
}

double GA::stop()
{
	LARGE_INTEGER l_int;
	QueryPerformanceCounter(&l_int);
	return double(l_int.QuadPart - counter) / frequency;
}

void GA::permutation(vector<vector<int>>& routes, int size, int cities){
	int*perm = new int[cities];
	vector<int> temp;
	for (int i = 0; i < size; i++){
		for (int c = 0; c < cities; c++)
		{
			bool check;
			int random=0;
			do{
				random = rand() % cities; //losuje jedno miasto
				check = true;
				for (int j = 0; j < c; j++){
					if (random == perm[j]){
						check = false;
						break;
					}
				}

			} while (!check);
			perm[c] = random;
		
			temp.push_back(perm[c]);
		}
		routes.push_back(temp);
		temp.clear();
	}
	delete[]perm;
			}



double GA::route(double **&matrix, int cities, vector<int> perm){
	double cost = 0;

	for (int i = 0; i < cities - 1; i++)
	{
		cost += matrix[perm[i]][perm[i + 1]];
	}
	cost += matrix[perm[cities - 1]][perm[0]];//droga od ostatniego do pierwszego miasta

	return cost;
}

vector<vector<int>> GA::createPopulation( int size, int cities){

	std::vector<vector<int>> population;
	/*tworze populacje skladajaca sie z wielkosc losowych drog*/
	permutation(population, size, cities);
	return population;
}

void GA::merge(double **&matrix, int cities, vector<vector<int>> &population, vector<vector<int>>& hlp, int beg, int mid, int end){

	int i, j, q;
	for (i = beg; i <= end; i++)
		hlp[i] = population[i];  // Skopiowanie danych do tablicy pomocniczej
	i = beg; 
	j = mid + 1; 
	q = beg;                 // Ustawienie wskaŸników tablic
	while (i <= mid && j <= end) {         // Przenoszenie danych z sortowaniem ze zbiorów pomocniczych do tablicy g³ównej
		if (route(matrix, cities, hlp[i]) < route(matrix, cities, hlp[j]))
			population[q++] = hlp[i++];
		else
			population[q++] = hlp[j++];
	}
	while (i <= mid)
		population[q++] = hlp[i++]; // Przeniesienie nieskopiowanych danych ze zbioru pierwszego w przypadku, gdy drugi zbiór siê skoñczy³

}

void GA::mergesort(double **&matrix, int cities, vector<vector<int>> &population, vector<vector<int>>& hlp, int beg,  int end){
	int mid;
	if (beg<end) {
		mid = (beg + end) / 2;
		mergesort(matrix, cities, population, hlp, beg, mid);    // Dzielenie lewej czêœci
		mergesort(matrix, cities, population, hlp, mid + 1, end);   // Dzielenie prawej czêœci
		merge(matrix,cities, population, hlp, beg, mid, end);   // £¹czenie czêœci lewej i prawej
	}

}

double GA::evolve(double**& matrix, int size, int cities, vector<vector<int>>& population){
	vector<int> parent1(cities);
	vector<int> parent2(cities);
	vector<int> child1(cities);
	vector<int> child2(cities);


	for (int i = 0; i < size; i++){
		//krzyzuje osobniki i powstale w wyniku krzyzowki dzieci wpisuje na koncu populacji
		//prawdopodobienstwo krzyzowania-0,8
		if (((double)rand() / (RAND_MAX)) <= 0.8){
			parent1 = tournamentSelection(matrix, size, cities, population, 5);
			parent2 = tournamentSelection(matrix, size, cities, population, 5);
			child1 = crossover(cities, parent1, parent2);
			child2 = crossover(cities, parent2, parent1);
			
				//mutuje powstale osobniki, prawdopodobienstwo zajscia mutacji=0,1
			if (((double)rand() / (RAND_MAX)) <= 0.1){
				mutate(cities, child1);
				mutate(cities, child2);
			}
			population.push_back(child1);
			population.push_back(child2);
		}
	}

	
	vector<vector<int>> hlp(population.size());
	//sortuje trasy po ich wartosciach malejaco
	mergesort(matrix, cities, population,  hlp,  0,  population.size() - 1);

	//i usuwam najgorsze z nich, aby w efekcie populacja po krzyzowaniu i mutacji byla wielkosci poczatkowej
	do{
		population.pop_back();
	} while (population.size() > size);

	//po sortowaniu najlepszy osobnik znajduje siê na pierwszej pozycji
	double best_route = route(matrix, cities, population[0]);
    return best_route;
}

vector<int> GA::findBest(double **&matrix, int cities, int size, vector<vector<int>> routes){
	vector<int> best(cities);
	
	for (int i = 0; i < cities; i++){
		best[i]= routes[0][i];
	}
	for (int i = 0; i < size; i++){
			if (route(matrix, cities, best)> route(matrix, cities, routes[i])){
				best = routes[i];
			
				}		
	}
	return best;
}

vector<int> GA::tournamentSelection(double **&matrix, int size, int cities, vector<vector<int>> routes, int tournament_size){
	int rand_id;
	vector<int> way(cities);
	vector<vector<int>> routes1(tournament_size, std::vector<int>(cities));	

		for (int i = 0; i < tournament_size; i++){
			rand_id = rand() % size;
			for (int j = 0; j < cities; j++){
				routes1[i][j] = routes[rand_id][j];
			
			}
		}

		way = findBest(matrix, cities, tournament_size, routes1);

		return way;


}

vector<int> GA::crossover( int cities,vector<int> parent1, vector<int> parent2){
		int  *position = new int[cities];
		bool *city_id = new bool[cities];
		int start;
		int end;
		start = rand() % cities;
		end = rand() % cities;
		if (start > end) {
			int temp = start;
			start = end;
			end = temp;
		}

		for (int i = 0; i < cities; i++) {
			city_id[i] = false;
			if (i >= start && i <= end)
				position[i] = parent1[i];
			else
				position[i] = parent2[i];
		}

		vector<int>child(cities);
		for (int i = 0; i < cities; i++) {
			if (!city_id[position[i]]) {
				child[i] = position[i];
				city_id[position[i]] = true;
			}
			else {
				for (int j = 0; j < cities; j++) {
					int index = parent2[j];
					if (!city_id[index]) {
						child[i] = index;
						city_id[index] = true;
						break;
					}
				}
			}
		}

		return child;
		delete[]position;
		delete[]city_id;
	
}

void GA::mutate( int cities, vector<int> way){
	int temp;
	for (int i = 0; i < cities; i++){
         	int j = rand() % cities;
			temp = way[i];
			way[i] = way[j];
			way[j]=temp;
	}
	
}

