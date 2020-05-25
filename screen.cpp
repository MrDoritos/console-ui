#include "screen.h"
#include "element.h"

#include <string.h>

#include <bitset>

screen::screen()
	:framebuffer(this)
{
	//while(!adv::ready);
	if (!adv::ready) {
		std::unique_lock<std::mutex> lk(adv::startLock);
		if (adv::ready)
			goto cond;
		adv::cvStart.wait(lk);
	}
	cond:;
		
	adv::setThreadState(false); //We do it manually
	//adv::setDrawingMode(DRAWINGMODE_COMPARE);
	
	run = false;
	
	child = new element(this);
	focused = child;
	setSize(console::getConsoleWidth(), console::getConsoleHeight());
	child->setc(get());
	setBackground(' ', BBLACK | FBLACK);
	clear();
	//Already know the screen resolution, and this will not change because the console buffer will remain the same.

}
	
void screen::start() {
	memset(keys, 0, sizeof(keys));
	run = true;
	loop();
}

void screen::focusPush() {
	bool token = child->focused();
	element* last = focused;
	for (int i = 0; i < 2; i++) {
		focusPush(child, token);
		if (last != focused)
			break; //Win
	}
}

bool screen::focusPush(element* current, bool& token) {
	for (auto* e : current->children) {
		token =
	}
}

//Reverse iterator, however you do dat
void screen::focusPop(element* current, bool& token) {
	
}
	
void screen::loop() {
	long fc = 0;
	while (run) {
		//Key presses
		int key = console::readKeyAsync();
		if (HASKEY(key, VK_ESCAPE)) {
			return;
		}			
		if (HASKEY(key, VK_TAB) && HASMOD(key, __CTRL)) {
			fprintf(stderr, "focus change %ld to ", focused);
			focusPush();
			fprintf(stderr, "%ld\r\n", focused);
		}
		if (key) {
			std::bitset<32> b(key);
			fprintf(stderr, "You pressed %i:%c [%s]\r\n", key, key, b.to_string().c_str());		
		}
		
		if (pressed == key) {
			held = key;
			child->keyheld();
			child->keypress();
		} else {
			pressed = key;
			child->keypress();
		}
		
		/*
		//Skip null key, null is no key
		for (int i = 1; i < 256; i++) {
			if (i == key) {
				if (keys[i].pressed = (!keys[i].held)) {
					pressed = i;
					child->keypress();
				}
				if (keys[i].held = (keys[i].pressed && !keys[i].held)) {
					keys[i].pressed = false;
					held = i;
					child->keyheld();
				}
			} else {
				if (keys[i].released = (keys[i].pressed || keys[i].held)) {
					keys[i].pressed = false;
					keys[i].held = false;
					released = i;
					child->keyrelease();
				}
				keys[i].pressed = false;
				keys[i].held = false;
			}
		}
		*/
						
		child->onClear();
		
		child->update();
		
		child->onFrame();		
		
		int c = getCount();
		/*
		#ifdef __WIN32
		{
			//CHAR_INFO* framebuffer = (CHAR_INFO*)alloca(sizeof(CHAR_INFO) * c);
			CHAR_INFO framebuffer[c];
			//fprintf(stderr, "%i\r\n", c);
			for (int i = 0; i < c; i++) {
				framebuffer[i].Char.UnicodeChar = fb[i];
				framebuffer[i].Attributes = cb[i];
			}
			console::write(&framebuffer[0], c);
		}
		#elif defined __linux__
		char* framebuffer = (char*)alloca(sizeof(char) * c);
		for (int i = 0; i < c; i++) {
			framebuffer[i] = (char)fb[i];
		}
		console::write(fb, cb, c);
		#endif		
		*/
		
		
		//snwprintf(fb, 10, L"%ld", fc++);
		
		console::write(fb, cb, c);
		
		//adv::write(fb, cb, c);
		
		//adv::draw();
				
		//30 updates per second with a peek frame count of 30 per second
		console::sleep(33);
		
	}
}

