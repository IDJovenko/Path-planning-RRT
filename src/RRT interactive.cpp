#define OEMRESOURCE
#include <Windows.h>
#include <fileapi.h>
#include <commctrl.h>
#include <string.h>
#include <fstream>
#include "../include/Common/SoftwareDefenition.h"

int CALLBACK wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int nCmdShow) {
	MSG msg{};
	WNDCLASSEX wc = NewWindowClass
	(
		reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)),
		LoadCursor(nullptr, IDC_ARROW),
		hInstance,
		LoadIcon(nullptr, IDI_ASTERISK),
		L"RRT_interactive",
		SoftwareMainProc
	);

	if (!RegisterClassEx(&wc)) {
		return EXIT_FAILURE;
	}
	hwnd = CreateWindow(wc.lpszClassName, L"RRT algorithm",
		WS_OVERLAPPEDWINDOW
		& ~WS_THICKFRAME   //WS_THICKFRAME  - has a sizing border
		& ~WS_MAXIMIZEBOX, //WS_MAXIMIZEBOX - has a maximize button
		(GetSystemMetrics(SM_CXSCREEN) >> 1) - (WWidth >> 1), // x centering main window
		(GetSystemMetrics(SM_CYSCREEN) >> 1) - (WHight >> 1), // y centering main window
		WWidth, WHight, nullptr, nullptr, wc.hInstance, nullptr);
	if (hwnd == INVALID_HANDLE_VALUE) {
		return EXIT_FAILURE;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<int>(msg.wParam); //or EXIT_SUCCESS
}

inline WNDCLASSEX CALLBACK NewWindowClass(HBRUSH hBrush, HCURSOR hCursor, HINSTANCE hInstance, HICON hIcon, LPCWSTR name, WNDPROC Proc)
{
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = hBrush;
	wc.hCursor = hCursor;
	wc.hIcon = hIcon;
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION); //or IDI_ASTERISK
	wc.hInstance = hInstance;
	wc.lpfnWndProc = Proc;
	wc.lpszClassName = name;
	wc.lpszMenuName = nullptr;
	wc.style = 0x000;
	return wc;
}

LRESULT CALLBACK SoftwareMainProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case AboutOnMenuClicked:
		{
			std::string about_s;
			about_s.assign("RRT algorithm\n");
			about_s.append("Version 1.0\n\n");
			about_s.append("Copyright © 2022-2023 JovenkoStyle\n");

			MessageBoxA(hwnd, about_s.c_str(), "About \"RRT algorith\" program", MB_OK);
		}
		return 0;
		case SaveOnMenuClicked:
		{
			if (GetSaveFileNameA(&ofn)) { SaveScene(filename); }
		}
		return 0;
		case LoadOnMenuClicked:
		{
			if (GetOpenFileNameA(&ofn)) { LoadScene(filename); }
			canvas_redraw();
		}
		return 0;
		case SetObsPoint:
		{
			try
			{
				BOOL A_xSuccess, A_ySuccess, D_xSuccess, D_ySuccess;
				long A_x, A_y, D_x, D_y;
				A_x = GetDlgItemInt(hwnd, ObsPointA_xEdit, &A_xSuccess, false);
				A_y = GetDlgItemInt(hwnd, ObsPointA_yEdit, &A_ySuccess, false);
				D_x = GetDlgItemInt(hwnd, ObsPointD_xEdit, &D_xSuccess, false);
				D_y = GetDlgItemInt(hwnd, ObsPointD_yEdit, &D_ySuccess, false);
				if (A_xSuccess && A_ySuccess && D_xSuccess && D_ySuccess) {
					ObsSet.push(new RectObstacle(
						hwnd, POINT{ A_x, A_y }, POINT{ D_x,D_y }, NULL, RGB(255, 116, 0))
					);
					canvas_redraw();
				}
			}
			catch (const std::exception& ex)
			{
				MessageBoxError(ex);
				return 0;
			}
		}
		return 0;
		case SetStartPoint:
		{
			try
			{
				SetGoalPoint(SetStartPoint);
			}
			catch (const std::exception& ex)
			{
				MessageBoxError(ex);
				return 0;
			}
		}
		return 0;
		case SetEndPoint:
		{
			try
			{
				SetGoalPoint(SetEndPoint);
			}
			catch (const std::exception& ex)
			{
				MessageBoxError(ex);
				return 0;
			}
		}
		return 0;
		case SetRemoveObsButton:
		{
			if (CapturesIsActive) {
				CapturesIsActive = false;
			}
			ObsSet.~Set();
			ObsSet = *(new Set<class RectObstacle>());
			canvas_redraw();
		}
		return 0;
		case StartRRTButton:
		{
			bWEnable = Disable;
			EnableWidgets(Disable);
			RECT ClientRect{ 0 };
			GetClientRect(hwnd, &ClientRect);

			BOOL EndRangeSuccess, SamplesNumSuccess;
			end_range = GetDlgItemInt(hwnd, EndRangeEdit, &EndRangeSuccess, false);
			int samples_num = GetDlgItemInt(hwnd, SamplesNumEdit, &SamplesNumSuccess, false);
			if (!EndRangeSuccess || !SamplesNumSuccess) {
				MessageBox(hwnd, L"Invalid argument!", L"Error!", NULL);
				return 0;
			}
			rrt_seed = GetDlgItemInt(hwnd, RRTSeedEdit, nullptr, false);

			/*auto tmp_rrt_p = std::make_shared<RRT>(
				new RRT(
				samples_num,
				std::make_pair(start.get_center().x, start.get_center().y),
				std::make_pair(end.get_center().x, end.get_center().y),
				end_range,
				{ 3 * ClientRect.right / 4, ClientRect.bottom })
				);
			rrt_p = tmp_rrt_p;*/
			//RRT rrt(samples_num,
			//	std::make_pair(start.get_center().x, start.get_center().y),
			//	std::make_pair(end.get_center().x, end.get_center().y),
			//	end_range,
			//	{ 3 * ClientRect.right / 4, ClientRect.bottom });
			
			rrt_impl = RRT(samples_num,
				std::make_pair(start.get_center().x, start.get_center().y),
				std::make_pair(end.get_center().x, end.get_center().y),
				end_range,
				{ 3 * ClientRect.right / 4, ClientRect.bottom });
			
			Set<Obstacle> ExObsSet;
			for (auto it : ObsSet) {
				ExObsSet.push(it);
			}
			rrt_impl.set_obstacles(ExObsSet);
			rrt_impl.set_seed(rrt_seed);
			rrt_path = rrt_impl.run();

			if (rrt_path.empty()) {
				MessageBoxA(hwnd, "Path not found\n Try change rrt parametrs", "RRT", MB_OK);
			}
			canvas_redraw();
		}
		return 0;
		case ResetButton:
		{
			bWEnable = Enable;
			EnableWidgets(Enable);

			rrt_path = {};
			canvas_redraw();
			// RRT_cnvs_clear();
		}
		return 0;
		default:
			break;
		}
	}
	return 0;
	case WM_PAINT:
	{
		BeginPaint(hwnd, &cnvsPS);
		canvas_redraw();
		EndPaint(hwnd, &cnvsPS);
	}
	
	return 0;
	case WM_CREATE:
	{
		MainWndAddMenu(hwnd);
		MainWndAddWidgets(hwnd);
		SetOpenFileParams(hwnd);
		canvas_redraw();
	}
	return 0;
	case WM_DESTROY:
	{
		PostQuitMessage(EXIT_SUCCESS);
		ExitProcess(NULL);
	}
	return 0;
	}
	
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

