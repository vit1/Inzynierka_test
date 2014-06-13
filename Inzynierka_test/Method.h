#ifndef METHOD_H
#define METHOD_H
#pragma once



namespace methods{
int mod (int a, int b)
{
   if(b < 0) 
     return mod(-a, -b);   
   int ret = a % b;
   if(ret < 0)
     ret+=b;
   return ret;
}


//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
									//VON NEUMANN
//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	void podczepSasiadowVonNeumanna(Cell **tab, Cell **oldTab , int size){
	for(int i=0 ; i<size ; ++i){
		for(int j=0 ; j<size ; ++j){
			int above = mod( (i-1) , size );
			int below = mod( (i+1) , size );
			int left =  mod( (j-1) , size );
			int right = mod( (j+1) , size );

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

void podczepSwpochSasiadowVonNeumanna(Cell **tab , int size){
	for(int i=0 ; i<size ; ++i){
		for(int j=0 ; j<size ; ++j){
			int above = mod( (i-1) , size );
			int below = mod( (i+1) , size );
			int left =  mod( (j-1) , size );
			int right = mod( (j+1) , size );

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



void rozrostVonNeumanna(Cell **tab, Cell **oldTab, int rozm){
	

	for(int i=0 ; i<rozm ; i++){
		for(int j=0 ; j<rozm ; j++){
			oldTab[i][j].state = tab[i][j].state;
			oldTab[i][j].id_ziarna  = tab[i][j].id_ziarna;
			oldTab[i][j].czyGranica = tab[i][j].czyGranica;
			oldTab[i][j].kolor = tab[i][j].kolor;


		}
	}

	for(int i=0 ; i<rozm ; i++){
		for(int j=0 ; j<rozm ; j++){
			if( oldTab[i][j].state == 2 ){

				for(int iter=0 ; iter<=3 ; ++iter){
					if(tab[i][j].pNeighbours[iter]->state != 2  && tab[i][j].previous_id == tab[i][j].pSwoiSasiedzi[iter]->previous_id ){
			
					tab[i][j].pSwoiSasiedzi[iter]->state = tab[i][j].state;
					tab[i][j].pSwoiSasiedzi[iter]->id_ziarna = tab[i][j].id_ziarna;
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


void podczepSasiadowMoorea(Cell **tab, Cell **oldTab , int size){
	for(int i=0 ; i<size ; ++i){
		for(int j=0 ; j<size ; ++j){
			int above = mod( (i-1) , size );
			int below = mod( (i+1) , size );
			int left =  mod( (j-1) , size );
			int right = mod( (j+1) , size );

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

void podczepSwpochSasiadowMoorea(Cell **tab , int size){
	for(int i=0 ; i<size ; ++i){
		for(int j=0 ; j<size ; ++j){
			int above = mod( (i-1) , size );
			int below = mod( (i+1) , size );
			int left =  mod( (j-1) , size );
			int right = mod( (j+1) , size );

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


void rozrostMoorea(Cell **tab, Cell **oldTab, int rozm){
	

	for(int i=0 ; i<rozm ; i++){
		for(int j=0 ; j<rozm ; j++){
			oldTab[i][j].state = tab[i][j].state;
			oldTab[i][j].id_ziarna  = tab[i][j].id_ziarna;
			//oldTab[i][j].czyGranica = tab[i][j].czyGranica;
			oldTab[i][j].kolor = tab[i][j].kolor;
			tab[i][j].czyGranica = false;

		}
	}

	for(int i=0 ; i<rozm ; i++){
		for(int j=0 ; j<rozm ; j++){
			if( oldTab[i][j].state == 2 ){
				for(int iter=0 ; iter<=7 ; ++iter){
					if(tab[i][j].pNeighbours[iter]->state != 2  && tab[i][j].previous_id == tab[i][j].pSwoiSasiedzi[iter]->previous_id ){
			
					tab[i][j].pSwoiSasiedzi[iter]->state = tab[i][j].state;
					tab[i][j].pSwoiSasiedzi[iter]->id_ziarna = tab[i][j].id_ziarna;
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

void podczepSasiadowHexR(Cell **tab, Cell **oldTab , int size){
	for(int i=0 ; i<size ; ++i){
		for(int j=0 ; j<size ; ++j){
			int above = mod( (i-1) , size );
			int below = mod( (i+1) , size );
			int left =  mod( (j-1) , size );
			int right = mod( (j+1) , size );

			
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
void podczepSwpochSasiadowHexRIGHT(Cell **tab , int size){
	for(int i=0 ; i<size ; ++i){
		for(int j=0 ; j<size ; ++j){
			int above = mod( (i-1) , size );
			int below = mod( (i+1) , size );
			int left =  mod( (j-1) , size );
			int right = mod( (j+1) , size );


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
void rozrostHexaRIGHT(Cell **tab, Cell **oldTab, int rozm){
	

	for(int i=0 ; i<rozm ; i++){
		for(int j=0 ; j<rozm ; j++){
			oldTab[i][j].state = tab[i][j].state;
			oldTab[i][j].id_ziarna  = tab[i][j].id_ziarna;
			oldTab[i][j].czyGranica = tab[i][j].czyGranica;
			oldTab[i][j].kolor = tab[i][j].kolor;

		}
	}

	for(int i=0 ; i<rozm ; i++){
		for(int j=0 ; j<rozm ; j++){
			if( oldTab[i][j].state == 2 ){
				if(tab[i][j].pNeighbours[0]->state != 2 && tab[i][j].pNeighbours[0]->czyGranica == false ){
			
					tab[i][j].pSwoiSasiedzi[0]->state = tab[i][j].state;
					tab[i][j].pSwoiSasiedzi[0]->id_ziarna = tab[i][j].id_ziarna;
					tab[i][j].pSwoiSasiedzi[0]->kolor = tab[i][j].kolor;
				}
				if(tab[i][j].pNeighbours[1]->state != 2 && tab[i][j].pNeighbours[1]->czyGranica == false ){
			
					tab[i][j].pSwoiSasiedzi[1]->state = tab[i][j].state;
					tab[i][j].pSwoiSasiedzi[1]->id_ziarna = tab[i][j].id_ziarna;
					tab[i][j].pSwoiSasiedzi[1]->kolor = tab[i][j].kolor;
				}
			    if(tab[i][j].pNeighbours[2]->state != 2 && tab[i][j].pNeighbours[2]->czyGranica == false ){
			
					tab[i][j].pSwoiSasiedzi[2]->state = tab[i][j].state;
					tab[i][j].pSwoiSasiedzi[2]->id_ziarna = tab[i][j].id_ziarna;
					tab[i][j].pSwoiSasiedzi[2]->kolor = tab[i][j].kolor;
				}

		  	    if(tab[i][j].pNeighbours[3]->state != 2 && tab[i][j].pNeighbours[3]->czyGranica == false ){
			
					tab[i][j].pSwoiSasiedzi[3]->state = tab[i][j].state;
					tab[i][j].pSwoiSasiedzi[3]->id_ziarna = tab[i][j].id_ziarna;
					tab[i][j].pSwoiSasiedzi[3]->kolor = tab[i][j].kolor;
				}
				if(tab[i][j].pNeighbours[4]->state != 2 && tab[i][j].pNeighbours[4]->czyGranica == false ){
			
					tab[i][j].pSwoiSasiedzi[4]->state = tab[i][j].state;
					tab[i][j].pSwoiSasiedzi[4]->id_ziarna = tab[i][j].id_ziarna;
					tab[i][j].pSwoiSasiedzi[4]->kolor = tab[i][j].kolor;
				}
		     	if(tab[i][j].pNeighbours[5]->state != 2 && tab[i][j].pNeighbours[5]->czyGranica == false ){
			
					tab[i][j].pSwoiSasiedzi[5]->state = tab[i][j].state;
					tab[i][j].pSwoiSasiedzi[5]->id_ziarna = tab[i][j].id_ziarna;
					tab[i][j].pSwoiSasiedzi[5]->kolor = tab[i][j].kolor;
				}
			
				
			}
			
			
		}

	}

	


	}


//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
								//	PENTA RIGHT
//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void podczepSwpochSasiadowPentaRight(Cell **tab , int size){
	for(int i=0 ; i<size ; ++i){
		for(int j=0 ; j<size ; ++j){
			int above = mod( (i-1) , size );
			int below = mod( (i+1) , size );
			int left =  mod( (j-1) , size );
			int right = mod( (j+1) , size );

		
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










}//end of namespace;

#endif 