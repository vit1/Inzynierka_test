#include "Automata.h"



Automata::Automata(int sizex , int sizey)
{
	Automata::sizeX = sizex;
	Automata::sizeY = sizey;
	id = 0;
	modulate = id + 100;
}


Automata::~Automata(void)
{

}

void Automata:: fillForMC(Cell **tab){
	for(int i=0 ; i < sizeX ; i++){
		for(int j=0 ; j<sizeY ; j++){
			tab[i][j].setState(1);
			tab[i][j].setGrainID(id);
			tab[i][j].kolor.setRandomColor();
			id++;
		}
	}
}

Cell** Automata::createTab(Cell **tab){
	tab = new Cell*[sizeX];

	for(int i=0 ; i<sizeX ; ++i){
			tab[i] = new Cell[sizeY];		
			
	}
	return tab;
}



void Automata::addSubGrains(Cell **tab , int ileZiaren){

	for(auto it=storage.begin() ; it!= storage.end() ; it++){
					for(int i=0 ; i<ileZiaren ; i++){
						int random = rand() %  it->second.size();
						Point coord(0,0);
						coord = it->second[random];

						tab[coord.x][coord.y].setState(2);
						tab[coord.x][coord.y].setGrainID(modulate);//!
						tab[coord.x][coord.y].kolor.setNewColor();

						modulate++;//!
					}
			}
}

void Automata::searchForGrains(Cell **tab, int sizeX , int sizeY){
	
	
	for(int i=0 ; i<sizeX ; i++){
		for(int j=0 ; j<sizeY ; j++){
			if(tab[i][j].getState() == 1 && tab[i][j].czyGranica == false){
				
				storage.insert(pair<int, vector<Point> >(tab[i][j].getGrainID(), vector<Point>() ));
				Point p(i,j);
				storage[tab[i][j].getGrainID()].push_back(p);
			}
		}
	}


}

 int Automata::energyCount(Cell **tab , int x, int y)	{
	
	int counter = 0;

	for(int i=0 ; i<=7 ; i++) {
		if( tab[x][y].getGrainID() != tab[x][y].pSwoiSasiedzi[i]->getGrainID() )
			counter++;
		
	}

	return counter;
}

 void Automata::monteCarlo(Cell** tab ,  int x, int y){
	
	 int ra = rand() % 8 + 0;

		int ene1st = energyCount(tab,x,y);
		int oldColor = tab[x][y].getGrainID();
		Color kol = tab[x][y].kolor;
	
		if(tab[x][y].getGrainID() != tab[x][y].pSwoiSasiedzi[ra]->getGrainID() ) {
			tab[x][y].setGrainID(tab[x][y].pSwoiSasiedzi[ra]->getGrainID());
				tab[x][y].kolor = tab[x][y].pSwoiSasiedzi[ra]->kolor;
			}

		int ene2nd = energyCount(tab, x, y);	
	
		int deltEne = ene2nd - ene1st;
		if(deltEne > 0){
			tab[x][y].setGrainID(oldColor);
			tab[x][y].kolor = kol;
		}	
		if(deltEne == 0){
			int r = rand() %2;
			if(r == 0){
				tab[x][y].setGrainID(oldColor);
				tab[x][y].kolor = kol;
			}
		}
		
 }

 void Automata::assignBorders(Cell **tab, int sizeX , int sizeY){

	podczepSwpochSasiadowPentaRight(tab,sizeX,sizeY);
	for(int i=0 ; i<sizeX ; ++i){
		for(int j=0 ; j<sizeY ; ++j){
			
			tab[i][j].previous_id = tab[i][j].getGrainID();//!

             for(int iter = 0 ; iter <= 4 ; iter ++){
				 if(tab[i][j].getGrainID() != tab[i][j].pSwoiSasiedzi[iter]->getGrainID()   )
					tab[i][j].czyGranica = true;
			 }
				
	

		}
	}
	podczepSwpochSasiadowMoorea(tab,sizeX,sizeY);

}

 int Automata::mod (int a, int b)
{
   if(b < 0) 
     return mod(-a, -b);   
   int ret = a % b;
   if(ret < 0)
     ret+=b;
   return ret;
}
  int mod (int a, int b)
{
   if(b < 0) 
     return mod(-a, -b);   
   int ret = a % b;
   if(ret < 0)
     ret+=b;
   return ret;
}


 void Automata::podczepSasiadowVonNeumanna(Cell **tab, Cell **oldTab , int sizeX , int sizeY){
	for(int i=0 ; i<sizeX ; ++i){
		for(int j=0 ; j<sizeY ; ++j){
			int above = mod( (i-1) , sizeX );
			int below = mod( (i+1) , sizeX );
			int left =  mod( (j-1) , sizeY );
			int right = mod( (j+1) , sizeY );

			tab[i][j].pNeighbours[0] = & oldTab[above][j];
			tab[i][j].pNeighbours[1] = & oldTab[i][right];
			tab[i][j].pNeighbours[2] = & oldTab[below][j];
			tab[i][j].pNeighbours[3] = & oldTab[i][left];

			tab[i][j].pNeighbours[4] = NULL;
			tab[i][j].pNeighbours[5] = NULL;
			tab[i][j].pNeighbours[6] = NULL;
			tab[i][j].pNeighbours[7] = NULL;

		}

	}

}
	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Automata::podczepSwpochSasiadowVonNeumanna(Cell **tab , int sizeX , int sizeY){
	for(int i=0 ; i<sizeX ; ++i){
		for(int j=0 ; j<sizeY ; ++j){
			int above = mod( (i-1) , sizeX );
			int below = mod( (i+1) , sizeX );
			int left =  mod( (j-1) , sizeY );
			int right = mod( (j+1) , sizeY );

			tab[i][j].pSwoiSasiedzi[0] = & tab[above][j];
			tab[i][j].pSwoiSasiedzi[1] = & tab[i][right];
			tab[i][j].pSwoiSasiedzi[2] = & tab[below][j];
			tab[i][j].pSwoiSasiedzi[3] = & tab[i][left];

			tab[i][j].pSwoiSasiedzi[4] = NULL;
			tab[i][j].pSwoiSasiedzi[5] = NULL;
			tab[i][j].pSwoiSasiedzi[6] = NULL;
			tab[i][j].pSwoiSasiedzi[7] = NULL;

		}
	}

}



