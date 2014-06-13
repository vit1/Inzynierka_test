
#include <iostream>
#include <stdlib.h> //Needed for "exit" function
#include "Cell.h"
#include "Point.h"
#include <time.h> 
#include <vector>
#include <map>

//FUNCTION DEFINITIONS


//FUNCTION DEFINITIONS


#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;
//GLOBAL VARIABLES
static int id;
static int button_x;
static int button_y;

const int rozm = 50;
const int PIXEL = 10 ;

static Cell **tab ;
static Cell **OldTab;


typedef vector<Point> vPoints;
typedef map<int, vPoints> Mapa;

typedef map<int, vPoints>::const_iterator MapIterator;

Mapa storage;
vPoints vectPoints;



//*******************************************************************
//*******************************************************************
int mod (int a, int b)
{
   if(b < 0) //you can check for b == 0 separately and do what you want
     return mod(-a, -b);   
   int ret = a % b;
   if(ret < 0)
     ret+=b;
   return ret;
}

//*******************************************************************

	void granice(Cell **tab, int rozmiar);
	void podczepSasiadowMoorea(Cell **tab, Cell **oldTab , int size);
	void podczepSwpochSasiadowMoorea(Cell **tab , int size);

//*******************************************************************

	void initRendering();
	void handleResize(int w, int h);
	void drawScene();
	static void MouseButton (int button , int state , int x , int y);
	void handleKeypress(unsigned char key, 	int x, int y) ;

//*******************************************************************

	int energyCount(Cell **tab , int rozmiar, int x, int y);
	void monteCarlo(Cell** tab , int rozmiar, int x, int y);
	void rozrostMoorea(Cell **tab, Cell **oldTab, int size);

//*******************************************************************

	Cell** createTab(Cell **tab);
	Cell** testFill(Cell **anyTab);
	void fillForMC(Cell **tab);
	void addToVector(Cell **tab, int rozm);
	void printMatrix(Cell **tab);

//TOD	void subStructure(Cell **tab, int rozm);
	void zerujPrzedSub(Cell **tab, int rozm);
	void dodajSubZiarna(Cell **tab, int rozm , int ileZiaren);
	void setColor(int ID, int x, int y);
	void printTab(Cell ** tab);


	void searchForGrains(Cell **tab, int rozm);

