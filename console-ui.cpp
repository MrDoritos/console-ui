#pragma once

#include "screen.h"
#include "element.h"

#include <string>
#include <math.h>
#include <ctime>

#define SX getSizeX()
#define SY getSizeY()
#define OX getOffX()
#define OY getOffY()

#define PI 3.1415926
#define RAD__(xs) (xs * PI / 180)

double rad(double deg) {
	return RAD__(deg);
}


struct vec3d {
	vec3d() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}
	vec3d(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	vec3d(double x0, double y0, double z0,
	      double x1, double y1, double z1) {
		this->x0 = x0; this->y0 = y0; this->z0 = z0;
		this->x1 = x1; this->y1 = y1; this->z1 = z1;
	}
	vec3d& operator+ (vec3d const & obj) {
		vec3d r;
		r.x = x + obj.x;
		r.y = y + obj.y;
		r.z = z + obj.z;
		return r;
	}
	vec3d& operator+=(vec3d const & obj) {
		x += obj.x;
		y += obj.y;
		z += obj.z;
		return *this;
	}
	vec3d& operator-=(vec3d const & obj) {
		x -= obj.x;
		y -= obj.y;
		z -= obj.z;
		return *this;
	}
	vec3d& operator*=(vec3d const & obj) {
		x *= obj.x;
		y *= obj.y;
		z *= obj.z;
		return *this;
	}
	vec3d operator- (vec3d const & obj) {
		vec3d r;
		r.x = x - obj.x;
		r.y = y - obj.y;
		r.z = z - obj.z;
		return r;
	}
	vec3d operator* (vec3d const & obj) {
		vec3d r;
		r.x = x * obj.x;
		r.y = y * obj.y;
		r.z = z * obj.z;
		return r;
	}
	vec3d operator/ (vec3d const & obj) {
		vec3d r;
		r.x = x / obj.x;
		r.y = y / obj.y;
		r.z = z / obj.z;
		return r;
	}


	double x, y, z;
	double x0, y0, z0;
	double x1, y1, z1;
};

struct triangle {
	triangle() {	}
	triangle(vec3d p0, vec3d p1, vec3d p2) {
		p[0] = p0;
		p[1] = p1;
		p[2] = p2;
	}
	vec3d p[3];
};

static int clipDistance = 16;

struct camera {
	vec3d position;
};

struct mesh {
	mesh() {
	
	}
	
	mesh(const triangle* tris, int length) {
		for (int i = 0; i < length; i++)
			this->tris.push_back(tris[i]);
	}

	std::vector<triangle> tris;
	
	void translate(vec3d t) {
		for (auto& tri : tris) {
			tri.p[0] += t;
			tri.p[1] += t;
			tri.p[2] += t;
		}
	}
	
	void scale(vec3d s, vec3d origin) {
		for (auto& tri : tris) {
			for (int i = 0; i < 3; i++) {
				tri.p[i] -= origin;
				tri.p[i] *= s;
				tri.p[i] += origin;
			}
		}
	}
	
	void rotate(vec3d r, vec3d origin) {
		for (auto& tri : tris) {
			//Upon x
			for (int i = 0; i < 3; i++) {
				//x
				if (r.x != 0.0d) {
					tri.p[i] -= origin;
					vec3d old = tri.p[i];
					tri.p[i].y = old.y * cos(rad(r.x)) - old.z * sin(rad(r.x));
					tri.p[i].z = old.y * sin(rad(r.x)) + old.z * cos(rad(r.x));
					tri.p[i] += origin;
				}
				//y
				if (r.y != 0.0d) {
					tri.p[i] -= origin;
					vec3d old = tri.p[i];
					tri.p[i].x = old.x * cos(rad(r.y)) + old.z * sin(rad(r.y));
					tri.p[i].z = -old.x * sin(rad(r.y)) + old.z * cos(rad(r.y));
					tri.p[i] += origin;
				}
				//z
				if (r.z != 0.0d) {
					tri.p[i] -= origin;
					vec3d old = tri.p[i];
					tri.p[i].x = old.x * cos(rad(r.z)) - old.y * sin(rad(r.z));
					tri.p[i].y = old.x * sin(rad(r.z)) + old.y * cos(rad(r.z));
					tri.p[i] += origin;
				}								
			}
		}
	}
};

