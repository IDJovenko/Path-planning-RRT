#pragma once
#include "../../include/GraphicObjects/Rectangle.h"
#include <cmath>

// TODO: add ability to set line style
void Rectangle::set_pen(COLORREF color) 
{
	HPEN HOldPen = this->hPen;
	this->hPen = CreatePen(PS_INSIDEFRAME, 1, color);
	/*GetObject(hPen, sizeof(LOGPEN), &logpen);*/
	r1 = GetRValue(color);
	g1 = GetGValue(color);
	b1 = GetBValue(color);
	DeletePen(HOldPen);
}

void Rectangle::draw(HDC &hdc) const
{
	if (is_suit(ppt[0], ppt[1], ppt[2], ppt[3]))
	{
		/*SelectPen(hdc, CreatePenIndirect(&logpen));*/
		SelectPen(hdc, hPen);
		MoveToEx(hdc, ppt[0].x, ppt[0].y, NULL);
		LineTo(hdc, ppt[1].x, ppt[1].y);
		LineTo(hdc, ppt[2].x, ppt[2].y);
		LineTo(hdc, ppt[3].x, ppt[3].y);
		LineTo(hdc, ppt[0].x, ppt[0].y);
	}
	else { /*throw 0;*/ }
}

void Rectangle::rotation(double angle)
{
	this->angle += angle;
	for (unsigned char i = 0; i < 4; i++) {
		double old_x = double(ppt_basic[i].x) - center_rotation_x;
		double old_y = double(ppt_basic[i].y) - center_rotation_y;
		ppt[i].x = long((old_x * cos(this->angle)) + (old_y * sin(this->angle))) + long(center_rotation_x);
		ppt[i].y = long((old_y * cos(this->angle)) - (old_x * sin(this->angle))) + long(center_rotation_y);
	}
}

void Rectangle::lightness(double percent)
{
	HPEN HehePen = CreatePen(PS_SOLID, 5, RGB(int(percent)%255, (int(percent)*5)%255, (int(percent)*19)%255));
}

void Rectangle::decrease(double intensity)
{
	//POINT difference_of_centre = { abs(center_rotation_x * intensity - center_rotation_x),
	//							   abs(center_rotation_y * intensity - center_rotation_y) };
	//ppt_basic[0].x = (intensity * ppt_basic[0].x - difference_of_centre.x);
	//ppt_basic[0].y = (intensity * ppt_basic[0].y - difference_of_centre.y);
	//ppt_basic[1].x = (intensity * ppt_basic[1].x - difference_of_centre.x);
	//ppt_basic[1].y = (intensity * ppt_basic[1].y - difference_of_centre.y);
	//ppt_basic[2].x = (intensity * ppt_basic[2].x - difference_of_centre.x);
	//ppt_basic[2].y = (intensity * ppt_basic[2].y - difference_of_centre.y);
	//ppt_basic[3].x = (intensity * ppt_basic[3].x - difference_of_centre.x);
	//ppt_basic[3].y = (intensity * ppt_basic[3].y - difference_of_centre.y);

	double ip = intensity + 1.0;
	ppt_basic[0].x = (intensity * ppt_basic[0].x + center_rotation_x) / ip;
	ppt_basic[0].y = (intensity * ppt_basic[0].y + center_rotation_y) / ip;
	ppt_basic[1].x = (intensity * ppt_basic[1].x + center_rotation_x) / ip;
	ppt_basic[1].y = (intensity * ppt_basic[1].y + center_rotation_y) / ip;
	ppt_basic[2].x = (intensity * ppt_basic[2].x + center_rotation_x) / ip;
	ppt_basic[2].y = (intensity * ppt_basic[2].y + center_rotation_y) / ip;
	ppt_basic[3].x = (intensity * ppt_basic[3].x + center_rotation_x) / ip;
	ppt_basic[3].y = (intensity * ppt_basic[3].y + center_rotation_y) / ip;
}

void Rectangle::add_red(BYTE point)
{
	r1 += point;
	HPEN HOldPen = this->hPen;
	COLORREF color = RGB(r1, g1, b1);
	this->hPen = CreatePen(PS_SOLID, 5, color);
	DeletePen(HOldPen);
}

