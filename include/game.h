#pragma once



#include "../include/struct.h"



extern int GAME_TURN;

#define TURN_NOTURN -1
#define TURN_PLAYER 0
#define TURN_AI 1
#define TURN_STANDBY 2
#define TURN_STANDBY_PL 3
#define TURN_STANDBY_AI 4

extern bool GAME_FRAMEDRAWN;

#define ANIM_STANDBY 0
#define ANIM_ONGOING 1
#define ANIM_FINISHED 2

extern Block *GAME_STOREDBLOCKS[3][12];
extern int GAME_ANIMBUFFER[3];
extern int GAME_CURSCORE,
           GAME_CURSET;
extern int GAME_MATCH[3];

void GAME_init();
void GAME_drawFrame();
void PlayerTurn(int input);
void AITurn();
void GAME_CHECKER();
