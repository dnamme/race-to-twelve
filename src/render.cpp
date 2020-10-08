#include <windows.h>
#include <cstring>
#include <cstdlib>
#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include "../include/GL/glext.h"

#include "../include/declarations.h"



// from declarations.h //
int _FRL = 1000 / _FPS;
int _CFR = 0;
int _TFR = 0;

TexCoordsUV *MMENU_PLAY,
            *MMENU_SETTINGS,
            *MMENU_EXIT,
            *GAME_Q_NO,
            *GAME_Q_YES,
			*GAME_SEL01,
			*GAME_SEL02,
			*GAME_SEL03;
             
int WINDOWTYPE = WT_PRELOADING,//WT_MAINMENU,
    NXWINDOWTYPE = WT_EMPTY; //WT_PRELOADING
int WT_CFR_MODE = 0;

int GAME_TURN = TURN_NOTURN;
bool GAME_FRAMEDRAWN = false;
// //

int WT_VAR_LOAD_rotateNum = 3 + (rand() % 3),
    WT_VAR_LOAD_cNum = 1,
	WT_VAR_LOAD_rNum = -1;
bool foo = false;
int saveWT = WINDOWTYPE;
int saveCF = WT_CFR_MODE;

float theta = 0.0f;



void drawFrame_PRELOADING() {
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f - (WT_VAR_PREL_isKBClicked ? (_TFR * (1.0f / WT_FRL_GLOB_FADE)) : 0.0f));
	glSetFont(CFONT_PREL_PRE);
	glPrint(-0.85f, -0.75f, std::string("> ") + LN_PRELOADING_PRE + "." + (_CFR <= 30 ? "." : "_"));
	
	if(WT_VAR_PREL_isKBClicked && _TFR >= WT_FRL_GLOB_FADE)
		WINDOWTYPE = WT_LOADING;
}

void drawFrame_LOADING() {
	if(WT_CFR_MODE == WT_CFRM_LOAD_blackScreen) { // black screen
		if(_TFR >= WT_FRL_LOAD_blackScreen)
			WT_CFR_MODE = WT_CFRM_LOAD_blkscrDev;
	} else if(WT_CFR_MODE == WT_CFRM_LOAD_blkscrDev) { // black screen + dev text
		if(_TFR >= WT_FRL_LOAD_blkscrDev)
			WT_CFR_MODE = WT_CFRM_LOAD_fadeIn;
	} else if(WT_CFR_MODE == WT_CFRM_LOAD_fadeIn) { // loading icon [fade in]
		glBegin(GL_QUADS);
			glColor4f(1.0f, (float) 191/255, 0.0f, _CFR / WT_FRL_GLOB_FADE);
			
			glVertex2i(-1,  1);
			glVertex2i( 1,  1);
			glVertex2i( 1, -1);
			glVertex2i(-1, -1);
		glEnd();
		
		
		if(WT_VAR_LOAD_rNum == -1) WT_VAR_LOAD_rNum = rand() % (sizeof(TEX_ICON_NUM) / sizeof(*TEX_ICON_NUM));
		
		
		glEnable(GL_TEXTURE_2D);
		glColor4f(1, 1, 1, 1);
		
		glBindTexture(GL_TEXTURE_2D, TEX_ICON_NUM[WT_VAR_LOAD_rNum]);
		glBegin(GL_QUADS);
			float UV_size_w = (_TFR / WT_FRL_GLOB_FADE) * toUVSize((SCREEN_HEIGHT / 3), SCREEN_WIDTH),
			      UV_size_h = (_TFR / WT_FRL_GLOB_FADE) * toUVSize((SCREEN_HEIGHT / 3), SCREEN_HEIGHT);
			glTexCoord2f(0, 0);		glVertex2f(0.0f - (UV_size_w / 2), 0.0f + (UV_size_h / 2));
			glTexCoord2f(1, 0);		glVertex2f(0.0f + (UV_size_w / 2), 0.0f + (UV_size_h / 2));
			glTexCoord2f(1, 1);		glVertex2f(0.0f + (UV_size_w / 2), 0.0f - (UV_size_h / 2));
			glTexCoord2f(0, 1);		glVertex2f(0.0f - (UV_size_w / 2), 0.0f - (UV_size_h / 2));
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
		
		
		glColor4f(0, 0, 0, 1);
		HFONT font = CreateFont((_CFR / WT_FRL_GLOB_FADE) * 40, 0, 0, 0, FW_HEAVY, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "m42_flight 721");
		glSetFont(font);
		glPrint(0.0f - (toUVSize(glGetBitmapStringSize((std::string(LN_LOADING_LOADING) + ".....").c_str(), font).cx, SCREEN_WIDTH) / 2), 0.0f - (0.5f * (_CFR / WT_FRL_GLOB_FADE)), LN_LOADING_LOADING);
		
		if(_TFR >= WT_FRL_GLOB_FADE)
			WT_CFR_MODE = WT_CFRM_LOAD_actualLoading;
	} else if(WT_CFR_MODE == WT_CFRM_LOAD_actualLoading) { // loading icon [rotate]
		glBegin(GL_QUADS);
			glColor4f(1.0f, (float) 191/255, 0.0f, 1.0f);
			
			glVertex2i(-1,  1);
			glVertex2i( 1,  1);
			glVertex2i( 1, -1);
			glVertex2i(-1, -1);
		glEnd();
		
		
		glPushMatrix();
		
		glRotatef(theta, 0, 1, 0);
		glEnable(GL_TEXTURE_2D);
		
		glColor4f(1, 1, 1, 1);
		
		if(WT_VAR_LOAD_cNum == WT_VAR_LOAD_rotateNum)
			WT_VAR_LOAD_rNum = 3;
		glBindTexture(GL_TEXTURE_2D, TEX_ICON_NUM[WT_VAR_LOAD_rNum]);
		glBegin(GL_QUADS);
			float UV_size_w = toUVSize((SCREEN_HEIGHT / 3), SCREEN_WIDTH),
			      UV_size_h = toUVSize((SCREEN_HEIGHT / 3), SCREEN_HEIGHT);
			glTexCoord2f(0, 0);		glVertex2f(0.0f - (UV_size_w / 2), 0.0f + (UV_size_h / 2));
			glTexCoord2f(1, 0);		glVertex2f(0.0f + (UV_size_w / 2), 0.0f + (UV_size_h / 2));
			glTexCoord2f(1, 1);		glVertex2f(0.0f + (UV_size_w / 2), 0.0f - (UV_size_h / 2));
			glTexCoord2f(0, 1);		glVertex2f(0.0f - (UV_size_w / 2), 0.0f - (UV_size_h / 2));
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
		
		glPopMatrix();
		
		theta += 360 / (WT_FRL_LOAD_iconRot - 1);
		
		glColor4f(0, 0, 0, 1);
		glSetFont(CFONT_LOAD_LOAD);
		
		std::string loadingPrd = ".";
		for(int i = 1; i <= (float) _CFR / (_FPS / 5) - (_CFR % _FPS == 0 ? 1 : 0); i++)
			loadingPrd += ".";
		
		glPrint(0.0f - (toUVSize(glGetBitmapStringSize((std::string(LN_LOADING_LOADING) + ".....").c_str(), CFONT_LOAD_LOAD).cx, SCREEN_WIDTH) / 2), -0.5f, LN_LOADING_LOADING + loadingPrd);
		
		if(_TFR >= WT_VAR_LOAD_rotateNum * WT_FRL_LOAD_iconRot) {
			WT_CFR_MODE = WT_CFRM_LOAD_loadingPause;
			theta = 0;
		} if(_TFR % WT_FRL_LOAD_iconRot == 0) {
			WT_VAR_LOAD_rNum = rand() % (sizeof(TEX_ICON_NUM) / sizeof(*TEX_ICON_NUM));
			WT_VAR_LOAD_cNum++;
		}
	} else if(WT_CFR_MODE == WT_CFRM_LOAD_loadingPause) { // pause after loading completion
		glBegin(GL_QUADS);
			glColor4f(1.0f, (float) 191/255, 0.0f, 1.0f);
			
			glVertex2i(-1,  1);
			glVertex2i( 1,  1);
			glVertex2i( 1, -1);
			glVertex2i(-1, -1);
		glEnd();
		
		
		glPushMatrix();
		
		glRotatef(theta, 0, 1, 0);
		glEnable(GL_TEXTURE_2D);
		
		glColor4f(1, 1, 1, 1);
		
		glBindTexture(GL_TEXTURE_2D, TEX_ICON_NUM[3]);
		glBegin(GL_QUADS);
			float UV_size_w = toUVSize((SCREEN_HEIGHT / 3), SCREEN_WIDTH),
			      UV_size_h = toUVSize((SCREEN_HEIGHT / 3), SCREEN_HEIGHT);
			glTexCoord2f(0, 0);		glVertex2f(0.0f - (UV_size_w / 2), 0.0f + (UV_size_h / 2));
			glTexCoord2f(1, 0);		glVertex2f(0.0f + (UV_size_w / 2), 0.0f + (UV_size_h / 2));
			glTexCoord2f(1, 1);		glVertex2f(0.0f + (UV_size_w / 2), 0.0f - (UV_size_h / 2));
			glTexCoord2f(0, 1);		glVertex2f(0.0f - (UV_size_w / 2), 0.0f - (UV_size_h / 2));
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		
		theta += (30 * ((float) _TFR / WT_FRL_LOAD_loadingPause)) >= 360 / (WT_FRL_LOAD_iconRot - 1) ? 30 * ((float) _TFR / WT_FRL_LOAD_loadingPause) : 360 / (WT_FRL_LOAD_iconRot - 1);
		
		
		if(_TFR >= WT_FRL_LOAD_loadingPause)
			WT_CFR_MODE = WT_CFRM_LOAD_fadeOut;
	} else if(WT_CFR_MODE == WT_CFRM_LOAD_fadeOut) { // screen [fade out], icon [move]
		glBegin(GL_QUADS);
			glColor4f(1, (float) 191/255, 0, 1.0f - _TFR / WT_FRL_GLOB_FADE);
			
			glVertex2i(-1,  1);
			glVertex2i( 1,  1);
			glVertex2i( 1, -1);
			glVertex2i(-1, -1);
		glEnd();
		
		
		
		glPushMatrix();
		
		glRotatef(theta, 0, 1, 0);
		glEnable(GL_TEXTURE_2D);
		
		glColor4f(1, 1, 1, 1.0f - _TFR / WT_FRL_GLOB_FADE);
		
		glBindTexture(GL_TEXTURE_2D, TEX_ICON_NUM[3]);
		glBegin(GL_QUADS);
			float UV_size_w = toUVSize((1 + (3 * _TFR / WT_FRL_GLOB_FADE)) * (SCREEN_HEIGHT / 3), SCREEN_WIDTH),
			      UV_size_h = toUVSize((1 + (3 * _TFR / WT_FRL_GLOB_FADE)) * (SCREEN_HEIGHT / 3), SCREEN_HEIGHT);
			glTexCoord2f(0, 0);		glVertex2f(0.0f - (UV_size_w / 2), 0.0f + (UV_size_h / 2));
			glTexCoord2f(1, 0);		glVertex2f(0.0f + (UV_size_w / 2), 0.0f + (UV_size_h / 2));
			glTexCoord2f(1, 1);		glVertex2f(0.0f + (UV_size_w / 2), 0.0f - (UV_size_h / 2));
			glTexCoord2f(0, 1);		glVertex2f(0.0f - (UV_size_w / 2), 0.0f - (UV_size_h / 2));
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		
		theta += 360 / (WT_FRL_LOAD_iconRot - 1);

		
		if(_TFR >= WT_FRL_GLOB_FADE)
			WT_CFR_MODE = WT_CFRM_LOAD_fadePause;
	} else if(WT_CFR_MODE == WT_CFRM_LOAD_fadePause) {
		if(_TFR >= WT_FRL_LOAD_fadePause)
			WINDOWTYPE = WT_MAINMENU;
	}
	
	
	
	if(WT_CFR_MODE > WT_CFRM_LOAD_blackScreen) { // [text] dev text
		if(WT_CFR_MODE == WT_CFRM_LOAD_fadeIn) {
			float bit = 1.0f - (_CFR / WT_FRL_GLOB_FADE);
			glColor4f(bit, bit, bit, 1.0f);
		} else if(WT_CFR_MODE == WT_CFRM_LOAD_actualLoading || WT_CFR_MODE == WT_CFRM_LOAD_loadingPause) glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		else if(WT_CFR_MODE == WT_CFRM_LOAD_fadeOut) {
			float bit = _CFR / WT_FRL_GLOB_FADE;
			glColor4f(bit, bit, bit, 1.0f);
		} else glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glSetFont(CFONT_DEV);
		glPrint(-0.95f, -0.95f, LN_DEV);
	}
}