const triangle triangles_cube[] = {
	triangle({vec3d(0,1,0),vec3d(1,1,1),vec3d(1,1,0)}),
	triangle({vec3d(0,1,0),vec3d(0,1,1),vec3d(1,1,1)}),

	triangle({vec3d(1,0,0),vec3d(0,0,1),vec3d(0,0,0)}),
	triangle({vec3d(1,0,0),vec3d(1,0,1),vec3d(0,0,1)}),

	triangle({vec3d(0,1,0),vec3d(1,0,0),vec3d(0,0,0)}),
	triangle({vec3d(0,1,0),vec3d(1,1,0),vec3d(1,0,0)}),

	triangle({vec3d(1,1,1),vec3d(0,0,1),vec3d(1,0,1)}),
	triangle({vec3d(1,1,1),vec3d(0,1,1),vec3d(0,0,1)}),

	triangle({vec3d(0,1,1),vec3d(0,1,0),vec3d(0,0,0)}),
	triangle({vec3d(0,1,1),vec3d(0,0,0),vec3d(0,0,1)}),

	triangle({vec3d(1,1,0),vec3d(1,1,1),vec3d(1,0,1)}),
	triangle({vec3d(1,1,0),vec3d(1,0,1),vec3d(1,0,0)}),
};

static mesh cube = mesh(&triangles_cube[0], 12);






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

struct element_cool : public helperElement<element_cool> {
	element_cool():helperElement(this) {}
	void onUpdate() override {
		
	}
	
	void onCreate() override {
		setc(parent->getc(0.25f, 0.5f));
	}
	
	float angle;
	
	void onFrame() override {
		angle += 0.005f;
		float length = 0.8f;
		float radius = (length / 2.0f) * (SX > SY ? SY : SX);
		int centerX = getOffsetX(0.5f), centerY = getOffsetY(0.5f);
		float radians = ((360.0f * angle) + 90.0f) * PI / 180.0f;
		int nX = round(centerX + radius * cos(radians));
		int nY = round(centerY + radius * sin(radians));
		drawLine(centerX, centerY, nX, nY, L' ', BRED | FBLACK);
	}
};

struct element_clock : public helperElement<element_clock> {
	element_clock():helperElement(this) {}
	void onUpdate() override {
		timet = time(0);
		tim = localtime(&timet);		
	}
	
	void onCreate() override {
		setc(parent->getc(0.25f, 0.5f));
	}
	
	time_t timet;
	tm *tim;
	//length = normalized
	//nPosition = normalized
	void drawNeedle(float length, float nPosition) {
		float radius = (length / 2.0f) * (SX > SY ? SY : SX);
		int centerX = getOffsetX(0.5f), centerY = getOffsetY(0.5f);
		float radians = ((360.0f * nPosition) + 90.0f) * PI / 180.0f;
		int nX = round(centerX + radius * cos(radians));
		int nY = round(centerY + radius * sin(radians));
		drawLine(centerX, centerY, nX, nY, L' ', BWHITE | FBLACK);
	}
	
	void onFrame() override {
		drawNeedle(0.5f, ((tim->tm_hour * 3600) + (tim->tm_min * 60) + (tim->tm_sec) + 21600) / 43200.0f); //Hour
		drawNeedle(1.0f, ((tim->tm_min * 60) + (tim->tm_sec + 1800)) / 3600.0f);
		drawNeedle(0.8f, (tim->tm_sec + 30) / 60.0f);
	}
};

struct model3d : public helperElement<model3d> {
	model3d():helperElement(this) {
		cam.position.z = 2;
		cam.position.x = 0;
		cam.position.y = -0.5;
		model = cube;
	}
		
	camera cam;	
	mesh model;
	
	void onUpdate() override {
		model.rotate({0,0,5.0d},{0,0,0});
	}
	
	void onCreate() override {
		setc(parent->getc(0.25f, 0.5f));
	}
	
	bool inView(const triangle& tri) {
		for (int i = 0; i < 3; i++) {
			if (tri.p[i].z > cam.position.z || cam.position.z - tri.p[i].z > clipDistance - 1)
				return false;
		}
		return true;
	}
	
