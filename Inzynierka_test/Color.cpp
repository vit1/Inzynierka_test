#include "Color.h"
#include <time.h> 
#include <stdlib.h> 

Color::Color(int red  , int green  , int blue )
{
	Color::R = red;
	Color::G = green;
	Color::B = blue;
}


Color::~Color(void)
{
}


void Color::setNewColor(){
	if(R >= G && R>= B){
		R = 255;
		int random = rand() % 8 + 3;
		G = 200/random;
		random = rand() % 8 + 3;
		B = 200/random;


	}
	else if(G >= R && G>= B){
		int random = rand() % 8 + 3;

		 R = 200/random;
		 G = 255;
		 random = rand() % 8 +3;
		 B = 200/random;
	}
	else if(B>=R && B>=G){

		int random = rand() % 8 + 3;
		 R = 200/random;
		 random = rand() % 8 + 3;
		 G = 200/random;
		 B = 255;
	}
}

void Color:: setRandomColor(){
	int ran = rand() % 255 + 1;
	Color::R = ran;
	ran = rand() % 255 + 1;
	Color::G = ran;
	ran = rand() % 255 + 1;
	Color::B = ran;
}

void Color ::setColor(int r, int g , int b){

	Color::R = r;
	Color::G = g;
	Color::B = b;
}
