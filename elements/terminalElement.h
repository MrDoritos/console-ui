#pragma once
#include "helperElement.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct terminalElement : public helperElement<terminalElement> {
	terminalElement():helperElement(this) {
		xCur = 0;
		yCur = 0;
		doUseScreen(false);//Signal the framebuffer to copy from an internal buffer instead of writing directly to the screen
		doUseClear(false); //Signal the framebuffer to not clear the written buffer at frametime
		doUseBorder(false);

		run = true;
		read = true;
		readLine = true;
	}
		
	int xCur, yCur;
	
	std::mutex mu, cout_mu;
	std::condition_variable cond;
	bool run;
	int inputBuffer;
	std::string currentLine;
	bool read;
	bool readLine;
	
	void onClose() override {		
		fprintf(stderr, "destructor called\r\n");
		run = false;
		cond.notify_all();
	}
	
	void onCreate() override {
		setc(0.5f,0.0f,0.5f,0.5f);
		clear(true);
	}
	
	void onResize() override {
		//setc(0.25f,0.25f,0.5f,0.5f);
		clear(true);
	}
	
	void terminalWriteLine(const char* str) {
		size_t linelen = strlen(str);
		char newStr[linelen + 3];
		memset(&newStr[0], 0, linelen + 3);
		memcpy(&newStr[0], str, linelen);
		newStr[linelen] = '\r';
		newStr[linelen+1] = '\n';
		terminalWrite(&newStr[0]);
	}
	
	void onKeypress() {
		//Make internal changes
		switch (NOMOD(screen->pressed)) {
			case VK_BACKSPACE:
				if (currentLine.length() > 0)
					currentLine.pop_back();
				break;
			case '\n':
				readLine = false;
				break;
			default:
				if (NOMOD(screen->pressed) < ' ' || NOMOD(screen->pressed) > '~')
					break;
				currentLine += NOMOD(screen->pressed);
				break;
		}
		
		//Notify consumers
		if (NOMOD(screen->pressed)) {
			{
				std::lock_guard<std::mutex> lk(mu);
				inputBuffer = NOMOD(screen->pressed);
				read = false;
				if (screen->pressed == '\n')
					readLine = false;
			}
			cond.notify_all();
		}
	}
	
	bool keyAvailable() {
		return !read;
	}
	
	bool lineAvailable() {
		return !readLine;
	}
	
	void terminalReadLine(std::string& str) {
		if (lineAvailable()) {
			readLine = true;
			str = currentLine;
			currentLine.clear();
			return;
		}
		while (run) {
			std::unique_lock<std::mutex> locker(mu);
			cond.wait(locker, [&]() { return !run || lineAvailable(); });
			if (!run)
				return;
			readLine = true;
			str = currentLine;
			currentLine.clear();
			return;
		}		
	}
	
	int terminalRead() {
		while (run) {
			std::unique_lock<std::mutex> locker(mu);
			cond.wait(locker, [&]() { return !run || inputBuffer; });
			if (!run)
				return 0;
			read = true;
			return inputBuffer;
		}
		return 0;
	}
	
	void terminalNewLine() {
		if (yCur + 2 > getSizeY()) {
			char* nCb = new char[getCount()];
			wchar_t* nFb = new wchar_t[getCount()];
			memset(nCb, 0, getCount());
			memset(nFb, 0, getCount() * 2);
			
			//Copy current framebuffer to new buffers, omitting first row
			//TODO know if border is drawn			
			for (int y = 0; y < getSizeY()-1; y++) {
				for (int x = 0; x < getSizeX(); x++) {
					nCb[(y * getSizeX()) + x] = cb[((y+1) * getSizeX()) + x];
					nFb[(y * getSizeX()) + x] = fb[((y+1) * getSizeX()) + x];
				}
			}
						
			delete cb;
			delete fb;
			
			cb = nCb;
			fb = nFb;		
		} else {			
			if (yCur - 1 < getSizeY())
				yCur++;
		}
	}
	
	void terminalWrite(char ch) {
		char str[2];
		str[0] = ch;
		str[1] = '\0';
		terminalWrite(&str[0]);
	}
	
	void terminalWrite(const char* str) {
		if (!run)
			return;
		
		size_t linelen = strlen(str);
		for (int i = 0; i < linelen; i++) {
			switch (str[i]) {
				case '\r':
					xCur = 0;
					break;
				case '\n':
					terminalNewLine();
					//Linux does not use carriage return
					xCur = 0;
					break;
				default:
					if (xCur + 1 > getSizeX()) {
						terminalNewLine();
						xCur = 0;
					}
					write(xCur++, yCur, str[i], FWHITE | BBLACK);
					break;
			}
		}
	}
	
	int test;
	
	void onUpdate() override {
		//write(0,0,char((('0' + ++test) % '9') + '0'), FWHITE | BBLACK);
		//terminalWrite((std::string("Ian's World") + std::to_string(test++%20)).c_str());
	}
};