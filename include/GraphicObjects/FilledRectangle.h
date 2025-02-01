#pragma once
#include "../Common/include.h"
#include "Rectangle.h"
#include "Filled.h"
class FilledRectangle: public Rectangle, public Filled 
{
public:
	FilledRectangle(HWND hwnd, POINT a = { 0, 0 }, POINT c = { 0, 0 },
		COLORREF color_pen = RGB(255, 255, 255), COLORREF color_brush = RGB(141, 141, 141)):
		Rectangle(hwnd, a, c, color_pen), Filled(color_brush){};
	FilledRectangle(FilledRectangle& other) : Rectangle(other),
					Filled(other.get_brush_color()) {};
	FilledRectangle(const FilledRectangle& other) : Rectangle(other),
		Filled(other.get_brush_color()) {};

	virtual void draw(HDC &hdc) const;
	//void rotation(double angle);
	virtual void lightness(double percent) {};
	virtual void add_colore(BYTE R, BYTE G, BYTE B);

	//virtual bool operator==(const Figure& other);
	//virtual bool operator<(const Figure& other);
};