void drawFrame_MAINMENU() {
	float fadeAlpha = (WT_CFR_MODE == WT_CFRM_MM_fadeOut) ? (_TFR / WT_FRL_GLOB_FADE) : 0;
	
	glBegin(GL_QUADS);
		glColor4f(1, 1, 1, 1.0f - fadeAlpha);
		
		glVertex2i(-1,  1);
		glVertex2i( 1,  1);
		glVertex2i( 1, -1);
		glVertex2i(-1, -1);
	glEnd();
	
	glColor4f(0, 0, 0, 1);
	glSetFont(CFONT_DEV);
	glPrint(-0.95f, -0.95f, LN_DEV);
	
	SIZE size_title = glGetBitmapStringSize(LN_MMENU_TITLE, CFONT_MMENU_TITLE),
	     size_text[3] = {
			glGetBitmapStringSize(LN_MMENU_PLAY, CFONT_MMENU_TEXT),
			glGetBitmapStringSize(LN_MMENU_SETTINGS, CFONT_MMENU_TEXT),
			glGetBitmapStringSize(LN_MMENU_EXIT, CFONT_MMENU_TEXT)
		 };
	float UV_sizeW_title	= toUVSize(size_title.cx, SCREEN_WIDTH);
	float UV_sizeH_text		= toUVSize(size_text[0].cy, SCREEN_HEIGHT);
	float UV_posX = 0.0f - (UV_sizeW_title / 2) + (UV_sizeW_title * (float) 1/16);
	
	
	glSetFont(CFONT_MMENU_TITLE);
	glPrint(0.0f - (UV_sizeW_title / 2), 0.5f, LN_MMENU_TITLE);
	
	glSetFont(CFONT_MMENU_TEXT);
	
	glPrint(UV_posX, 0.0f - (2.0f * UV_sizeH_text), LN_MMENU_PLAY);
//	glPrint(UV_posX, 0.0f - (4.5f * UV_sizeH_text), LN_MMENU_SETTINGS);
	glPrint(UV_posX, 0.0f - (7.0f * UV_sizeH_text), LN_MMENU_EXIT);
	
	MMENU_PLAY		= new TexCoordsUV(
						UV_posX,
						UV_posX + toUVSize(size_text[0].cx, SCREEN_WIDTH),
						-2.0f * UV_sizeH_text,
						-2.0f * UV_sizeH_text + toUVSize(size_text[0].cy, SCREEN_HEIGHT)
					);
	MMENU_SETTINGS	= new TexCoordsUV(
						UV_posX,
						UV_posX + toUVSize(size_text[1].cx, SCREEN_WIDTH),
						-4.5f * UV_sizeH_text,
						-4.5f * UV_sizeH_text + toUVSize(size_text[0].cy, SCREEN_HEIGHT)
					);
	MMENU_EXIT		= new TexCoordsUV(
						UV_posX,
						UV_posX + toUVSize(size_text[2].cx, SCREEN_WIDTH),
						-7.0f * UV_sizeH_text,
						-7.0f * UV_sizeH_text + toUVSize(size_text[0].cy, SCREEN_HEIGHT)
					);
	
	if(WT_CFR_MODE == WT_CFRM_MM_exit) {
		glBegin(GL_QUADS);
			glColor4f(0, 0, 0, 0.75f);
			
			glVertex2i(-1,  1);
			glVertex2i( 1,  1);
			glVertex2i( 1, -1);
			glVertex2i(-1, -1);
		glEnd();
		
		glSetFont(CFONT_GAME_TEXT);
		glColor4f(1, 1, 1, 1);
		glPrint(0.0f - (toUVSize(glGetBitmapStringSize(LN_MMENU_EXIT_Q, CFONT_GAME_TEXT).cx, SCREEN_WIDTH) / 2), 0.1f, LN_MMENU_EXIT_Q);
		
		SIZE size_no	= glGetBitmapStringSize(LN_CHC_NO, CFONT_GAME_CHC),
		     size_yes	= glGetBitmapStringSize(LN_CHC_YES, CFONT_GAME_CHC);
		
		glSetFont(CFONT_GAME_CHC);
		glColor4f(1, 1, 1, 1);
		glPrint(-0.15f - (toUVSize(size_no.cx, SCREEN_WIDTH) / 2), -0.25f, LN_CHC_NO);
		glPrint( 0.15f - (toUVSize(size_yes.cx, SCREEN_WIDTH) / 2), -0.25f, LN_CHC_YES);
		
		GAME_Q_NO	= new TexCoordsUV(
							-0.15f - (toUVSize(size_no.cx, SCREEN_WIDTH) / 2),
							-0.15f - (toUVSize(size_no.cx, SCREEN_WIDTH) / 2) + toUVSize(size_no.cx, SCREEN_WIDTH),
							-0.25f,
							-0.25f + toUVSize(size_no.cy, SCREEN_HEIGHT)
						);
		GAME_Q_YES	= new TexCoordsUV(
							0.15f - (toUVSize(size_yes.cx, SCREEN_WIDTH) / 2),
							0.15f - (toUVSize(size_yes.cx, SCREEN_WIDTH) / 2) + toUVSize(size_yes.cx, SCREEN_WIDTH),
							-0.25f,
							-0.25f + toUVSize(size_yes.cy, SCREEN_HEIGHT)
						);
	}
	
	if(NXWINDOWTYPE != WT_EMPTY && _TFR >= WT_FRL_GLOB_FADE) {
		WINDOWTYPE = NXWINDOWTYPE;
		NXWINDOWTYPE = WT_EMPTY;
	}
}

