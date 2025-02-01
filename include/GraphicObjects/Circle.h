#pragma once
#include "../Common/include.h"
#include "Figure.h"
class Circle : public Figure
{
protected:
	POINT center;
	double diameter;
	HPEN hPen;/* LOGPEN logpen;*/
	HBRUSH hBrush;
	BYTE r1, g1, b1;
	int PenStyle; bool UseNullBrush;
	bool is_suit(POINT center, double diameter) const;
public:
	Circle (
		HWND hwnd,
		POINT center = { 1, 1 }, double diameter = 1,
		COLORREF color_pen = RGB(255, 255, 255),
		int pen_style = PS_INSIDEFRAME, bool use_null_brushe = false
		);
	Circle(Circle& other);
	Circle(const Circle& other);
	~Circle();
	POINT get_center() const { return center; }
	double get_diameter() const { return diameter; }
	HPEN get_pen() const { return hPen; }
	COLORREF get_pen_color() const { return RGB(r1, g1, b1); }
	void set_pen(COLORREF color); //TODO move method to parent Figure class and remove it from other children;
	void set_brush(COLORREF color);
	void set_center(POINT center);
	void set_diameter(double diameter) { this->diameter = diameter; };
	virtual POINT* get_ppt_basic() const { return new POINT[2]{ { center.x - long(diameter), center.y - long(diameter) },
																  center.x + long(diameter), center.y + long(diameter) }; }; //TODO
	virtual void draw(HDC& hdc) const;
	virtual void rotation(double angle) {}; //TODO
	virtual void lightness(double percent) {}; //TODO
	//void decrease(double intensity); //TODO

	//void add_red(BYTE point); //TODO
	//void add_green(BYTE point); //TODO
	//void add_blue(BYTE point); //TODO
	virtual void add_colore(BYTE R, BYTE G, BYTE B);

	virtual bool operator==(const Figure& other) { return true; }; //TODO
	virtual bool operator<(const Figure& other) { return true; }; //TODO
};

