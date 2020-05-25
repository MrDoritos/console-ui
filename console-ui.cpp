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
			handler.assign(FLG_CREATE, func(textbox, textbox::create));
			setSize(10,10);
			setOffset(0,0);
			setBackground('#', BWHITE | FBLACK);
			clear();
			doUseScreen(true);
			doUseBorder(true);
			setBorder(FBLUE | BBLACK);
		}
		
	void create() {
		setc(parent->getc(0.0f, 0.25f));
	}
		
	void keypress() {
		if (!focused())
			return;
		
		switch (NOMOD(screen->pressed)) {
			case VK_BACKSPACE:
			{
				if (sbuffer.length() > 0)
					sbuffer.pop_back();
				break;
			}			
			default:
			{
				if (NOMOD(screen->pressed) < ' ' || NOMOD(screen->pressed) > '~')
					break;
				sbuffer += NOMOD(screen->pressed);
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
		if (focused())
			drawFancyBorder(CHARACTER_BORDER, FRED | BBLACK);
				
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
	
	textbox textbox1(&wscreen);
	textbox textbox2(&wscreen);
	textbox textbox3(&wscreen);
	textbox textbox4(&wscreen);
	
	testelement.add(&textbox1);
	testelement.add(&textbox2);
	testelement.add(&textbox3);
	testelement.add(&textbox4);
	
	textbox1.setc(0.0f, 0.0f, 0.5f, 0.5f);
	textbox2.setc(0.5f, 0.0f, 0.5f, 0.5f);
	textbox3.setc(0.0f, 0.5f, 0.5f, 0.5f);
	textbox4.setc(0.5f, 0.5f, 0.5f, 0.5f);	
	
	wscreen.start();

	//console::write(0,0,"Hello world", BRED | FBLUE);
	//while (console::readKey() != 27);
	return 1;
}