int size_bgblock = SCREEN_WIDTH / 16;
int TOTX;
int TOTY;
bool GAME_BG_hasGen = false;
Block GAME_BG[16][9];

void GAME_GAME_GENBG() {
	TOTX = SCREEN_WIDTH / size_bgblock;
	TOTY = SCREEN_HEIGHT / size_bgblock;
	for(int i = 0; i < TOTX; i++) {
		for(int ii = 0; ii < TOTY; ii++) {
//			int posX = rand() % 8;
//			int posY = rand() % 4;
			GAME_BG[i][ii] = new Block(true);
		}
	}
}

void GAME_GAME_DRAWBACKGROUND() {
	if(!GAME_BG_hasGen) {
		GAME_GAME_GENBG();
		GAME_BG_hasGen = true;
	}
	
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, TEX_BLOCK_METAL_SMOOTH);
	int foo = 0;
	for(int i = 0; i < TOTX; i++) {
		for(int ii = 0; ii <= TOTY; ii++) {
			glBegin(GL_QUADS);
				glTexCoord2f(        0.125 * GAME_BG[i][ii].texX, 0.125 + 0.125 * GAME_BG[i][ii].texY);	glVertex2f(                                       toSTCoord(size_bgblock * i, AXIS_X), toSTCoord(size_bgblock * ii, AXIS_Y) + toUVSize(size_bgblock, SCREEN_HEIGHT));
				glTexCoord2f(0.125 + 0.125 * GAME_BG[i][ii].texX, 0.125 + 0.125 * GAME_BG[i][ii].texY);	glVertex2f(toUVSize(size_bgblock, SCREEN_WIDTH) + toSTCoord(size_bgblock * i, AXIS_X), toSTCoord(size_bgblock * ii, AXIS_Y) + toUVSize(size_bgblock, SCREEN_HEIGHT));
				glTexCoord2f(0.125 + 0.125 * GAME_BG[i][ii].texX,         0.125 * GAME_BG[i][ii].texY);	glVertex2f(toUVSize(size_bgblock, SCREEN_WIDTH) + toSTCoord(size_bgblock * i, AXIS_X), toSTCoord(size_bgblock * ii, AXIS_Y));
				glTexCoord2f(        0.125 * GAME_BG[i][ii].texX,         0.125 * GAME_BG[i][ii].texY);	glVertex2f(                                       toSTCoord(size_bgblock * i, AXIS_X), toSTCoord(size_bgblock * ii, AXIS_Y));
			glEnd();
			foo++;
		}
	}
		
	glDisable(GL_TEXTURE_2D);
	
	glBegin(GL_QUADS);
		glColor4f(0, 0, 0, 0.75f);
		glVertex2f(-1,  1);
		glVertex2f( 1,  1);
		glVertex2f( 1, -1);
		glVertex2f(-1, -1);
	glEnd();
}

