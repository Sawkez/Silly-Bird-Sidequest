#pragma once

#include "3rdparty/lvgl/lvgl.h"

class Menu {
   protected:
	lv_obj_t* _screen;

   public:
	Menu() : _screen(lv_obj_create(NULL)) {};

	virtual void Activate() { lv_screen_load(_screen); }
};