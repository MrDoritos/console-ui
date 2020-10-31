#pragma once

#include <cmath>

struct box {
	box(int offsetx, int offsety, int sizex, int sizey);
	box(int sizex, int sizey);
	box();
	void clip(int& x, int &y) {
		x = x >= sizex ? sizex - 1 : x;
		y = y >= sizey ? sizey - 1 : y;
		x = x < 0 ? 0 : x;
		y = y < 0 ? 0 : y;
	}
		
	int get(int x, int y) {
		return (y * sizex) + x;
	}
	
	box get() {
		box box(offsetx, offsety, sizex, sizey);
		return box;
	}
	
	int getOffsetXint(float scale, int length) {
		if (length < 1.0f) {
			return (int)round(float(sizex) * scale);
		}
		if (length > sizex)
			return 0.0f;
		return (int)round(float(sizex) * scale) - (float(length) / 2);
	}
	
	int getOffsetX(float scale, float length) {
		if (length < 1) {
			return (float(sizex) * scale);
		}
		if (length > sizex)
			return 0.0f;
		
		return (float(sizex) * scale) - (length / 2);
	}
	
	int getOffsetX(float scale) {
		return getOffsetX(scale, 0);
	}
	
	int getOffsetY(float scale) {
		return getOffsetY(scale, 0);
	}
	
	int getOffsetY(float scale, float length) {
		if (length < 1) {
			return (float(sizex) * scale);
		}
		if (length > sizex)
			return 0.0f;
		
		return (float(sizex) * scale) - (length / 2);
	}
	
	int getOffsetYint(float scale, int length) {
		if (length < 1.0f) {
			return (int)round(float(sizey) * scale);
		}			
		if (length > sizex)
			return 0.0f;
		return (int)round(float(sizey) * scale) - (float(length) / 2);
	}
	
	int getCount() {
		return sizex * sizey;		
	}
	
	int sizex;
	int sizey;
	int offsetx;
	int offsety;
};