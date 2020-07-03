#include "framebuffer.h"

#include "screen.h"

framebuffer::framebuffer(screen* screen)
	:box(0,0,0,0)
{
	scr = screen;
	backgroundcharacter = ' ';
	backgroundcolor = BBLACK | FWHITE;
	borderColor = FRED | BBLACK;
	useScreen = false;
	useBackground = false;
	useBorder = false;
	useNull = false;
	doClear = true;
}

void framebuffer::setBox(box box) {
	setSize(box.sizex, box.sizey);
	setOffset(box.offsetx, box.offsety);
	clear();
}

void framebuffer::setBorder(char color) {
	borderColor = color;
}

void framebuffer::setBorder(char color, bool state) {
	doUseBorder(state);
	setBorder(color);
}

void framebuffer::doUseBorder(bool uB) {
	useBorder = uB;
}

int framebuffer::getSizeX() {
	if (useBorder)
		return sizex - 2;
	return sizex;
}

int framebuffer::getSizeY() {
	if (useBorder)
		return sizey - 2;
	return sizey;
}

int framebuffer::getOffX() {
	if (useBorder)
		return offsetx + 1;
	return offsetx;
}

int framebuffer::getOffY() {
	if (useBorder)
		return offsety + 1;
	return offsety;
}

void framebuffer::drawFancyBorder(int type, char color) {
	int sizex = this->sizex - 1;
	int sizey = this->sizey - 1;
	switch (type) {
		case CHARACTER_BORDER: {
				for (int x = 0; x < sizex; x++) {
					write(x, 0, '-', color);
					write(x, sizey - 1, '-', color);
				}
				for (int y = 0; y < sizey; y++) {
					write(0, y, '|', color);
					write(sizex - 1, y, '|', color);
				}			
				write(0,0, '+', color);
				write(0,sizey - 1, '+', color);
				write(sizex - 1, 0, '+', color);
				write(sizex - 1, sizey - 1, '+', color);
			break;
		}
		case BLOCK_BORDER: {
				drawBorder(L'█', color);
			break;
		}
		case DITHER_BORDER: {
				drawBorder(L'░', color);
				write(0,0, L'▒', color);
				write(0,sizey, L'▒', color);
				write(sizex,0, L'▒', color);
				write(sizex, sizey, L'▒', color);				
			break;
		}		
		case LINE_BORDER: {
				for (int x = 0; x < sizex + 1; x++) {
					write(x, 0, L'─', color);
					write(x, sizey, L'─', color);
				}
				for (int y = 0; y < sizey + 1; y++) {
					write(0, y, L'│', color);
					write(sizex, y, L'│', color);
				}			
				write(0,0, L'┌', color);
				write(0,sizey, L'└', color);
				write(sizex,0, L'┐', color);
				write(sizex, sizey, L'┘', color);			
			break;
		}
		case DOUBLE_BORDER: {
				for (int x = 0; x < sizex + 1; x++) {
					write(x, 0, L'═', color);
					write(x, sizey, L'═', color);
				}
				for (int y = 0; y < sizey + 1; y++) {
					write(0, y, L'║', color);
					write(sizex, y, L'║', color);
				}			
				write(0,0, L'╔', color);
				write(0,sizey, L'╚', color);
				write(sizex,0, L'╗', color);
				write(sizex, sizey, L'╝', color);
			break;
		}		
	}
}

void framebuffer::drawBorder(char character, char color) {
	drawBorder((wchar_t)character, color);
}

void framebuffer::drawBorder(wchar_t character, char color) {
	for (int x = 0; x < sizex; x++) {
		write(x, 0, character, color, true);
		write(x, sizey - 1, character, color, true);
	}
	for (int y = 0; y < sizey; y++) {
		write(0, y, character, color, true);
		write(sizex - 1, y, character, color, true);
	}	
}

void framebuffer::setBackground(char character, char color) {
	useBackground = true;
	backgroundcharacter = character;
	backgroundcolor = color;
}

void framebuffer::doUseScreen(bool uS) {
	if (uS) {
		
	} else {
		allocate(sizex, sizey);
	}
	useScreen = uS;
}

void framebuffer::doUseBackground(bool uB) {
	useBackground = uB;
}

void framebuffer::doUseClear(bool c) {
	doClear = c;
}

void framebuffer::setSizeX(int sizex) {
	if (this->sizex == sizex)
		return;
	this->sizex = sizex;
	if (!useScreen)		
		allocate(sizex, sizey);
}

void framebuffer::setSizeY(int sizey) {
	if (this->sizey == sizey)
		return;
	this->sizey = sizey;
	if (!useScreen)		
		allocate(sizex, sizey);
}

void framebuffer::setSize(int sizex, int sizey) {
	if (this->sizey == sizey && this->sizex == sizex)
		return;
	this->sizey = sizey;
	this->sizex = sizex;
	if (!useScreen)
		allocate(sizex, sizey);
}

void framebuffer::setOffset(int offsetx, int offsety) {
	this->offsetx = offsetx;
	this->offsety = offsety;
}

void framebuffer::allocate(int sizex, int sizey) {
	fb = new wchar_t[sizex * sizey];
	cb = new char[sizex * sizey];
}

void framebuffer::frame() {
	//Add border
	if (useBorder)
		drawFancyBorder(BLOCK_BORDER, borderColor);
	
	//We don't need to copy if we already use the screen's buffer
	if (!useScreen) {
		for (int x = 0; x < sizex; x++)
			for (int y = 0; y < sizey; y++)
				scr->write(offsetx + x, offsety + y, fb[get(x, y)], cb[get(x, y)], true);
	}
}

void framebuffer::clear() {
	if (!doClear)
		return;
	
	if (useBackground)
			clear(backgroundcharacter, backgroundcolor);
	else 
		if (useNull)
			clear('\0', backgroundcolor);
		else
			clear(' ', backgroundcolor);
}

void framebuffer::clear(char character, char color) {
	for (int x = 0; x < sizex; x++)
		for (int y = 0; y < sizey; y++)
			write(x, y, character, color, true);
}	

void framebuffer::write(int x, int y, wchar_t character, char color, bool borderOverride = false) {
	if (useScreen) {
		int nX = x, nY = y;
		if (useBorder && !borderOverride) {
			nX += getOffX();
			nY += getOffY();
		} else {
			nX += offsetx;
			nY += offsety;
		}
		scr->clip(nX, nY);
		scr->write(nX, nY, character, color, true);
	} else {
		if (useBorder && !borderOverride) {
			x++;
			y++;
		}
		fb[get(x, y)] = character;
		cb[get(x, y)] = color;		
	}
}

void framebuffer::write(int x, int y, char character, char color, bool borderOverride = false) {
	write(x, y, (wchar_t)character, color, borderOverride);
	/*
	if (useScreen) {
		int nX = x + offsetx, nY = y + offsety;
		scr->clip(nX, nY);
		scr->write(nX, nY, character, color);
	} else {
		fb[get(x, y)] = character;
		cb[get(x, y)] = color;
	}
	*/
}