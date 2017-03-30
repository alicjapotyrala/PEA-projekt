#include"stdafx.h"
#include"Simulated_Annealing.h"

void SA::start() 
{
	LARGE_INTEGER l_int;

	QueryPerformanceFrequency(&l_int); 
	frequency = double(l_int.QuadPart); 

	QueryPerformanceCounter(&l_int); 
	counter = l_int.QuadPart;
}

double SA::stop() 
{
	LARGE_INTEGER l_int; 
	QueryPerformanceCounter(&l_int);
	return double(l_int.QuadPart - counter) / frequency; 
}

double SA::route(double **matrix, int cities, int *perm){
	double cost = 0;

	for (int i = 0; i < cities - 1; i++)
	{

		cost += matrix[perm[i]][perm[i + 1]];

	}

	cost += matrix[perm[cities - 1]][perm[0]];//droga od ostatniego do pierwszego miasta

	return cost;
}

void SA::permutation(int *& perm, int cities){
	double* num = new double[cities];
	int random;

	for (int i = 0; i < cities; i++)
	{
		num[i] = i; 
	}

	for (int i = cities; i > 0; i--)
	{
		random = rand() % i; //losuje jedno miasto
		perm[i - 1] = num[random];
		num[random] = num[i - 1];
	}

	delete[] num;
}

bool SA::probability(int length, int length1, double temperature){

	double p = pow(e, ((-1 * (length1 - length)) / temperature));

	double r = (double)rand() / RAND_MAX; // wybieramy cyfrê z przedzia³u <0,1>

	return (r < p); //jezeli prawdopodobienstwo ze wzoru jest wieksze od losowo wybranej liczby z przedzialu<0,1>

}

double SA::sa(double ** matrix1, int cities, double constant, int & solution){

	start();

	int * permutation1 = new int[cities];
	int * permutation2 = new int[cities];

	int city1, city2;
	double temperature, delta_max = 0, delta, cost1, cost2;;

	
	for (int i = 0; i < cities; i++)
	{
		permutation(permutation1,cities);
		permutation(permutation2,cities);
		delta = abs(route(matrix1,cities,permutation1) - route(matrix1,cities,permutation2));
		if (delta > delta_max)
		{
			delta_max = delta;
		}

		delete[] permutation1;
		delete[] permutation2;
		permutation1 = new int[cities];
		permutation2 = new int[cities];
	}

	temperature = delta_max;

	//tworze permutacje i obliczam koszt drogi dla niej
	permutation(permutation1,cities);
	cost1 = route(matrix1,cities,permutation1);

	//tworze zapasowa tablice permutacja2 
	for (int i = 0; i < cities; i++)
	{

		permutation2[i] = permutation1[i];
	}

	while (temperature > min_temperature)
	{

		do
		{
			//losowanie 2 nowych miast do zamiany w permutacji
			city1 = rand() % cities;
			city2 = rand() % cities;

		} while (city1 == city2);//petla wykonuje sie dopoki miasta nie beda rozne

		//zamiana miast
		permutation2[city2] = permutation1[city1];
		permutation2[city1] = permutation1[city2];


		cost2 = route(matrix1,cities,permutation2);

		if (cost2 <= cost1 || probability(cost1, cost2, temperature)) 
		{
			cost1 = cost2;//
			if (cost1 <= solution)
			{
				solution = cost1;
			}

			permutation1[city1] = permutation2[city1]; //koszt dla permutacji 2 by³ wiêkszy wiêc staje sie "lepsza" permutacja
			permutation1[city2] = permutation2[city2];
		}
		else
		{
			permutation2[city1] = permutation1[city1];
			permutation2[city2] = permutation1[city2];
		}


		temperature *= constant;
	}

	delete[] permutation1;
	delete[] permutation2;

	return stop();
}

