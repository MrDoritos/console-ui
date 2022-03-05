#include <algorithm>

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

bool framebuffer::bound(int x, int y) {
	/*if (useBorder)
		return (x > offsetx + 1 && y > offsety + 1 && x < offsetx + sizex - 2 && y < offsety + sizey - 2);
	else*/
		return (x > -1 && y > -1 && x < sizex && y < sizey);
		//return true;
}

void framebuffer::setBox(box box) {
	setSize(box.sizex, box.sizey);
	setOffset(box.offsetx, box.offsety);
	clear();
}

void framebuffer::setBorder(color_t color) {
	borderColor = color;
}

void framebuffer::setBorder(color_t color, bool state) {
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

void framebuffer::drawFancyBorder(int type, color_t color) {
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

void framebuffer::drawBorder(char character, color_t color) {
	drawBorder((wchar_t)character, color);
}

void framebuffer::drawBorder(wchar_t character, color_t color) {
	for (int x = 0; x < sizex; x++) {
		write(x, 0, character, color, true);
		write(x, sizey - 1, character, color, true);
	}
	for (int y = 0; y < sizey; y++) {
		write(0, y, character, color, true);
		write(sizex - 1, y, character, color, true);
	}	
}

void framebuffer::setBackground(char character, color_t color) {
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
	cb = new color_t[sizex * sizey];
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

void framebuffer::clear(bool force) {
	if (!doClear && !force)
		return;
	
	if (useBackground)
			clear(backgroundcharacter, backgroundcolor);
	else 
		if (useNull)
			clear('\0', backgroundcolor);
		else
			clear(' ', backgroundcolor);
}

void framebuffer::clear(char character, color_t color) {
	for (int x = 0; x < sizex; x++)
		for (int y = 0; y < sizey; y++)
			write(x, y, character, color, true);
}	

void framebuffer::write(int x, int y, const char *cstring, color_t color, bool borderOverride) {
	for (int i = 0; i < strlen(cstring); i++)
		write(x + i,y,cstring[i],color,borderOverride);
}

void framebuffer::write(int x, int y, const wchar_t *cstring, color_t color, bool borderOverride) {
	for (int i = 0; i < wcslen(cstring); i++)
		write(x + i,y,cstring[i],color,borderOverride);
}

void framebuffer::write(int x, int y, wchar_t character, color_t color, bool borderOverride) {
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
		//if (bound(nX, nY))
		scr->write(nX, nY, character, color, true);
	} else {
		if (useBorder && !borderOverride) {
			x++;
			y++;
		}
		if (!bound(x, y))
			return;
		fb[get(x, y)] = character;
		cb[get(x, y)] = color;		
	}
}

void framebuffer::write(int x, int y, char character, color_t color, bool borderOverride) {
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

//Drawing functions
void framebuffer::drawLine(int x1, int y1, int x2, int y2, wchar_t character, color_t color) {	
	int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;
	
	dx=x2-x1;
	dy=y2-y1;
	
	dx1=fabs(dx);
	dy1=fabs(dy);
	
	px=2*dy1-dx1;
	py=2*dx1-dy1;

	if(dy1<=dx1) {
		if(dx>=0) {
			x=x1;
			y=y1;
			xe=x2;
		} else {
			x=x2;
			y=y2;
			xe=x1;
		}
		if (bound(x,y)) {
			write(x, y, character, color);
			//fb[get(x,y)] = character;
			//cb[get(x,y)] = color;
		}
		
		for(i=0;x<xe;i++) {
			x=x+1;
			if(px<0) {
				px=px+2*dy1;
			} else {
				if((dx<0 && dy<0) || (dx>0 && dy>0)) {
					y=y+1;
				} else {
					y=y-1;
				}
				px=px+2*(dy1-dx1);
			}
			
			if (bound(x,y)) {
				write(x, y, character, color);
				//fb[get(x,y)] = character;
				//cb[get(x,y)] = color;
			}
		}
	} else {
		if(dy>=0) {				
			x=x1;
			y=y1;
			ye=y2;
		} else {
			x=x2;
			y=y2;
			ye=y1;
		}  
		
		//write(x, y, character, color);
		if (bound(x,y)) {
			write(x, y, character, color);
			//fb[get(x,y)] = character;
			//cb[get(x,y)] = color;
		}

		for(i=0;y<ye;i++) {
			y=y+1;
			if(py<=0) {
				py=py+2*dx1;
			} else {
				if((dx<0 && dy<0) || (dx>0 && dy>0)) {
					x=x+1;
				} else {
					x=x-1;
				}
				
				py=py+2*(dx1-dy1);
			}	
			
			if (bound(x,y)) {
				write(x, y, character, color);
				//fb[get(x,y)] = character;
				//cb[get(x,y)] = color;
			}
		}
	}
	
	//modify = true;
}

void framebuffer::drawLine(int x0, int y0, int x1, int y1, char character, color_t color) {	
	drawLine(x0, y0, x1, y1, (wchar_t)character, color);
}


void framebuffer::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, wchar_t character, color_t color) {
	if (!bound(x0, y0) ||
		!bound(x1, y1) ||
		!bound(x2, y2))
		//return;
		{ fprintf(stderr, "unbound\r\n");}
	
	if (y0 > y1) {
		std::swap(y0, y1);
		std::swap(x0, x1);
	}
	if (y0 > y2) {
		std::swap(y0, y2);
		std::swap(x0, x2);
	}
	if (y1 > y2) {
		std::swap(y1, y2);
		std::swap(x1, x2);
	}
	
	int total_height = y2 - y0;
	
	for (int y = y0; y <= y1; y++) {
		int segment_height = y1-y0+1; 
		float alpha = (float)(y-y0)/total_height; 
		float beta  = (float)(y-y0)/segment_height; // be careful with divisions by zero 
		//Vec2i A = t0 + (t2-t0)*alpha;
		int Ay = y0 + (y2 - y0) * alpha;
		int Ax = x0 + (x2 - x0) * alpha;
		//Vec2i B = t0 + (t1-t0)*beta; 
		int By = y0 + (y1 - y0) * beta;
		int Bx = x0 + (x1 - x0) * beta;
		
		if (Ax>Bx) { std::swap(Ax, Bx); std::swap(Ay, By); }
		
		for (unsigned int j=Ax; j<=Bx; j++) { 
			if (bound(j, y)) {
				write(j, y, character, color);
			}
		} 			
	}
			
	for (unsigned int y=y1; y<=y2; y++) { 
		int segment_height =  y2-y1+1; 
		float alpha = (float)(y-y0)/total_height; 
		float beta  = (float)(y-y1)/segment_height; // be careful with divisions by zero 			
		int Ay = y0 + (y2 - y0) * alpha;
		int Ax = x0 + (x2 - x0) * alpha;			
		int By = y1 + (y2 - y1) * beta;
		int Bx = x1 + (x2 - x1) * beta;
		
		if (Ax>Bx) { std::swap(Ax, Bx); std::swap(Ay, By); }
					
		for (unsigned int j=Ax; j<=Bx; j++) { 
			if (bound(j, y)) {
				write(j, y, character, color);
			}
		} 
	} 
}

void framebuffer::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, char character, color_t color) {
	drawTriangle(x0, y0, x1, y1, x2, y2, (wchar_t)character, color);
}

void framebuffer::drawRectangle(int x0, int y0, int x1, int y1, wchar_t character, color_t color) {
	drawLine(x0, y0, x0, y1, character, color);
	drawLine(x1, y0, x1, y1, character, color);
	drawLine(x0, y0, x1, y0, character, color);
	drawLine(x0, y1, x1, y1, character, color);	
}

void framebuffer::drawRectangle(int x0, int y0, int x1, int y1, char character, color_t color) {
	drawRectangle(x0, y0, x1, y1, (wchar_t)character, color);	
}

void framebuffer::drawFill(int x0, int y0, int x1, int y1, wchar_t character, color_t color) {
	for (int x = x0; x < x1; x++)
		for (int y = y0; y < y1; y++)
			write(x, y, character, color);
}

void framebuffer::drawFill(int x0, int y0, int x1, int y1, char character, color_t color) {
	drawFill(x0, y0, x1, y1, (wchar_t)character, color);
}