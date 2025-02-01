#pragma once
#include "../Common/include.h"
class Filled
{
protected:
	Filled(COLORREF color): hBrush(CreateSolidBrush(color)),
							r2(GetRValue(color)), g2(GetGValue(color)), b2(GetBValue(color)) {};
	HBRUSH hBrush;
	int r2, g2, b2; //BYTE?!
public:
	void set_brush(COLORREF color);
	COLORREF get_brush_color() const { return RGB(r2, g2, b2); }
};