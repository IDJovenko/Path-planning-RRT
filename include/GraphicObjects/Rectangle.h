#pragma once
#include "../Common/include.h"
#include "Figure.h"
class Rectangle : public Figure
{
protected:
	HPEN hPen; /*LOGPEN logpen;*/
	BYTE r1, g1, b1;
	POINT ppt[4], ppt_basic[4]; //coordinates
	bool is_suit(POINT a, POINT c) const;
	bool is_suit(POINT a, POINT b, POINT c, POINT d) const;
public:
	Rectangle(HWND hwnd, POINT a = { 0, 0 }, POINT c = { 0, 0 }, COLORREF color_pen = RGB(255, 255, 255));
	//Rectangle(Rectangle& other);
	Rectangle(const Rectangle& other);
	POINT get_a() const { return ppt[0]; }
	POINT get_b() const { return ppt[1]; }
	POINT get_c() const { return ppt[2]; }
	POINT get_d() const { return ppt[3]; }
	POINT get_i(BYTE i) const { return ppt[i]; }
	POINT get_basic_i(BYTE i) const { return ppt_basic[i]; }
	HPEN get_pen() const { return hPen; }
	COLORREF get_pen_color() const { return RGB(r1, g1, b1); }
	double get_angle() const { return angle; };
	void set_pen(COLORREF color);
	void set_i_point(BYTE i, POINT i_point) { ppt[i] = i_point; };
	virtual POINT* get_ppt_basic() const { POINT tmp[4]; memcpy(tmp, ppt, sizeof tmp); return tmp; };

	virtual void draw(HDC &hdc) const;
	virtual void rotation(double angle);
	virtual void lightness(double percent);
	void decrease(double intensity);

	void add_red(BYTE point);
	void add_green(BYTE point);
	void add_blue(BYTE point);
	virtual void add_colore(BYTE R, BYTE G, BYTE B);

	virtual bool operator==(const Figure& other);
	virtual bool operator<(const Figure& other);
	/*friend std::ostream& operator<<(std::ostream& s, Figure& F);*/
};