void drawFrame_PLAY() {
	if(WT_CFR_MODE == WT_CFRM_PLAY_blackScreen) {
		if(_TFR >= WT_FRL_PLAY_blackScreen)
			WT_CFR_MODE = WT_CFRM_PLAY_popupTuto;
	} else if(WT_CFR_MODE == WT_CFRM_PLAY_popupTuto) {
		glSetFont(CFONT_GAME_TEXT);
		glColor4f(1, 1, 1, 1);
		glPrint(0.0f - (toUVSize(glGetBitmapStringSize(LN_TUTORIAL_TXT, CFONT_GAME_TEXT).cx, SCREEN_WIDTH) / 2), 0.25f, LN_TUTORIAL_TXT);
		
		SIZE size_tuto_text[3] = {
							glGetBitmapStringSize(LN_TUTORIAL_LINE01, CFONT_GAME_TEXT),
							glGetBitmapStringSize(LN_TUTORIAL_LINE02, CFONT_GAME_TEXT),
							glGetBitmapStringSize(LN_TUTORIAL_LINE03, CFONT_GAME_TEXT)
						};
		
		glPrint(0.0f - toUVSize(size_tuto_text[0].cx / 2, SCREEN_WIDTH),  0.05f, LN_TUTORIAL_LINE01);
		glPrint(0.0f - toUVSize(size_tuto_text[1].cx / 2, SCREEN_WIDTH), -0.05f, LN_TUTORIAL_LINE02);
		glPrint(0.0f - toUVSize(size_tuto_text[2].cx / 2, SCREEN_WIDTH), -0.15f, LN_TUTORIAL_LINE03);
		
		
		SIZE size_ok	= glGetBitmapStringSize(LN_CHC_OK, CFONT_GAME_CHC);
		
		glSetFont(CFONT_GAME_CHC);
		glColor4f(1, 1, 1, 1);
		glPrint(0.0f - (toUVSize(size_ok.cx, SCREEN_WIDTH) / 2), -0.5f, LN_CHC_OK);
		
		GAME_Q_YES = new TexCoordsUV(
							0.0f - (toUVSize(size_ok.cx, SCREEN_WIDTH) / 2),
							0.0f + (toUVSize(size_ok.cx, SCREEN_WIDTH) / 2),
							-0.5f,
							-0.5f + toUVSize(size_ok.cy, SCREEN_HEIGHT)
						);
	} else if(WT_CFR_MODE == WT_CFRM_PLAY_tutoFade) {
		glSetFont(CFONT_GAME_TEXT);
		glColor4f(1, 1, 1, 1.0f - ((float) _TFR / WT_FRL_GLOB_FADE_SLOW));
		glPrint(0.0f - (toUVSize(glGetBitmapStringSize(LN_TUTORIAL_TXT, CFONT_GAME_TEXT).cx, SCREEN_WIDTH) / 2), 0.25f, LN_TUTORIAL_TXT);
		
		SIZE size_tuto_text[3] = {
							glGetBitmapStringSize(LN_TUTORIAL_LINE01, CFONT_GAME_TEXT),
							glGetBitmapStringSize(LN_TUTORIAL_LINE02, CFONT_GAME_TEXT),
							glGetBitmapStringSize(LN_TUTORIAL_LINE03, CFONT_GAME_TEXT)
						};
		
		glPrint(0.0f - toUVSize(size_tuto_text[0].cx / 2, SCREEN_WIDTH),  0.05f, LN_TUTORIAL_LINE01);
		glPrint(0.0f - toUVSize(size_tuto_text[1].cx / 2, SCREEN_WIDTH), -0.05f, LN_TUTORIAL_LINE02);
		glPrint(0.0f - toUVSize(size_tuto_text[2].cx / 2, SCREEN_WIDTH), -0.15f, LN_TUTORIAL_LINE03);
		
		
		SIZE size_ok	= glGetBitmapStringSize(LN_CHC_OK, CFONT_GAME_CHC);
		
		glSetFont(CFONT_GAME_CHC);
		glColor4f(1, 1, 1, 1);
		glPrint(0.0f - (toUVSize(size_ok.cx, SCREEN_WIDTH) / 2), -0.5f, LN_CHC_OK);
		
		GAME_Q_YES = new TexCoordsUV(
							0.0f - (toUVSize(size_ok.cx, SCREEN_WIDTH) / 2),
							0.0f + (toUVSize(size_ok.cx, SCREEN_WIDTH) / 2),
							-0.5f,
							-0.5f + toUVSize(size_ok.cy, SCREEN_HEIGHT)
						);
/*		glSetFont(CFONT_GAME_TEXT);
		glColor4f(1, 1, 1, 1.0f - ((float) _TFR / WT_FRL_GLOB_FADE_SLOW));
		glPrint(0.0f - (toUVSize(glGetBitmapStringSize(LN_TUTORIAL_TXT, CFONT_GAME_TEXT).cx, SCREEN_WIDTH) / 2), 0.1f, LN_TUTORIAL_TXT);
		
		SIZE size_no	= glGetBitmapStringSize(LN_CHC_NO, CFONT_GAME_CHC),
		     size_yes	= glGetBitmapStringSize(LN_CHC_YES, CFONT_GAME_CHC);
		
		glSetFont(CFONT_GAME_CHC);
		glColor4f(1, 1, 1, 1.0f - (GAME_VAR_TUTO ? ((float) _TFR / WT_FRL_GLOB_FADE_SLOW) : 0));
		glPrint(-0.15f - (toUVSize(size_no.cx, SCREEN_WIDTH) / 2), -0.25f, LN_CHC_NO);
		glColor4f(1, 1, 1, 1.0f - (GAME_VAR_TUTO ? 0 : ((float) _TFR / WT_FRL_GLOB_FADE_SLOW)));
		glPrint( 0.15f - (toUVSize(size_yes.cx, SCREEN_WIDTH) / 2), -0.25f, LN_CHC_YES);*/
		
		if(_TFR >= WT_FRL_GLOB_FADE_SLOW)
			WT_CFR_MODE = WT_CFRM_PLAY_tutoFadePause;
	} else if(WT_CFR_MODE == WT_CFRM_PLAY_tutoFadePause) {
		SIZE size_ok	= glGetBitmapStringSize(LN_CHC_OK, CFONT_GAME_CHC);
		
		glSetFont(CFONT_GAME_CHC);
		glColor4f(1, 1, 1, 1);
		glPrint(0.0f - (toUVSize(size_ok.cx, SCREEN_WIDTH) / 2), -0.5f, LN_CHC_OK);
		
		GAME_Q_YES = new TexCoordsUV(
							0.0f - (toUVSize(size_ok.cx, SCREEN_WIDTH) / 2),
							0.0f + (toUVSize(size_ok.cx, SCREEN_WIDTH) / 2),
							-0.5f,
							-0.5f + toUVSize(size_ok.cy, SCREEN_HEIGHT)
						);
/*		glSetFont(CFONT_GAME_CHC);
		glColor4f(1, 1, 1, 1);
		glPrint((GAME_VAR_TUTO ? 0.15f : -0.15f) - (toUVSize(glGetBitmapStringSize(GAME_VAR_TUTO ? LN_CHC_YES : LN_CHC_NO, CFONT_GAME_CHC).cx, SCREEN_WIDTH) / 2), -0.25f, GAME_VAR_TUTO ? LN_CHC_YES : LN_CHC_NO);
		*/
		if(_TFR >= WT_FRL_PLAY_tutoFadePause)
			WT_CFR_MODE = WT_CFRM_PLAY_gameFadeIn;
	} else if(WT_CFR_MODE == WT_CFRM_PLAY_gameFadeIn) {
		SIZE size_ok	= glGetBitmapStringSize(LN_CHC_OK, CFONT_GAME_CHC);
		
		glSetFont(CFONT_GAME_CHC);
		glColor4f(1, 1, 1, 1.0f - ((float) _TFR / WT_FRL_GLOB_FADE));
		glPrint(0.0f - (toUVSize(size_ok.cx, SCREEN_WIDTH) / 2), -0.5f, LN_CHC_OK);
		
		GAME_Q_YES = new TexCoordsUV(
							0.0f - (toUVSize(size_ok.cx, SCREEN_WIDTH) / 2),
							0.0f + (toUVSize(size_ok.cx, SCREEN_WIDTH) / 2),
							-0.5f,
							-0.5f + toUVSize(size_ok.cy, SCREEN_HEIGHT)
						);
/*		glSetFont(CFONT_GAME_CHC);
		glColor4f(1, 1, 1, 1.0f - ((float) _TFR / WT_FRL_GLOB_FADE));
		glPrint((GAME_VAR_TUTO ? 0.15f : -0.15f) - (toUVSize(glGetBitmapStringSize(GAME_VAR_TUTO ? LN_CHC_YES : LN_CHC_NO, CFONT_GAME_CHC).cx, SCREEN_WIDTH) / 2), -0.25f, GAME_VAR_TUTO ? LN_CHC_YES : LN_CHC_NO);
		*/
		if(_TFR >= WT_FRL_GLOB_FADE)
			WT_CFR_MODE = WT_CFRM_PLAY_game;
	} else if(WT_CFR_MODE >= WT_CFRM_PLAY_game) {
		if(GAME_TURN == TURN_NOTURN)
			GAME_init();
				
		glBegin(GL_QUADS);
			glColor4f(1, 1, 1, 1);
			glVertex2f(-1,  1);
			glVertex2f( 1,  1);
			glVertex2f( 1, -1);
			glVertex2f(-1, -1);
		glEnd();
		
		GAME_GAME_DRAWBACKGROUND(); /********************************************/
		
		if(!GAME_FRAMEDRAWN) GAME_drawFrame();		
		
		float UV_sizeW_text[2] = {
									toUVSize(glGetBitmapStringSize(LN_PLAY_PLAYER, CFONT_GAME_TEXT).cx, SCREEN_WIDTH),
									toUVSize(glGetBitmapStringSize(LN_PLAY_AI, CFONT_GAME_TEXT).cx, SCREEN_WIDTH)
								};
								
		int size_sel = SCREEN_WIDTH / 10;
		float UV_sizeW_sel = toUVSize(size_sel, SCREEN_WIDTH),
		      UV_sizeH_sel = toUVSize(size_sel, SCREEN_HEIGHT);
		int posL = (SCREEN_WIDTH / 2) - (0.85 * SCREEN_WIDTH / 2),
		    posR = (SCREEN_WIDTH / 2) - (0.85 * SCREEN_WIDTH / 2) + size_sel;
		
		
		glSetFont(CFONT_GAME_TEXT);
		glColor4f(1, 1, 1, 1);
		glPrint(-0.85f + (UV_sizeW_sel / 2) - (UV_sizeW_text[0] / 2), 0.65f, LN_PLAY_PLAYER);
		glPrint( 0.85f - (UV_sizeW_sel / 2) - (UV_sizeW_text[1] / 2), 0.65f, LN_PLAY_AI);
		
		
		glSetFont(CFONT_MMENU_TITLE);
		std::string str = LN_PLAY_GAME + std::to_string(GAME_CURSET) + " / 3";
		glPrint(0.0f - (toUVSize(glGetBitmapStringSize(str.c_str(), CFONT_MMENU_TITLE).cx, SCREEN_WIDTH) / 2), 0.825f, str);
		
		glEnable(GL_TEXTURE_2D);
		
		glColor4f(1, 1, 1, 1);
		
		glBindTexture(GL_TEXTURE_2D, TEX_BLOCK_NUM);
		
		glBegin(GL_QUADS);
			glTexCoord2f(GAME_CURSCORE + 1 <= 12 ? (TEX_BLOCK_NUM_isClicked[0] ? 0.25f : 0.00f) : 0.50f, 0.000f);
				glVertex2f(               -0.85f, UV_sizeH_sel + (UV_sizeH_sel / 2) + (2 * UV_sizeH_sel / 16));
			glTexCoord2f(GAME_CURSCORE + 1 <= 12 ? (TEX_BLOCK_NUM_isClicked[0] ? 0.50f : 0.25f) : 0.75f, 0.000f);
				glVertex2f(-0.85f + UV_sizeW_sel, UV_sizeH_sel + (UV_sizeH_sel / 2) + (2 * UV_sizeH_sel / 16));
			glTexCoord2f(GAME_CURSCORE + 1 <= 12 ? (TEX_BLOCK_NUM_isClicked[0] ? 0.50f : 0.25f) : 0.75f, 0.125f);
				glVertex2f(-0.85f + UV_sizeW_sel,                  UV_sizeH_sel / 2 + (2 * UV_sizeH_sel / 16));
			glTexCoord2f(GAME_CURSCORE + 1 <= 12 ? (TEX_BLOCK_NUM_isClicked[0] ? 0.25f : 0.00f) : 0.50f, 0.125f);
				glVertex2f(               -0.85f,                  UV_sizeH_sel / 2 + (2 * UV_sizeH_sel / 16));
		glEnd();
		
		GAME_SEL01 = new TexCoordsUV(
							-0.85f,
							-0.85f + UV_sizeW_sel,
							(UV_sizeH_sel / 2) + (2 * UV_sizeH_sel / 16),
							UV_sizeH_sel + (UV_sizeH_sel / 2) + (2 * UV_sizeH_sel / 16)
						);
		
		
		glBegin(GL_QUADS);
			glTexCoord2f(GAME_CURSCORE + 2 <= 12 ? (TEX_BLOCK_NUM_isClicked[1] ? 0.25f : 0.00f) : 0.50f, 0.000f);
				glVertex2f(               -0.85f,        UV_sizeH_sel / 2);
			glTexCoord2f(GAME_CURSCORE + 2 <= 12 ? (TEX_BLOCK_NUM_isClicked[1] ? 0.50f : 0.25f) : 0.75f, 0.000f);
				glVertex2f(-0.85f + UV_sizeW_sel,        UV_sizeH_sel / 2);
			glTexCoord2f(GAME_CURSCORE + 2 <= 12 ? (TEX_BLOCK_NUM_isClicked[1] ? 0.50f : 0.25f) : 0.75f, 0.125f);
				glVertex2f(-0.85f + UV_sizeW_sel, -1 * (UV_sizeH_sel / 2));
			glTexCoord2f(GAME_CURSCORE + 2 <= 12 ? (TEX_BLOCK_NUM_isClicked[1] ? 0.25f : 0.00f) : 0.50f, 0.125f);
				glVertex2f(               -0.85f, -1 * (UV_sizeH_sel / 2));
		glEnd();
		
		GAME_SEL02 = new TexCoordsUV(
							-0.85f,
							-0.85f + UV_sizeW_sel,
							-1 * (UV_sizeH_sel / 2),
							UV_sizeH_sel / 2
						);
		
		
		glBegin(GL_QUADS);
			glTexCoord2f(GAME_CURSCORE + 3 <= 12 ? (TEX_BLOCK_NUM_isClicked[2] ? 0.25f : 0.00f) : 0.50f, 0.000f);
				glVertex2f(               -0.85f, (-1 * UV_sizeH_sel) + (UV_sizeH_sel / 2) - (2 * UV_sizeH_sel / 16));
			glTexCoord2f(GAME_CURSCORE + 3 <= 12 ? (TEX_BLOCK_NUM_isClicked[2] ? 0.50f : 0.25f) : 0.75f, 0.000f);
				glVertex2f(-0.85f + UV_sizeW_sel, (-1 * UV_sizeH_sel) + (UV_sizeH_sel / 2) - (2 * UV_sizeH_sel / 16));
			glTexCoord2f(GAME_CURSCORE + 3 <= 12 ? (TEX_BLOCK_NUM_isClicked[2] ? 0.50f : 0.25f) : 0.75f, 0.125f);
				glVertex2f(-0.85f + UV_sizeW_sel, (-1 * UV_sizeH_sel) - (UV_sizeH_sel / 2) - (2 * UV_sizeH_sel / 16));
			glTexCoord2f(GAME_CURSCORE + 3 <= 12 ? (TEX_BLOCK_NUM_isClicked[2] ? 0.25f : 0.00f) : 0.50f, 0.125f);
				glVertex2f(               -0.85f, (-1 * UV_sizeH_sel) - (UV_sizeH_sel / 2) - (2 * UV_sizeH_sel / 16));
		glEnd();
		
		GAME_SEL03 = new TexCoordsUV(
							-0.85f,
							-0.85f + UV_sizeW_sel,
							(-1 * UV_sizeH_sel) - (UV_sizeH_sel / 2) - (2 * UV_sizeH_sel / 16),
							(-1 * UV_sizeH_sel) + (UV_sizeH_sel / 2) - (2 * UV_sizeH_sel / 16)
						);
		
		//
		//
		//
		
//		glBindTexture(GL_TEXTURE_2D, TEX_BLOCK_NUM);
//		glColor4f(0, 1, 1, 1);
		glBegin(GL_QUADS);
			glTexCoord2f(GAME_CURSCORE + 1 <= 12 ? (TEX_BLOCK_NUM_AI_isClicked[0] ? 0.25f : 0.00f) : 0.50f, 0.125f);
				glVertex2f(0.85f - UV_sizeW_sel, UV_sizeH_sel + (UV_sizeH_sel / 2) + (2 * UV_sizeH_sel / 16));
			glTexCoord2f(GAME_CURSCORE + 1 <= 12 ? (TEX_BLOCK_NUM_AI_isClicked[0] ? 0.50f : 0.25f) : 0.75f, 0.125f);
				glVertex2f(               0.85f, UV_sizeH_sel + (UV_sizeH_sel / 2) + (2 * UV_sizeH_sel / 16));
			glTexCoord2f(GAME_CURSCORE + 1 <= 12 ? (TEX_BLOCK_NUM_AI_isClicked[0] ? 0.50f : 0.25f) : 0.75f, 0.250f);
				glVertex2f(               0.85f,                  UV_sizeH_sel / 2 + (2 * UV_sizeH_sel / 16));
			glTexCoord2f(GAME_CURSCORE + 1 <= 12 ? (TEX_BLOCK_NUM_AI_isClicked[0] ? 0.25f : 0.00f) : 0.50f, 0.250f);
				glVertex2f(0.85f - UV_sizeW_sel,                  UV_sizeH_sel / 2 + (2 * UV_sizeH_sel / 16));
		glEnd();
		
		glBegin(GL_QUADS);
			glTexCoord2f(GAME_CURSCORE + 2 <= 12 ? (TEX_BLOCK_NUM_AI_isClicked[1] ? 0.25f : 0.00f) : 0.50f, 0.125f);
				glVertex2f(0.85f - UV_sizeW_sel,        UV_sizeH_sel / 2);
			glTexCoord2f(GAME_CURSCORE + 2 <= 12 ? (TEX_BLOCK_NUM_AI_isClicked[1] ? 0.50f : 0.25f) : 0.75f, 0.125f);
				glVertex2f(               0.85f,        UV_sizeH_sel / 2);
			glTexCoord2f(GAME_CURSCORE + 2 <= 12 ? (TEX_BLOCK_NUM_AI_isClicked[1] ? 0.50f : 0.25f) : 0.75f, 0.250f);
				glVertex2f(               0.85f, -1 * (UV_sizeH_sel / 2));
			glTexCoord2f(GAME_CURSCORE + 2 <= 12 ? (TEX_BLOCK_NUM_AI_isClicked[1] ? 0.25f : 0.00f) : 0.50f, 0.250f);
				glVertex2f(0.85f - UV_sizeW_sel, -1 * (UV_sizeH_sel / 2));
		glEnd();
		
		glBegin(GL_QUADS);
			glTexCoord2f(GAME_CURSCORE + 3 <= 12 ? (TEX_BLOCK_NUM_AI_isClicked[2] ? 0.25f : 0.00f) : 0.50f, 0.125f);
				glVertex2f(0.85f - UV_sizeW_sel, (-1 * UV_sizeH_sel) + (UV_sizeH_sel / 2) - (2 * UV_sizeH_sel / 16));
			glTexCoord2f(GAME_CURSCORE + 3 <= 12 ? (TEX_BLOCK_NUM_AI_isClicked[2] ? 0.50f : 0.25f) : 0.75f, 0.125f);
				glVertex2f(               0.85f, (-1 * UV_sizeH_sel) + (UV_sizeH_sel / 2) - (2 * UV_sizeH_sel / 16));
			glTexCoord2f(GAME_CURSCORE + 3 <= 12 ? (TEX_BLOCK_NUM_AI_isClicked[2] ? 0.50f : 0.25f) : 0.75f, 0.250f);
				glVertex2f(               0.85f, (-1 * UV_sizeH_sel) - (UV_sizeH_sel / 2) - (2 * UV_sizeH_sel / 16));
			glTexCoord2f(GAME_CURSCORE + 3 <= 12 ? (TEX_BLOCK_NUM_AI_isClicked[2] ? 0.25f : 0.00f) : 0.50f, 0.250f);
				glVertex2f(0.85f - UV_sizeW_sel, (-1 * UV_sizeH_sel) - (UV_sizeH_sel / 2) - (2 * UV_sizeH_sel / 16));
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
		
		
		glSetFont(CFONT_GAME_NUMCHC);
		if(GAME_TURN == TURN_PLAYER) {
			if(TEX_BLOCK_NUM_isClicked[0]) {
				if(GAME_CURSCORE + 1 <= 12) glColor4f((float) 255/255, (float) 242/255, 0, 1);
				else glColor4f((float) 177/255, 0, 0, 1);
			} else glColor4f(1, 1, 1, 1);
		} else glColor4f((float) 175/255, 0, 0, 1);
		glPrint(-0.85f + (UV_sizeW_sel / 2) - toUVSize(glGetBitmapStringSize("1", CFONT_GAME_NUMCHC).cx / 2, SCREEN_WIDTH), (UV_sizeH_sel / 2) + (2 * UV_sizeH_sel / 16) + (UV_sizeH_sel / 2) - toUVSize(glGetBitmapStringSize("1", CFONT_GAME_NUMCHC).cy / 2, SCREEN_WIDTH), "1");
		
		if(GAME_TURN == TURN_PLAYER) {
			if(TEX_BLOCK_NUM_isClicked[1]) {
				if(GAME_CURSCORE + 2 <= 12) glColor4f((float) 255/255, (float) 242/255, 0, 1);
				else glColor4f((float) 177/255, 0, 0, 1);
			} else glColor4f(1, 1, 1, 1);
		} else glColor4f((float) 175/255, 0, 0, 1);
		glPrint(-0.85f + (UV_sizeW_sel / 2) - toUVSize(glGetBitmapStringSize("2", CFONT_GAME_NUMCHC).cx / 2, SCREEN_WIDTH), -1 * toUVSize(glGetBitmapStringSize("2", CFONT_GAME_NUMCHC).cy / 2, SCREEN_WIDTH), "2");
		
		if(GAME_TURN == TURN_PLAYER) {
			if(TEX_BLOCK_NUM_isClicked[2]) {
				if(GAME_CURSCORE + 3 <= 12) glColor4f((float) 255/255, (float) 242/255, 0, 1);
				else glColor4f((float) 177/255, 0, 0, 1);
			} else glColor4f(1, 1, 1, 1);
		} else glColor4f((float) 175/255, 0, 0, 1);
		glPrint(-0.85f + (UV_sizeW_sel / 2) - toUVSize(glGetBitmapStringSize("3", CFONT_GAME_NUMCHC).cx / 2, SCREEN_WIDTH), -1 * (UV_sizeH_sel / 2) - (2 * UV_sizeH_sel / 16) - (UV_sizeH_sel / 2) - toUVSize(glGetBitmapStringSize("3", CFONT_GAME_NUMCHC).cy / 2, SCREEN_WIDTH), "3");
		
		
		if(TEX_BLOCK_NUM_AI_isClicked[0]) glColor4f((float) 255/255, (float) 242/255, 0, 1);
		else glColor4f(1, 1, 1, 1);
		glPrint(0.85f - (UV_sizeW_sel / 2) - toUVSize(glGetBitmapStringSize("1", CFONT_GAME_NUMCHC).cx / 2, SCREEN_WIDTH), (UV_sizeH_sel / 2) + (2 * UV_sizeH_sel / 16) + (UV_sizeH_sel / 2) - toUVSize(glGetBitmapStringSize("1", CFONT_GAME_NUMCHC).cy / 2, SCREEN_WIDTH), "1");
		
		if(TEX_BLOCK_NUM_AI_isClicked[1]) glColor4f((float) 255/255, (float) 242/255, 0, 1);
		else glColor4f(1, 1, 1, 1);
		glPrint(0.85f - (UV_sizeW_sel / 2) - toUVSize(glGetBitmapStringSize("2", CFONT_GAME_NUMCHC).cx / 2, SCREEN_WIDTH), -1 * toUVSize(glGetBitmapStringSize("2", CFONT_GAME_NUMCHC).cy / 2, SCREEN_WIDTH), "2");
		
		if(TEX_BLOCK_NUM_AI_isClicked[2]) glColor4f((float) 255/255, (float) 242/255, 0, 1);
		else glColor4f(1, 1, 1, 1);
		glPrint(0.85f - (UV_sizeW_sel / 2) - toUVSize(glGetBitmapStringSize("3", CFONT_GAME_NUMCHC).cx / 2, SCREEN_WIDTH), -1 * (UV_sizeH_sel / 2) - (2 * UV_sizeH_sel / 16) - (UV_sizeH_sel / 2) - toUVSize(glGetBitmapStringSize("3", CFONT_GAME_NUMCHC).cy / 2, SCREEN_WIDTH), "3");
	} if(WT_CFR_MODE == WT_CFRM_PLAY_popupPause) {
		if(_TFR >= WT_FRL_PLAY_popupPause)
			WT_CFR_MODE = WT_CFRM_PLAY_popupText;
	} else if(WT_CFR_MODE == WT_CFRM_PLAY_popupText) {
		glColor4f(0, 0, 0, 0.75f);
		glBegin(GL_QUADS);
			glVertex2f(-1,  1);
			glVertex2f( 1,  1);
			glVertex2f( 1, -1);
			glVertex2f(-1, -1);
		glEnd();
		
		glColor4f(1, 1, 1, 1);
		glSetFont(CFONT_MMENU_TITLE);
		glPrint(0.0f - toUVSize(glGetBitmapStringSize((LN_PLAY_GAME + std::to_string(GAME_CURSET) + ": " + (GAME_MATCH[GAME_CURSET - 1] == 0 ? LN_PLAY_PLAYER : LN_PLAY_AI) + LN_PLAY_WIN).c_str(), CFONT_MMENU_TITLE).cx / 2, SCREEN_WIDTH), 0.1f, LN_PLAY_GAME + std::to_string(GAME_CURSET) + ": " + (GAME_MATCH[GAME_CURSET - 1] == 0? LN_PLAY_PLAYER : LN_PLAY_AI) + LN_PLAY_WIN);
		
		if(_TFR >= WT_FRL_PLAY_popupText)
			WT_CFR_MODE = WT_CFRM_PLAY_popupFadeOut;
	} else if(WT_CFR_MODE == WT_CFRM_PLAY_popupFadeOut) {
		glColor4f(0, 0, 0, 0.75f - (0.75f * _TFR / WT_FRL_GLOB_FADE));
		glBegin(GL_QUADS);
			glVertex2f(-1,  1);
			glVertex2f( 1,  1);
			glVertex2f( 1, -1);
			glVertex2f(-1, -1);
		glEnd();
		
		if(_TFR >= WT_FRL_GLOB_FADE) {
			WT_CFR_MODE = WT_CFRM_PLAY_game;
			if(GAME_CURSET < 3) {
				GAME_CURSET++;
				GAME_CURSCORE = 0;
				GAME_TURN = rand() % 2;
			} else {
				WINDOWTYPE = WT_MAINMENU;
				WT_CFR_MODE = WT_CFRM_MM_MM;
			}
		}
	}
}

