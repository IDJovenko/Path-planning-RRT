#pragma once
#include "Filled.h"

void Filled::set_brush(COLORREF color)
{
	HBRUSH HOldBrush = this->hBrush;
	this->hBrush = CreateSolidBrush(color);
	r2 = GetRValue(color);
	g2 = GetGValue(color);
	b2 = GetBValue(color);
	DeleteBrush(HOldBrush);
}