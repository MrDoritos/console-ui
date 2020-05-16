#pragma once
#include <vector>
#include "eventHandler.h"
#include "framebuffer.h"

struct screen;

struct element : public framebuffer {
	element(screen* scr)
		:framebuffer(scr)
	{
		this->screen = scr;
		parent = nullptr;
		closed = false;
		focused = false;
		trapArrowKeys = false;
		doFrame = true;
	}
	
	void update();
	void keypress();
	void keyheld();
	void keyrelease();
	void create();
	void resize();
	void onFrame();
	void onClear();
	void close();
	void focus();
	element* add(element* element);
	element* add(element& element);
	
	//For transformations relative to this element.
	//I know it doesn't make sense but setc
	void setc(box box);
	//Relative to parent
	void setc(float positionx, float positiony, float scalex, float scaley);
	
	box getc(float position, float scale); //Position relative to this element, and a scale also relative
	box getc(float positionx, float positiony, float scalex, float scaley);
	
	
	
	eventHandler handler;
	//framebuffer buffer; //Should deconstruct and clean up memory for us
	std::vector<element*> children;
	screen* screen;
	element* parent;
	bool closed;
	bool focused;
	bool trapArrowKeys;
	bool doFrame;
};