#include "stdafx.h"
#include "Tabu_Search_v.h"
#include "Matrix.h"


void TS::start()
{
	LARGE_INTEGER l_int;

	QueryPerformanceFrequency(&l_int);
	frequency = double(l_int.QuadPart);

	QueryPerformanceCounter(&l_int);
	counter = l_int.QuadPart;
}

double TS::stop()
{
	LARGE_INTEGER l_int;
	QueryPerformanceCounter(&l_int);
	return double(l_int.QuadPart - counter) / frequency;
}

tList::tList(int x, int y, int cad){
	city1= x;
	city2 = y;
	nCadence = cad;
}

double TS::route(double **matrix, int cities, int *perm){
	double cost = 0;

	for (int i = 0; i < cities - 1; i++)
	{

		cost += matrix[perm[i]][perm[i + 1]];

	}

	cost += matrix[perm[cities - 1]][perm[0]];//droga od ostatniego do pierwszego miasta

	return cost;
}



void TS::permutation(int *& perm, int cities){

	for (int c = 0; c <cities; c++)
	{
		bool check;
		int random;
		do{
			random = rand() % cities; //losuje jedno miasto
			check = true;
			for (int j = 0; j <c; j++){
				if (random == perm[j]){
					check = false;
					break;
				}
			}

		} while (!check);
		perm[c] = random;
	}

}

//dywersyfikacja- tworze N (gdzie N-liczba miast) nowych tras i wybieram najlepsza z nich
int* TS::diversification( int cities, double **matrix){
	double current, best = DBL_MAX;
	int *best_route = new int[cities];
	int *current_route = new int[cities];
		for (int i= 0; i < cities; i++){
			int *permutation1 = new int[cities];
		
			permutation(permutation1, cities);
			current=route(matrix, cities, permutation1);
			assign(current_route, permutation1, cities);

			if (current <= best){
				best = current;
				assign(best_route, current_route, cities);
			}
			delete[]permutation1;
		}
	
	return best_route;
	delete[]best_route;
	delete[]current_route;
}

void TS::assign(int *arr1, int *arr2, int cities){
	for (int i = 0; i < cities; i++){
		arr1[i] = arr2[i];
	}
}

void TS::swap1(int *route, int a, int b) {
	int temp = route[a];
	route[a] = route[b];
	route[b] = temp;
}

void TS::addToTabuList(int a, int b, int cities)
{
	tabu.push_back(tList(a, b, 10));
	
}
bool TS::isPresentOnTabuList(int a, int b){
	for (int i = 0; i < tabu.size(); i++) {
		if ((tabu.at(i).city1 == a && tabu.at(i).city2 == b || tabu.at(i).city2 == a && tabu.at(i).city1 == b) && tabu.at(i).nCadence > 0) {
			return true;
		}
	}
	return false;
}
void TS::decreaseCadence(){
	for (int i = 0; i < tabu.size(); i++) {
		tabu.at(i).nCadence--;
		if (tabu.at(i).nCadence == 0) {
			std::swap(tabu.at(i), tabu.at(tabu.size() - 1));
			tabu.pop_back();
		}
	}
}

void TS::resetTabu(){
	for (int i = 0; i < tabu.size(); i++){
		tabu.push_back(tList(NULL, NULL, NULL));
		tabu.at(i).nCadence = 0;
	}
}

int * TS::bestNeighbourhood(int *way, double **matrix, int cities, float asp){
	double current_route, best_route;
	float improvement;
	int *best = new int[cities];

	assign(best, way, cities);

	for (int i = 0; i < cities; i++)
	{
		for (int j = 0; j < cities; j++){

			swap1(way, i, j);//wyznaczenie sasiedztwa-zamiana 2 miast w trasie
			decreaseCadence();

			current_route = route(matrix, cities, way);
			best_route = route(matrix, cities, best);

			//jezeli przejscie miedzy miastami i,j znajduje sie na liscie tabu
			if (isPresentOnTabuList(i, j) != false) {

				if (current_route < best_route) {
					//kryterium aspiracji- nowa najlepsza trasa przynajmniej 40% lepsza od obecnie branej jako najlepsza
					improvement = (best_route - current_route) / best_route;

					if (improvement >= asp) {
						resetTabu();

						delete[] best;
						best = new int[cities];

						assign(best, way, cities);
							
						addToTabuList(i, j, cities);
						//dodajPrzejscieDoTabu(j, i, miasta);

						//wielkosc listy-3*N
						if (tabu.size() > 3*cities){//jezeli lista wieksza od 3*N-usuwam z niej najdawniej dodane przejscie
							tabu.pop_back();
						}
						
					}
				}
				swap1(way, i, j);
				break;
			}
			//jezeli przejscie miedzy miastami i,j nie znajduje sie na liscie tabu
			else if (current_route < best_route) {

				delete[] best;
				best = new int[cities];

				assign(best, way, cities);
				addToTabuList(i, j, cities);
				//dodajPrzejscieDoTabu(j, i, miasta);
				//wielkosc listy-3*N
				if (tabu.size() > 3*cities){//jezeli lista wieksza od 3*N-usuwam z niej najdawniej dodane przejscie
					tabu.pop_back();
				}
			}
			swap1(way, i, j);
			
		}
	}
	return best;
	delete[] best;

}

double TS::ts(time_t runtime, double ** matrix1, int cities, int & solution)
{
	start();
	int *best_route = new int[cities];
	int *permutation1 = new int[cities];
	int bylo = 0, licznik = 0;
	
	permutation(permutation1, cities);

	assign(best_route, permutation1, cities);
	int *current_route = new int[cities];
	assign(current_route, best_route, cities);

	time_t finish;
	time_t start1 = time(NULL);

	finish = start1 + runtime;
	std::ofstream outFile("wynik.txt", std::ofstream::out);
	std::cout << "Czas rozpoczecia pomiaru: " << ctime(&start1) << std::endl;

	while (start1 < finish)
	{
		       current_route = bestNeighbourhood(current_route, matrix1, cities, 0.1);//wyszukiwanie nowej trasy w sasiedztwie obecnej
			if (route(matrix1, cities, current_route) < route(matrix1, cities, best_route)) {
				delete[] best_route;
				best_route = new int[cities];
				assign(best_route, current_route, cities);	

			}else
				bylo++; //jezeli nie znaleziono lepszej trasy 
			//
  
			//if (bylo == miasta){ //jezeli przez 10 przejsc nie znaleziono lepszej trasy nastepuje dywersyfikacja
			//	delete[]current_route;
			//	current_route = new int[miasta];
			//	current_route = dywersyfikacja(miasta, macierz1);
			//	
			//}


			//if (bylo == 2*miasta){//jezeli przez N przejsc nie znaleziono lepszej trasy, cala jest losowana od nowa
			//	delete[] current_route;
			//	current_route = new int[miasta];
			//	int *permutacja2 = new int[miasta];
			//	permutacja(permutacja2, miasta);
			//	przypisz(current_route, permutacja2, miasta);
			//	bylo = 0;
			//	delete[]permutacja2;
			//
			//}

			start1 = time(NULL);
			stop();
			solution = route(matrix1, cities, best_route);
			outFile << stop()<<": " << solution << std::endl;
			}
	     outFile.close();
		
		 solution = route(matrix1, cities, best_route);


	return stop();
	delete[]best_route;
	delete[]permutation1;
	delete[]current_route;
}

