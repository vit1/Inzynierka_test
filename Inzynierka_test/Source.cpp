
#include <iostream>
#include <stdlib.h> 
#include "Cell.h"
#include "Point.h"
#include <time.h> 
#include <vector>
#include <map>
#include <cstdio>
#include "FreeImage.h"
#include <omp.h>
#include "Automata.h"
#include <fstream>
#include <mpi.h>
#include <cstdio>


#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


using namespace std;
enum{
	MOORE,
	VON_NEAUMANN,
	//PENTA_RIGHT,
	HEXA
};
//GOBAL MPI_____________________________________

int globalRank;
int mamsebuffor;


//GLOBAL VARIABLES***************************************************+
unsigned char* bmpData;
static int id;
static int button_x;
static int button_y;


const int sizeX = 600;
const int sizeY = 600;
const int PIXEL = 1 ;



const int buffSize = sizeY;
int sendBuffor[buffSize];
int recvBuffor[buffSize];

int sendMerge[sizeX/2][sizeY];
int recvMerge[sizeX/2][sizeY];

Automata autom(sizeX,sizeY);
static int growth = VON_NEAUMANN;
MPI_Status status;
MPI_Status status2;

//*****************************************************************************




void addIDs(){
	vector<Color> vect;
	vect.push_back(autom.tab[0][0].kolor);
	id =0;

	for(int i=0 ; i< sizeX ; i ++)
		for(int j=0 ; j<sizeY ; j++){
			int counter =0 ;

			for(unsigned int k=0 ; k<vect.size() ; k++){
				if(autom.tab[i][j].kolor.R == vect.at(k).R && autom.tab[i][j].kolor.G == vect.at(k).G && autom.tab[i][j].kolor.B == vect.at(k).B)
					counter++;

			}

			if(counter == 0){
				vect.push_back(autom.tab[i][j].kolor);
			}

			for(unsigned int l=0 ; l<vect.size() ; l++){
				if(autom.tab[i][j].kolor.R == vect.at(l).R && autom.tab[i][j].kolor.G == vect.at(l).G && autom.tab[i][j].kolor.B == vect.at(l).B)
					autom.tab[i][j].setGrainID(l);
			}

		}


}


//************************OPENGL FUNCTIONS *********************

	void initRendering();
	void handleResize(int w, int h);
	void drawScene();
	static void MouseButton (int button , int state , int x , int y);
	void handleKeypress(unsigned char key, 	int x, int y) ;
	void printTab(Cell ** tab);
	void menu(int value);


