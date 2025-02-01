#pragma once
#include "../../include/GraphicObjects/Circle.h"

bool Circle::is_suit(POINT center, double diameter) const
{
	if(hwnd==0)
		return true;
	RECT rt;
	GetClientRect(hwnd, &rt);
	return ((center.x - diameter >= 0) & (center.y - diameter >= 0) & (center.x + diameter < rt.right) & (center.y + diameter < rt.bottom)) ? true : false;
}

Circle::Circle(HWND hwnd, POINT center, double diameter, COLORREF color_pen, int pen_style, bool use_null_brush):
	Figure(hwnd)
{
	if (is_suit(center, diameter)) {
		this->center = center;
		this->diameter = diameter;
		PenStyle = pen_style;
		UseNullBrush = use_null_brush;
		set_pen(color_pen);
		set_brush(color_pen);
	}
	// TODO:
	else throw std::invalid_argument("Oooops: wrong data!");
}

Circle::Circle(Circle& other):
	Figure(other.hwnd)
{
	this->center = other.center;
	this->diameter = other.diameter;
	set_pen(other.get_pen_color());
	set_brush(other.get_pen_color());
}

Circle::Circle(const Circle& other):
	Figure(other.hwnd)
{
	this->center = other.center;
	this->diameter = other.diameter;
	set_pen(other.get_pen_color());
	set_brush(other.get_pen_color());
}

Circle::~Circle()
{
	DeletePen(hPen);
	DeleteBrush(hBrush);
}

void Circle::set_pen(COLORREF color)
{
	HPEN HOldPen = this->hPen;
	this->hPen = CreatePen(PenStyle, 5, color);
	/*GetObject(hPen, sizeof(LOGPEN), &logpen);*/
	r1 = GetRValue(color);
	g1 = GetGValue(color);
	b1 = GetBValue(color);
	DeletePen(HOldPen);
}

void Circle::set_brush(COLORREF color)
{
	HBRUSH HOldBrush = this->hBrush;
	this->hBrush = UseNullBrush?
		GetStockBrush(NULL_BRUSH): CreateSolidBrush(color);
	/*GetObject(hPen, sizeof(LOGPEN), &logpen);*/
	r1 = GetRValue(color);
	g1 = GetGValue(color);
	b1 = GetBValue(color);
	DeleteBrush(HOldBrush);
}

void Circle::set_center(POINT center)
{
	if (is_suit(center, diameter)) {
		this->center = center;
	}
	else {
		throw std::invalid_argument("The circle goes beyond the canvas");
	}
}

void Circle::draw(HDC &hdc) const
{
	if (is_suit(center, diameter))
	{
		/*SelectPen(hdc, CreatePenIndirect(&logpen));*/
		//SelectPen(hdc, CreatePen(PS_SOLID, 5, RGB(255, 0, 0)));
		/*HBRUSH hdcBrush;
		hdcBrush = */SelectBrush(hdc, this->hBrush);
		SelectPen(hdc, this->hPen);
		Ellipse(hdc, center.x - diameter + 1, center.y - diameter + 1, center.x + diameter - 1, center.y + diameter - 1); // TODO move spare 1 to calc obstacles logic
		/*SelectBrush(hdc, hdcBrush);*/
	}
	else { /*throw 0;*/ }
}

void Circle::add_colore(BYTE R, BYTE G, BYTE B)
{
	r1 += R;
	g1 += G;
	b1 += B;
	COLORREF color = RGB(r1, g1, b1);
	HPEN HOldPen = this->hPen;
	this->hPen = CreatePen(PS_SOLID, 5, color);
	DeletePen(HOldPen);
}