inline void MainWndAddMenu(HWND hwnd)
{
	HMENU hMenu = CreateMenu();
	HMENU SubMenu = CreateMenu();

	AppendMenu(SubMenu, MF_STRING, SaveOnMenuClicked, L"Save scene");
	AppendMenu(SubMenu, MF_STRING, LoadOnMenuClicked, L"Load scene");

	AppendMenu(hMenu, MF_POPUP, (UINT)SubMenu, L"File");
	AppendMenu(hMenu, MF_STRING, AboutOnMenuClicked, L"About");

	SetMenu(hwnd, hMenu);
}

inline void MainWndAddWidgets(HWND hwnd)
{
	RECT ClientRect{ 0 };
	GetClientRect(hwnd, &ClientRect);
	int CWidth = ClientRect.right;
	int CHight = ClientRect.bottom;
	cnvs = CreateWindowA(
		"static", NULL,
		WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | SS_NOTIFY | SS_WHITERECT,
		0, 0, 3 * CWidth / 4, CHight,
		hwnd, nullptr, nullptr, nullptr
	);
	/*cnvs_plug = CreateWindowA(
		"static", NULL,
		WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | SS_NOTIFY | SS_GRAYRECT | WS_EX_LAYERED,
		0, 0, 3 * CWidth / 4, CHight,
		hwnd, nullptr, nullptr, nullptr
	);
	SetLayeredWindowAttributes(cnvs_plug, RGB(100,100,100), 50, LWA_COLORKEY);*/
	//EnableWindow(cnvs_plug, false);

	wpOrigCanvasProc = (WNDPROC)SetWindowLong(cnvs, GWL_WNDPROC, (LONG)CanvasSubclassProc);
	cnvsDC = GetDC(cnvs);
	SetBkColor(GetDC(cnvs), RGB(255, 255, 255));
	GetClientRect(cnvs, &cnvsRCT);
	start.set_hwnd(cnvs);
	end.set_hwnd(cnvs);
	SetClassLong(cnvs, GCL_HCURSOR, LONG(LoadCursor(NULL, IDC_CROSS))); //IDC_SIZEALL
	// separator line
	CreateWindowA(
		"static", NULL, WS_CHILD | WS_VISIBLE | SS_GRAYRECT,
		3 * CWidth / 4, 0, WSeparatorWidth, CHight,
		hwnd, nullptr, nullptr, nullptr
	);

	// getting text metric to extract text hight
	HDC hdc = GetDC(hwnd);
	/*SetBkColor(hdc, RGB(255, 255, 255));*/
	TEXTMETRICA tm{ 0 };
	GetTextMetricsA(hdc, &tm);
	//text-widget hight
	auto THeight = tm.tmHeight + 2 * tm.tmExternalLeading;
	CreateWindowA(
		"static", "interactive mode:",
		WS_CHILD | WS_VISIBLE,
		3 * CWidth / 4 + WSeparatorWidth, 0, CWidth / 4 - WSeparatorWidth, THeight,
		hwnd, nullptr, nullptr, nullptr
	);
	// creating radiobutton
	widgets[0] = CreateWindowA("button", "set start point",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		3 * CWidth / 4 + WSeparatorWidth, THeight,
		CWidth / 4 - WSeparatorWidth, THeight,
		hwnd, (HMENU)RadioButtonSP, nullptr, nullptr
	);
	widgets[1] = CreateWindowA("button", "set end point",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		3 * CWidth / 4 + WSeparatorWidth, 2 * THeight,
		CWidth / 4 - WSeparatorWidth, THeight,
		hwnd, (HMENU)RadioButtonEP, nullptr, nullptr
	);
	widgets[2] = CreateWindowA("button", "set obstacle",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		3 * CWidth / 4 + WSeparatorWidth, 3 * THeight,
		CWidth / 4 - WSeparatorWidth, THeight,
		hwnd, (HMENU)RadioButtonSetObs, nullptr, nullptr
	);
	widgets[3] = CreateWindowA("button", "select obstacle",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		3 * CWidth / 4 + WSeparatorWidth, 4 * THeight,
		CWidth / 4 - WSeparatorWidth, THeight,
		hwnd, (HMENU)RadioButtonSelectObs, nullptr, nullptr
	);
	CheckRadioButton(hwnd, RadioButtonSP, RadioButtonSetObs, RadioButtonSP);
	CreateWindowA(
		"static", NULL, WS_CHILD | WS_VISIBLE | SS_GRAYRECT,
		3 * CWidth / 4 + WSeparatorWidth, 5 * THeight,
		CWidth / 4 - WSeparatorWidth, WSeparatorWidth,
		hwnd, nullptr, nullptr, nullptr
	);
	//CreateWindowSeparator(4 * THeight, CWidth);

	// start point widget creating:
	CreateWindowA(
		"static", "start point: ",
		WS_CHILD | WS_VISIBLE,
		3 * CWidth / 4 + WSeparatorWidth, 5 * THeight + WSeparatorWidth,
		CWidth / 4 - WSeparatorWidth, THeight,
		hwnd, nullptr, nullptr, nullptr
	);
	widgets[4] = StartPointX = CreateWindowA(
		"edit", NULL,
		WS_CHILD | WS_VISIBLE | ES_NUMBER,
		3 * CWidth / 4 + WSeparatorWidth, 6 * THeight + WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 3, THeight,
		hwnd, (HMENU)StartPointXEdit, nullptr, nullptr
	);
	SendMessage(StartPointX, EM_SETCUEBANNER, 0, (LPARAM)L"x pos");
	widgets[5] = StartPointY = CreateWindowA(
		"edit", NULL,
		WS_CHILD | WS_VISIBLE | ES_NUMBER,
		3 * CWidth / 4 + WSeparatorWidth + (CWidth / 4 - WSeparatorWidth) / 3,
		6 * THeight + WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 3, THeight,
		hwnd, (HMENU)StartPointYEdit, nullptr, nullptr
	);
	SendMessage(StartPointY, EM_SETCUEBANNER, 0, (LPARAM)L"y pos");
	widgets[6] = CreateWindowA(
		"button", "set",
		WS_CHILD | WS_VISIBLE,
		3 * CWidth / 4 + WSeparatorWidth + 2 * (CWidth / 4 - WSeparatorWidth) / 3,
		6 * THeight + WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 4, THeight,
		hwnd, (HMENU)SetStartPoint, nullptr, nullptr
	);
	// end point widget creating :
	CreateWindowA(
		"static", "end point: ",
		WS_CHILD | WS_VISIBLE,
		3 * CWidth / 4 + WSeparatorWidth, 7 * THeight + WSeparatorWidth,
		CWidth / 4 - WSeparatorWidth, THeight,
		hwnd, nullptr, nullptr, nullptr
	);
	widgets[7] = EndPointX = CreateWindowA(
		"edit", NULL,
		WS_CHILD | WS_VISIBLE | ES_NUMBER,
		3 * CWidth / 4 + WSeparatorWidth, 8 * THeight + WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 3, THeight,
		hwnd, (HMENU)EndPointXEdit, nullptr, nullptr
	);
	SendMessage(EndPointX, EM_SETCUEBANNER, 0, (LPARAM)L"x pos");
	widgets[8] = EndPointY = CreateWindowA(
		"edit", NULL,
		WS_CHILD | WS_VISIBLE | ES_NUMBER,
		3 * CWidth / 4 + WSeparatorWidth + (CWidth / 4 - WSeparatorWidth) / 3,
		8 * THeight + WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 3, THeight,
		hwnd, (HMENU)EndPointYEdit, nullptr, nullptr
	);
	SendMessage(EndPointY, EM_SETCUEBANNER, 0, (LPARAM)L"y pos");
	widgets[9] = CreateWindowA(
		"button", "set",
		WS_CHILD | WS_VISIBLE,
		3 * CWidth / 4 + WSeparatorWidth + 2 * (CWidth / 4 - WSeparatorWidth) / 3,
		8 * THeight + WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 4, THeight,
		hwnd, (HMENU)SetEndPoint, nullptr, nullptr
	);
	// obstacle widget creating:
	CreateWindowA(
		"static", "obstacle: ",
		WS_CHILD | WS_VISIBLE,
		3 * CWidth / 4 + WSeparatorWidth, 9 * THeight + WSeparatorWidth,
		CWidth / 4 - WSeparatorWidth, THeight,
		hwnd, nullptr, nullptr, nullptr
	);
	widgets[10] = ObsPointA_x = CreateWindowA(
		"edit", NULL,
		WS_CHILD | WS_VISIBLE | ES_NUMBER,
		3 * CWidth / 4 + WSeparatorWidth,
		10 * THeight + WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 3, THeight,
		hwnd, (HMENU)ObsPointA_xEdit, nullptr, nullptr
	);
	SendMessage(ObsPointA_x, EM_SETCUEBANNER, 0, (LPARAM)L"A_x pos");
	widgets[11] = ObsPointA_y = CreateWindowA(
		"edit", NULL,
		WS_CHILD | WS_VISIBLE | ES_NUMBER,
		3 * CWidth / 4 + WSeparatorWidth + (CWidth / 4 - WSeparatorWidth) / 3,
		10 * THeight + WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 3, THeight,
		hwnd, (HMENU)ObsPointA_yEdit, nullptr, nullptr
	);
	SendMessage(ObsPointA_y, EM_SETCUEBANNER, 0, (LPARAM)L"A_y pos");
	widgets[12] = ObsPointD_x = CreateWindowA(
		"edit", NULL,
		WS_CHILD | WS_VISIBLE | ES_NUMBER,
		3 * CWidth / 4 + WSeparatorWidth,
		11 * THeight + WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 3, THeight,
		hwnd, (HMENU)ObsPointD_xEdit, nullptr, nullptr
	);
	SendMessage(ObsPointD_x, EM_SETCUEBANNER, 0, (LPARAM)L"D_x pos");
	widgets[13] = ObsPointD_y = CreateWindowA(
		"edit", NULL,
		WS_CHILD | WS_VISIBLE | ES_NUMBER,
		3 * CWidth / 4 + WSeparatorWidth + (CWidth / 4 - WSeparatorWidth) / 3,
		11 * THeight + WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 3, THeight,
		hwnd, (HMENU)ObsPointD_yEdit, nullptr, nullptr
	);
	SendMessage(ObsPointD_y, EM_SETCUEBANNER, 0, (LPARAM)L"D_y pos");
	widgets[14] = CreateWindowA(
		"button", "set",
		WS_CHILD | WS_VISIBLE,
		3 * CWidth / 4 + WSeparatorWidth + 2 * (CWidth / 4 - WSeparatorWidth) / 3,
		10 * THeight + WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 4, 2 * THeight,
		hwnd, (HMENU)SetObsPoint, nullptr, nullptr
	);
	CreateWindowA(
		"static", NULL, WS_CHILD | WS_VISIBLE | SS_GRAYRECT,
		3 * CWidth / 4 + WSeparatorWidth,
		12 * THeight + 1.5 * WSeparatorWidth,
		CWidth / 4 - WSeparatorWidth, WSeparatorWidth,
		hwnd, nullptr, nullptr, nullptr
	);
	//CreateWindowSeparator(11 * THeight + 1.5*WSeparatorWidth, CWidth);
	widgets[15] = CreateWindowA(
		"button", "remove all obstacles",
		WS_CHILD | WS_VISIBLE,
		3 * CWidth / 4 + WSeparatorWidth,
		12 * THeight + 2.5 * WSeparatorWidth,
		CWidth / 4 - WSeparatorWidth, THeight,
		hwnd, (HMENU)SetRemoveObsButton, nullptr, nullptr
	);
	CreateWindowA(
		"static", NULL, WS_CHILD | WS_VISIBLE | SS_GRAYRECT,
		3 * CWidth / 4 + WSeparatorWidth,
		13 * THeight + 2.5 * WSeparatorWidth,
		CWidth / 4 - WSeparatorWidth, WSeparatorWidth,
		hwnd, nullptr, nullptr, nullptr
	);
	CreateWindowA(
		"static", NULL, WS_CHILD | WS_VISIBLE | SS_GRAYRECT,
		3 * CWidth / 4 + WSeparatorWidth,
		14 * THeight + 2.5 * WSeparatorWidth,
		CWidth / 4 - WSeparatorWidth, WSeparatorWidth,
		hwnd, nullptr, nullptr, nullptr
	);
	CreateWindowA(
		"static", "RRT parameters:", WS_CHILD | WS_VISIBLE,
		3 * CWidth / 4 + WSeparatorWidth,
		14 * THeight + 3.5 * WSeparatorWidth,
		CWidth / 4 - WSeparatorWidth, THeight,
		hwnd, nullptr, nullptr, nullptr
	);

	//
	// window to set RRT parameters
	//
	sampl_num_hwnd = CreateWindowA(
		"edit", NULL,
		WS_CHILD | WS_VISIBLE | ES_NUMBER,
		3 * CWidth / 4 + WSeparatorWidth, 15 * THeight + 3.5 * WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 3, THeight,
		hwnd, (HMENU)SamplesNumEdit, nullptr, nullptr
	);
	SendMessage(sampl_num_hwnd, EM_SETCUEBANNER, 0, (LPARAM)L"Samlpe N");
	
	end_range_hwnd = CreateWindowA(
		"edit", NULL,
		WS_CHILD | WS_VISIBLE | ES_NUMBER,
		3 * CWidth / 4 + WSeparatorWidth + (CWidth / 4 - WSeparatorWidth) / 3,
		15 * THeight + 3.5 * WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 3, THeight,
		hwnd, (HMENU)EndRangeEdit, nullptr, nullptr
	);
	SendMessage(end_range_hwnd, EM_SETCUEBANNER, 0, (LPARAM)L"End range");
	
	set_rrt_seed_hwnd = CreateWindowA(
		"edit", NULL,
		WS_CHILD | WS_VISIBLE | ES_NUMBER,
		3 * CWidth / 4 + WSeparatorWidth, 16 * THeight + 3.5 * WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 3, THeight,
		hwnd, (HMENU)RRTSeedEdit, nullptr, nullptr
	);
	SendMessage(set_rrt_seed_hwnd, EM_SETCUEBANNER, 0, (LPARAM)L"seed");


	CreateWindowA(
		"static", NULL, WS_CHILD | WS_VISIBLE | SS_GRAYRECT,
		3 * CWidth / 4 + WSeparatorWidth,
		17 * THeight + 3.5 * WSeparatorWidth,
		CWidth / 4 - WSeparatorWidth, WSeparatorWidth,
		hwnd, nullptr, nullptr, nullptr
	);
	widgets[16] = CreateWindowA(
		"button", "Start RRT", WS_CHILD | WS_VISIBLE,
		3 * CWidth / 4 + WSeparatorWidth,
		17 * THeight + 4.5 * WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 2, THeight,
		hwnd, (HMENU)StartRRTButton, nullptr, nullptr
	);
	reset_hwnd = CreateWindowA(
		"button", "Reset", WS_CHILD | WS_VISIBLE,
		3 * CWidth / 4 + WSeparatorWidth + (CWidth / 4 - WSeparatorWidth) / 2,
		17 * THeight + 4.5 * WSeparatorWidth,
		(CWidth / 4 - WSeparatorWidth) / 2, THeight,
		hwnd, (HMENU)ResetButton, nullptr, nullptr
	);
	CreateWindowA(
		"static", NULL, WS_CHILD | WS_VISIBLE | SS_GRAYRECT,
		3 * CWidth / 4 + WSeparatorWidth,
		18 * THeight + 4.5 * WSeparatorWidth,
		CWidth / 4 - WSeparatorWidth, WSeparatorWidth,
		hwnd, nullptr, nullptr, nullptr
	);
}

