#include <windows.h>
#include <windowsx.h>
#include <cstring>
#include <string>

#include <stdio.h>
#include <stdarg.h>

#include "../include/GL/glut.h"

#include "../include/declarations.h"



GLuint base;

HFONT CFONT_PREL_PRE,
      CFONT_DEV,
	  CFONT_LOAD_LOAD,
	  CFONT_MMENU_TITLE,
	  CFONT_MMENU_TEXT,
	  CFONT_GAME_TEXT,
	  CFONT_GAME_CHC,
	  CFONT_GAME_NUMCHC,
	  CFONT_DEBUG;



void AddCFontResources() {
	AddFontResource("../res/fonts/abstract.ttf");
	AddFontResource("../res/fonts/digitalix.ttf");
	AddFontResource("../res/fonts/m42.ttf");
	AddFontResource("../res/fonts/neoletters.ttf");
	AddFontResource("../res/fonts/PIXEAB__.ttf");
//	AddFontResource("../res/fonts/PIXEARG__.ttf");
	
	base = glGenLists(96);
	
	CFONT_PREL_PRE		= CreateFont(24, 16, 0, 0, FW_DONTCARE, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "neoletters");
	CFONT_DEV			= CreateFont(18, 12, 0, 0, FW_DONTCARE, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "neoletters");
	CFONT_LOAD_LOAD		= CreateFont(40, 0, 0, 0, FW_THIN, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "m42_flight 721");
	CFONT_MMENU_TITLE	= CreateFont(56, 0, 0, 0, FW_HEAVY, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "abstract");
	CFONT_MMENU_TEXT	= CreateFont(24, 0, 0, 0, FW_THIN, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "digitalix");
	CFONT_GAME_TEXT		= CreateFont(36, 0, 0, 0, FW_DONTCARE, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "pixel arial 11");
	CFONT_GAME_CHC		= CreateFont(28, 0, 0, 0, FW_DONTCARE, false, true, false, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "pixel arial 11");
	CFONT_GAME_NUMCHC	= CreateFont(48, 0, 0, 0, FW_THIN, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "neoletters");
	CFONT_DEBUG			= CreateFont(24, 0, 0, 0, FW_NORMAL, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Consolas");
}

void RemoveCFontResources() {
	RemoveFontResource("../res/fonts/abstract.ttf");
	RemoveFontResource("../res/fonts/digitalix.ttf");
	RemoveFontResource("../res/fonts/m42.ttf");
	RemoveFontResource("../res/fonts/neoletters.ttf");
	RemoveFontResource("../res/fonts/PIXEAB__.ttf");
//	RemoveFontResource("../res/fonts/PIXEARG__.ttf");
	
	DeleteFont(CFONT_PREL_PRE);
	DeleteFont(CFONT_DEV);
	DeleteFont(CFONT_LOAD_LOAD);
	DeleteFont(CFONT_MMENU_TITLE);
	DeleteFont(CFONT_MMENU_TEXT);
	DeleteFont(CFONT_GAME_TEXT);
	DeleteFont(CFONT_GAME_CHC);
	DeleteFont(CFONT_GAME_NUMCHC);
	DeleteFont(CFONT_DEBUG);
	
	glDeleteLists(base, 96);
}

HFONT glSetFont(HFONT font, bool removeFont) {
	HDC hDC = GetDC(GetActiveWindow());
	
	HFONT oldFont = (HFONT) SelectObject(hDC, font);
	wglUseFontBitmaps(hDC, 32, 96, base);
	if(removeFont) SelectObject(hDC, oldFont);
	else return oldFont;
}

GLvoid glPrint(float x, float y, std::string str, ...) {
	glRasterPos2f(x, y);
	
	const char *fmt = str.c_str();
	char text[256];
	va_list ap;
	
	if(fmt == NULL) return;
	
	va_start(ap, str);
	vsprintf(text, fmt, ap);
	va_end(ap);
	
	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

SIZE glGetBitmapStringSize(const char* text, HFONT font) {
	SIZE textSize;
	
	HFONT oldFont = glSetFont(font, false);
	GetTextExtentPoint32(GetDC(GetActiveWindow()), text, strlen(text), &textSize);
	SelectObject(GetDC(GetActiveWindow()), oldFont);
	
	return textSize;
}

float toUVSize(long dim, int comp) {
	return dim / ((float) comp / 2);
}

float toSTCoord(int pos, int axis) {
	int comp;
	if(axis == AXIS_X) {
		comp = SCREEN_WIDTH;
		int rem = pos - (comp / 2);
		
		return toUVSize(rem, comp);
	} if(axis == AXIS_Y) {
		comp = SCREEN_HEIGHT;
		int inv = comp - pos;
		int rem = inv - (comp / 2);
		
		return toUVSize(rem, comp);
	}
}
