#include "Cell.h"


Cell::Cell(void)
{
	this->setGrainID(0);
	this->setState(0);
	Cell::czyGranica = false;
	Cell::isMarked = false;
	Cell::border2 = false;
	
}


Cell::~Cell(void)
{
	delete [] *pNeighbours;
	delete [] *pSwoiSasiedzi;
}

void Cell::setGrainID(int anID)
{
	this->grainID = anID;
}

int Cell::getGrainID()
{
	return this->grainID;
}

void Cell::setState(int aState)
{
	this->state = aState;
}

int Cell::getState()
{
	return this->state;
}