LRESULT CALLBACK CanvasSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//SendMessage(cptrA, WM_NCPAINT, 1, 0);
	try
	{
		if (bWEnable == Disable)
			return CallWindowProc(wpOrigCanvasProc, hWnd, uMsg, wParam, lParam);

		switch (uMsg)
		{
		case WM_MOUSEMOVE:
		{
			if (SelCptrLButtonPressed) {
				try
				{
					int x_2 = GET_X_LPARAM(lParam);
					int y_2 = GET_Y_LPARAM(lParam);
					SetObsAndCptrCoord(x_2, y_2, IndexSelectedCptr);
					canvas_redraw();
				}
				catch (const std::exception& ex)
				{
					MessageBoxError(ex);
				}
				return 0;
			}
			if (ObsSet.is_empty())
				return 0;
			if (SetObsLButtonPressed) {
				try
				{
					int x_2 = GET_X_LPARAM(lParam);
					int y_2 = GET_Y_LPARAM(lParam);
					//POINT A = GetLastObs->get_a();
					POINT A = GetLastObs->get_a();
					POINT B({ x_2, A.y });
					GetLastObs->set_i_point(1, B);
					POINT C({ x_2, y_2 });
					GetLastObs->set_i_point(2, C);
					POINT D({ A.x, y_2 });
					GetLastObs->set_i_point(3, D);

					canvas_redraw();
				}
				catch (const std::exception& ex)
				{
					MessageBoxError(ex);
				}
				/*catch (...) {}*/
			}
		}
		return 0;
		case WM_LBUTTONDOWN:
		{
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);
			if (CapturesIsActive) {
				Circle* SelectedCptr = CptrSetColision(x,y);
				if (SelectedCptr) {
					SelCptrLButtonPressed = true;
					
					CptrDetPos(IndexSelectedCptr);
					GetClipCursor(&rcOldClip);
					RECT rcCanvasClip;
					GetWindowRect(cnvs, &rcCanvasClip);
					POINT opposite_corner = SelectedObs->get_i((IndexSelectedCptr + 2) % 4);
					switch (SelCptrPos)
					{
					case left_top:
						rcCanvasClip.left++;
						rcCanvasClip.top++;
						rcCanvasClip.right = rcCanvasClip.left + opposite_corner.x - 2;
						rcCanvasClip.bottom = rcCanvasClip.top + opposite_corner.y - 2;
						break;
					case left_bottom:
						rcCanvasClip.left++;
						rcCanvasClip.top = rcCanvasClip.top + opposite_corner.y + 2;
						rcCanvasClip.right = rcCanvasClip.left + opposite_corner.x - 2;
						rcCanvasClip.bottom--;
						break;
					case right_top:
						rcCanvasClip.left = rcCanvasClip.left + opposite_corner.x + 2;
						rcCanvasClip.top++;
						rcCanvasClip.right--;
						rcCanvasClip.bottom = rcCanvasClip.top + opposite_corner.y - 2;
						break;
					case right_bottom:
						rcCanvasClip.left = rcCanvasClip.left + opposite_corner.x + 2;
						rcCanvasClip.top = rcCanvasClip.top + opposite_corner.y + 2;
						rcCanvasClip.right--;
						rcCanvasClip.bottom--;
						break;
					default:
						break;
					}
					ClipCursor(&rcCanvasClip);
					return 0;
				}
				else {
					CapturesIsActive = false;
					canvas_redraw();
				}
			}
			
			if (IsDlgButtonChecked(hwnd, RadioButtonSP)) {
				start.set_center(POINT{ x, y });
				canvas_redraw();
				return 0;
			}
			if (IsDlgButtonChecked(hwnd, RadioButtonEP)) {
				end.set_center(POINT{ x, y });
				canvas_redraw();
				return 0;
			}
			if (IsDlgButtonChecked(hwnd, RadioButtonSetObs)) {
				if (SetObsLButtonPressed) return 0;
				/*ObsSet.push(new FilledRectangle(
					hwnd, POINT{ x, y }, POINT{ x, y }, NULL, PumpkinColor)
				);*/
				ObsSet.push(new RectObstacle(
					hwnd, POINT{ x, y }, POINT{ x, y }, NULL, PumpkinColor)
				);
				//canvas_redraw(); zero-size rect - redrow isn't needed
				SetObsLButtonPressed = true;
				return 0;
			}
			if (IsDlgButtonChecked(hwnd, RadioButtonSelectObs)) {
				SelectedObs = ObsSetCollition(x,y);
				if (!SelectedObs) {
					return 0;
				}
				CapturesIsActive = true;
				/*POINT A = SelectedObs->get_a();
				POINT D = SelectedObs->get_d();*/
				//char buff[100]("\0");
				//std::string bb = std::to_string(SelectedObs->get_a().x);
				//strcat_s(buff, bb.c_str());
				//strcat_s(buff, ";");
				//bb = std::to_string(SelectedObs->get_a().y);
				//strcat_s(buff, bb.c_str());
				//MessageBoxA(hwnd, buff, "obs is finded", MB_OK);
				
				for (auto i = 0; i != 4; i++) {
					auto A = SelectedObs->get_i(i);
					(CptrArr[i])->set_center(A);
				}
				canvas_redraw();
			}
		}
		return 0;
		case WM_LBUTTONUP:
		{
			SetObsLButtonPressed = false;
			SelectedCptr = nullptr;
			if (SelCptrLButtonPressed == true) {
				SelCptrLButtonPressed = false;
				ClipCursor(&rcOldClip);
			}
		}
		return 0;
		default:
			break;
		}
	}
	catch (const std::exception& ex)
	{
		MessageBoxError(ex);
	}
	return CallWindowProc(wpOrigCanvasProc, hWnd, uMsg, wParam, lParam);
}

