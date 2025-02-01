#pragma once
//add an application manifest that indicates that ComCtl32.dll version 6 should be used
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "../GraphicObjects/Circle.h"
#include "../GraphicObjects/Rectangle.h"
#include "../GraphicObjects/RectObstacle.h"
#include "../GraphicObjects/FilledRectangle.h"
#include "../Containers/Set.h"
#include "../RRT/RRT.h"
#include <memory>
#include <vector>

#define WSeparatorWidth 2
//defining menu id
#define AboutOnMenuClicked 1
#define SaveOnMenuClicked 2
#define LoadOnMenuClicked 3
//defining vidget id
#define RadioButtonSP 10
#define RadioButtonEP 11
#define RadioButtonSetObs 12
#define RadioButtonSelectObs 13 
#define StartPointXEdit 14
#define StartPointYEdit 15
#define SetStartPoint 16
#define EndPointXEdit 17
#define EndPointYEdit 18
#define SetEndPoint 19
#define ObsPointA_xEdit 20
#define ObsPointA_yEdit 21
#define ObsPointD_xEdit 22
#define ObsPointD_yEdit 23
#define SetObsPoint 24
#define SetRemoveObsButton 25

#define StartRRTButton 26
#define ResetButton 27
#define EndRangeEdit 28
#define SamplesNumEdit 29
#define RRTSeedEdit 30

//defining constant
#define WHight 600
#define WWidth 800
#define StartAndEndPointSize 5
#define CaptureRadius 6
// other defines
#define GetLastObs (*(--ObsSet.end()))
#define PumpkinColor RGB(255, 117, 24)
#define EndPointColor RGB(236, 40, 40)
#define StartPointColor RGB(45, 160, 226)
#define CaptureColor RGB(255, 255, 0)
#define EndRangeElipseColor RGB(10, 200, 100)
#define PathColor RGB(100, 220, 110)
#define Enable true
#define Disable false

#include <string>


HWND hwnd {0};

HWND cnvs {0};
HWND widgets[17]{};
	bool bWEnable {Enable};
	HWND reset_hwnd {0};
WNDPROC wpOrigCanvasProc;
	bool SetObsLButtonPressed{ false };
	bool SelCptrLButtonPressed{ false };
	class Rectangle* SelectedObs{ nullptr };
		const int left_top     = 0;
		const int left_bottom  = 1;
		const int right_top    = 2;
		const int right_bottom = 3;
	class Rectangle* ObsSetCollition(int x, int y);
	bool RectCollition(int x, int y, POINT A, POINT C);
	bool CapturesIsActive{ false };
	class Circle* SelectedCptr{ nullptr };
		BYTE IndexSelectedCptr{ 0 };
		int SelCptrPos{ 0 };
		class Circle* CptrSetColision(int x, int y);
	void SetObsAndCptrCoord(int x, int y, BYTE IndexSelectedCptr);
		inline void SetObsAndCptrCoo(BYTE index, POINT coord);
	RECT rcOldClip{};
HDC cnvsDC {0};
RECT cnvsRCT {0};
PAINTSTRUCT cnvsPS;

RRT rrt_impl{};
unsigned rrt_seed = 0;
std::vector<std::pair<long, long>> rrt_path{};
double end_range{1};

//HBRUSH BlueSolidBrush = CreateSolidBrush(RGB(27, 27, 179));
HBRUSH PumpkinSolidBrush = CreateSolidBrush(PumpkinColor);
HPEN EndRangeElipsePen = CreatePen(PS_SOLID, 2, EndRangeElipseColor);
HPEN PathPen = CreatePen(PS_SOLID, 4, PathColor);

HWND StartPointX{ 0 }, StartPointY{ 0 };
HWND EndPointX{ 0 }, EndPointY{ 0 };
HWND ObsPointA_x{ 0 }, ObsPointA_y{ 0 }, ObsPointD_x{ 0 }, ObsPointD_y{ 0 };
HWND sampl_num_hwnd{ 0 }; HWND end_range_hwnd{ 0 };
HWND set_rrt_seed_hwnd{ 0 };
class Circle start(cnvs, { WWidth/4, WHight/5 }, StartAndEndPointSize, StartPointColor);
class Circle   end(cnvs, { WWidth/2, WHight/3 }, StartAndEndPointSize, EndPointColor);
Set<class RectObstacle> ObsSet;
// arr of capture to resize & remove selected obstacle
std::vector<Circle*> CptrArr (
	{ new Circle(cnvs, { WWidth / 2, WHight / 3 }, CaptureRadius, CaptureColor, PS_SOLID, true),
	  new Circle(cnvs, { WWidth / 2, WHight / 3 }, CaptureRadius, CaptureColor, PS_SOLID, true),
	  new Circle(cnvs, { WWidth / 2, WHight / 3 }, CaptureRadius, CaptureColor, PS_SOLID, true),
	  new Circle(cnvs, { WWidth / 2, WHight / 3 }, CaptureRadius, CaptureColor, PS_SOLID, true) }
);

inline LRESULT CALLBACK SoftwareMainProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
inline LRESULT CALLBACK CanvasSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
inline WNDCLASSEX CALLBACK NewWindowClass
(
	HBRUSH hBrush, HCURSOR hCursor, HINSTANCE hInstance, HICON hIcon, LPCWSTR name, WNDPROC Proc
);

inline void MainWndAddMenu(HWND hwnd);
inline void MainWndAddWidgets(HWND hwnd);

void canvas_redraw();
void SetGoalPoint(UINT uGOAL);
void CptrDetPos(BYTE index_selected_cptr);
//inline void CreateWindowSeparator(int  y, int CWidth);
void EnableWidgets(bool bEnable);
void MessageBoxError(const std::exception& ex);


char filename[260];
OPENFILENAMEA ofn;
void SetOpenFileParams(HWND hwnd);
void SaveScene(LPCSTR path);
void LoadScene(LPCSTR path);
