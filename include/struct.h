#pragma once



class TexCoordsUV {
	public:
		float posL,
		      posR,
		      posB,
		      posT;
		TexCoordsUV() {}
		TexCoordsUV(float l, float r, float b, float t) {
			posL = l;
			posR = r;
			posB = b;
			posT = t;
		}
};

class Block {
	public:
		int row,
		    col,
		    type, //player(0) or ai(1)
		    texX,
		    texY,
			animationState;
		float ST_rposX,
		      ST_rposY,
		      ST_posX,
		      ST_posY;
		bool complete;
		Block() {
			complete = false;
		}
		Block(int r, int c, int t) {
			row = r;
			col = c;
			type = t;
			texX = rand() % 8;//x;
			texY = rand() % 5;//y;
			animationState = 0; //ANIM_STANDBY
			complete = true;
		}
		Block(bool isBg) {
			if(rand() % 100 + 1 <= 75) {
				texX = 7;//rand() % 8;
				texY = 1;//rand() % 4;
			} else {
				texX = 7;
				texY = 2;
			}
		}
};