void Automata::rozrostVonNeumanna(Cell **tab, Cell **oldTab, int sizeX , int sizeY){
	

	for(int i=0 ; i<sizeX ; ++i){
		for(int j=0 ; j<sizeY ; ++j){
			oldTab[i][j].setState(tab[i][j].getState());  
			oldTab[i][j].setGrainID(tab[i][j].getGrainID());
			oldTab[i][j].czyGranica = tab[i][j].czyGranica;
			oldTab[i][j].kolor = tab[i][j].kolor;


		}
	}

	for(int i=0 ; i<sizeX ; i++){
		for(int j=0 ; j<sizeY ; j++){
			if( oldTab[i][j].getState() == 2 ){

				for(int iter=0 ; iter<=3 ; ++iter){
					if(tab[i][j].pNeighbours[iter]->getState() != 2  && tab[i][j].previous_id == tab[i][j].pSwoiSasiedzi[iter]->previous_id && tab[i][j].czyGranica == false){
			
						tab[i][j].pSwoiSasiedzi[iter]->setState(  tab[i][j].getState() );
						tab[i][j].pSwoiSasiedzi[iter]->setGrainID( tab[i][j].getGrainID());
						tab[i][j].pSwoiSasiedzi[iter]->kolor = tab[i][j].kolor;
				
					}
				}				
			
		}

	}

	


}
}

//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
									//MOORE
