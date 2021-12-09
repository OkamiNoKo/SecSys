#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

#include "Clusterisation.h"
#include "Rewrotter.h"
#include "Analisys.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int h = 0;
int g = 0;
int g0 = 0;
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TCHAR szClassName[] = "Mein Klass"; // строка с именем класса
	HWND hMainWnd; // создаём дескриптор будущего окошка
	MSG msg; // создём экземпляр структуры MSG для обработки сообщений
	WNDCLASSEX wc; // создаём экземпляр, для обращения к членам класса WNDCLASSEX
	wc.cbSize = sizeof(wc); // размер структуры (в байтах)
	wc.style = CS_HREDRAW | CS_VREDRAW; // стиль класса окошка
	wc.lpfnWndProc = WndProc; // указатель на пользовательскую функцию
	wc.lpszMenuName = NULL; // указатель на имя меню (у нас его нет)
	wc.lpszClassName = szClassName; // указатель на имя класса
	wc.cbWndExtra = NULL; // число освобождаемых байтов в конце структуры
	wc.cbClsExtra = NULL; // число освобождаемых байтов при создании экземпляра приложения
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // декриптор пиктограммы
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // дескриптор маленькой пиктограммы (в трэе)
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // дескриптор курсора
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // дескриптор кисти для закраски фона окна
	wc.hInstance = hInst; // указатель на строку, содержащую имя меню, применяемого для класса
	if (!RegisterClassEx(&wc)) {
		// в случае отсутствия регистрации класса:
		MessageBox(NULL, "Не получилось зарегистрировать класс!", "Ошибка", MB_OK);
		return NULL; // возвращаем, следовательно, выходим из WinMain
	}
	// Функция, создающая окошко:
	hMainWnd = CreateWindow(
		szClassName, // имя класса
		"Start Page", // имя окошка (то что сверху)
		WS_OVERLAPPEDWINDOW , // режимы отображения окошка
		CW_USEDEFAULT, // позиция окошка по оси х
		NULL, // позиция окошка по оси у (раз дефолт в х, то писать не нужно)
		1600, // ширина окошка
		600, // высота окошка (раз дефолт в ширине, то писать не нужно)
		(HWND)NULL, // дескриптор родительского окна
		NULL, // дескриптор меню
		hInst, // дескриптор экземпляра приложения
		NULL); // ничего не передаём из WndProc

	HWND button0 = CreateWindow(
		"BUTTON", /* this makes a "button" */
		"Start reWriting", /* this is the text which will appear in the button */
		WS_VISIBLE | WS_CHILD,
		740, /* these four lines are the position and dimensions of the button */
		50,
		220,
		120,
		hMainWnd, /* this is the buttons parent window */
		(HMENU)100, /* these next two lines pretty much tell windows what to do when the button is pressed */
		(HINSTANCE)GetWindowLong(hMainWnd, GWL_HINSTANCE),
		NULL);

	HWND button1 = CreateWindow(
		"BUTTON", /* this makes a "button" */
		"winSplitting", /* this is the text which will appear in the button */
		WS_VISIBLE | WS_CHILD,
		740, /* these four lines are the position and dimensions of the button */
		220,
		220,
		120,
		hMainWnd, /* this is the buttons parent window */
		(HMENU)101, /* these next two lines pretty much tell windows what to do when the button is pressed */
		(HINSTANCE)GetWindowLong(hMainWnd, GWL_HINSTANCE),
		NULL);

	HWND button2 = CreateWindow(
		"BUTTON", /* this makes a "button" */
		"Vector Calculation", /* this is the text which will appear in the button */
		WS_VISIBLE | WS_CHILD,
		740, /* these four lines are the position and dimensions of the button */
		380,
		220,
		120,
		hMainWnd, /* this is the buttons parent window */
		(HMENU)102, /* these next two lines pretty much tell windows what to do when the button is pressed */
		(HINSTANCE)GetWindowLong(hMainWnd, GWL_HINSTANCE),
		NULL);

	HWND button3 = CreateWindow(
		"BUTTON", /* this makes a "button" */
		"Dump Analising", /* this is the text which will appear in the button */
		WS_VISIBLE | WS_CHILD,
		50, /* these four lines are the position and dimensions of the button */
		60,
		480,
		150,
		hMainWnd, /* this is the buttons parent window */
		(HMENU)103, /* these next two lines pretty much tell windows what to do when the button is pressed */
		(HINSTANCE)GetWindowLong(hMainWnd, GWL_HINSTANCE),
		NULL);

	HWND button4 = CreateWindow(
		"BUTTON", /* this makes a "button" */
		"visualisation", /* this is the text which will appear in the button */
		WS_VISIBLE | WS_CHILD,
		50, /* these four lines are the position and dimensions of the button */
		300,
		480,
		150,
		hMainWnd, /* this is the buttons parent window */
		(HMENU)104, /* these next two lines pretty much tell windows what to do when the button is pressed */
		(HINSTANCE)GetWindowLong(hMainWnd, GWL_HINSTANCE),
		NULL);



	if (!hMainWnd) {
		// в случае некорректного создания окошка (неверные параметры и тп):
		MessageBox(NULL, "Error in window creating", "ERROR", MB_OK);
		return NULL;
	}
	ShowWindow(hMainWnd, nCmdShow); // отображаем окошко
	UpdateWindow(hMainWnd);// обновляем окошко
	while (GetMessage(&msg, NULL, NULL, NULL)) { // извлекаем сообщения из очереди, посылаемые фу-циями, ОС
		TranslateMessage(&msg); // интерпретируем сообщения
		DispatchMessage(&msg); // передаём сообщения обратно ОС
	}
	return msg.wParam; // возвращаем код выхода из приложения
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	RECT rectPlace;
	HFONT hFont;
	static char text[8] = { ' ','\0' };
	HDC hDC;
	RECT rect, rect1, rect2, rect3, rect4,rectV;
	PAINTSTRUCT ps;
	COLORREF colorText = RGB(140, 0, 55);
	switch (uMsg)
	{
	case WM_PAINT:

		hDC = BeginPaint(hWnd, &ps); // инициализируем контекст устройства
		if (g != 0)
		{
			g = g % 6;
			if (g == 1)
			{
				HBRUSH brush = CreateHatchBrush(HS_BDIAGONAL, RGB(100, 100, 100)); // Создаём кисть определённого стиля и цвета
				SelectObject(hDC, brush); //Выбираем кисть
				Rectangle(hDC, 0, 0, 1600, 1200); 
				DeleteObject(brush);
			}

			if (g == 2)
			{
				HBRUSH brush = CreateSolidBrush(RGB(200, 200, 200)); // Создаём кисть определённого стиля и цвета
				SelectObject(hDC, brush); //Выбираем кисть
				Rectangle(hDC, 0, 0, 1600, 1200); 
				DeleteObject(brush);
			}

			if (g == 3)
			{
				HBRUSH brush = CreateHatchBrush(HS_DIAGCROSS, RGB(180, 0, 120)); // Создаём кисть определённого стиля и цвета
				SelectObject(hDC, brush); //Выбираем кисть
				Rectangle(hDC, 0, 0, 1600, 1200); //Нарисовали прямоугольник, закрашенный неким стилем
				DeleteObject(brush);
			}
			if (g == 4)
			{
				HBRUSH brush = CreateSolidBrush(RGB(150, 100, 100)); // Создаём кисть определённого стиля и цвета
				SelectObject(hDC, brush); //Выбираем кисть
				Rectangle(hDC, 0, 0, 1600, 1200); //Нарисовали прямоугольник, закрашенный неким стилем
				DeleteObject(brush);
			}
			if (g == 5)
			{
				HBRUSH brush = CreateSolidBrush(RGB(0, 100, 100)); // Создаём кисть определённого стиля и цвета
				SelectObject(hDC, brush); //Выбираем кисть
				Rectangle(hDC, 0, 0, 1600, 1200); //Нарисовали прямоугольник, закрашенный неким стилем
				DeleteObject(brush);
			}
		}
		
		GetClientRect(hWnd, &rectPlace);
		SetTextColor(hDC, NULL);
		hFont = CreateFont(70, 0, 0, 0, 0, 0, 0, 0,
			DEFAULT_CHARSET,
			0, 0, 0, 0,
			L"Arial Bold"
		);
		
		if (h != 0)
		{
			int N = 1;
			int M = 2;
			float **points = vPoint(&N, &M);

			for (int i = 0; i < N; i++)
			{
				SetTextColor(hDC, RGB((int)(points[i][2] * 255), (int)(points[i][3] * 255), (int)(points[i][4] * 255)));
				rectV.bottom = 450 + points[i][1] * 30;
				rectV.top = 435 + points[i][1] * 30;
				rectV.left = 1150 + points[i][0] * 30;
				rectV.right = 1175 + points[i][0] * 30;
				DrawText(hDC, "*", -1, &rectV, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
			for (int i = N; i < N + M; i++)
			{
				SetTextColor(hDC, RGB(255, 0, 255));
				rectV.bottom = 450 + points[i][1] * 30;
				rectV.top = 435 + points[i][1] * 30;
				rectV.left = 1150 + points[i][0] * 30;
				rectV.right = 1175 + points[i][0] * 30;
				DrawText(hDC, "#", -1, &rectV, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
		}
		if (g0 != 0)
		{
			HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255)); // Создаём кисть определённого стиля и цвета
			SelectObject(hDC, brush); //Выбираем кисть
			Rectangle(hDC, 0, 0, 1600, 1200); //Нарисовали прямоугольник, закрашенный неким стилем
			DeleteObject(brush);
			h = 0;
			g = 0;
			g0 = 0;
		}
		SetTextColor(hDC, colorText);
		rect.bottom = 50;
		rect.top = 30;
		rect.left = 660;
		rect.right = 1060;
		DrawText(hDC, "#Based on dump file named Dump.pcap", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		rect1.bottom = 210;
		rect1.top = 180;
		rect1.left = 740;
		rect1.right = 980;
		DrawText(hDC, "% splitting data.Result file: reF.txt   ", -1, &rect1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		rect2.bottom = 370;
		rect2.top = 340;
		rect2.left = 700;
		rect2.right = 1020;
		DrawText(hDC, "/* Creating Vec, that convert nD to 2D */", -1, &rect2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		rect3.bottom = 50;
		rect3.top = 20;
		rect3.left = 50;
		rect3.right = 550;
		DrawText(hDC, "// Based on nextDump.pcap", -1, &rect3, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		rect4.bottom = 290;
		rect4.top = 260;
		rect4.left = 50;
		rect4.right = 550;
		DrawText(hDC, "%% Press Ctrl or press the button to visualise ", -1, &rect4, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		SelectObject(hDC, hFont);
		rectPlace.left = 550;
		rectPlace.right = 740;
		SetTextColor(hDC, RGB(70, 0, 140));
		DrawText(hDC, (LPCWSTR)text, -1, &rectPlace, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_HOME:case VK_END:case VK_PRIOR:
		case VK_NEXT:case VK_LEFT:case VK_RIGHT:
		case VK_UP:case VK_DOWN:case VK_DELETE:
		case VK_SPACE:
		case VK_CAPITAL:case VK_MENU:case VK_TAB:
		case VK_RETURN:
			break;
		case VK_BACK:
			g0++;
			text[0] = 'C'; text[1] = 'l'; text[2] = 'e'; text[3] = 'a'; text[4] = 'r'; text[5] = '\0';
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_SHIFT:
			g++;
			text[0] = 'B'; text[1] = 'a'; text[2] = 'c'; text[3] = 'k'; text[4] = '.'; text[5] = '\0';
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_CONTROL:
			text[0] = 'V'; text[1] = 'i'; text[2] = 's'; text[3] = 'u'; text[4] = 'a'; text[5] = 'l'; text[6] = '.'; text[7] = '\0';
			h++;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		default:
			text[0] = (char)wParam;
			text[1] = '\0';
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND: 
		if (wParam == 100)
		{
			reFile("Dump.pcap","reF.txt");
			MessageBox(NULL,L" Operation complited successfully ", L"Success", 0);
		}
		if (wParam == 101)
		{
			winReW("reF.txt","winF.txt", "212.193.209.129");
			MessageBox(NULL, " Operation complited successfully ", "Success", 0);
		}
		if (wParam == 102)
		{
			ChnCreator("winF.txt", "winF2.txt");
			main_func();
			lim_fun(0.85);
			MessageBox(NULL, " Operation complited successfully ", "Success", 0);
		}
		if (wParam == 103)
		{
			Analisys("Dump1.pcap" , 0.70, 0);
			MessageBox(NULL, " Operation complited successfully ", "Success", 0);
		}
		if (wParam == 104)
		{
			h++;
			text[0] = 'V'; text[1] = 'i'; text[2] = 's'; text[3] = 'u'; text[4] = 'a'; text[5] = 'l'; text[6] = '.'; text[7] = '\0';
			InvalidateRect(hWnd, NULL, TRUE);
			MessageBox(NULL, " Operation failed successfully ", "Success", 0);
		}
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return NULL;
}