class Rectangle* ObsSetCollition(int x, int y)
{
	if (ObsSet.is_empty()) {
		return nullptr;
	}
	for (auto obs = --ObsSet.end(); obs != ObsSet.begin(); --obs)
	{
		if (RectCollition(x, y, (*obs)->get_a(), (*obs)->get_c()))
			return *obs;
	}
	class Rectangle* first_obs = *ObsSet.begin();
	if (RectCollition(x, y, first_obs->get_a(), first_obs->get_c()))
		return first_obs;
	return nullptr;
}

class Circle* CptrSetColision(int x, int y)
{
	// TODO: check usefullness statement
	if (CptrArr.empty()) {
		return nullptr;
	}
	BYTE i = 0;
	for (auto p_cptr = CptrArr.begin(); p_cptr != CptrArr.end(); p_cptr++, i++)
	{
		POINT left_top = { (*p_cptr)->get_center().x - CaptureRadius, (*p_cptr)->get_center().y - CaptureRadius };
		POINT right_buttom = { (*p_cptr)->get_center().x + CaptureRadius, (*p_cptr)->get_center().y + CaptureRadius };
		if (!RectCollition(x, y, left_top, right_buttom))
			continue;
		IndexSelectedCptr = i;
		return *p_cptr;
	}
	return nullptr;
}

