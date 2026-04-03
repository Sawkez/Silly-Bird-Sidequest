#pragma once

#include <SDL.h>

#include <vector>

#include "3rdparty/lvgl/lvgl.h"
#include "engine/ui/Menu.hpp"
#include "game/ui/PauseMenu.hpp"

class UIManager {
   private:
	lv_display_t* _display = NULL;
	static inline std::vector<uint8_t> _buf;
	lv_indev_t* _inputDevice = NULL;
	lv_obj_t* _label = NULL;

	SDL_Texture* _texture = NULL;
	SDL_Renderer* _renderer;

	Menu* _menus[1];

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

	SDL_Point GetWindowSize(SDL_Window* window) {
		int w, h;
		SDL_GetWindowSizeInPixels(window, &w, &h);
		return {w, h};
	}

	static void TouchReadCallback(lv_indev_t* inputDevice, lv_indev_data_t* data) {
		// ???
	}

   public:
	UIManager(SDL_Renderer* renderer, SDL_Window* window) : UIManager(renderer, GetWindowSize(window)) {}

	UIManager(SDL_Renderer* renderer, SDL_Point windowSize) : _renderer(renderer) {
		SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
		lv_init();

		lv_tick_set_cb(SDL_GetTicks);

		_display = lv_display_create(windowSize.x, windowSize.y);
		lv_display_set_user_data(_display, this);
		lv_display_set_color_format(_display, LV_COLOR_FORMAT_ARGB8888);

		Resize(windowSize.x, windowSize.y);

		lv_display_set_flush_cb(_display, FlushCallback);

		_inputDevice = lv_indev_create();
		lv_indev_set_type(_inputDevice, LV_INDEV_TYPE_POINTER);
		lv_indev_set_read_cb(_inputDevice, TouchReadCallback);

		_menus[0] = new PauseMenu();

		_menus[0]->Activate();
	}

	void Process() { lv_timer_handler(); }

	void Resize(int windowWidth, int windowHeight) {
		_buf.resize(windowWidth * windowHeight * 10 / 4);

		lv_display_set_resolution(_display, windowWidth, windowHeight);
		lv_display_set_buffers(_display, _buf.data(), NULL, _buf.size(), LV_DISPLAY_RENDER_MODE_PARTIAL);

		if (_texture != NULL) {
			SDL_DestroyTexture(_texture);
		}

		_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight);

		if (_texture == NULL) {
			std::cerr << "ERROR creating UI texture: " << SDL_GetError() << std::endl;
			return;
		};

		SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
	}

	bool HandleEvent(const SDL_Event& event) {
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
			Resize(event.window.data1, event.window.data2);
			return true;
		}

		return false;
	}

	void Draw() { SDL_RenderCopy(_renderer, _texture, NULL, NULL); }
};