#include <cstdlib>


#include "../include/declarations.h"



int GAME_CURSCORE = 0,
    GAME_CURSET = 1;
Block *GAME_STOREDBLOCKS[3][12];
int GAME_ANIMBUFFER[3] = { -1, -1, -1 };
int GAME_MATCH[3];


void GAME_CHECKER() {
	if(GAME_CURSCORE == 12) {
//		GAME_MATCH[GAME_CURSET - 1] = GAME_TURN == TURN_STANDBY_AI ? 0 : 1;
		GAME_TURN = TURN_STANDBY;
		WT_CFR_MODE = WT_CFRM_PLAY_popupPause;
	}
}

void PlayerTurn(int input) {
	GAME_TURN = TURN_STANDBY_AI;
	_TFR = _CFR = 1;
	
	for(int i = 0; i < 3; i++)
		GAME_ANIMBUFFER[i] = -1;
	for(int i = 1; i <= input; i++) {
		GAME_STOREDBLOCKS[GAME_CURSET - 1][GAME_CURSCORE - 1 + i] = new Block(GAME_CURSET, GAME_CURSCORE + i, 0);
		GAME_ANIMBUFFER[i - 1] = ((GAME_CURSET - 1) * 12) + (GAME_CURSCORE + i - 1);
	}
	
	GAME_CURSCORE += input;
	if(GAME_CURSCORE == 12) GAME_MATCH[GAME_CURSET - 1] = 0;
}

int AI_AI() {
	if(GAME_CURSCORE >= 9)
		return 12 - GAME_CURSCORE;
	else return 1 + rand() % 3;
}

void AITurn() {
	GAME_TURN = TURN_STANDBY_PL;
	_TFR = _CFR = 1;
	
	int input = AI_AI();
	
	TEX_BLOCK_NUM_AI_isClicked[input - 1] = true;
	TEX_BLOCK_NUM_P_ = input - 1;
	
	for(int i = 0; i < 3; i++)
		GAME_ANIMBUFFER[i] = -1;
	for(int i = 1; i <= input; i++) {
		GAME_STOREDBLOCKS[GAME_CURSET - 1][GAME_CURSCORE - 1 + i] = new Block(GAME_CURSET, GAME_CURSCORE + i, 1);
		GAME_ANIMBUFFER[i - 1] = ((GAME_CURSET - 1) * 12) + (GAME_CURSCORE + i - 1);
	}
	
	GAME_CURSCORE += input;
	if(GAME_CURSCORE == 12) GAME_MATCH[GAME_CURSET - 1] = 1;
}

void GAME_init() {
	GAME_TURN = rand() % 2;
	GAME_CURSCORE = 0;
	GAME_CURSET = 1;
	
	for(int i = 0; i < 3; i++) {
		GAME_ANIMBUFFER[i] = -1;
		for(int ii = 0; ii < 12; ii++)
			GAME_STOREDBLOCKS[i][ii] = new Block();
	}
}