void drawFrame(bool newFrame) {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	if(saveWT != WINDOWTYPE || saveCF != WT_CFR_MODE) {
		if(saveWT != WINDOWTYPE) WT_CFR_MODE = 0;
		saveWT = WINDOWTYPE;
		saveCF = WT_CFR_MODE;
		_TFR = _CFR = 1;
	}
	

	if(WINDOWTYPE == WT_PRELOADING)		drawFrame_PRELOADING();
	else if(WINDOWTYPE == WT_LOADING)	drawFrame_LOADING();
	else if(WINDOWTYPE == WT_MAINMENU)	drawFrame_MAINMENU();
	else if(WINDOWTYPE == WT_PLAY)		drawFrame_PLAY();
	
	
//	glColor4f(1, 0, 0, 1);
//	glSetFont(CFONT_DEBUG);
//	glPrint(1.0f - toUVSize(glGetBitmapStringSize((std::to_string(WINDOWTYPE) + " " + std::to_string(WT_CFR_MODE) + "   " + std::to_string(_CFR) + "  " + std::to_string(_TFR)).c_str(), CFONT_DEBUG).cx, SCREEN_WIDTH), -1.0f,
//	                                               std::to_string(WINDOWTYPE) + " " + std::to_string(WT_CFR_MODE) + "   " + std::to_string(_CFR) + "  " + std::to_string(_TFR));
	
	if(newFrame) Sleep(_FRL / 2);
	SwapBuffers(GetDC(GetActiveWindow()));
	
	if(newFrame) {
		if(_CFR == _FPS) _CFR = 1;
		else _CFR++;
		_TFR++;
	}
}



