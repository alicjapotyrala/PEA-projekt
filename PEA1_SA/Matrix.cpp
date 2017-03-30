#include"stdafx.h"
#include "Matrix.h"

using namespace std;

bool Macierz::read_file(string filename, double **&matrix, int &cities, int &optimum){

	fstream file;
	file.open((filename + ".txt").c_str(), ios::in);

	if (file.good()) {

		file >> cities >> optimum;
		

		matrix = new double *[cities];

		for (int i = 0; i < cities; ++i) {
			*(matrix + i) = new double[cities]; // number of columns
			for (int j = 0; j < cities; ++j) {

				file >> matrix[i][j];

			}

		}

		//displaying matrix

		/*for (int i = 0; i < cities; i++){
		for (int j = 0; j < cities; j++){

		cout << setw(2) << matrix[i][j] << " ";
		}
		cout << endl;
		}*/

		cout << "Plik odczytany poprawnie." << endl;
		cout << endl;

		file.close();
		return true;

	}
	else {
		cout << "Blad. " << endl;
		system("pause");
		file.close();
		return false;
	}
}