//*******************************************************************
int main(int argc, char** argv) {


	// Open MPI
	
	int  nproc, rank;
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	globalRank = rank;
	MPI_Comm_size( MPI_COMM_WORLD, &nproc );
	printf( "Hello world from proc: %d , of %d \n" , rank, nproc);


	

	srand (time(NULL));
	
	autom.tab = autom.createTab(autom.tab);
	autom.OldTab = autom.createTab(autom.OldTab);
	


		//autom.podczepSasiadowMoorea(autom.tab, autom.OldTab , sizeX,sizeY);
		autom.podczepSwpochSasiadowMoorea(autom.tab, sizeX,sizeY);
		//autom.podczepSwpochSasiadowVonNeumanna(autom.tab, sizeX,sizeY);
		
		autom.fillForMC(autom.tab);
	
			cout << autom.tab[20][20].getGrainID() << endl;
glutInit( & argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowSize( sizeX * PIXEL	, sizeY * PIXEL );
    glutCreateWindow( "Inz" );



	glViewport( 10, 10, 600, 600 );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0,PIXEL*sizeX,PIXEL*sizeY,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor( 0.0, 0.1, 0.0, 0.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	glFlush();
	glutSwapBuffers();

	
	glutCreateMenu( menu );
	 #ifdef WIN32
    
    glutAddMenuEntry( "Rozrost Moorea", MOORE );

    glutAddMenuEntry( "Rozrost Von Neumanna", VON_NEAUMANN );
	//glutAddMenuEntry( "Penta Right", PENTA_RIGHT );
	glutAddMenuEntry( "HEXA", HEXA );

    #else
    
    glutAddMenuEntry( "Rozrost Moorea", MOORE );
    glutAddMenuEntry( "Rozrost Von Neumanna", VON_NEAUMANN );
//	glutAddMenuEntry( "Penta Right", PENTA_RIGHT );
	glutAddMenuEntry( "HEXA", HEXA );
    #endif
    

   
	  glutAttachMenu( GLUT_RIGHT_BUTTON );
 
	
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	 glutMouseFunc( MouseButton );
	//  glutMotionFunc( MouseMotion );
	


	glutMainLoop(); //Start the main loop.  glutMainLoop doesn't return.
	MPI_Finalize();
	return 0; 
}



	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void MouseButton (int button , int state , int x , int y){

	if (button == GLUT_LEFT_BUTTON)
	{
		if ( state == GLUT_DOWN){

		
		}
		
	}
	
}

 //***************************************************
void handleKeypress(unsigned char key, int x, int y) {    
	
	switch (key) {
	case '+':{
		cout<<"+"<<endl;

		if(globalRank == 0){
		#pragma omp parallel for
		for(int iter =0 ; iter < 3 ; ++iter){

			for(int i=0 ; i<sizeX/2 ; ++i){
				for(int j=0 ; j<sizeY ; ++j){
					autom.monteCarlo(autom.tab , i , j);
				

				}
				

			}
		}//iter

	//WYSLANIE/ODEBRANIE BRZEGOW ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

			for(int i=0 ; i< buffSize ; ++i){
				//sendBuffor[i] = autom.tab[i][sizeY/2-1].getGrainID();
				sendBuffor[i] = autom.tab[sizeY/2-1][i].getGrainID();
			}

			
			MPI_Send(sendBuffor , buffSize, MPI_INT, 1, 0, MPI_COMM_WORLD);
			MPI_Recv(recvBuffor, buffSize, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD,  &status);
			//cout << "Otrzymalem od prc 1: " << recvBuffor[2] << endl;

			for(int i=0 ; i<buffSize ; ++i) {
				autom.tab[sizeY/2][i].setGrainID( recvBuffor[i] );
				//autom.tab[i][sizeY/2].setGrainID( recvBuffor[i] );
			}
			
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//ODEBRANIE MERGE!!!!!!!!!!!!!!!!!
		
			MPI_Recv( &(recvMerge[0][0]), (sizeX/2)*sizeY, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD,  &status2);


			cout << "odebralem recvMerge " << recvMerge[2][2] << endl;
			for(int i=0 ; i<sizeX/2 ; ++i){
				for(int j=0 ; j<sizeY ; ++j){
					autom.tab[i+sizeX/2][j].setGrainID(recvMerge[i][j]);

				}
			}

		}//ifRANK



	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~RANK_1~~~~~~~~~~~~~~~~~
		else if(globalRank == 1){
			#pragma omp parallel for
		for(int iter =0 ; iter < 3 ; ++iter){

			for(int i=sizeX/2 ; i<sizeX ; ++i){
				for(int j=0 ; j<sizeY ; ++j){
					autom.monteCarlo(autom.tab , i , j);
		
				}
			}
		}//iter
		
	//WYSLANIE/ODEBRANIE BRZEGOW ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		for(int i=0 ; i< buffSize ; ++i)
			sendBuffor[i] = autom.tab[i][sizeY/2 ].getGrainID();
			

		MPI_Send(sendBuffor , buffSize, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Recv(recvBuffor, buffSize, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD,  &status);

		for(int i=0 ; i<buffSize ; ++i) {
				autom.tab [sizeY/2 -1][i].setGrainID ( recvBuffor[i] );
			}

		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		//Wyslanie MERGE!!!!!!!!!!!!!!!!!

		for(int i=0 ; i<sizeX/2 ; i++)
			for(int j=0 ; j<sizeY ; j++){
				sendMerge[i][j] = autom.tab[sizeX/2 + i][j].getGrainID();
				

			}

		cout << sendMerge[2][2] << " : send merger22" << endl; ;
		MPI_Send(&(sendMerge[0][0]) , (sizeX/2)*sizeY, MPI_INT, 0, 0, MPI_COMM_WORLD);
		
		}//ifRank
	   //} //iter
		printTab(autom.tab);
		break;
			 
			 
			 }

	case 'p' : {
			printTab(autom.tab);
			break;
		}


	case 'i' :{
			for(int i=0 ; i<sizeX ; i++ ){
				for(int j=0 ; j<sizeY ; j++){
					cout<<autom.tab[i][j].getGrainID() <<" ";
				}
				cout<<endl;
			}
			break;
		}


		case 27: //Escape key
			exit(0); //Exit the program
	}
}

void menu(int value){
	switch (value){
	case  MOORE :
		{
			growth = MOORE;
			cout<<"MOOREA"<<endl;
			break;
		}
	case VON_NEAUMANN :
		{
			growth = VON_NEAUMANN;
			cout<<"Von neumann "<<endl;
			break;
		}
	case HEXA :
		{
			growth = HEXA;
			cout<< "Heksa"<<endl;
			break;
		}

	}

}


void drawScene() {
	glClearColor( 0.0, 0.1, 0.0, 0.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	printTab(autom.tab);
	glutSwapBuffers();
	

}

	
	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void initRendering() {
	glEnable(GL_DEPTH_TEST);

}

	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void handleResize(int w, int h) {
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);
	printTab(autom.tab);
	
}
	

void maluj(Color kol){
	glColor3ub(kol.R, kol.G , kol.B);

}
void printTab(Cell **tab){
	
	glPointSize(PIXEL);

	if(globalRank == 0  ){


	for( int i=0 ; i<sizeX ; i++){
		for(int  j=0 ; j<sizeY ; j++){
			if(tab[i][j].getState() == 1 && tab[i][j].czyGranica  == true) {
			
					tab[i][j].kolor.setColor(100,100,100);
				
			}

			if(tab[i][j].getGrainID() % 2 == 0 ){
				tab[i][j].kolor.R = 255;
				tab[i][j].kolor.G = 0;
				tab[i][j].kolor.B = 0;
			}
			else if(tab[i][j].getGrainID() % 3 == 0 ){
				tab[i][j].kolor.R = 0;
				tab[i][j].kolor.G = 255;
				tab[i][j].kolor.B = 0;
			}
			else if(tab[i][j].getGrainID() % 5 == 0 ){
				tab[i][j].kolor.R = 0;
				tab[i][j].kolor.G = 0;
				tab[i][j].kolor.B = 255;
			}
			else if(tab[i][j].getGrainID() % 7 == 0 ){
				tab[i][j].kolor.R = 255;
				tab[i][j].kolor.G = 255;
				tab[i][j].kolor.B = 0;
			}
				else if(tab[i][j].getGrainID() % 11 == 0 ){
				tab[i][j].kolor.R = 255;
				tab[i][j].kolor.G = 255;
				tab[i][j].kolor.B = 255;
			}
			else if(tab[i][j].getGrainID() % 13 == 0 ){
				tab[i][j].kolor.R = 0;
				tab[i][j].kolor.G = 255;
				tab[i][j].kolor.B = 255;
			}
			else{
				tab[i][j].kolor.R = 110;
				tab[i][j].kolor.G = 100;
				tab[i][j].kolor.B = 100;
			}
			
			maluj(tab[i][j].kolor);
		
		    glBegin(GL_POINTS);
					glVertex2f(i*PIXEL , j*PIXEL);
			glEnd();
			
		}
	
	}
	}// rank 0 


	
	if(globalRank == 1  ){


	for( int i=sizeX/2 - 1 ; i<sizeX ; i++){
		for(int  j=0 ; j<sizeY ; j++){
			if(tab[i][j].getState() == 1 && tab[i][j].czyGranica  == true) {
			
					tab[i][j].kolor.setColor(100,100,100);
				
			}

		
			if(tab[i][j].getGrainID() % 2 == 0 ){
				tab[i][j].kolor.R = 255;
				tab[i][j].kolor.G = 0;
				tab[i][j].kolor.B = 0;
			}
			else if(tab[i][j].getGrainID() % 3 == 0 ){
				tab[i][j].kolor.R = 0;
				tab[i][j].kolor.G = 255;
				tab[i][j].kolor.B = 0;
			}
			else if(tab[i][j].getGrainID() % 5 == 0 ){
				tab[i][j].kolor.R = 0;
				tab[i][j].kolor.G = 0;
				tab[i][j].kolor.B = 255;
			}
			else if(tab[i][j].getGrainID() % 7 == 0 ){
				tab[i][j].kolor.R = 255;
				tab[i][j].kolor.G = 255;
				tab[i][j].kolor.B = 0;
			}
				else if(tab[i][j].getGrainID() % 11 == 0 ){
				tab[i][j].kolor.R = 255;
				tab[i][j].kolor.G = 255;
				tab[i][j].kolor.B = 255;
			}
			else if(tab[i][j].getGrainID() % 13 == 0 ){
				tab[i][j].kolor.R = 0;
				tab[i][j].kolor.G = 255;
				tab[i][j].kolor.B = 255;
			}
			else{
				tab[i][j].kolor.R = 110;
				tab[i][j].kolor.G = 100;
				tab[i][j].kolor.B = 100;
			}
			
			maluj(tab[i][j].kolor);
		
		    glBegin(GL_POINTS);
					glVertex2f(i*PIXEL , j*PIXEL);
			glEnd();
			
		}
	
	}
	}// rank 0 



	glutSwapBuffers(); // mocno zwalnia, dlatego  poza forem
	}


