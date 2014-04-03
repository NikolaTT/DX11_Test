#ifndef WINDOW_INITIALIZER_H
#define WINDOW_INITIALIZER_H

#include <windows.h>

class WindowInitializer{

public:
	HINSTANCE               g_hInst = nullptr;
	HWND                    g_hWnd = nullptr;


	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);

	WindowInitializer(HINSTANCE hInstance, int nCmdShow);
	WindowInitializer();
	
	//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	

	
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif