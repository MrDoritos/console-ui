#pragma once

#define FLG_UPDATE 1
#define FLG_KEYPRESS 2
#define FLG_KEYHELD 3
#define FLG_KEYRELEASE 4
#define FLG_CLICK 5
#define FLG_CREATE 6
#define FLG_RESIZE 7
#define FLG_FRAME 8
#define FLG_CLOSE 9
#define FLG_FOCUS 10
#define FLG_CLEAR 11

#define FLG_NUM 12

#define func(cs, xs) ( (void(*)(void*)) ((void(*)(cs*))&xs) )


struct eventHandler {
	public:
	eventHandler() {
		avEv.a = 0u;
		_referer = 0u;
	}
	void assign(int flg, void (*f)(void*)) {
		if (flg > FLG_NUM || flg < 1) {
			return;				
		}
		if (f == nullptr) {
			return;
		}
		funcEvent[flg - 1] = f;
		avEv.a |= (1 << flg);
	}
	void unassign(int flg) {
		if (flg > FLG_NUM || flg < 1)
			return;
		avEv.a &= ~(1 << flg);
	}
	void handle(int flg) {
		if (flg > FLG_NUM || flg < 1) {
			return;
		}
		if (!((1 << flg) & avEv.a)) {
			return;
		}
		if (_referer == nullptr) {
			return;			
		}
	
		(*funcEvent[flg - 1])(_referer);
	}
	void setReferer(void* obj) {
		_referer = obj;
	}
	//All events
	void (*funcEvent[FLG_NUM])(void*);
	void* _referer;
	
	union __av {
	char avEvents[4];
	int a;
	} avEv;
};