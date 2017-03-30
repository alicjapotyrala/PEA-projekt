// PEA1_SA.cpp : Defines the entry point for the console application.
//
#include"stdafx.h"
#include"Matrix.h"
#include"Simulated_Annealing.h"
#include"Tabu_Search_v.h"
#include"Genetic_Algorithm.h"

using namespace std;

int main()
{
	srand(time(NULL));
	double alfa[] = { 0.90, 0.95, 0.99, 0.995, 0.999, 0.9999 };

	Macierz *m = new Macierz();
	SA *w = new SA();
	TS *t = new TS();
	GA *p = new GA();

	string filename;
	int cities = 0, optimum, number;
	double  avg_time = 0.0, avg_distance = 0.0, max_result = DBL_MAX, worst = 0.0, measurement, measurement1, measurement_error = 0.0;
	


	double **matrix1, how_many;
	do{
		cout << "Problem komiwojazera" << endl;
		cout << "1. Algorytm symulowanego wyzarzania" << endl;
		cout << "2. Tabu Search" << endl;
		cout << "3. Algorytm genetyczny" << endl;
		cout << "4. Koniec programu." << endl;
		cout << "Wybierz numer algorytmu:" << endl;
		cin >> number;
		while (cin.fail()) {
			cin >> number;
		}

		switch (number){
		case 1:
			cout << "Pliki testowe symetryczne: gr17, rat195, berlin52" << endl;
			cout << "Pliki testowe asymetryczne: br17, ftv70, ftv170" << endl;
			cout << "Wybierz plik (bez rozszerzenia):" << endl;
			cin >> filename;
			while (filename != "gr17" && filename != "berlin52" && filename != "rat195" && filename != "br17"  && filename != "ftv70" && filename != "ftv170") {
				cout << " Wybrany plik nie znajduje sie na liscie." << endl;
				cin >> filename;
			}

			m->read_file(filename, matrix1, cities, optimum);
			cout << "Ile razy algorytm ma byc wykonany dla kazdej stalej?" << endl;
			cin >> how_many;
			while (cin.fail()){
				cout << "Blad!" << endl;
				cin >> how_many;
			}
			cout << endl;

			if (m->read_file(filename, matrix1, cities, optimum) == true)
			{

				for (int i = 0; i < 6; i++)
				{
					cout.setf(ios::showpoint);
					cout << "Stala: ";
					cout << alfa[i] << endl;

					for (int j = 0; j < how_many; j++)
					{
						int result = INT_MAX;

						measurement = w->sa(matrix1, cities, alfa[i], result);

						avg_time += measurement;
						avg_distance += result;

						if (result < max_result)
						{
							max_result = result;
						}
					}
					double x = max_result - optimum;
					avg_distance /= how_many;
					avg_time /= how_many;

					measurement_error = (x * 100) / optimum;
					cout << "Najkrotsza znana odleglosc: ";
					cout << optimum << endl;
					cout << "Sredni obliczony koszt: ";
					cout << avg_distance << endl;
					cout << "Najmniejszy obliczony koszt: ";
					cout << max_result << endl;
					cout << "Sredni czas: ";
					cout << avg_time << endl;
					cout << "Blad: ";
					cout << measurement_error << "%" << endl;

					cout << endl << endl;
					avg_time = 0;
					avg_distance = 0;
					max_result = DBL_MAX;
				}
				cout << endl << endl << "Zakonczono obliczenia." << endl << endl;

				cout << endl;

				for (int i = 0; i < cities; i++)
				{
					delete[] matrix1[i];
				}
				delete[] matrix1;
			}
			else
			{
				return -1;
			}
			break;

		case 2:
			cout << "Pliki testowe symetryczne: gr17, rat195, berlin52" << endl;
			cout << "Pliki testowe asymetryczne: br17, ftv70, ftv170" << endl;
			cout << "Wybierz plik (bez rozszerzenia):" << endl;
			cin >> filename;
			while (filename != "gr17" && filename != "berlin52" && filename != "rat195" && filename != "br17"  && filename != "ftv70" && filename != "ftv170") {
				cout << " Wybrany plik nie znajduje sie na liscie." << endl;
				cin >> filename;
			}

			cout << endl;
			

			m->read_file(filename, matrix1, cities, optimum);
		
			cout << endl;



			if (m->read_file(filename, matrix1, cities, optimum) == true){



				int result1 = INT_MAX;
				time_t runtime;
				cout << "Podaj czas dzialania algorytmu (w sekundach): " <<endl;
				cin >> runtime;
					measurement1 = t->ts( runtime, matrix1, cities, result1);

					avg_time += measurement1;
					avg_distance += result1;
					if (result1 < max_result)
					{
						max_result = result1;
					}

					avg_time /= 1;
					avg_distance /= 1;
					double x = avg_distance - optimum;
					cout << "Wynik: " << endl;
					measurement_error = (x * 100) / optimum;
					cout << "Najkrotsza znana odleglosc: ";
					cout << optimum << endl;
					cout << "Sredni obliczony koszt: ";
					cout << avg_distance << endl;
					cout << "Najmniejszy obliczony koszt: ";
					cout << result1 << endl;
					cout << "Sredni czas: ";
					cout << avg_time << endl;
					cout << "Blad: ";
					cout << measurement_error << "%" << endl;

					cout << endl << endl;
					avg_time = 0;
					avg_distance = 0;
				
				
				
			}

				break;
		case 3:
		{
				  cout << "Pliki testowe symetryczne: gr17, rat195, berlin52" << endl;
				  cout << "Pliki testowe asymetryczne: br17, ftv70, ftv170" << endl;
				  cout << "Wybierz plik (bez rozszerzenia):" << endl;
				  cin >> filename;
				  while (filename != "gr17" && filename != "berlin52" && filename != "rat195" && filename != "br17"  && filename != "ftv70" && filename != "ftv170") {
					  cout << " Wybrany plik nie znajduje sie na liscie." << endl;
					  cin >> filename;
				  }

				  cout << endl;
				  vector<vector<int>> pop;
				  m->read_file(filename, matrix1, cities, optimum);
				  double best_way=99999.999;
				  double current_way = 0;
				  cout << endl;
				  p->start();
				  pop= p->createPopulation(100, cities);
				  for (int i = 0; i < 200; i++){
					  current_way = p->evolve(matrix1, 100, cities, pop);	
					  if (current_way < best_way){
						  best_way = current_way;
					  }

				  }
				  p->stop();
				  cout << "Najkrotsza znaleziona droga: " << best_way << endl;
				  cout << "Czas: " << p->stop()<<endl;
				  cout << endl;
		}
			break;
			}

		} while (number != 4);

		if (number == 4){

			system("pause");
			return 0;
		}


	}

