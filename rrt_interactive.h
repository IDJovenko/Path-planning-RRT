#pragma once
#include "OOP L1/include.h"
#include "Figure.h"
#include "Filled.h"
#include "Circle.h"
#include "FilledRectangle.h"
#include "CombiRectangle.h"
#include "Set.h"

#define _WIN32_WINNT 0x0A00

void start_()
{
	// получаем идентификатор окна
	HWND hwnd = GetConsoleWindow();
	// получаем контекст отображения
	HDC hdc = GetDC(hwnd);
	//SetBkColor(hdc, RGB(0, 0, 0));

	Set<Figure> obstacles, samples;
	class Circle start(hdc), end(hdc);

	/*class Figure* F1, * F2, * F3;
	class Figure* FCirc;

	class FilledRectangle R1(hdc, POINT{ 100, 200 }, POINT{ 500, 250 });
	class Rectangle R2(hdc, POINT{ 550, 212 }, POINT{ 750, 237 });
	class CombiRectangle R3(hdc, POINT{ 350, 150 }, POINT{ 600, 400 }, POINT{ 365, 160 }, POINT{ 585, 390 });
	class Circle C1(hdc, POINT{ 50, 100 }, 40, RGB(255, 0, 0));

	R1.set_pen(RGB(0, 0, 255));	R3.set_pen(RGB(255, 0, 0));

	F1 = &R1;
	F2 = &R2;
	F3 = &R3;
	FCirc = &C1;

	f1.push(F1);
	f2.push(F2);
	f3.push(F3);*/
	////RECT rt;
	//int check = 0;

	int key_code = 37;
	do {
		bool i_d = 1;
		switch (key_code)
		{
		case 101: // e
		{
			long a, b; double d;
			std::cin >> a >> b >> d;
			end.set_center(POINT{ a, b });
			end.set_diameter(d);
			end.set_pen(RGB(255, 148, 33));
			end.draw();
			break;
		}
		case 111: // o
		{
			long a, b, c, d;
			std::cin >> a >> b >> c >> d;
			class FilledRectangle* newR = new FilledRectangle(hdc, POINT{ a, b }, POINT{ c, d });
			newR->set_pen(RGB(0, 180, 180));
			obstacles.push(newR);
			for (auto obs = obstacles.begin(); obs != obstacles.end(); obs++)
			{
				(*obs)->draw();
			}
			break;
		}
		case 115: // s
		{
			LPPOINT cPnt, dPnt;
			cPnt = (LPPOINT)malloc(sizeof(*cPnt));
			dPnt = (LPPOINT)malloc(sizeof(*dPnt));
			BYTE flag = 0;
			while(true)
			{
				GetCursorPos(cPnt);
				ScreenToClient(hwnd, cPnt);
				if (GetAsyncKeyState(VK_LBUTTON) < 0) {
					//GetCursorPos(cPnt);
					if (flag == 0) {
						start.set_center(POINT{ cPnt[0].x, cPnt[0].y });
						start.set_diameter(5);
						start.set_pen(RGB(255, 0, 0));
						start.draw();
						flag++;
					}
					else if (flag == 1) {
						double diameter = pow(pow(cPnt[0].x - dPnt[0].x, 2) + pow(cPnt[0].y - dPnt[0].y, 2), 1 / 2);
						start.set_diameter(diameter);
						start.set_pen(RGB(255, 148, 33));
						start.draw();
						flag++;
					}
					if (flag == 2) {
						break;
					}
				}
				Sleep(50);
			}
			break;
			/*long a, b; double d;
			std::cin >> a >> b >> d;
			start.set_center(POINT{ a, b });
			start.set_diameter(d);
			start.set_pen(RGB(255, 148, 33));
			start.draw();
			break;*/
		}
		default:
			system("cls");
			start.draw();
			end.draw();
			for (auto obs = obstacles.begin(); obs != obstacles.end(); obs++)
			{
				(*obs)->draw();
			}
			for (auto samp = samples.begin(); samp != samples.end(); samp++)
			{
				(*samp)->draw();
			}
			break;
		}
		////if (R1.*g_mem_ptr == NULL)
		//if (check == 1110)
		//{
		//	check = 0;
		//}
		//check++;
	} while (key_code = _getch(), key_code != 27);

	ReleaseDC(hwnd, hdc);
}