bool RectCollition(int x, int y, POINT A, POINT C)
{
	if (((x > A.x && x < C.x) || (x > C.x && x < A.x)) &&
		((y > A.y && y < C.y) || (y > C.y && y < A.y)))
		return true;
	return false;
}

void SetObsAndCptrCoord(int x, int y, BYTE index)
{
	POINT Caught = SelectedObs->get_i(index);
	SelectedObs->set_i_point(index, { x,y });
	CptrArr[index]->set_center({ x,y });

	index = (++index) % 4;
	POINT CaughtAdj_1 = SelectedObs->get_i(index);
	index = (2 + index) % 4;
	POINT CaughtAdj_2 = SelectedObs->get_i(index);

	switch (SelCptrPos)
	{
	case left_top:
	{
		if (CaughtAdj_1.y < CaughtAdj_2.y) {
			SetObsAndCptrCoo(index, { x, CaughtAdj_2.y });
			index = (2 + index) % 4;
			SetObsAndCptrCoo(index, { CaughtAdj_1.x, y });
		}
		else {
			SetObsAndCptrCoo(index, { CaughtAdj_2.x, y });
			index = (2 + index) % 4;
			SetObsAndCptrCoo(index, { x, CaughtAdj_1.y });
		}
	}
	break;
	case left_bottom:
	{
		if (CaughtAdj_1.y < CaughtAdj_2.y) {
			SetObsAndCptrCoo(index, { CaughtAdj_2.x, y });
			index = (2 + index) % 4;
			SetObsAndCptrCoo(index, { x, CaughtAdj_1.y });
		}
		else {
			SetObsAndCptrCoo(index, { x, CaughtAdj_2.y });
			index = (2 + index) % 4;
			SetObsAndCptrCoo(index, { CaughtAdj_1.x, y });
		}
	}
	break;
	case right_top:
	{
		if (CaughtAdj_1.y < CaughtAdj_2.y) {
			SetObsAndCptrCoo(index, { x, CaughtAdj_2.y });
			index = (2 + index) % 4;
			SetObsAndCptrCoo(index, { CaughtAdj_1.x, y });
		}
		else {
			SetObsAndCptrCoo(index, { CaughtAdj_2.x, y });
			index = (2 + index) % 4;
			SetObsAndCptrCoo(index, { x, CaughtAdj_1.y });
		}
	}
	break;
	case right_bottom:
	{
		if (CaughtAdj_1.y < CaughtAdj_2.y) {
			SetObsAndCptrCoo(index, { CaughtAdj_2.x, y });
			index = (2 + index) % 4;
			SetObsAndCptrCoo(index, { x, CaughtAdj_1.y });
		}
		else {
			SetObsAndCptrCoo(index, { x, CaughtAdj_2.y });
			index = (2 + index) % 4;
			SetObsAndCptrCoo(index, { CaughtAdj_1.x, y });
		}
	}
	break;
	}

	/*POINT Caught = SelectedObs->get_i(index);
	SelectedObs->set_i_point(index, { x,y });

	index = (++index) % 4;
	POINT CaughtAdj_1 = SelectedObs->get_i(index);
	index = (2 + index) % 4;
	POINT CaughtAdj_2 = SelectedObs->get_i(index);

	switch (SelCptrPos)
	{
	case left_top:
	{
		if (CaughtAdj_1.y < CaughtAdj_2.y) {
			SelectedObs->set_i_point(index, { x, CaughtAdj_2.y });
			index = (2 + index) % 4;
			SelectedObs->set_i_point(index, { CaughtAdj_1.x, y });
		}
		else {
			SelectedObs->set_i_point(index, { CaughtAdj_2.x, y });
			index = (2 + index) % 4;
			SelectedObs->set_i_point(index, { x, CaughtAdj_1.y });
		}
	}
	break;
	case left_bottom:
	{
		if (CaughtAdj_1.y < CaughtAdj_2.y) {
			SelectedObs->set_i_point(index, { CaughtAdj_2.x, y });
			index = (2 + index) % 4;
			SelectedObs->set_i_point(index, { x, CaughtAdj_1.y });
		}
		else {
			SelectedObs->set_i_point(index, { x, CaughtAdj_2.y });
			index = (2 + index) % 4;
			SelectedObs->set_i_point(index, { CaughtAdj_1.x, y });
		}
	}
	break;
	case right_top:
	{
		if (CaughtAdj_1.y < CaughtAdj_2.y) {
			SelectedObs->set_i_point(index, { x, CaughtAdj_2.y });
			index = (2 + index) % 4;
			SelectedObs->set_i_point(index, { CaughtAdj_1.x, y });
		}
		else {
			SelectedObs->set_i_point(index, { CaughtAdj_2.x, y });
			index = (2 + index) % 4;
			SelectedObs->set_i_point(index, { x, CaughtAdj_1.y });
		}
	}
	break;
	case right_bottom:
	{
		if (CaughtAdj_1.y < CaughtAdj_2.y) {
			SelectedObs->set_i_point(index, { CaughtAdj_2.x, y });
			index = (2 + index) % 4;
			SelectedObs->set_i_point(index, { x, CaughtAdj_1.y });
		}
		else {
			SelectedObs->set_i_point(index, { x, CaughtAdj_2.y });
			index = (2 + index) % 4;
			SelectedObs->set_i_point(index, { CaughtAdj_1.x, y });
		}
	}
	break;
	}*/

	// TODO: code commented below is more time-effective, but need to add left-rigth adj checking.
	//POINT guide_vector{x - Caught.x, y - Caught.y};
	//POINT CaughtAdj = SelectedObs->get_i(index);
	//POINT CaughtAdjGuide{ x - CaughtAdj.x, 0 };
	//if (dot_product(guide_vector, CaughtAdjGuide) < 0) {
	//	SelectedObs->set_i_point(index, { CaughtAdj.x, y });
	//	index = (2 + index) % 4;
	//	CaughtAdj = SelectedObs->get_i(index);
	//	SelectedObs->set_i_point(index, { x, CaughtAdj.y });
	//}
	//SelectedObs->set_i_point(index, { x, CaughtAdj.y });
	//index = (2 + index) % 4;
	//CaughtAdj = SelectedObs->get_i(index);
	//SelectedObs->set_i_point(index, { CaughtAdj.x , y });
}

