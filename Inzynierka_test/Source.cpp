
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

//GLOBAL VARIABLES***************************************************
unsigned char* bmpData;
static int id;
static int button_x;
static int button_y;


const int sizeX = 400;
const int sizeY = 400;
const int PIXEL = 1 ;

Automata autom(sizeX,sizeY);
static int growth = MOORE;

//*******************************************************************
//unsigned char* readBMP(char* filename)
//{
//    int i;
//    FILE* f = fopen(filename, "rb");
//
//    if(f == NULL)
//        throw "Argument Exception";
//
//    unsigned char info[54];
//    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
//
//    // extract image height and width from header
//    int width = *(int*)&info[18];
//    int height = *(int*)&info[22];
//
//    cout << endl;
//    cout << "  Name: " << filename << endl;
//    cout << " Width: " << width << endl;
//    cout << "Height: " << height << endl;
//
//    int row_padded = (width*3 + 3) & (~3);
//    unsigned char* data = new unsigned char[row_padded];
//    unsigned char tmp;
//
//    for(int i = 0; i < height; i++)
//    {
//        fread(data, sizeof(unsigned char), row_padded, f);
//        for(int j = 0; j < width*3; j += 3)
//        {
//            // Convert (B, G, R) to (R, G, B)
//            tmp = data[j];
//            data[j] = data[j+2];
//            data[j+2] = tmp;
//
//           // cout << "R: "<< (int)data[j] << " G: " << (int)data[j+1]<< " B: " << (int)data[j+2]<< endl;
//			//autom.tab[i][j/3].kolor.setColor( (int)data[j] , (int)data[j+1] ,  (int)data[j+2] );
//        }
//    }
//
//	
//
//    fclose(f);
//    return data;
//}
	

unsigned char* readBMP(char* filename)
{
    int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);

    for(i = 0; i < size; i += 3)
    {
            unsigned char tmp = data[i];
            data[i] = data[i+2];
            data[i+2] = tmp;
			// cout << "R: "<< (int)data[i] << " G: " << (int)data[i+1]<< " B: " << (int)data[i+2]<< endl;
    }

	

    return data;
}




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
	void getScreenShot(){
		

		BYTE* pixels = new BYTE[ 3 *glutGet(GLUT_WINDOW_WIDTH) * glutGet(GLUT_WINDOW_HEIGHT)];
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glReadPixels(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), GL_RGB, GL_UNSIGNED_BYTE, pixels);

			
		FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 3 * glutGet(GLUT_WINDOW_WIDTH), 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
		FreeImage_Save(FIF_BMP, image, "C:/test.bmp", 0);

			
			FreeImage_Unload(image);
			delete [] pixels;
	}

	void saveToFile(){
		ofstream file;
		file.open("data.txt");
		if(file.good() == true){
			cout << "Plik dziala" <<endl;
		for(int i=0 ; i<sizeX ; ++i)
			for(int j=0 ; j<sizeY ; ++j){

				file << autom.tab[i][j].getGrainID() << " : " << i << " " << j <<endl;
			}

		}

		else 
		cout<<"nie dziala"<<endl;

	}





//*******************************************************************
int main(int argc, char** argv) {
	
	srand (time(NULL));
	autom.tab = autom.createTab(autom.tab);
	autom.OldTab = autom.createTab(autom.OldTab);
	


	autom.podczepSasiadowMoorea(autom.tab, autom.OldTab , sizeX,sizeY);
	autom.podczepSwpochSasiadowMoorea(autom.tab, sizeX,sizeY);
	autom.fillForMC(autom.tab);

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
	return 0; 
}



	//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void MouseButton (int button , int state , int x , int y){

	if (button == GLUT_LEFT_BUTTON)
	{
		if ( state == GLUT_DOWN)
		{
	/*	button_x = x;
		button_y = y;

		cout<< "Coord X: "<<button_x <<endl;
		cout<< "Coord Y: "<<button_y <<endl;*/

		int results[4];
		
		glGetIntegerv( GL_VIEWPORT, results );

		cout<<results[0]<<" "<<results[1] <<" "<< results[2] << " "<< results[3] << endl;
		
		}
		
	}
	
}

 //***************************************************
