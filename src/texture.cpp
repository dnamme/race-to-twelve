#include <stdio.h>
#include <vector>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include "../include/GL/glext.h"
#include "../include/lodepng.h"

#include "../include/declarations.h"



GLuint _loadTexture(const char* imagePath);



// from declarations.h //
GLuint TEX_BG_LOAD,
       TEX_ICON_NUM[8],
	   TEX_BLOCK_NUM,
	   TEX_BLOCK_METAL_SMOOTH,
	   TEX_BLOCK_GOLD_TEXTURED,
	   TEX_BLOCK_METAL_TEXTURED,
	   TEX_BLOCK_CHROME_TEXTURED;
// //

std::string TEX_ICON_NAMES[4] = { "6", "12", "16", "18" };


void loadTextures() {
	TEX_BG_LOAD = _loadTexture("../res/images/gold.png");
	
	for(int i = 0; i < sizeof(TEX_ICON_NAMES) / sizeof(*TEX_ICON_NAMES); i++) {
		TEX_ICON_NUM[i * 2]		= _loadTexture(("../res/images/icon/" + TEX_ICON_NAMES[i] + ".png").c_str());
		TEX_ICON_NUM[i * 2 + 1]	= _loadTexture(("../res/images/icon/" + TEX_ICON_NAMES[i] + "_border.png").c_str());
	}
	
	TEX_BLOCK_NUM				= _loadTexture("../res/images/block/buttons.png");
	TEX_BLOCK_METAL_SMOOTH		= _loadTexture("../res/images/block/metal_smooth.png");
	TEX_BLOCK_GOLD_TEXTURED		= _loadTexture("../res/images/block/gold_textured.png");
	TEX_BLOCK_METAL_TEXTURED	= _loadTexture("../res/images/block/metal_textured.png");
	TEX_BLOCK_CHROME_TEXTURED	= _loadTexture("../res/images/block/chrome_textured.png");
}

GLuint _loadTexture(const char* imagePath) {
	glEnable(GL_TEXTURE_2D);
	
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, imagePath);
	size_t u2 = 1; while(u2 < width) u2 *= 2;
	size_t v2 = 1; while(v2 < height) v2 *= 2;
	double u3 = (double) width / u2;
	double v3 = (double) height / v2;
	
	std::vector<unsigned char> image2(u2 * v2 * 4);
	for(size_t y = 0; y < height; y++)
		for(size_t x = 0; x < width; x++)
			for(size_t c = 0; c < 4; c++)
				image2[4 * u2 * y + 4 * x + c] = image[4 * width * y + 4 * x + c];
		
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image2[0]);
	
	glDisable(GL_TEXTURE_2D);
	
	return textureId;
}
