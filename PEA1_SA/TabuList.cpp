#include"stdafx.h"
#include"TabuList.h"
#include"Macierz.h"


 TabuList::TabuList(int &miasta, int lKadencji){
	tabuList = new int *[miasta];
	for (int i = 0; i < miasta; i++){
		tabuList[i] = new int[miasta];
		for (int j = 0; j < miasta; j++){
			tabuList[i][j] = 0;

		}
	}
}

 
void TabuList::ruch_tabu(int miasto1, int miasto2, int miasta){
    //funkcja dodaje ruch miêdzy miastem1 i miastem2 na listê na okreslona liczbe kadencji
	tabuList[miasto1][miasto2] = lKadencji;
	tabuList[miasto2][miasto1] = lKadencji;

}

void TabuList::decrement_tabu(int miasta){
	for (int i = 0; i < miasta; i++){
		for (int j = 0; j < miasta; j++){
			if (tabuList[i][j] != 0){
				tabuList[i][j] --;
			}
		}
	}
}
bool TabuList::czyTabu(int miasto1, int miasto2){
	return tabuList[miasto1][miasto2]!= 0;
}

int TabuList::nrKadencji(int miasto1, int miasto2){
	int nr = tabuList[miasto1][miasto2];
	return nr;
}

void TabuList::reset(int miasta)
{
	for (int i = 0; i <miasta ; i++)
	{
		for (int k = 0; k < miasta; k++)
		{
			tabuList[i][k] = 0;
		}
	}
}