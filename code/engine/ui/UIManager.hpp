#pragma once

#include <SDL.h>

#include "3rdparty/lvgl/lvgl.h"

class UIManager {
   private:
	lv_display_t* _display = NULL;
	static uint8_t _buf[480 * 272 / 10 * 2];
	lv_indev_t* _inputDevice = NULL;
	lv_obj_t* _label = NULL;

	SDL_Texture* _texture;

	static void FlushCallback(lv_display_t* display, const lv_area_t* area, uint8_t* pixelData) {
		auto* instance = (UIManager*)lv_display_get_user_data(display);

		void* outPixels;
		int pitch;

		int w = (area->x2 - area->x1) + 1;
		int h = (area->y2 - area->y1) + 1;

		SDL_Rect rect = {area->x1, area->y1, w, h};

		if (SDL_LockTexture(instance->_texture, &rect, &outPixels, &pitch) < 0) {
			std::cerr << "Failed to lock UI texture!" << std::endl;
			lv_display_flush_ready(display);
			return;
		}

		size_t bytesPerPixel = LV_COLOR_DEPTH / 8;
		size_t bytesPerRow = w * bytesPerPixel;

		for (int y = 0; y < h; y++) {
			uint8_t* dst = (uint8_t*)outPixels + (y * pitch);
			uint8_t* src = pixelData + (y * bytesPerRow);

			memcpy(dst, src, bytesPerRow);
		}

		SDL_UnlockTexture(instance->_texture);

		lv_display_flush_ready(display);
	}

	static void TouchReadCallback(lv_indev_t* inputDevice, lv_indev_data_t* data) {
		// ???
	}

   public:
	UIManager(SDL_Renderer* renderer) : _texture(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 480, 272)) {
		SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
		lv_init();

		lv_tick_set_cb(SDL_GetTicks);

		_display = lv_display_create(480, 272);
		lv_display_set_user_data(_display, this);
		lv_display_set_color_format(_display, LV_COLOR_FORMAT_ARGB8888);

		lv_display_set_buffers(_display, _buf, NULL, sizeof(_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

		lv_display_set_flush_cb(_display, FlushCallback);

		_inputDevice = lv_indev_create();
		lv_indev_set_type(_inputDevice, LV_INDEV_TYPE_POINTER);
		lv_indev_set_read_cb(_inputDevice, TouchReadCallback);

		lv_obj_set_style_bg_opa(lv_screen_active(), LV_OPA_TRANSP, LV_PART_MAIN);

		_label = lv_label_create(lv_screen_active());
		lv_label_set_text(_label, "i eated bug is something wrong w me ? 🧟");
		lv_obj_center(_label);
	}

	void Process() { lv_timer_handler(); }

	void Draw(SDL_Renderer* renderer) { SDL_RenderCopy(renderer, _texture, NULL, NULL); }
};

uint8_t UIManager::_buf[480 * 272 / 10 * 2];