//*******************************************************************
int main(int argc, char** argv) {
	 srand (time(NULL));
	tab = createTab(tab);
	OldTab = createTab(OldTab);
//	tab[1][1].state = 1;
	//tab[1][1].id_ziarna = 2;
	//fillForMC(tab);

	podczepSasiadowMoorea(tab, tab , rozm);
	podczepSwpochSasiadowMoorea(tab, rozm);
	
	
	
	//for(int i=0 ; i<rozm ; i++)
	//{
	//	for(int j=0 ; j<rozm ; j++){

	//		cout<<tab[i][j].state<<" ";
	//	}
	//	cout <<endl;


	//}
	

	
glutInit( & argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowSize( 600,600 );
    glutCreateWindow( "Inz" );
	glViewport( 0, 0, 600, 600 );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,PIXEL*rozm,PIXEL*rozm,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor( 0.0, 0.1, 0.0, 0.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	glFlush();
	glutSwapBuffers();
	//Create the window

	//initRendering(); //Initialize rendering
	
	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	 glutMouseFunc( MouseButton );
	//  glutMotionFunc( MouseMotion );
	glutMainLoop(); //Start the main loop.  glutMainLoop doesn't return.
	return 0; //This line is never reached
}



	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void MouseButton (int button , int state , int x , int y){

	if (button == GLUT_LEFT_BUTTON)
	{
		if ( state == GLUT_DOWN)
		{
		button_x = x;
		button_y = y;

		cout<< "Coord X: "<<button_x <<endl;
		cout<< "Coord Y: "<<button_y <<endl;

		fillForMC(tab);
		//rozrostMoorea(tab ,  OldTab, rozm);
		printMatrix(tab);
		
	
		}
		
	}

}

 //***************************************************
void handleKeypress(unsigned char key, //The key that was pressed
					int x, int y) {    //The current mouse coordinates
	switch (key) {
	case '+':{
		cout<<"+"<<endl;
		for(int i=0 ; i<rozm ; i++){
			for(int j=0 ; j<rozm ; j++){
				monteCarlo(tab, rozm , i , j);
				

			}

		}
		printTab(tab);
		break;
			 }
	case '-':
		{
			granice(tab,rozm);
			printTab(tab);
				
				break;

		}
	case '*' :
		{

			searchForGrains(tab,rozm);

		
			break;
		}
	case 'q' :
		{
			cout<<"Q"<<endl;
			for (auto it=storage.begin(); it!=storage.end(); ++it){
			cout<<it->first<<endl;

			for(unsigned int i=0 ;i< it->second.size() ; ++i){
				cout<<"ID: "<<it->first<<"X/Y: "<<it->second[i].x<<" "<<it->second[i].y<<endl;
			}
			}
		
			
			break;
		}
	case 'p' :
		{
			//printMatrix(tab);
			printTab(tab);
			break;
		}
	case 's' :
		{
			//addToVector(tab, rozm);
			//zerujPrzedSub(tab,rozm);
			dodajSubZiarna(tab,rozm,4);

	for(int i=0 ; i<rozm ; i++)
	{
		for(int j=0 ; j<rozm ; j++){

			cout<<tab[i][j].id_ziarna<<" ";
		}
		cout <<endl;


	}
			break;
		}
		case 27: //Escape key
			exit(0); //Exit the program
	}
}
	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Cell** createTab(Cell **tab){
	tab = new Cell*[rozm];

	for(int i=0 ; i<rozm ; i++){
			tab[i] = new Cell[rozm];		
			
}
	return tab;
}
	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Cell** testFill(Cell **anyTab){
	for(int i=0 ; i<rozm ; i++){
		for(int j=0 ; j<rozm ; j++){
			anyTab[i][j].id_ziarna = i;

		}


	
	}
	return anyTab;
}
	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void printMatrix(Cell **tab){

	glPointSize(PIXEL);
	for(int i=0 ; i<rozm ; i++){
		for(int j=0 ; j<rozm ; j++){
			
			if(tab[i][j].state == 1){
				if(tab[i][j].id_ziarna == 1){
					glColor3f(0.0, 1.0,  0.0);
				
				}
				if(tab[i][j].id_ziarna == 2)
					glColor3f(0.0, 1.0,  0.0);
				if(tab[i][j].id_ziarna == 3)
					glColor3f(0.0, 0.0,  1.0);
				if(tab[i][j].czyGranica == true)
					glColor3f(1.0,1.0,1.0);
				glBegin(GL_POINTS);
					glVertex2f(i*PIXEL , j*PIXEL);
				glEnd();

			}

		}
	}
	glutSwapBuffers(); // mocno zwalnia, dlatego  poza forem
}
	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void rozrostMoorea(Cell **tab, Cell **oldTab, int size){
	

	for(int i=0 ; i<rozm ; i++){
		for(int j=0 ; j<rozm ; j++){
			oldTab[i][j].state = tab[i][j].state;
			oldTab[i][j].id_ziarna  = tab[i][j].id_ziarna;

		}
	}

	for(int i=0 ; i<rozm ; i++){
		for(int j=0 ; j<rozm ; j++){
			if( oldTab[i][j].state == 1 ){
				if(tab[i][j].pNeighbours[0]->state == 0 ){
			
					tab[i][j].pSwoiSasiedzi[0]->state = 1;
					tab[i][j].pSwoiSasiedzi[0]->id_ziarna = tab[i][j].id_ziarna;
				}
				if(tab[i][j].pNeighbours[1]->state == 0 ){
			
					tab[i][j].pSwoiSasiedzi[1]->state = 1;
					tab[i][j].pSwoiSasiedzi[1]->id_ziarna = tab[i][j].id_ziarna;
				}
				if(tab[i][j].pNeighbours[2]->state == 0 ){
			
					tab[i][j].pSwoiSasiedzi[2]->state = 1;
					tab[i][j].pSwoiSasiedzi[2]->id_ziarna = tab[i][j].id_ziarna;
				}

				if(tab[i][j].pNeighbours[3]->state == 0 ){
			
					tab[i][j].pSwoiSasiedzi[3]->state = 1;
					tab[i][j].pSwoiSasiedzi[3]->id_ziarna = tab[i][j].id_ziarna;
				}
				if(tab[i][j].pNeighbours[4]->state == 0 ){
			
					tab[i][j].pSwoiSasiedzi[4]->state = 1;
					tab[i][j].pSwoiSasiedzi[4]->id_ziarna = tab[i][j].id_ziarna;
				}
				if(tab[i][j].pNeighbours[5]->state == 0 ){
			
					tab[i][j].pSwoiSasiedzi[5]->state = 1;
					tab[i][j].pSwoiSasiedzi[5]->id_ziarna = tab[i][j].id_ziarna;
				}
				if(tab[i][j].pNeighbours[6]->state == 0 ){
			
					tab[i][j].pSwoiSasiedzi[6]->state = 1;
					tab[i][j].pSwoiSasiedzi[6]->id_ziarna = tab[i][j].id_ziarna;
				}
				if(tab[i][j].pNeighbours[6]->state == 0 ){
			
					tab[i][j].pSwoiSasiedzi[6]->state = 1;
					tab[i][j].pSwoiSasiedzi[6]->id_ziarna = tab[i][j].id_ziarna;
				}
				if(tab[i][j].pNeighbours[7]->state == 0 ){
			
					tab[i][j].pSwoiSasiedzi[7]->state = 1;
					tab[i][j].pSwoiSasiedzi[7]->id_ziarna = tab[i][j].id_ziarna;
				}
				
			}
			
			
		}

		}
	printMatrix(tab);


	}

	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void drawScene() {
	//Clear information from last draw
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho( 0 , rozm*PIXEL , rozm*PIXEL , 0 , -1, 1);
	//glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	//glLoadIdentity(); //Reset the drawing perspective
	glClearColor( 0.0, 0.1, 0.0, 0.0 );
	glClear( GL_COLOR_BUFFER_BIT );

	glutSwapBuffers();
	//printMatrix(tab);
	

}
//                        /END
  //
   //


	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void fillForMC(Cell **tab){
	for(int i=0 ; i < rozm ; i++){
		for(int j=0 ; j<rozm ; j++){
			tab[i][j].state = 1;
		//	tab[i][j].id_ziarna = rand() % 3 + 1;
			tab[i][j].id_ziarna = id;
			id++;
		
		}

	}

}
	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 int energyCount(Cell **tab , int rozmiar, int x, int y)	{
	
	int counter = 0;
	if( tab[x][y].id_ziarna != tab[x][y].pNeighbours[0]->id_ziarna )
		counter++;
	
	if( tab[x][y].id_ziarna != tab[x][y].pNeighbours[1]->id_ziarna )
		counter++;
	
	if( tab[x][y].id_ziarna != tab[x][y].pNeighbours[2]->id_ziarna )
		counter++;
	
	////////////////////////////////////////////////
	if( tab[x][y].id_ziarna != tab[x][y].pNeighbours[3]->id_ziarna )
		counter++;
	
	if( tab[x][y].id_ziarna != tab[x][y].pNeighbours[4]->id_ziarna )
		counter++;
	
	///////////////////////////////////////////////
	if( tab[x][y].id_ziarna != tab[x][y].pNeighbours[5]->id_ziarna )
		counter++;
	
	if( tab[x][y].id_ziarna != tab[x][y].pNeighbours[6]->id_ziarna )
		counter++;
	
	if( tab[x][y].id_ziarna != tab[x][y].pNeighbours[7]->id_ziarna )
		counter++;
	

	return counter;
}
 	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void monteCarlo(Cell** tab , int rozmiar, int x, int y){
	

	int ra = rand() % 8 + 0;

		int a= 0;

		int enePrzed = energyCount(tab, rozmiar,x,y);

		int oldColor = tab[x][y].id_ziarna;
		Color kol = tab[x][y].kolor;
		
		if(tab[x][y].id_ziarna != tab[x][y].pNeighbours[0]->id_ziarna   && ra == 0)
			tab[x][y].id_ziarna = tab[x][y].pNeighbours[0]->id_ziarna;
		
		if(tab[x][y].id_ziarna != tab[x][y].pNeighbours[1]->id_ziarna   && ra == 1)
			tab[x][y].id_ziarna = tab[x][y].pNeighbours[1]->id_ziarna;
		
		if(tab[x][y].id_ziarna != tab[x][y].pNeighbours[2]->id_ziarna   && ra == 2)
			tab[x][y].id_ziarna = tab[x][y].pNeighbours[2]->id_ziarna;
		
		////////////////////////////////////////////////
		if(tab[x][y].id_ziarna != tab[x][y].pNeighbours[3]->id_ziarna   && ra == 3)
			tab[x][y].id_ziarna = tab[x][y].pNeighbours[3]->id_ziarna;
		
		if(tab[x][y].id_ziarna != tab[x][y].pNeighbours[4]->id_ziarna   && ra == 4)
			tab[x][y].id_ziarna = tab[x][y].pNeighbours[4]->id_ziarna;
		
		///////////////////////////////////////////////
		if(tab[x][y].id_ziarna != tab[x][y].pNeighbours[5]->id_ziarna   && ra == 5)
			tab[x][y].id_ziarna = tab[x][y].pNeighbours[5]->id_ziarna;
		
		if(tab[x][y].id_ziarna != tab[x][y].pNeighbours[6]->id_ziarna   && ra == 6)
			tab[x][y].id_ziarna = tab[x][y].pNeighbours[6]->id_ziarna;
		
		if(tab[x][y].id_ziarna != tab[x][y].pNeighbours[7]->id_ziarna   && ra == 7)
			tab[x][y].id_ziarna = tab[x][y].pNeighbours[7]->id_ziarna;
		
		int enePo = energyCount(tab, rozmiar, x, y);
		
		if(enePrzed < enePo){
			tab[x][y].id_ziarna = oldColor;
			tab[x][y].kolor = kol;
		}

		
		
 }
	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void initRendering() {
	//Makes 3D drawing work when something is in front of something else
	glEnable(GL_DEPTH_TEST);

}

//Called when the window is resized
	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void handleResize(int w, int h) {
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);
	

}
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
	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void granice(Cell **tab, int rozmiar){


	for(int i=0 ; i<rozmiar ; ++i){
		for(int j=0 ; j<rozmiar ; ++j){

				if(tab[i][j].id_ziarna != tab[i][j].pSwoiSasiedzi[0]->id_ziarna   && tab[i][j].pSwoiSasiedzi[0]->czyGranica == false)
					tab[i][j].czyGranica = true;
				if(tab[i][j].id_ziarna != tab[i][j].pSwoiSasiedzi[1]->id_ziarna   && tab[i][j].pSwoiSasiedzi[1]->czyGranica == false)
					tab[i][j].czyGranica = true;
				if(tab[i][j].id_ziarna != tab[i][j].pSwoiSasiedzi[2]->id_ziarna   && tab[i][j].pSwoiSasiedzi[2]->czyGranica == false)
					tab[i][j].czyGranica = true;
				////////////////////////////////////////////////
				if(tab[i][j].id_ziarna != tab[i][j].pSwoiSasiedzi[3]->id_ziarna   && tab[i][j].pSwoiSasiedzi[3]->czyGranica == false)
					tab[i][j].czyGranica = true;
				if(tab[i][j].id_ziarna != tab[i][j].pSwoiSasiedzi[4]->id_ziarna   && tab[i][j].pSwoiSasiedzi[4]->czyGranica == false)
					tab[i][j].czyGranica = true;
				///////////////////////////////////////////////
				if(tab[i][j].id_ziarna != tab[i][j].pSwoiSasiedzi[5]->id_ziarna   && tab[i][j].pSwoiSasiedzi[5]->czyGranica == false)
					tab[i][j].czyGranica = true;
				if(tab[i][j].id_ziarna != tab[i][j].pSwoiSasiedzi[6]->id_ziarna   && tab[i][j].pSwoiSasiedzi[6]->czyGranica == false)
					tab[i][j].czyGranica = true;
				if(tab[i][j].id_ziarna != tab[i][j].pSwoiSasiedzi[7]->id_ziarna   && tab[i][j].pSwoiSasiedzi[7]->czyGranica == false)
					tab[i][j].czyGranica = true;
		}
	}

}





	void zerujPrzedSub(Cell **tab, int rozm){
		for(int i=0 ; i<rozm ; i++){
			for(int j=0 ; j<rozm ; j++){

				if(tab[i][j].czyGranica == false)
					tab[i][j].state = 0;
				tab[i][j].id_ziarna = 0;
			}

		}

	}

void dodajSubZiarna(Cell **tab, int rozm , int ileZiaren){

	for(auto it=storage.begin() ; it!= storage.end() ; it++){
					for(int i=0 ; i<ileZiaren ; i++){
						int random = rand() %  it->second.size();
						Point coord(0,0);
						coord = it->second[random];

						tab[coord.x][coord.y].state = 2;
					}
			}
			

		
}


void setColor(int ID, int x , int y){

	
	
}
void maluj(Color kol){
	glColor3ub(kol.R, kol.G , kol.B);

}
void printTab(Cell **tab){

	glPointSize(PIXEL);
	
	for(int i=0 ; i<rozm ; i++){
		for(int j=0 ; j<rozm ; j++){
			
			if(tab[i][j].id_ziarna % 16 == 0)//Czerw
				tab[i][j].kolor.setColor(255, 0 , 0);

			else if(tab[i][j].id_ziarna % 16 == 1)
				tab[i][j].kolor.setColor(0, 255 , 0);

			else if(tab[i][j].id_ziarna % 16 == 2)
				tab[i][j].kolor.setColor(0, 255 , 0);

			else if(tab[i][j].id_ziarna % 16 == 3)
				tab[i][j].kolor.setColor(0, 0 , 255);

			else if(tab[i][j].id_ziarna % 16 == 4)
				tab[i][j].kolor.setColor(255,255  , 0);

			else if(tab[i][j].id_ziarna % 16 == 5)
				tab[i][j].kolor.setColor(0, 255 ,255);

			else if(tab[i][j].id_ziarna % 16 == 6)
				tab[i][j].kolor.setColor(255, 0 , 255);

			else if(tab[i][j].id_ziarna % 16 == 7)
				tab[i][j].kolor.setColor(200, 120 , 100);

			else if(tab[i][j].id_ziarna % 16 == 8)
				tab[i][j].kolor.setColor(20, 120 ,100);

			else if(tab[i][j].id_ziarna % 16 == 9)
				tab[i][j].kolor.setColor(150, 50 , 100);

			else if(tab[i][j].id_ziarna % 16 == 10)
				tab[i][j].kolor.setColor(40,255  ,100);

			else if(tab[i][j].id_ziarna % 16 == 11)
				tab[i][j].kolor.setColor(100,200  , 250);

			else if(tab[i][j].id_ziarna % 16 == 12)
				tab[i][j].kolor.setColor(250, 200, 80);

			else if(tab[i][j].id_ziarna % 16 == 13)
				tab[i][j].kolor.setColor(100, 80 , 160);

			else if(tab[i][j].id_ziarna % 16 == 14)
				tab[i][j].kolor.setColor(160, 30 , 90);
			else if(tab[i][j].id_ziarna % 16 == 15)
				tab[i][j].kolor.setColor(180, 120 , 90);

			if(tab[i][j].state == 2)
				tab[i][j].kolor.setColor(255,255,255);
		
			if(tab[i][j].czyGranica == true)
				tab[i][j].kolor.setColor(204,204,204);
			maluj(tab[i][j].kolor);
			glBegin(GL_POINTS);
					glVertex2f(i*PIXEL , j*PIXEL);
				glEnd();
			
	}
	
}
	glutSwapBuffers(); // mocno zwalnia, dlatego  poza forem
}

void searchForGrains(Cell **tab, int rozm){
	
	
	for(int i=0 ; i<rozm ; i++){
		for(int j=0 ; j<rozm ; j++){
			if(tab[i][j].state == 1 && tab[i][j].czyGranica == false){
				
				storage.insert(pair<int, vector<Point> >(tab[i][j].id_ziarna, vector<Point>()));
				Point p(i,j);
				storage[tab[i][j].id_ziarna].push_back(p);
			}
		}
	}



	
}
