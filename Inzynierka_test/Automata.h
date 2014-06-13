
#ifndef AUTOMATA_H
#define AUTOMATA_H
#pragma once



#include "Cell.h"
#include <vector>
#include <map>


class Automata
{
public:
	Cell **tab;
	Cell **OldTab;

	 //int size;

	 int sizeX;
	 int sizeY;
	 int PIXEL;

	int id;
	int modulate;


	int mod (int a, int b);
	typedef vector<Point> vPoints;
	typedef map<int, vPoints> Mapa;
	typedef map<int, vPoints>::const_iterator MapIterator;

	Mapa storage;
	vPoints vectPoints;

	void fillForMC(Cell **tab);
	Cell** createTab(Cell **tab);
	void assignBorders(Cell **tab, int sizeX , int sizeY);

	int energyCount(Cell **tab , int x, int y);
	void monteCarlo(Cell** tab , int x, int y);




	void addSubGrains(Cell **tab , int ileZiaren);
	void searchForGrains(Cell **tab, int sizeX , int sizeY);



	void podczepSasiadowVonNeumanna(Cell **tab, Cell **oldTab , int sizeX , int sizeY);
	void podczepSwpochSasiadowVonNeumanna(Cell **tab , int sizeX , int sizeY);
	void rozrostVonNeumanna(Cell **tab, Cell **oldTab, int sizeX , int sizeY);

	void podczepSasiadowMoorea(Cell **tab, Cell **oldTab , int sizeX , int sizeY);
	void podczepSwpochSasiadowMoorea(Cell **tab , int sizeX , int sizeY);
	void rozrostMoorea(Cell **tab, Cell **oldTab, int sizeX , int sizeY);

	void podczepSasiadowHexR(Cell **tab, Cell **oldTab , int sizeX , int sizeY);
	void podczepSwpochSasiadowHexRIGHT(Cell **tab , int sizeX , int sizeY);
	void rozrostHexaRIGHT(Cell **tab, Cell **oldTab, int sizeX , int sizeY);

	void podczepSwpochSasiadowPentaRight(Cell **tab , int sizeX , int sizeY);
	Automata(int sizex , int sizey);
	~Automata(void);
};

#endif 