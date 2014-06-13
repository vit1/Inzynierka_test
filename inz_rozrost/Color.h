#pragma once
class Color
{public:
	int R;
	int G;
	int B;

	Color(int red = 0, int green = 0  , int blue = 0);
	void setNewColor();
	void setColor(int r, int g, int b);
	void setRandomColor();
	~Color(void);
};

