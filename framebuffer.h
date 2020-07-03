#pragma once

#include "box.h"

#define BLOCK_BORDER 1
#define DITHER_BORDER 2
#define LINE_BORDER 3
#define DOUBLE_BORDER 4
#define CHARACTER_BORDER 5

struct screen;

struct framebuffer : public box {
	framebuffer(screen* screen);
	int getSizeX();
	int getSizeY();
	int getOffX();
	int getOffY();
	void setSizeX(int sizex);
	void setSizeY(int sizey);
	void setSize(int sizex, int sizey);
	void setBox(box box);
	void setOffset(int offsetx, int offsety);
	void changeOffsetX(int offsetX);
	void changeOffsetY(int offsetY);
	void setMaxX(int sizex);
	void setMaxY(int sizey);
	void setMax(int sizex, int sizey);
	void setBackground(char character, char color);
	void setBorder(char color);
	void setBorder(char color, bool state);
	void drawBorder(char character, char color);
	void drawBorder(wchar_t character, char color);
	void drawFancyBorder(int type, char color);
	void doUseScreen(bool uS);
	void doUseNull(bool uN);
	void doUseBackground(bool uB);
	void doUseBorder(bool uB);
	void doUseClear(bool c);
	void frame();
	void clear();
	void clear(char character, char color);
	void write(int x, int y, char character, char color, bool borderOverride = false);
	void write(int x, int y, wchar_t character, char color, bool borderOverride = false);
	
	wchar_t* fb;
	char* cb;
	
	private:
	void allocate(int sizex, int sizey);
	
	bool useScreen;
	screen* scr; //For getting max size
	char backgroundcharacter, backgroundcolor, borderColor;
	bool useBackground;
	bool useBorder;
	bool useNull;
	bool doClear;
};