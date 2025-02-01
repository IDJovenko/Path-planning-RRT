#pragma once
#include "../../include/GraphicObjects/FilledRectangle.h"

void FilledRectangle::draw(HDC &hdc) const
{
	if (is_suit(ppt[0], ppt[1], ppt[2], ppt[3]))
	{
		SelectPen(hdc, hPen);
		SelectBrush(hdc, hBrush);
		Polygon(hdc, ppt, 4);
		MoveToEx(hdc, ppt[0].x, ppt[0].y, NULL);
		LineTo(hdc, ppt[1].x, ppt[1].y);
		LineTo(hdc, ppt[2].x, ppt[2].y);
		LineTo(hdc, ppt[3].x, ppt[3].y);
		LineTo(hdc, ppt[0].x, ppt[0].y);
	}
	else { /*throw 0;*/ }
}

void FilledRectangle::add_colore(BYTE R, BYTE G, BYTE B)
{
	r1 += R;
	g1 += G;
	b1 += B;
	COLORREF color = RGB(r1, g1, b1);
	HPEN HOldPen = this->hPen;
	this->hPen = CreatePen(PS_SOLID, 5, color);
	DeletePen(HOldPen);
	r2 += R;
	g2 += G;
	b2 += B;
	color = RGB(r1, g1, b1);
	HBRUSH HOldBrush = this->hBrush;
	this->hBrush = CreateSolidBrush(color);
	DeleteBrush(HOldBrush);
}
//bool FilledRectangle::operator==(const Figure& other)
//{
//	return true;
//}
//bool FilledRectangle::operator<(const Figure& other)
//{
//	return true;
//}
//void FilledRectangle::rotation(double angle){}
