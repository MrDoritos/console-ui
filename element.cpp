#include "element.h"

#include "screen.h"
/*
element::element(screen* scr)
	:framebuffer(scr)
{
	this->screen = scr;
}
	*/

//We also have to notify the framebuffer
//and call resize
void element::setc(box box) {
	setBox(box);
	resize();
}

void element::setc(float positionx, float positiony, float scalex, float scaley) {
	box b = parent->getc(positionx, positiony, scalex, scaley);
	setc(b);
}

void element::setc(float position, float scale) {
	box b = parent->getc(position, position, scale, scale);
	setc(b);
}
	
box element::getc(float position, float scale) {
	int nSizeX = scale * sizex; //-1
	int nSizeY = scale * sizey; //-1
	int nOffsetX = position * sizex;//((position * sizex) - (nSizeX / 2.0f));
	int nOffsetY = position * sizey;//((position * sizey) - (nSizeY / 2.0f));
	box box(nOffsetX + offsetx, nOffsetY + offsety, nSizeX, nSizeY);
	return box;
}

box element::getc(float positionx, float positiony, float scalex, float scaley) {
	int nSizeX = scalex * sizex; //-1
	int nSizeY = scaley * sizey; //-1
	int nOffsetX = positionx * sizex;
	int nOffsetY = positiony * sizey;
	box box(nOffsetX + offsetx, nOffsetY + offsety, nSizeX, nSizeY);
	return box;	
}
	
void element::update() {
	for (auto* e : children)
		e->update();
	handler.handle(FLG_UPDATE);
}

void element::keypress() { 
	for (auto* e : children)
		e->keypress();
	handler.handle(FLG_KEYPRESS);	
}

void element::keyheld() {
	for (auto* e : children)
		e->keyheld();
	handler.handle(FLG_KEYHELD);
}

void element::keyrelease() {
	for (auto* e : children)
		e->keyrelease();
	handler.handle(FLG_KEYRELEASE);
}

void element::create() {
	for (auto* e : children)
		e->create();
	handler.handle(FLG_CREATE);
}

void element::resize() {
	for (auto* e : children)
		e->resize();
	handler.handle(FLG_RESIZE);
}

void element::onFrame() {
	clear();
	handler.handle(FLG_FRAME);
	frame();
	for (auto* e : children) {
		if (e->doFrame)
			e->onFrame();
	}
	
	//clear(); frame();  children; handler();
}

void element::onClear() {
	clear();
	for (auto* e : children) {
		e->onClear();
	}
	handler.handle(FLG_CLEAR);
}

void element::close() {
	for (auto* e : children)
		e->close();
	handler.handle(FLG_CLOSE);
}

void element::focus() {
	//Must not call children when focused, focus is a single element type deal, ya know
	//for (auto* e : children)
	//	e->focus();
	//focused = true;
	handler.handle(FLG_FOCUS);
}

bool element::focused() {
	return screen->focused == this;
}

element* element::add(element* element) {
	element->parent = this; //Why the fuck ain't this workin
	children.push_back(element);
	//Call the resize? Sure.. can't call resize, call create instead
	//You idiot, call the child's create
	element->create();
	return element;
}

element* element::add(element& element) {
	element.parent = this;
	children.push_back(&element);
	element.create();
	return &element;
}