void Rectangle::add_green(BYTE point)
{
	g1 += point;
	COLORREF color = RGB(r1, g1, b1);
	HPEN HOldPen = this->hPen;
	this->hPen = CreatePen(PS_SOLID, 5, color);
	DeletePen(HOldPen);
}

void Rectangle::add_blue(BYTE point)
{
	b1 += point;
	COLORREF color = RGB(r1, g1, b1);
	HPEN HOldPen = this->hPen;
	this->hPen = CreatePen(PS_SOLID, 5, color);
	DeletePen(HOldPen);
}

void Rectangle::add_colore(BYTE R, BYTE G, BYTE B)
{
	r1 += R;
	g1 += G;
	b1 += B;
	COLORREF color = RGB(r1, g1, b1);
	HPEN HOldPen = this->hPen;
	this->hPen = CreatePen(PS_SOLID, 5, color);
	DeletePen(HOldPen);
}

bool Rectangle::operator==(const Figure& other)
{
	return (this->ppt_basic == other.get_ppt_basic());
}

bool Rectangle::operator<(const Figure& other)
{
	return true;
}

Rectangle::Rectangle(HWND hwnd, POINT a, POINT c, COLORREF color_pen):
	Figure(hwnd)
{
	if (is_suit(a, c)) {
		ppt_basic[0] = a;
		ppt_basic[2] = c;
		ppt_basic[1].x = c.x;
		ppt_basic[1].y = a.y;
		ppt_basic[3].x = a.x;
		ppt_basic[3].y = c.y;

		if (ppt_basic[0].x == ppt_basic[1].x) {
			center_rotation_x = (ppt_basic[0].x + ppt_basic[3].x) / 2;
			center_rotation_y = (ppt_basic[0].y + ppt_basic[1].y) / 2;
		}
		else {
			center_rotation_x = (ppt_basic[0].x + ppt_basic[1].x) / 2;
			center_rotation_y = (ppt_basic[0].y + ppt_basic[2].y) / 2;
		}
		for (unsigned char i = 0; i < 4; i++)
		{
			ppt[i] = ppt_basic[i];
		}
		angle = 0;
		set_pen(color_pen);
	}
	else throw std::invalid_argument("The rectangle goes beyond the canvas");
}

Rectangle::Rectangle(const Rectangle& other):
	Figure(other.hwnd)
{
	this->angle = other.angle;
	this->center_rotation_x = other.center_rotation_x;
	this->center_rotation_y = other.center_rotation_y;
	this->b1 = other.b1;
	this->g1 = other.g1;
	set_pen(other.get_pen_color());
	/*this->hPen = CreatePenIndirect(&(other.logpen));*/
	this->permanent_angle = other.permanent_angle;
	memcpy(this->ppt, other.ppt, sizeof this->ppt);
	memcpy(this->ppt_basic, other.ppt_basic, sizeof this->ppt_basic);
	this->r1 = other.r1;
}

bool Rectangle::is_suit(POINT a, POINT c) const
{
	if (hwnd == 0)
		return true;
	RECT rt;
	GetClientRect(hwnd, &rt);
	return ((a.x >= 0) & (a.y >= 0) & (c.x >= 0) & (c.y >= 0) & (c.x < rt.right) & (c.y < rt.bottom) & (a.x < rt.right) & (a.y < rt.bottom)) ? true : false;
}

bool Rectangle::is_suit(POINT a, POINT b, POINT c, POINT d) const
{
	if (hwnd == 0)
		return true;
	RECT rt;
	GetClientRect(hwnd, &rt);
	return ((a.x > 0) & (a.y > 0) & (c.x > 0) & (c.y > 0) & (c.x < rt.right) & (c.y < rt.bottom) & (a.x < rt.right) & (a.y < rt.bottom)
		& (b.x > 0) & (b.y > 0) & (b.x < rt.right) & (b.y < rt.bottom) & (d.x > 0) & (d.y > 0) & (d.x < rt.right) & (d.y < rt.bottom)) ? true : false;
}