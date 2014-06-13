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
		G = 0;
		B = 0;
	}
	else if(G >= R && G>= B){
		 R = 0;
		 G = 255;
		 B = 0;
	}
	else if(B>=R && B>=G){
		 R = 0;
		 G = 0;
		 B = 255;
	}
}
void Color:: setRandomColor(){
	int ran = rand() % 255 + 1;
	R = ran;
	ran = rand() % 255 + 1;
	G = ran;
	ran = rand() % 255 + 1;
	B = ran;
}

void Color ::setColor(int r, int g , int b){

	Color::R = r;
	Color::G = g;
	Color::B = b;
}
