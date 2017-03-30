#include "stdafx.h"
#include "Tabu_Search.h"
#include "Macierz.h"


void TabuSearch::start()
{
	LARGE_INTEGER l_int;

	QueryPerformanceFrequency(&l_int);
	frequency = double(l_int.QuadPart);

	QueryPerformanceCounter(&l_int);
	counter = l_int.QuadPart;
}

double TabuSearch::stop()
{
	LARGE_INTEGER l_int;
	QueryPerformanceCounter(&l_int);
	return double(l_int.QuadPart - counter) / frequency;
}

double TabuSearch::droga(double **macierz, int miasta, int *perm){
	double koszt = 0;

	for (int i = 0; i < miasta - 1; i++)
	{
		koszt += macierz[perm[i]][perm[i + 1]];
	}
	koszt += macierz[perm[miasta - 1]][perm[0]];//droga od ostatniego do pierwszego miasta

	return koszt;
}



void TabuSearch::permutacja(int *& perm, int miasta){

	for (int c = 0; c <miasta; c++)
	{
		bool sprawdz;
		int losowe;
		do{
			losowe = rand() % miasta; //losuje jedno miasto
			sprawdz = true;
			for (int j = 0; j <c; j++){
				if (losowe == perm[j]){
					sprawdz = false;
					break;
				}
			}

		} while (!sprawdz);
		perm[c] = losowe;
	}

}

//dywersyfikacja- tworze N (gdzie N-liczba miast) nowych tras i wybieram najlepsza z nich
int* TabuSearch::dywersyfikacja(int miasta, double **macierz){

	double obecna, najlepsza = DBL_MAX;
	int *najlepsza_droga = new int[miasta];
	int *obecna_droga = new int[miasta];
	for (int i = 0; i < miasta; i++){
		int *permutacja1 = new int[miasta];

		permutacja(permutacja1, miasta);
		obecna = droga(macierz, miasta, permutacja1);
		przypisz(obecna_droga, permutacja1, miasta);

		if (obecna < najlepsza){
			najlepsza = obecna;
			przypisz(najlepsza_droga, obecna_droga, miasta);
		}
		delete[]permutacja1;
	}

	return najlepsza_droga;
	delete[]najlepsza_droga;
	delete[]obecna_droga;
}

void TabuSearch::przypisz(int *tab1, int *tab2, int miasta){
	for (int i = 0; i < miasta; i++){
		tab1[i] = tab2[i];
	}
}

void TabuSearch::swap1(int *trasa, int a, int b) {
	int temp = trasa[a];
	trasa[a] = trasa[b];
	trasa[b] = temp;
	temp = 0;
}



int * TabuSearch::najlepszeSasiedztwo(int *trasa, double **macierz, int miasta, TabuList *tlist, float asp){
	double obecna_droga, najlepsza_droga;
	float improvment;
	int *najlepsza = new int[miasta];

	for (int i = 0; i < miasta; i++){
		najlepsza[i] = trasa[i];
	}

	for (int i = 0; i < miasta; i++)
	{
		for (int j = 0; j < miasta; j++)
		{
			swap1(trasa, i, j);//metoda wyznaczania sasiedztwa- zamiana dwoch miast w trasie
			tlist->decrement_tabu(miasta);

			obecna_droga = droga(macierz, miasta, trasa);
			najlepsza_droga = droga(macierz, miasta, najlepsza);

			if (tlist->nrKadencji(i, j) != 0) {
				if (obecna_droga < najlepsza_droga) {
					improvment = (najlepsza_droga - obecna_droga) / najlepsza_droga;
					if (improvment >= asp) {
						tlist->reset(miasta);
						delete[] najlepsza;
						najlepsza = new int[miasta];
						przypisz(najlepsza, trasa, miasta);
							tlist->ruch_tabu(i, j, miasta);
					}
				}
				swap1(trasa, i, j);
				break;
			}
			else if (obecna_droga < najlepsza_droga) {
				delete[] najlepsza;
				najlepsza = new int[miasta];
				przypisz(najlepsza, trasa, miasta);
				tlist->ruch_tabu(i, j, miasta);
			}
			swap1(trasa, i, j);
			
		
		}
	}
	return najlepsza;
	delete[] najlepsza;

}

double TabuSearch::ts(int dlugosc_kadencji, double ** macierz1, int miasta, int liczba_iteracji, int & rozwiazanie)
{
	start();
	TabuList *tlist = new TabuList(miasta, dlugosc_kadencji);
	int *najlepsza_trasa = new int[miasta];
	int *permutacja1 = new int[miasta];
	int bylo = 0;
	permutacja(permutacja1, miasta);

	przypisz(najlepsza_trasa, permutacja1, miasta);

	int *obecna_trasa = new int[miasta];
	przypisz(obecna_trasa, najlepsza_trasa, miasta);

	time_t endwait;
	time_t start1 = time(NULL);
	time_t seconds = 600; 

	endwait = start1 + seconds;
	std::ofstream outFile("newData.txt", std::ofstream::out);
	printf("start time is : %s", ctime(&start1));

			while (start1 < endwait)
			{
				
		algorytm:
			if (droga(macierz1, miasta, obecna_trasa) < droga(macierz1, miasta, najlepsza_trasa)) {
				delete[] najlepsza_trasa;
				najlepsza_trasa = new int[miasta];
				for (int j = 0; j < miasta; j++){
					najlepsza_trasa[j] = obecna_trasa[j];
				}
				obecna_trasa = najlepszeSasiedztwo(obecna_trasa, macierz1, miasta, tlist, 0.5);
			}
			else
				bylo++;
			

			if (bylo == 10){
				delete[] obecna_trasa;
				obecna_trasa = new int[miasta];
				obecna_trasa = dywersyfikacja(miasta, macierz1);
				//goto algorytm;
			}

			if (bylo ==20){
				delete[] obecna_trasa;
				obecna_trasa = new int[miasta];
				int*permutacja2 = new int[miasta];
				permutacja(permutacja2, miasta);
				przypisz(obecna_trasa, permutacja2, miasta);
				bylo = 0;
				delete[]permutacja2;
				//goto algorytm;
			}

			//if (bylo == 30 * miasta){
			//	rozwiazanie = droga(macierz1, miasta, best_route);

			//	bylo = 0;
			//	break;

			//	}
			start1 = time(NULL);
			stop();
			
			rozwiazanie = droga(macierz1, miasta, najlepsza_trasa);
			outFile<<stop()<<": "<< rozwiazanie << std::endl;
		}
	rozwiazanie = droga(macierz1, miasta, najlepsza_trasa);
	outFile.close();


	return stop();
	delete[]najlepsza_trasa;
	delete[]permutacja1;

	delete[]obecna_trasa;
}

