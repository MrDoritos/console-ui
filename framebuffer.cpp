#include "framebuffer.h"

#include "screen.h"

framebuffer::framebuffer(screen* screen)
	:box(0,0,0,0)
{
	scr = screen;
	backgroundcharacter = ' ';
	backgroundcolor = BBLACK | FWHITE;
	useScreen = false;
	useBackground = false;
	useNull = false;
	doClear = true;
}

int framebuffer::getSizeX() {
	return sizex;
}

void framebuffer::setBox(box box) {
	setSize(box.sizex, box.sizey);
	setOffset(box.offsetx, box.offsety);
	clear();
}

void framebuffer::drawFancyBorder(int type, char color) {
	switch (type) {
		case CHARACTER_BORDER: {
				for (int x = 0; x < sizex + 1; x++) {
					write(x, 0, '-', color);
					write(x, sizey, '-', color);
				}
				for (int y = 0; y < sizey + 1; y++) {
					write(0, y, '|', color);
					write(sizex, y, '|', color);
				}			
				write(0,0, '+', color);
				write(0,sizey, '+', color);
				write(sizex,0, '+', color);
				write(sizex, sizey, '+', color);
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
	for (int x = 0; x < sizex + 1; x++) {
		write(x, 0, character, color);
		write(x, sizey, character, color);
	}
	for (int y = 0; y < sizey + 1; y++) {
		write(0, y, character, color);
		write(sizex, y, character, color);
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
	//We don't need to copy if we already use the screen's buffer
	if (!useScreen) {
		for (int x = 0; x < sizex; x++)
			for (int y = 0; y < sizey; y++)
				write(x, y, fb[get(x, y)], cb[get(x, y)]);
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
			write(x, y, character, color);
}	

void framebuffer::write(int x, int y, wchar_t character, char color) {
	if (useScreen) {
		int nX = x + offsetx, nY = y + offsety;
		scr->clip(nX, nY);
		scr->write(nX, nY, character, color);		
	} else {
		fb[get(x, y)] = character;
		cb[get(x, y)] = color;		
	}
}

void framebuffer::write(int x, int y, char character, char color) {
	write(x, y, (wchar_t)character, color);
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