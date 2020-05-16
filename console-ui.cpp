#pragma once

#include "screen.h"
#include "element.h"

#include <string>

struct textbox : public element {
	textbox(void* screen)
		:element(screen)
		{
			tick = 0;
			tickCount = 20;
			handler.setReferer(this);
			handler.assign(FLG_UPDATE, func(textbox, textbox::update));
			handler.assign(FLG_FRAME, func(textbox, textbox::frame));
			handler.assign(FLG_KEYPRESS, func(textbox, textbox::keypress));
			setSize(10,10);
			setOffset(0,0);
			setBackground('#', BWHITE | FBLACK);
			clear();
			doUseScreen(true);
		}
		
	void keypress() {
		switch (screen->pressed) {
			case 8:
			{
				if (sbuffer.length() > 0)
					sbuffer.pop_back();
				break;
			}			
			default:
			{
				sbuffer += screen->pressed;				
			}			
		}
	}
		
	void update() {
		tick++;
		if (tick > 0)
			write(sbuffer.length() % sizex, sbuffer.length() / sizex, '|', FWHITE | BBLACK);
		else 
			write(sbuffer.length() % sizex, sbuffer.length() / sizex, ' ', FWHITE | BBLACK);
		
		if (tick > tickCount)
			tick = -tickCount;
	}
	
	void frame() {
		for (int i = 0; i < sbuffer.length(); i++)
			write(i % sizex, i / sizex, sbuffer[i], FWHITE | BBLACK);
	}
	
	std::string sbuffer;
	
	int tick;
	int tickCount;
};

struct stest : public element {
	stest(void* screen)
		: element(screen)
		{
			handler.setReferer(this);
			handler.assign(FLG_UPDATE, func(stest, stest::update));
			handler.assign(FLG_KEYRELEASE, func(stest, stest::keyreleased));
			handler.assign(FLG_KEYPRESS, func(stest, stest::keypressed));
			handler.assign(FLG_KEYHELD, func(stest, stest::keyheld));			
			setSize(25, 25);			
			setOffset(0, 0);
			setBackground('-', BRED | FBLUE);
			clear();
			doUseScreen(true);
			//console::write(fb, cb, getCount());
		}
	
	void update() {
		//console::write(0,0,messages[rand() % 4], BBLACK | FWHITE);
	}
	
	void keyreleased() {
		//console::write(0,1,"Released:");
		//console::write(0,2,(char)screen->released);
	}
	
	void keypressed() {
		//console::write(0,3,"Pressed:");
		//console::write(0,4,(char)screen->pressed);		
	}
	
	void keyheld() {
		//console::write(0,5,"Held:");
		//console::write(0,6,(char)screen->held);		
	}
	
	const char* messages[20] = {
		"Hello World",
		"Your mom gay",
		"Test test",
		"Ian's program"		
	};
};

int main() {
	screen wscreen;
	stest testelement = stest(&wscreen);
	wscreen.child->add(&testelement);
	textbox textbox(&wscreen);
	//wscreen.child->add(&textbox);
	testelement.add(&textbox);
	wscreen.start();

	//console::write(0,0,"Hello world", BRED | FBLUE);
	//while (console::readKey() != 27);
	return 0;
}