inline void SetObsAndCptrCoo(BYTE index, POINT coord) {
	SelectedObs->set_i_point(index, coord);
	CptrArr[index]->set_center(coord);
}

// determine position (left-top, left-bottom, right-top, right-bottom)
void CptrDetPos(BYTE index) {
	POINT Caught = SelectedObs->get_i(index);

	index = (++index) % 4;
	POINT CaughtAdj_1 = SelectedObs->get_i(index);
	index = (2 + index) % 4;
	POINT CaughtAdj_2 = SelectedObs->get_i(index);
	//if (Caught.x < CaughtAdj_1.x || Caught.x < CaughtAdj_2.x) {
	//	if (Caught.y < CaughtAdj_2.y || Caught.y < CaughtAdj_1.y) {
	//		// left-top
	//		SelCptrPos = left_top;
	//		return;
	//	}
	//	// left-bottom
	//	SelCptrPos = left_bottom;
	//	return;
	//}
	//if (Caught.y < CaughtAdj_2.y || Caught.y < CaughtAdj_1.y) {
	//	// right-top
	//	SelCptrPos = right_top;
	//	return;
	//}
	// right-bottom
	//SelCptrPos = right_bottom;
	if (Caught.x < CaughtAdj_1.x || Caught.x < CaughtAdj_2.x) {
		if (Caught.y < CaughtAdj_2.y || Caught.y < CaughtAdj_1.y) {
			// left-top
			SelCptrPos = left_top;
		}
		else {
			// left-bottom
			SelCptrPos = left_bottom;
		}
	}
	else {
		if (Caught.y < CaughtAdj_2.y || Caught.y < CaughtAdj_1.y) {
			// right-top
			SelCptrPos = right_top;
		}
		else {
			// right-bottom
			SelCptrPos = right_bottom;
		}
	}
}


