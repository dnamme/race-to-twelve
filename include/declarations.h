#pragma once



#include <windows.h>
#include <string>

#include "../include/GL/glut.h"

#include "../include/game.h"
#include "../include/lang.h"
#include "../include/struct.h"



#define _FPS 60
extern int _FRL;
extern int _CFR;
extern int _TFR;


#define AXIS_X 0
#define AXIS_Y 1

#define SCREEN_WIDTH 16 * 75
#define SCREEN_HEIGHT 9 * 75

extern int WINDOWTYPE;
extern int NXWINDOWTYPE;
#define WT_EMPTY 0
#define WT_PRELOADING 1
#define WT_LOADING 2
#define WT_MAINMENU 3
#define WT_PLAY 4

extern bool WT_VAR_PREL_isKBClicked;
extern bool WT_VAR_GLOBAL_isLBClicked;
extern int WT_VAR_GLOBAL_type;

#define WT_BTN_MMENU_PLAY 0
#define WT_BTN_MMENU_SETTINGS 1
#define WT_BTN_MMENU_EXIT 2

extern int WT_CFR_MODE;

#define WT_FRL_GLOB_FADE (_FPS * (float) 1/6)
#define WT_FRL_GLOB_FADE_SLOW (WT_FRL_GLOB_FADE * 1)


#define WT_FRL_LOAD_blackScreen (_FPS * 1)
#define WT_FRL_LOAD_blkscrDev (_FPS * 1)
#define WT_FRL_LOAD_iconRot (_FPS * 1)
#define WT_FRL_LOAD_loadingPause (_FPS * 3)
#define WT_FRL_LOAD_fadePause (_FPS * 1)

#define WT_CFRM_LOAD_blackScreen 0
#define WT_CFRM_LOAD_blkscrDev 1
#define WT_CFRM_LOAD_fadeIn 2
#define WT_CFRM_LOAD_actualLoading 3
#define WT_CFRM_LOAD_loadingPause 4
#define WT_CFRM_LOAD_fadeOut 5
#define WT_CFRM_LOAD_fadePause 6

#define WT_CFRM_MM_MM 0
#define WT_CFRM_MM_fadeOut 1
#define WT_CFRM_MM_exit 2


#define WT_FRL_PLAY_blackScreen (_FPS * 1)
#define WT_FRL_PLAY_tutoFadePause (_FPS * 1)
#define WT_FRL_PLAY_cubeAnimation (_FPS * (float) 1/6)
#define WT_FRL_PLAY_AIPause (_FPS * (float) 1/2)
#define WT_FRL_PLAY_AIClick (_FPS * (float) 1/10)
#define WT_FRL_PLAY_popupPause (_FPS * (float) 1/2)
#define WT_FRL_PLAY_popupText (_FPS * 1)

#define WT_CFRM_PLAY_blackScreen 0
#define WT_CFRM_PLAY_popupTuto 1
#define WT_CFRM_PLAY_tutoFade 2
#define WT_CFRM_PLAY_tutoFadePause 3
#define WT_CFRM_PLAY_gameFadeIn 4
#define WT_CFRM_PLAY_game 5
#define WT_CFRM_PLAY_popupPause 6
#define WT_CFRM_PLAY_popupText 7
#define WT_CFRM_PLAY_popupFadeOut 8

extern HFONT CFONT_PREL_PRE,
       CFONT_DEV,
	   CFONT_LOAD_LOAD,
	   CFONT_MMENU_TITLE,
	   CFONT_MMENU_TEXT,
	   CFONT_GAME_TEXT,
	   CFONT_GAME_CHC,
	   CFONT_GAME_NUMCHC,
	   CFONT_DEBUG;



extern TexCoordsUV *MMENU_PLAY,
                   *MMENU_SETTINGS,
                   *MMENU_EXIT,
                   *GAME_Q_NO,
                   *GAME_Q_YES,
				   *GAME_SEL01,
				   *GAME_SEL02,
				   *GAME_SEL03;



extern bool GAME_VAR_TUTO;
extern bool GAME_VAR_isRunning;



extern GLuint TEX_BG_LOAD,
              TEX_ICON_NUM[8],
              TEX_BLOCK_NUM,
              TEX_BLOCK_METAL_SMOOTH,
			  TEX_BLOCK_GOLD_TEXTURED,
			  TEX_BLOCK_METAL_TEXTURED,
			  TEX_BLOCK_CHROME_TEXTURED;

extern int TEX_BLOCK_NUM_P_;

extern bool TEX_BLOCK_NUM_isClicked[3];
extern bool TEX_BLOCK_NUM_AI_isClicked[3];

void drawFrame(bool newFrame = true);

void AddCFontResources();
void RemoveCFontResources();

HFONT glSetFont(HFONT font, bool removeFont = true);
GLvoid glPrint(float x, float y, std::string str, ...);
SIZE glGetBitmapStringSize(const char* text, HFONT font);
float toUVSize(long dim, int comp);
float toSTCoord(int pos, int axis);

void loadTextures();

GLuint loadBMP(const char* imagePath);