//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void Automata::podczepSasiadowMoorea(Cell **tab, Cell **oldTab , int sizeX , int sizeY){
	for(int i=0 ; i<sizeX ; ++i){
		for(int j=0 ; j<sizeY ; ++j){
			int above = mod( (i-1) , sizeX );
			int below = mod( (i+1) , sizeX );
			int left =  mod( (j-1) , sizeY );
			int right = mod( (j+1) , sizeY );

			tab[i][j].pNeighbours[0] = & oldTab[above][left];
			tab[i][j].pNeighbours[1] = & oldTab[above][j];
			tab[i][j].pNeighbours[2] = & oldTab[above][right];
			tab[i][j].pNeighbours[3] = & oldTab[i][right];

			tab[i][j].pNeighbours[4] = & oldTab[below][right];
			tab[i][j].pNeighbours[5] = & oldTab[below][j];
			tab[i][j].pNeighbours[6] = & oldTab[below][left];
			tab[i][j].pNeighbours[7] = & oldTab[i][left];

		}

	}

}
	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Automata::podczepSwpochSasiadowMoorea(Cell **tab , int sizeX , int sizeY){
	for(int i=0 ; i<sizeX ; ++i){
		for(int j=0 ; j<sizeY ; ++j){
			int above = mod( (i-1) , sizeX );
			int below = mod( (i+1) , sizeX );
			int left =  mod( (j-1) , sizeY);
			int right = mod( (j+1) , sizeY );

			tab[i][j].pSwoiSasiedzi[0] = & tab[above][left];
			tab[i][j].pSwoiSasiedzi[1] = & tab[above][j];
			tab[i][j].pSwoiSasiedzi[2] = & tab[above][right];
			tab[i][j].pSwoiSasiedzi[3] = & tab[i][right];

			tab[i][j].pSwoiSasiedzi[4] = & tab[below][right];
			tab[i][j].pSwoiSasiedzi[5] = & tab[below][j];
			tab[i][j].pSwoiSasiedzi[6] = & tab[below][left];
			tab[i][j].pSwoiSasiedzi[7] = & tab[i][left];

		}
	}

}


void Automata::rozrostMoorea(Cell **tab, Cell **oldTab, int sizeX , int sizeY){
	

	for(int i=0 ; i<sizeX ; ++i){
		for(int j=0 ; j<sizeY ; ++j){
			oldTab[i][j].setState( tab[i][j].getState() );
			oldTab[i][j].setGrainID( tab[i][j].getGrainID() );
			oldTab[i][j].czyGranica = tab[i][j].czyGranica;
			oldTab[i][j].kolor = tab[i][j].kolor;
			tab[i][j].czyGranica = false;

		}
	}

	for(int i=0 ; i<sizeX ; i++){
		for(int j=0 ; j<sizeY ; j++){
			if( oldTab[i][j].getState() == 2 ){
				for(int iter=0 ; iter<=7 ; ++iter){
					if(tab[i][j].pNeighbours[iter]->getState() != 2 
						&& tab[i][j].previous_id == tab[i][j].pSwoiSasiedzi[iter]->previous_id && tab[i][j].czyGranica == false){
			
							tab[i][j].pSwoiSasiedzi[iter]->setState( tab[i][j].getState() );
							tab[i][j].pSwoiSasiedzi[iter]->setGrainID( tab[i][j].getGrainID() );
							tab[i][j].pSwoiSasiedzi[iter]->kolor = tab[i][j].kolor;
					}
				}
		
				
			}
			
			
		}

	}

	


	}

//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
								//	HeksaGONAl RIGHT
//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Automata::podczepSasiadowHexR(Cell **tab, Cell **oldTab , int sizeX , int sizeY){
	for(int i=0 ; i<sizeX ; ++i){
		for(int j=0 ; j<sizeY ; ++j){
			int above = mod( (i-1) , sizeX );
			int below = mod( (i+1) , sizeX );
			int left =  mod( (j-1) , sizeY );
			int right = mod( (j+1) , sizeY );

			
			tab[i][j].pNeighbours[0] = & oldTab[above][j];
			tab[i][j].pNeighbours[1] = & oldTab[above][right];
			tab[i][j].pNeighbours[2] = & oldTab[i][right];

			tab[i][j].pNeighbours[3] = & oldTab[below][j];
			tab[i][j].pNeighbours[4] = & oldTab[below][left];
			tab[i][j].pNeighbours[5] = & oldTab[i][left];
			
			
			//////////////////////////////////////
			tab[i][j].pNeighbours[6] = NULL;
			tab[i][j].pNeighbours[7] = NULL;

		}

	}

}

