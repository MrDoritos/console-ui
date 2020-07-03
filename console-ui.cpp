#pragma once

#include "screen.h"
#include "element.h"

#include <string>

#define SX getSizeX()
#define SY getSizeY()
#define OX getOffX()
#define OY getOffY()

static void* s_screen;

template<typename T>
struct helperElement : public element {
	void* derived;
	helperElement(void* derived)
		:element(s_screen) {
			this->derived = derived;
			if (derived)
				handler.setReferer(derived);
			handler.assign(FLG_CLEAR, func(T, T::onClear));
			handler.assign(FLG_CLICK, func(T, T::onClick));
			handler.assign(FLG_CLOSE, func(T, T::onClose));
			handler.assign(FLG_CREATE, func(T, T::onCreate));
			handler.assign(FLG_FOCUS, func(T, T::onFocus));
			handler.assign(FLG_FRAME, func(T, T::onFrame));
			handler.assign(FLG_KEYHELD, func(T, T::onKeyheld));
			handler.assign(FLG_KEYPRESS, func(T, T::onKeypress));
			handler.assign(FLG_KEYRELEASE, func(T, T::onKeyrelease));
			handler.assign(FLG_RESIZE, func(T, T::onResize));
			handler.assign(FLG_UPDATE, func(T, T::onUpdate));
		}
		
	virtual void onClear() {}
	virtual void onClick() {}
	virtual void onClose() {}
	virtual void onCreate() {}
	virtual void onFocus() {}
	virtual void onFrame() {}
	virtual void onKeyheld() {}
	virtual void onKeypress() {}
	virtual void onKeyrelease() {}
	virtual void onResize() {}
	virtual void onUpdate() {}
};

struct newTb : public helperElement<newTb> {
	newTb():helperElement(this) {}
	
	void onCreate() override {
		//setBackground('#', FRED | BWHITE);
		//setBorder(FBLUE | BWHITE, true);
		//doUseBackground(true);
		//border = 0;
		//doUseScreen(true);
	}
	
	unsigned char border;
	
	void onUpdate() override {
		doUseBorder(border++ % 60 > 30);
	}
	
	void onFrame() override {
		//write(getOffsetX(0.5f), getOffsetY(0.5f), '#', FRED | BWHITE);
		const char* str = "Hello World! Hitler 2000 Hitler 2000 Hitler 2000";
		int len = strlen(str);
		
		for (int i = 0; i < len; i++)
			write(i % SX, i / SX, str[i], FWHITE | BBLACK);
	}
};

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
			//doUseBorder(true);
			setBorder(FBLUE | BBLACK, true);
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
			write(sbuffer.length() % SX, sbuffer.length() / SX, '|', FWHITE | BBLACK);
		else 
			write(sbuffer.length() % SX, sbuffer.length() / SX, ' ', FWHITE | BBLACK);
		
		if (tick > tickCount)
			tick = -tickCount;
	}
	
	void frame() {
		//if (focused())
		//	drawFancyBorder(CHARACTER_BORDER, FRED | BBLACK);
				
		for (int i = 0; i < sbuffer.length(); i++)
			write(i % SX, i / SX, sbuffer[i], FWHITE | BBLACK);
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

int wmain() {
	screen wscreen;
	s_screen = &wscreen;
	newTb n;
	wscreen.child->add(&n);
	//wscreen.child = &n;
	/*
	stest testelement = stest(&wscreen);
	wscreen.child->add(&testelement);
	*/
	//textbox textbox1(&wscreen);
	//n.add(&textbox1);
	n.setc(0.25f,0.25f,0.25f,0.25f);
	/*
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
	*/
	
	wscreen.start();

	//console::write(0,0,"Hello world", BRED | FBLUE);
	//while (console::readKey() != 27);
	return 1;
}