void MessageBoxError(const std::exception& ex)
{
	char buff[200]("\0");
	strcat_s(buff, ex.what());
	strcat_s(buff, "\n");
	strcat_s(buff, typeid(ex).name());
	MessageBoxA(hwnd, buff, NULL, MB_ICONERROR);
}

inline void CreateWindowSeparator(int  y, int CWidth) {
	CreateWindowA(
		"static", NULL, WS_CHILD | WS_VISIBLE | SS_GRAYRECT,
		3 * CWidth / 4 + WSeparatorWidth, y,
		CWidth / 4 - WSeparatorWidth, WSeparatorWidth,
		hwnd, nullptr, nullptr, nullptr
	);
};

void canvas_redraw() {
	HDC memDC = CreateCompatibleDC(cnvsDC);
	HBITMAP memBM = CreateCompatibleBitmap(cnvsDC, cnvsRCT.right - cnvsRCT.left, cnvsRCT.bottom - cnvsRCT.top);
	SelectObject(memDC, memBM);
		HBRUSH OldBrush = SelectBrush(memDC, GetStockBrush(WHITE_BRUSH));
		Rectangle(memDC, cnvsRCT.left, cnvsRCT.top, cnvsRCT.right, cnvsRCT.bottom);
		SelectBrush(memDC, OldBrush);
		
		//obstacle drawing
		try
		{
			for (auto obs = ObsSet.begin(); obs != ObsSet.end(); obs++) {
				(*obs)->draw(memDC);
			}
		}
		catch (const std::exception& ex)
		{
			MessageBoxError(ex);
			return;
		}
		SelectBrush(memDC, OldBrush);
		//end_radius drawing
		HPEN OldPen = SelectPen(memDC, EndRangeElipsePen);
		Ellipse(memDC,
			end.get_center().x - end_range, end.get_center().y - end_range,
			end.get_center().x + end_range, end.get_center().y + end_range);
		//tree drawing
		SelectPen(memDC, GetStockPen(BLACK_PEN));
		for (auto vertex_it = rrt_impl.begin(); vertex_it != rrt_impl.end(); vertex_it++) {
			std::pair<long, long> cur_vertex = (*vertex_it).first;
			auto outgoing_vrtxs = (*vertex_it).second;
			for (auto v : outgoing_vrtxs) {
				MoveToEx(memDC, cur_vertex.first, cur_vertex.second, NULL);
				LineTo(memDC, v.first, v.second);
			}
		}
		// path drawing
		SelectPen(memDC, PathPen);
		if (!rrt_path.empty()) {
			std::pair<long, long> s_point = rrt_path[0];
			MoveToEx(memDC, s_point.first, s_point.second, NULL);
			for (auto i = ++(rrt_path.begin()); i != rrt_path.end(); i++) {
				LineTo(memDC, (*i).first, (*i).second);
			}
		}
		SelectPen(memDC, OldPen);

		try
		{
			start.draw(memDC);
		}
		catch (const std::exception& ex)
		{
			MessageBoxError(ex);
			return;
		}
		try
		{
			end.draw(memDC);
		}
		catch (const std::exception& ex)
		{
			MessageBoxError(ex);
			return;
		}

		if (CapturesIsActive)
			try
			{
				for (auto p_cptr = CptrArr.begin(); p_cptr != CptrArr.end(); p_cptr++) {
					(*p_cptr)->draw(memDC);
				}
			}
			catch (const std::exception& ex)
			{
				MessageBoxError(ex);
				return;
			}

	BitBlt(cnvsDC, 0, 0, cnvsRCT.right - cnvsRCT.left, cnvsRCT.bottom - cnvsRCT.top, memDC, 0, 0, SRCCOPY);
	DeleteDC(memDC);
	DeleteObject(memBM);
}

