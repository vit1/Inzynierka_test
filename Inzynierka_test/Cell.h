#include "Color.h"
#include "Point.h"
#include <vector>

using namespace std;
#pragma once

class Cell
{
public:
	Cell(void);
	~Cell(void);

	bool czyGranica;
	bool border2;
	bool isMarked;
	Color kolor;
	Cell *pNeighbours[8];
	Cell *pSwoiSasiedzi[8];
	int previous_id;

	void setGrainID(int anID);
	int getGrainID();

	void setState(int aState);
	int getState();

private:
	int grainID;
	int state;


};

