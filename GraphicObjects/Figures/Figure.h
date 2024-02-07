#pragma once
#include "../../include.h"
class Figure
{
private:
	/*friend std::ostream& operator<<(std::ostream& s, Figure& F)
	{
		F.draw(); s.clear(); return s;
	}*/
protected:
	double angle, permanent_angle;
	double center_rotation_x, center_rotation_y;
	HWND hwnd;
	/*HPEN hPen;
	HBRUSH hBrush;*/
public:
	Figure(HWND hwnd):
		hwnd(hwnd){}; /*, angle(0), permanent_angle(0), center_rotation_x(0), center_rotation_y(0)*/
	double get_rotation_center_x() { return center_rotation_x; }
	double get_rotation_center_y() { return center_rotation_y; }
	void set_hwnd(HWND hwnd) {
		this->hwnd = hwnd;
	};
	void set_angle(double angle) { this->angle = angle; };
	void set_rotation_center(double x, double y)
		{ this->center_rotation_x = x; this->center_rotation_y = y; };
	virtual void draw(HDC& hdc) const = 0;
	virtual void rotation(double angle) = 0;
	virtual void lightness(double percent) = 0;
	//virtual void decrease(int intensity) = 0;
	
	virtual void add_colore(BYTE R, BYTE G, BYTE B) = 0;
	virtual POINT* get_ppt_basic() const = 0;

	virtual bool operator==(const Figure& other) = 0;
	virtual bool operator<(const Figure& other) = 0;
};