#ifndef MACIERZ_H
#define MACIERZ_H
#include"stdafx.h"

class Macierz{
public:

	bool read_file(std::string filename, double **&matrix, int &cities, int &optimum);

};
#endif