void SetGoalPoint(UINT uGOAL)
{
	BOOL xSuccess, ySuccess;
	long x, y;
	switch (uGOAL)
	{
	case SetStartPoint:
		x = GetDlgItemInt(hwnd, StartPointXEdit, &xSuccess, false);
		y = GetDlgItemInt(hwnd, StartPointYEdit, &ySuccess, false);
		if (xSuccess && ySuccess) {
			start.set_center(POINT{ x, y });
			canvas_redraw();
		}
		return;
	case SetEndPoint:
		x = GetDlgItemInt(hwnd, EndPointXEdit, &xSuccess, false);
		y = GetDlgItemInt(hwnd, EndPointYEdit, &ySuccess, false);
		if (xSuccess && ySuccess) {
			end.set_center(POINT{ x, y });
			canvas_redraw();
		}
		return;
	default:
		return;
	}
}

void EnableWidgets(bool bEnable) {
	for (auto& p : widgets) {
		EnableWindow(p, bEnable);
	}
	//SetLayeredWindowAttributes(cnvs_plug, RGB(100, 100, 100), 50, LWA_ALPHA);
}

const unsigned startData = 20;
void SaveScene(LPCSTR path) {

	std::ofstream fout;
	fout.open(path);

	//obstacle writing
	size_t ObsNum = 0;
	// allocate space for ObsNum and size of file
	for (auto i = 0; i < startData; i++) { fout << ' '; }
	for (auto it : ObsSet) {
		ObsNum++;
		fout << it->get_a().x << ' ' << it->get_a().y << ' '
			<< it->get_b().x << ' ' << it->get_b().y << ' '
			<< it->get_c().x << ' ' << it->get_c().y << ' '
			<< it->get_d().x << ' ' << it->get_d().y << ' ';
	}

	// writing QInit and QGoal
	fout << start.get_center().x << ' ' << start.get_center().y << ' ';
	fout << end.get_center().x << ' ' << end.get_center().y << ' ';
	
	// writing RRT param
	end_range = GetDlgItemInt(hwnd, EndRangeEdit, nullptr, false);
	int samples_num = GetDlgItemInt(hwnd, SamplesNumEdit, nullptr, false);
	rrt_seed = GetDlgItemInt(hwnd, RRTSeedEdit, nullptr, false);
	fout << end_range << ' ' << samples_num << ' ' << rrt_seed;


	//// writing path vector
	//for (auto i = 0; i < startData; i++)
	//	{ fout << ' '; } // allocate space for Tree
	//for (auto vertex_it = rrt_impl.begin(); vertex_it != rrt_impl.end(); vertex_it++) {
	//	std::pair<long, long> cur_vertex = (*vertex_it).first;
	//	auto outgoing_vrtxs = (*vertex_it).second;
	//	for (auto v : outgoing_vrtxs) {
	//		MoveToEx(memDC, cur_vertex.first, cur_vertex.second, NULL);
	//		LineTo(memDC, v.first, v.second);
	//	}
	//}



	// write size of file
	std::streampos fend = fout.tellp();
	fout.seekp(0, std::ios::beg);
	fout << ObsNum;
	fout.seekp(10, std::ios::beg);
	fout << fend;

	fout.close();
}

void LoadScene(LPCSTR path) {
	
	rrt_impl.delete_tree();
	rrt_path.clear();
	ObsSet.clear();

	std::ifstream fin(path);
	
	size_t ObsNum;
	fin >> ObsNum;
	// skip file info space
	fin.seekg(startData, std::ios::beg);

	std::string Rez;
	for (auto i = 0; i < ObsNum; i++)
	{
		std::pair<long, long> rect[4];
		for (auto& vrtx : rect) {
			fin >> vrtx.first >> vrtx.second;
			//check wrong data
			if (vrtx.first < 0 || vrtx.second < 0 ||
				vrtx.first > cnvsRCT.right - cnvsRCT.left ||
				vrtx.second > cnvsRCT.bottom - cnvsRCT.top) {
				MessageBoxError(std::invalid_argument("Obstacle size doesn't confirm to current app demention"));
				rrt_impl.delete_tree();
				rrt_path.clear();
				ObsSet.clear();
				return;
			}
		}
		ObsSet.push(new RectObstacle(
			hwnd, 
			POINT{ rect[0].first, rect[0].second},
			POINT{ rect[2].first, rect[2].second },
			NULL, RGB(255, 116, 0))
		);
		Rez += '\n';
	}

	// read QInit and QGoal
	POINT tmpp;
	try
	{
	fin >> tmpp.x >> tmpp.y;
	start.set_center(tmpp);
	fin >> tmpp.x >> tmpp.y;
	end.set_center(tmpp);
	}
	catch (const std::exception& ex)
	{
		MessageBoxError(ex);
		rrt_impl.delete_tree();
		rrt_path.clear();
		ObsSet.clear();
	}

	// read RRT param
	fin >> end_range; //GetDlgItemInt(hwnd, EndRangeEdit, nullptr, false);
	std::string samples_num;//GetDlgItemInt(hwnd, SamplesNumEdit, nullptr, false);
	fin >> samples_num;
	fin >> rrt_seed; //GetDlgItemInt(hwnd, RRTSeedEdit, nullptr, false);
	std::string tmpstr = std::to_string(long(end_range));
	SetWindowTextA(end_range_hwnd, tmpstr.c_str());
	SetWindowTextA(sampl_num_hwnd, samples_num.c_str());
	tmpstr = std::to_string(rrt_seed);
	SetWindowTextA(set_rrt_seed_hwnd, tmpstr.c_str());

	fin.close();

	/*std::string data;
	data += Rez;
	MessageBoxA(hwnd, data.c_str(), "Loading information", MB_OK);*/

	/*HANDLE FileToLoad = CreateFileA(
		path,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	char data[10];
	DWORD bytesIterate;
	if (ReadFile(FileToLoad, data, 10, &bytesIterate, NULL)) {
		MessageBoxA(hwnd, data, "Loading information", MB_OK);
	}

	CloseHandle(FileToLoad);*/
}

void SetOpenFileParams(HWND hwnd) {
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = sizeof(filename);
	ofn.lpstrFilter = "*.rrti";
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
}