float UV_sizeW_block = toUVSize(SCREEN_WIDTH / 28, SCREEN_WIDTH),
      UV_sizeH_block = toUVSize(SCREEN_WIDTH / 28, SCREEN_HEIGHT);

void GAME_drawFrame() {
	if(GAME_TURN == TURN_AI && _TFR >= WT_FRL_PLAY_cubeAnimation + WT_FRL_PLAY_AIPause)
		AITurn();
	
	for(int i = 0; i < 3; i++) {
		for(int ii = 0; ii < 12; ii++) {
			if(GAME_STOREDBLOCKS[i][ii]->animationState == ANIM_FINISHED && GAME_STOREDBLOCKS[i][ii]->complete) {
				glEnable(GL_TEXTURE_2D);
				
				glBindTexture(GL_TEXTURE_2D, GAME_STOREDBLOCKS[i][ii]->type == 0 ? TEX_BLOCK_GOLD_TEXTURED : TEX_BLOCK_CHROME_TEXTURED);
				
				glColor4f(1, 1, 1, 1);
				
				glBegin(GL_QUADS);
					glTexCoord2f(         GAME_STOREDBLOCKS[i][ii]->texX * 0.125f,          GAME_STOREDBLOCKS[i][ii]->texY * 0.125f);	glVertex2f(GAME_STOREDBLOCKS[i][ii]->ST_posX - (UV_sizeW_block / 2), GAME_STOREDBLOCKS[i][ii]->ST_posY + (UV_sizeH_block / 2));
					glTexCoord2f(0.125f + GAME_STOREDBLOCKS[i][ii]->texX * 0.125f,          GAME_STOREDBLOCKS[i][ii]->texY * 0.125f);	glVertex2f(GAME_STOREDBLOCKS[i][ii]->ST_posX + (UV_sizeW_block / 2), GAME_STOREDBLOCKS[i][ii]->ST_posY + (UV_sizeH_block / 2));
					glTexCoord2f(0.125f + GAME_STOREDBLOCKS[i][ii]->texX * 0.125f, 0.125f + GAME_STOREDBLOCKS[i][ii]->texY * 0.125f);	glVertex2f(GAME_STOREDBLOCKS[i][ii]->ST_posX + (UV_sizeW_block / 2), GAME_STOREDBLOCKS[i][ii]->ST_posY - (UV_sizeH_block / 2));
					glTexCoord2f(         GAME_STOREDBLOCKS[i][ii]->texX * 0.125f, 0.125f + GAME_STOREDBLOCKS[i][ii]->texY * 0.125f);	glVertex2f(GAME_STOREDBLOCKS[i][ii]->ST_posX - (UV_sizeW_block / 2), GAME_STOREDBLOCKS[i][ii]->ST_posY - (UV_sizeH_block / 2));
				glEnd();
				
				glDisable(GL_TEXTURE_2D);
			}
		}
	} for(int i = 0; i < 3; i++) {
		if(GAME_ANIMBUFFER[i] != -1) {
			int set = GAME_ANIMBUFFER[i] / 12;
			int ind = GAME_ANIMBUFFER[i] % 12;
			if(GAME_STOREDBLOCKS[set][ind]->animationState == ANIM_STANDBY) {
				GAME_STOREDBLOCKS[set][ind]->animationState = ANIM_ONGOING;
				GAME_STOREDBLOCKS[set][ind]->ST_rposX = toSTCoord(rand() % SCREEN_WIDTH, AXIS_X);
				GAME_STOREDBLOCKS[set][ind]->ST_rposY = toSTCoord(rand() % SCREEN_HEIGHT, AXIS_Y);
				GAME_STOREDBLOCKS[set][ind]->ST_posX = 0.0f + (((set - 1) * 1.5) * UV_sizeW_block);
				GAME_STOREDBLOCKS[set][ind]->ST_posY = -1.0 + (UV_sizeH_block / 2) + (ind * UV_sizeH_block);
			} else if(GAME_STOREDBLOCKS[set][ind]->animationState == ANIM_ONGOING) {
				float difX = GAME_STOREDBLOCKS[set][ind]->ST_posX - GAME_STOREDBLOCKS[set][ind]->ST_rposX;
				float difY = GAME_STOREDBLOCKS[set][ind]->ST_posY - GAME_STOREDBLOCKS[set][ind]->ST_rposY;
		 		
				glEnable(GL_TEXTURE_2D);
				
				glBindTexture(GL_TEXTURE_2D, GAME_STOREDBLOCKS[set][ind]->type == 0 ? TEX_BLOCK_GOLD_TEXTURED : TEX_BLOCK_CHROME_TEXTURED);
				
				glColor4f(1, 1, 1, _TFR < WT_FRL_PLAY_cubeAnimation / 2 ? _TFR / (WT_FRL_PLAY_cubeAnimation / 2) : 1);
				
				glBegin(GL_QUADS);
					glTexCoord2f(         GAME_STOREDBLOCKS[set][ind]->texX * 0.125f,          GAME_STOREDBLOCKS[set][ind]->texY * 0.125f);	glVertex2f(GAME_STOREDBLOCKS[set][ind]->ST_rposX - (UV_sizeW_block / 2) + (difX * _TFR / WT_FRL_PLAY_cubeAnimation), GAME_STOREDBLOCKS[set][ind]->ST_rposY + (UV_sizeH_block / 2) + (difY * _TFR / WT_FRL_PLAY_cubeAnimation));
					glTexCoord2f(0.125f + GAME_STOREDBLOCKS[set][ind]->texX * 0.125f,          GAME_STOREDBLOCKS[set][ind]->texY * 0.125f);	glVertex2f(GAME_STOREDBLOCKS[set][ind]->ST_rposX + (UV_sizeW_block / 2) + (difX * _TFR / WT_FRL_PLAY_cubeAnimation), GAME_STOREDBLOCKS[set][ind]->ST_rposY + (UV_sizeH_block / 2) + (difY * _TFR / WT_FRL_PLAY_cubeAnimation));
					glTexCoord2f(0.125f + GAME_STOREDBLOCKS[set][ind]->texX * 0.125f, 0.125f + GAME_STOREDBLOCKS[set][ind]->texY * 0.125f);	glVertex2f(GAME_STOREDBLOCKS[set][ind]->ST_rposX + (UV_sizeW_block / 2) + (difX * _TFR / WT_FRL_PLAY_cubeAnimation), GAME_STOREDBLOCKS[set][ind]->ST_rposY - (UV_sizeH_block / 2) + (difY * _TFR / WT_FRL_PLAY_cubeAnimation));
					glTexCoord2f(         GAME_STOREDBLOCKS[set][ind]->texX * 0.125f, 0.125f + GAME_STOREDBLOCKS[set][ind]->texY * 0.125f);	glVertex2f(GAME_STOREDBLOCKS[set][ind]->ST_rposX - (UV_sizeW_block / 2) + (difX * _TFR / WT_FRL_PLAY_cubeAnimation), GAME_STOREDBLOCKS[set][ind]->ST_rposY - (UV_sizeH_block / 2) + (difY * _TFR / WT_FRL_PLAY_cubeAnimation));
				glEnd();
				
				glDisable(GL_TEXTURE_2D);
				
				if(_TFR >= WT_FRL_PLAY_AIClick) {
					TEX_BLOCK_NUM_AI_isClicked[TEX_BLOCK_NUM_P_] = false;
					TEX_BLOCK_NUM_P_ = -1;
				} if(_TFR >= WT_FRL_PLAY_cubeAnimation) {
					GAME_CHECKER();
					if(GAME_TURN == TURN_STANDBY_PL) GAME_TURN = TURN_PLAYER;
					else if(GAME_TURN == TURN_STANDBY_AI) GAME_TURN = TURN_AI;
					GAME_STOREDBLOCKS[set][ind]->animationState = ANIM_FINISHED;
				}
			}
		}
	}
}

//MessageBox(GetActiveWindow(), "ada", "dad", MB_OK);