void handleKeypress(unsigned char key, 
					int x, int y) {    
	switch (key) {
	case '+':{
		cout<<"+"<<endl;
#pragma omp parallel for
		for(int iter =0 ; iter < 100 ; ++iter){

		for(int i=0 ; i<sizeX ; ++i){
			for(int j=0 ; j<sizeY ; ++j){
				autom.monteCarlo(autom.tab , i , j);
				

			}

		}
		}
		printTab(autom.tab);
		break;
			 }
	case '-':
		{
			autom.assignBorders(autom.tab,sizeX,sizeY);
			printTab(autom.tab);
			break;

		}
	case '*' :
		{

			autom.searchForGrains(autom.tab,sizeX,sizeY);
			break;
		}

	case 'p' :
		{
			printTab(autom.tab);
			break;
		}

	case 's' :
		{
			
			autom.addSubGrains(autom.tab,4);
			printTab(autom.tab);
			break;
		}

	case 'z':
		{
			if(growth == MOORE ){
				autom.podczepSasiadowMoorea(autom.tab, autom.OldTab, sizeX,sizeY);
				autom.podczepSwpochSasiadowMoorea(autom.tab,sizeX,sizeY);
				autom.rozrostMoorea(autom.tab,autom.OldTab,sizeX,sizeY);
			}
			else if(growth == VON_NEAUMANN ) {
				autom.podczepSasiadowVonNeumanna(autom.tab,autom.OldTab, sizeX,sizeY);
				autom.podczepSwpochSasiadowVonNeumanna(autom.tab,sizeX,sizeY);
				autom.rozrostVonNeumanna(autom.tab,autom.OldTab,sizeX,sizeY);
			}
			else if(growth == HEXA ){
				autom.podczepSasiadowHexR(autom.tab,autom.OldTab , sizeX,sizeY);
				autom.podczepSwpochSasiadowHexRIGHT(autom.tab,sizeX,sizeY);
				autom.rozrostHexaRIGHT(autom.tab,autom.OldTab,sizeX,sizeY);


			}

			printTab(autom.tab);
			break;

		}

	case 'x' :
		{
			saveToFile();
			/*for(int i=0 ; i<sizeX ; i++ ){
				for(int j=0 ; j<sizeY ; j++){
					cout<< autom.tab[i][j].czyGranica;
				}
				     cout << endl;
				}*/
			break;
		}
	case 'i' :
		{
			for(int i=0 ; i<sizeX ; i++ ){
				for(int j=0 ; j<sizeY ; j++){
					cout<<autom.tab[i][j].getGrainID() <<" ";
				//	cout<<i <<" _ "<<j <<" "<<autom.tab[i][j].kolor.R << " G: " << autom.tab[i][j].kolor.B << " B: " << autom.tab[i][j].kolor.B <<endl;
				}
				cout<<endl;
			}
			break;
		}

	case 'a' :
		{
			//getScreenShot();
			bmpData = readBMP("test.bmp");
			// data[j * width + i], data[j * width + i + 1] and data[j * width + i + 2].

			int width = sizeX;
			int height = sizeY;
			for(int i=0 ; i<width ; i++ )
				for(int j=0 ; j<height ; j++){

						autom.tab[i][j].kolor.setColor( (int)bmpData[3* i * width + 3*j + 2] , (int)bmpData[3 *i * width + 3*j + 1] ,  (int)bmpData[3 *i*width + 3*j] );

						
				}
				printTab(autom.tab);
				addIDs();

			break;
		}

		case 27: //Escape key
			exit(0); //Exit the program
	}
}

void menu(int value){
	switch (value)
	{
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
	//Clear information from last draw
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho( 0 , rozm*PIXEL , rozm*PIXEL , 0 , -1, 1);
	//glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	//glLoadIdentity(); //Reset the drawing perspective
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
	int ilekolorow = 35;//35
	glPointSize(PIXEL);


	for( int i=0 ; i<sizeX ; i++){
		for(int  j=0 ; j<sizeY ; j++){
			if(tab[i][j].getState() == 1 && tab[i][j].czyGranica  == true) {
			
					tab[i][j].kolor.setColor(100,100,100);
				
			}

		//	else if (tab[i][j].getState() == 2 ){
			//	tab[i][j].kolor.setColor(tab[i][j].kolor.R , tab[i][j].kolor.G , tab[i][j].kolor.B);
			//	if(tab[i][j].czyGranica == true)
			//				

		//	}
			
			maluj(tab[i][j].kolor);
		
		    glBegin(GL_POINTS);
					glVertex2f(i*PIXEL , j*PIXEL);
			glEnd();
			
		}
	
	}
	glutSwapBuffers(); // mocno zwalnia, dlatego  poza forem
	}


