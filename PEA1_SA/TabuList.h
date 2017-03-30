

class TabuList {
public:
	
	int **tabuList;
	int lKadencji;
	TabuList(int &miasta, int lKadencji);
	void ruch_tabu(int miasto1, int miasto2, int miasta);
	void decrement_tabu(int miasta);
	bool czyTabu(int miasto1, int miasto2);
	int nrKadencji(int miasto1, int miasto2);
	void reset(int miasta);
};