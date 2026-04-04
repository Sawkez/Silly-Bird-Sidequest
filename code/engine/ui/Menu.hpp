#pragma once

#include "3rdparty/lvgl/lvgl.h"

class Menu {
   protected:
	lv_obj_t* _screen = NULL;

   public:
	virtual void Init() { _screen = lv_obj_create(NULL); }
	virtual void Activate() {
		if (_screen == NULL) {
			this->Init();
		}

		lv_screen_load(_screen);
	}
	virtual void Deactivate() {}
};