//--------------------
void Automata::podczepSwpochSasiadowHexRIGHT(Cell **tab , int sizeX , int sizeY){
	for(int i=0 ; i<sizeX ; ++i){
		for(int j=0 ; j<sizeY ; ++j){
			int above = mod( (i-1) , sizeX );
			int below = mod( (i+1) , sizeX );
			int left =  mod( (j-1) , sizeY );
			int right = mod( (j+1) , sizeY );


			tab[i][j].pSwoiSasiedzi[0] = & tab[above][j];
			tab[i][j].pSwoiSasiedzi[1] = & tab[above][right];
			tab[i][j].pSwoiSasiedzi[2] = & tab[i][right];

			tab[i][j].pSwoiSasiedzi[3] = & tab[below][j];
			tab[i][j].pSwoiSasiedzi[4] = & tab[below][left];
			tab[i][j].pSwoiSasiedzi[5] = & tab[i][left];
			
			
			//////////////////////////////////////
			tab[i][j].pSwoiSasiedzi[6] = NULL;
			tab[i][j].pSwoiSasiedzi[7] = NULL;



		}
	}

}
void Automata::rozrostHexaRIGHT(Cell **tab, Cell **oldTab, int sizeX , int sizeY){
	

	for(int i=0 ; i<sizeX ; ++i){
		for(int j=0 ; j<sizeY ; ++j){
			oldTab[i][j].setState( tab[i][j].getState());
			oldTab[i][j].setGrainID ( tab[i][j].getGrainID());
			oldTab[i][j].czyGranica = tab[i][j].czyGranica;
			oldTab[i][j].kolor = tab[i][j].kolor;

		}
	}

	
	for(int i=0 ; i<sizeX ; i++){
		for(int j=0 ; j<sizeY ; j++){
			if( oldTab[i][j].getState() == 2 ){
				for(int iter=0 ; iter<=5 ; ++iter){
					if(tab[i][j].pNeighbours[iter]->getState() != 2 
						&& tab[i][j].previous_id == tab[i][j].pSwoiSasiedzi[iter]->previous_id && tab[i][j].czyGranica == false){
			
							tab[i][j].pSwoiSasiedzi[iter]->setState( tab[i][j].getState() );
							tab[i][j].pSwoiSasiedzi[iter]->setGrainID( tab[i][j].getGrainID() );
							tab[i][j].pSwoiSasiedzi[iter]->kolor = tab[i][j].kolor;
					}
				}
		
				
			}
			
			
		}

	}
			
				
}
			
			


//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
								//	PENTA RIGHT
//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void Automata::podczepSwpochSasiadowPentaRight(Cell **tab , int sizeX , int sizeY){
	for(int i=0 ; i<sizeX ; ++i){
		for(int j=0 ; j<sizeY ; ++j){
			int above = mod( (i-1) , sizeX );
			int below = mod( (i+1) , sizeX );
			int left =  mod( (j-1) , sizeY );
			int right = mod( (j+1) , sizeY );

		
			tab[i][j].pSwoiSasiedzi[0] = & tab[above][j];
			tab[i][j].pSwoiSasiedzi[1] = & tab[above][right];
			tab[i][j].pSwoiSasiedzi[2] = & tab[i][right];

			tab[i][j].pSwoiSasiedzi[3] = & tab[below][right];
			tab[i][j].pSwoiSasiedzi[4] = & tab[below][j];

			tab[i][j].pSwoiSasiedzi[5] = NULL;
			tab[i][j].pSwoiSasiedzi[6] = NULL;
			tab[i][j].pSwoiSasiedzi[6] = NULL;

		}
	}

}