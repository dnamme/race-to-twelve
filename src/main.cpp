#include <windows.h>
#include <windowsx.h>
#include <ctime>

#include "../include/GL/glut.h"

#include "../include/declarations.h"



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void EnableOpenGL(HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
void handleMouseClick(int x, int y);
void handleMouseUp();



// from declarations.h //
bool WT_VAR_PREL_isKBClicked = false;
bool GAME_VAR_TUTO;
bool GAME_VAR_isRunning = false;

bool TEX_BLOCK_NUM_isClicked[3] = { false, false, false };
bool TEX_BLOCK_NUM_AI_isClicked[3] = { false, false, false };
int TEX_BLOCK_NUM_P_ = -1;
// //

bool WT_VAR_GLOBAL_isLBClicked;
int WT_VAR_GLOBAL_type;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow) {
	WNDCLASS wc;
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	BOOL bQuit = FALSE;
	
	std::srand(std::time(0));
	
	/* register window class */
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, "A");
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Race to Twelve";
	RegisterClass(&wc);
	
	/* create main window */
	hWnd = CreateWindow("Race to Twelve", "Race to Twelve: A Minigame", WS_VISIBLE | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT + 35, NULL, NULL, hInstance, NULL);
		
	/* enable OpenGL for the window */
	EnableOpenGL(hWnd, &hDC, &hRC);
	AddCFontResources();
	
	SetTimer(hWnd, 1, _FRL / 2, (TIMERPROC) NULL);
	
	/* program main loop */
	while (!bQuit)	{
		/* check for messages */
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			/* handle or dispatch messages */
			if (msg.message == WM_QUIT) {
				bQuit = TRUE;
			} else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} else {
			/* OpenGL animation code goes here */
//			drawFrame();
		}
	}
	
	/* shutdown OpenGL */
	DisableOpenGL(hWnd, hDC, hRC);
	RemoveCFontResources();
	
	/* destroy the window explicitly */
	DestroyWindow(hWnd);
	
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {
		case WM_CREATE: {
			return 0;
		}
		case WM_CLOSE: {
			PostQuitMessage(0);
			return 0;
		}
		case WM_DESTROY: {
			return 0;
		}
		case WM_KEYDOWN: {
			if(WINDOWTYPE == WT_PRELOADING && !WT_VAR_PREL_isKBClicked) {
				WT_VAR_PREL_isKBClicked = true;
				_TFR = _CFR = 1;
			}
			return 0;
		}
		case WM_LBUTTONDOWN: {
			handleMouseClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		}
		case WM_LBUTTONUP: {
			handleMouseUp();
			return 0;
		}
/*		case WM_RBUTTONDOWN: {
			WINDOWTYPE = WT_MAINMENU;
			return 0;
		}*/
/*		case WM_PAINT: {
			drawFrame(false);
			return 0;
		}*/
		case WM_TIMER: {
			drawFrame();
			return 0;
		}
		default: {
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
}

void EnableOpenGL(HWND hWnd, HDC *hDC, HGLRC *hRC) {
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;
	
	/* get the device context (DC) */
	*hDC = GetDC(hWnd);
	
	/* set the pixel format for the DC */
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	iFormat = ChoosePixelFormat(*hDC, &pfd);
	SetPixelFormat(*hDC, iFormat, &pfd);
	
	/* create and enable the render context (RC) */
	*hRC = wglCreateContext(*hDC);
	wglMakeCurrent(*hDC, *hRC);
		
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	
	loadTextures();
}

void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC) {
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
}

bool isInRect(int XX, int YY, TexCoordsUV *rect) {
	float x = toSTCoord(XX, AXIS_X);
	float y = toSTCoord(YY, AXIS_Y);
	if(x >= rect->posL && x <= rect->posR && y >= rect->posB && y <= rect->posT) return true;
	else return false;
}

void handleMouseClick(int x, int y) {
	if(WINDOWTYPE == WT_MAINMENU) {
		if(WT_CFR_MODE == WT_CFRM_MM_MM) {
			if(isInRect(x, y, MMENU_PLAY)) {
				WT_CFR_MODE = WT_CFRM_MM_fadeOut;
				NXWINDOWTYPE = WT_PLAY;
			} else if(isInRect(x, y, MMENU_SETTINGS)) {
			} else if(isInRect(x, y, MMENU_EXIT))
				WT_CFR_MODE = WT_CFRM_MM_exit;
		} else if(WT_CFR_MODE == WT_CFRM_MM_exit) {
			if(isInRect(x, y, GAME_Q_NO))
				WT_CFR_MODE = WT_CFRM_MM_MM;
			else if(isInRect(x, y, GAME_Q_YES))
				PostQuitMessage(0);
		}
	} else if(WINDOWTYPE == WT_PLAY) {
		if(WT_CFR_MODE == WT_CFRM_PLAY_popupTuto) {
			/*if(isInRect(x, y, GAME_Q_NO)) {
				GAME_init();
				GAME_VAR_TUTO = false;
				WT_CFR_MODE = WT_CFRM_PLAY_tutoFade;
			} else */if(isInRect(x, y, GAME_Q_YES)) {
				GAME_init();
				GAME_VAR_TUTO = true;
				WT_CFR_MODE = WT_CFRM_PLAY_tutoFade;
			}
		} else if(WT_CFR_MODE == WT_CFRM_PLAY_game) {
			if(isInRect(x, y, GAME_SEL01)) {
				TEX_BLOCK_NUM_isClicked[0] = true;
				TEX_BLOCK_NUM_P_ = 0;
			} else if(isInRect(x, y, GAME_SEL02)) {
				TEX_BLOCK_NUM_isClicked[1] = true;
				TEX_BLOCK_NUM_P_ = 1;
			} else if(isInRect(x, y, GAME_SEL03)) {
				TEX_BLOCK_NUM_isClicked[2] = true;
				TEX_BLOCK_NUM_P_ = 2;
			}
		}
	}
}

void handleMouseUp() {
	if(WINDOWTYPE == WT_PLAY) {
		if(WT_CFR_MODE == WT_CFRM_PLAY_game && TEX_BLOCK_NUM_P_ != -1) {
			TEX_BLOCK_NUM_isClicked[TEX_BLOCK_NUM_P_] = false;
			if(GAME_TURN == TURN_PLAYER && GAME_CURSCORE + TEX_BLOCK_NUM_P_ + 1 <= 12) PlayerTurn(TEX_BLOCK_NUM_P_ + 1);
			TEX_BLOCK_NUM_P_ = -1;
		}
	}
}
