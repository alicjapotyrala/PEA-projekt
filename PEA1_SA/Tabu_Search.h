#include "stdafx.h"
#include "Macierz.h"
#include "TabuList.h"

class TabuSearch{
public:
	__int64 counter;
	double frequency;

	void start();
	double stop();
	void swap1(int *trasa, int a, int b);
	int* najlepszeSasiedztwo(int *trasa, double **macierz, int miasta, TabuList *tlist, float asp);
	int* dywersyfikacja(int miasta, double **macierz);
	double droga(double **macierz, int miasta, int *perm);
	void permutacja(int *&perm, int miasta);
	void przypisz(int *tab1, int *tab2, int miasta);
	double ts(int max_cadence, double ** macierz1, int miasta, int liczba_iteracji, int & rozwiazanie);



};