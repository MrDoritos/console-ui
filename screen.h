#pragma once
#include "framebuffer.h" //Includes box.h
//#include "element.h"
#include "advancedConsole.h"
#include "eventHandler.h"

#include <vector>

struct element;

struct screen : public framebuffer {
	screen();
	
	void start();
	
	void loop();
	
	element* child;
	
	element* focused;
	
	bool run;
	
	void focusPush(element* current, bool& token); //Move upwards in focus. Depth first
	
	void focusPop(element* current, bool& token); //Move backwards in focus. Depth first
	
	struct key {
		bool pressed;
		bool released;
		bool held;
	} keys[256];
	char pressed, released, held;
};
