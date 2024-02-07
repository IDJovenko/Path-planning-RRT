#pragma once
#include "OOP L1/include.h"
#include "Figure.h"
#include "Filled.h"
#include "Rectangle.h"
#include "Circle.h"
#include "FilledRectangle.h"
#include "CombiRectangle.h"
#include <algorithm>
#include <ranges>
//#include <map>
//#include <list>
//#include <functional>
//typedef HPEN Rectangle::* mem_ptr_template;
//mem_ptr_template g_mem_ptr;
//
//template <mem_ptr_template p>
//class GetPrivatePen {
//	static bool b;
//};
//template <mem_ptr_template p>
//bool GetPrivatePen<p>::b = (g_mem_ptr = p, false);
//
//template class GetPrivatePen<&Rectangle::hPen>;
//
//bool fun(int a)
//{
//	return a / 2 == 0 ? 1 : 0;
//}

#include "Set.h"
#include <vector>

//void soup(Figure* f) { std::cout << "fff"; f->draw(); };
void set_demonstration()
{
	using namespace std;
	//Set<int> s;
	//vector<int> i = { 1, 2, 3, 4, 5 };
	//for (auto elem: i)
	//{
	//	s.push(&i[elem-1]);
	//}
	//s.reverse_print(' ');
	//s.erase(--s.end());
	//s.direct_print(' ');
	//SetIterator<int> finded = s.find(new int(2));
	//s.erase(finded);
	//s.direct_print(' ');
	//cout << *s.pop();
	//s.~Set();
	
	Set<Figure> f1, f2, f3;
	// получаем идентификатор окна
	HWND hwnd = GetConsoleWindow();
	// получаем контекст отображения
	HDC hdc = GetDC(hwnd);
	//SetBkColor(hdc, RGB(0, 0, 0));

	class Figure* F1, * F2, * F3;
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
	f3.push(F3);
	
	int a = 0;

	Figure* CF1, * CF2, * CF3;
	int key_code = 37;
	do {
		bool i_d = 1;
		switch (key_code)
		{
		case 97: // a
			F1->draw(); F2->draw(); F3->draw(); FCirc->draw();
			F1->rotation(0.7); F2->rotation(-1.4); F3->rotation(-0.7);
			break;
		case 100: // d
			F1->draw(); F2->draw(); F3->draw(); FCirc->draw();
			F1->rotation(-0.7); F2->rotation(1.4); F3->rotation(0.7);
			break;
		case 114: // r
			F1->add_colore(10, 0, 0);
			R2.add_red(10);
			R3.add_red(10);
			break;
		case 103: // g
			F1->add_colore(0, 10, 0);
			R2.add_green(10);
			R3.add_green(10);
			break;
		case 98: // b
			F1->add_colore(0, 0, 10);
			R2.add_blue(10);
			R3.add_blue(10);
			break;
		case 57: // 9/"("
			F1->add_colore(10, 10, 10);
			F2->add_colore(10, 10, 10);
			F3->add_colore(10, 10, 10);
			F1->draw(); F2->draw(); F3->draw(); FCirc->draw();
			F1->rotation(0.7); F2->rotation(-1.4); F3->rotation(-0.7);
			break;
		case 48: // 0/")"
			F1->add_colore(5, 6, 4);
			F2->add_colore(6, 7, 5);
			F3->add_colore(7, 8, 6);
			F1->draw(); F2->draw(); F3->draw(); FCirc->draw();
			F1->rotation(-0.7); F2->rotation(1.4); F3->rotation(0.7);
			R1.decrease(90); R2.decrease(80); R3.decrease(50);
			{
				class FilledRectangle* CR1 = new FilledRectangle(R1);
				class Rectangle* CR2 = new class Rectangle(R2);
				class CombiRectangle* CR3 = new CombiRectangle(R3);
				CF1 = CR1; CF2 = CR2; CF3 = CR3;
				f1.push(CF1); f2.push(CF2); f3.push(CF3);
			}
			break;
		case 49: // 1 влево 75 вправо 77
			for(auto F = f1.begin(); F!=f1.end(); F++)
			{
				(*F)->rotation(0.7);
				(*F)->draw();
			}
			break;
		case 50: // 2
			for(auto F = f2.begin(); F!=f2.end(); F++)
			{
				(*F)->rotation(0.7);
				(*F)->draw();
			}
			break;
		case 51: // 3
			for(auto F = f3.begin(); F != f3.end(); F++)
			{
				(*F)->rotation(0.7);
				(*F)->draw();
				a++;
			}
			cout << a; a = 0;
			break;
		default:
			system("cls");
			break;
		}	
	} while (key_code = _getch(), key_code != 27);

	ReleaseDC(hwnd, hdc);
}