	void rasterTriangle(triangle* tri) {
		triangle translation;
		for (int i = 0; i < 3; i++)
			tri->p[i].y = -tri->p[i].y;
		
		const double fov = rad(90.0d);
		const double hFov = fov / 2.0d;
		const double tanFov = tan(hFov);
		double width = getSizeX() - 4.0d;
		double height = getSizeY() - 4.0d;
		
		for (int i = 0; i < 3; i++) {
			translation.p[i].x = ((tri->p[i].x - (cam.position.x - (tanFov * fabs(tri->p[i].z - cam.position.z)))) / ((tanFov * fabs(tri->p[i].z - cam.position.z)) * 2)) * (getSizeX() - 4.0d);			
			translation.p[i].y = ((tri->p[i].y - (cam.position.y - (tanFov * fabs(tri->p[i].z - cam.position.z)))) / ((tanFov * fabs(tri->p[i].z - cam.position.z)) * 2)) * (getSizeX() - 4.0d);
		}
		
		const char zdepth[] = "#oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
					
		if (cam.position.z - tri->p[0].z < clipDistance) {
			char c = zdepth[(int)floor(((cam.position.z - tri->p[0].z) / clipDistance) * sizeof(zdepth))];
			char color = FWHITE | BBLACK;
			drawLine(translation.p[0].x, translation.p[0].y, translation.p[1].x, translation.p[1].y, c, color);
			drawLine(translation.p[1].x, translation.p[1].y, translation.p[2].x, translation.p[2].y, c, color);
			drawLine(translation.p[2].x, translation.p[2].y, translation.p[0].x, translation.p[0].y, c, color);
		}
	}
	
	void renderMesh(mesh* mesh) {
		std::sort(mesh->tris.begin(), mesh->tris.end(), [](triangle& a, triangle& b){
			return (a.p[0].z < b.p[0].z);
		});
		for (auto t : mesh->tris)
			if (inView(t))
				rasterTriangle(&t);
	}
	
	void onFrame() override {
		renderMesh(&model);
	}
};

struct drawTest : public helperElement<drawTest> {
	drawTest():helperElement(this) {}
	
	void onCreate() override {
		setc(parent->getc(0.25f, 0.5f));
	}
	
	void onFrame() override {
		write(0,0,'H',FWHITE|BBLACK);
		drawTriangle(0,0,4,4,8,0, L'.', BBLACK | FWHITE);
	}
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
			setBorder(FBLUE | BBLACK, false);
		}
		
	void create() {
		setc(parent->getc(0.25f, 0.5f));
	}
		
	void keypress() {
		//if (!focused())
		//	return;
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
				setBackground(' ', ((char)screen->pressed + rand()) % 255);
			}
		}
	}
		
	void update() {
	}
	
	void frame() {
		//if (focused())
		//	drawFancyBorder(CHARACTER_BORDER, FRED | BBLACK);
				
		for (int i = 0; i < sbuffer.length(); i++)
			write(i % SX, i / SX, sbuffer[i], FWHITE | BBLACK);
		
		tick++;
		if (tick > 0)
			write(sbuffer.length() % SX, sbuffer.length() / SX, '|', FWHITE | BBLACK);
		else 
			write(sbuffer.length() % SX, sbuffer.length() / SX, ' ', FWHITE | BBLACK);
		
		if (tick > tickCount)
			tick = -tickCount;
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
	//newTb n;
	//textbox n(s_screen);
	//textbox o(s_screen);
	//model3d n1;
	element_clock n1;
	model3d n2;
	element_cool n3;
	model3d n4;
	//drawTest n;
	wscreen.child->add(&n1);
	wscreen.child->add(&n2);
	wscreen.child->add(&n3);
	wscreen.child->add(&n4);
	n1.setc(0.0f, 0.0f, 0.5f, 0.5f);
	n2.setc(0.5f, 0.0f, 0.5f, 0.5f);
	n3.setc(0.0f, 0.5f, 0.5f, 0.5f);
	n4.setc(0.5f, 0.5f, 0.5f, 0.5f);
	//n.setc(0.25f,0.25f,0.25f,0.25f);
	//n.setc(0.25f,0.5f);
	//n.add(&o);
	//wscreen.child = &n;
	/*
	stest testelement = stest(&wscreen);
	wscreen.child->add(&testelement);
	*/
	//textbox textbox1(&wscreen);
	//n.add(&textbox1);
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