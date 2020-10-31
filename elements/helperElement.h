#pragma once
#include "screen.h"
#include "element.h"

struct screen;

static void* s_screen = nullptr;

template<typename T>
struct helperElement : public element {
	void* derived;
	
	helperElement(void* derived)
		:element((struct screen*)s_screen) {
			this->derived = derived;
			if (derived != nullptr)
				handler.setReferer(derived);
			else
				fprintf(stderr, "Warning! derived not set %p\r\n", derived);
			if (s_screen == nullptr)
				fprintf(stderr, "Warning! s_screen not set %p\r\n", s_screen);
			if (derived == s_screen)
				fprintf(stderr, "Warning! derived equals screen %p:%p\r\n\tProgram does not exit here. Program is bound to exit soon.\r\n", derived, s_screen);
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