void demonstration()
{
	// получаем идентификатор окна
	HWND hwnd = GetConsoleWindow();
	// получаем контекст отображения
	HDC hdc = GetDC(hwnd);
	//SetBkColor(hdc, RGB(0, 0, 0));

	class Figure* F1, * F2, * F3;

	class FilledRectangle R1(hdc, POINT{ 100, 200 }, POINT{ 500, 250 });
	class Rectangle R2(hdc, POINT{ 550, 212 }, POINT{ 750, 237 });
	class CombiRectangle R3(hdc, POINT{ 350, 150 }, POINT{ 600, 400 }, POINT{ 365, 160 }, POINT{ 585, 390 });

	R1.set_pen(RGB(0, 0, 255));
	R3.set_pen(RGB(255, 0, 0));

	F1 = &R1;
	F2 = &R2;
	F3 = &R3;
	////RECT rt;
	//int check = 0;
	int key_code = 37;
	do {
		bool i_d = 1;
		switch (key_code)
		{
		case 97: // a
			F1->draw(); F2->draw(); F3->draw();
			F1->rotation(0.7); F2->rotation(-1.4); F3->rotation(-0.7);
			break;
		case 100: // d
			F1->draw(); F2->draw(); F3->draw();
			F1->rotation(-0.7); F2->rotation(1.4); F3->rotation(0.7);
			break;
		case 114: // r
			F1->add_colore(10, 0, 0);
			R2.add_red(10);
			R3.add_red(10);
			break;
		case 103: // g
			F1->add_colore(0, 10, 0);
			R2.add_green(10);
			R3.add_green(10);
			break;
		case 98: // b
			F1->add_colore(0, 0, 10);
			R2.add_blue(10);
			R3.add_blue(10);
			break;
		case 57: // 9/"("
			F1->add_colore(10, 10, 10);
			F2->add_colore(10, 10, 10);
			F3->add_colore(10, 10, 10);
			F1->draw(); F2->draw(); F3->draw();
			F1->rotation(0.7); F2->rotation(-1.4); F3->rotation(-0.7);
			break;
		case 48: // 0/")"
			F1->add_colore(5, 6, 4);
			F2->add_colore(6, 7, 5);
			F3->add_colore(7, 8, 6);
			F1->draw(); F2->draw(); F3->draw();
			F1->rotation(-0.7); F2->rotation(1.4); F3->rotation(0.7);
			R1.decrease(90); R2.decrease(80); R3.decrease(50);
			break;
		default:
			system("cls");
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
	//using namespace std;
	// /*std::vector<int> v;
	//v.push_back(3);
	//int a = *std::ind(v.begin(), v.end(), 3);
	//std::cout << a << std::endl;
	//
	//std::set<int> s;
	//s.insert(1);
	//int b = *s.find(1);
	//std::cout << b;
	//
	//std::map<int, int> m;
	//m.insert(23, 2);
	//m.find(23);*/
	//
	//
	//
	////list<int> l;
	////l.push_back(3);
	////auto i = l.end();
	//////i++;
	////cout << *(l.begin());
	////cout << endl;
	////cout << *i; //ex
	////cout << endl;
	////i--;
	////cout << *i;
	////cout << endl;
	////i--;
	////cout << *i; //ex
	////cout << endl;
	//
	////less<int> cmp;
	//
	////system("pause");
	//
	////map<int, int> m;
	////m.insert(m.begin(), { 5, 1 });
	////m.erase(m.begin());
	////m.erase(m.begin());
	//
	////int* pos = s.pop();
	//
	////int a2 = 10;
	////s.push(&a2);
	////int a3 = 5;
	////s.push(&a3);
	////cout << *(*(++s.begin()));
	////int a4 = 15;
	////s.push(&a4);
	////auto be = s.begin();
	////be++;
	////cout << *(*(++(++s.begin())));
	//
	////bool (*f_ptr)(int);
	////f_ptr = fun;
	//
	////cout<<f_ptr(2);
}

void lightness_demo()
{
	// получаем идентификатор окна
	HWND hwnd = GetConsoleWindow();
	// получаем контекст отображения
	HDC hdc = GetDC(hwnd);
	//SetBkColor(hdc, RGB(0, 0, 0));

	/*class CombiRectangle R3(hdc, POINT{ 350, 150 }, POINT{ 600, 400 }, POINT{ 365, 160 }, POINT{ 585, 390 });
	class CombiRectangle* CR3 = new CombiRectangle(R3);
	Figure* F1 = &R3, *F2 = CR3;
	Set<Figure> s;
	s.push(F1);
	s.push(F2);
	for ( auto F = s.begin(); F!=s.end(); F++)
	{
		(*F)->draw();
	}
	(*(s.begin()))->rotation(0.75);

	(*(s.begin()))->draw();
	(*(++(s.begin())))->draw();*/


	class Rectangle R(hdc, POINT{ 350, 150 }, POINT{ 600, 400 }, RGB(255, 0, 0));
	int key_code = 0; long int bk = 1;
	do
	{
		R.lightness(bk++); if ((bk % 128) == 0) std::cout << bk << " ";
		R.draw();
	} while (key_code = _getch(), key_code != 27 && key_code != 32);
	
}