#pragma once

#include <SDL.h>

#include <queue>
#include <vector>

#include "3rdparty/lvgl/lvgl.h"
#include "engine/input/UIInputManager.hpp"
#include "engine/ui/Menu.hpp"
#include "game/ui/Styles.hpp"

class UIManager {
   private:
	UIManager() = delete;
	static inline lv_display_t* _display = NULL;
	static inline std::vector<uint8_t> _buf;

	static inline SDL_Texture* _texture = NULL;
	static inline SDL_Renderer* _renderer;

	static inline bool _visible = false;

	static void FlushCallback(lv_display_t* display, const lv_area_t* area, uint8_t* pixelData) {
		void* outPixels;
		int pitch;

		int w = (area->x2 - area->x1) + 1;
		int h = (area->y2 - area->y1) + 1;

		SDL_Rect rect = {area->x1, area->y1, w, h};

		if (SDL_LockTexture(_texture, &rect, &outPixels, &pitch) < 0) {
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

		SDL_UnlockTexture(_texture);

		lv_display_flush_ready(display);
	}

	static SDL_Point GetWindowSize(SDL_Window* window) {
		int w, h;
		SDL_GetWindowSizeInPixels(window, &w, &h);
		return {w, h};
	}

   public:
	/*
	UIManager(SDL_Renderer* renderer, SDL_Window* window) : UIManager(renderer, GetWindowSize(window)) {}

	UIManager(SDL_Renderer* renderer, SDL_Point windowSize) : _display(InitLVGL(windowSize)), _renderer(renderer), UIInputManager::) {
		Resize(windowSize.x, windowSize.y);
		Menus::Init(*this, GetMainGroup());
	}
	*/

	static void Init(SDL_Renderer* renderer, SDL_Window* window) { Init(renderer, GetWindowSize(window)); }

	static void Init(SDL_Renderer* renderer, SDL_Point windowSize) {
		_display = InitLVGL(windowSize);
		UIInputManager::Init();
		Styles::Init();
		_renderer = renderer;
		Resize(windowSize.x, windowSize.y);
	}

	static lv_display_t* InitLVGL(SDL_Point windowSize) {
		lv_init();

		lv_tick_set_cb(SDL_GetTicks);
		lv_display_t* display = lv_display_create(windowSize.x, windowSize.y);
		lv_display_set_color_format(display, LV_COLOR_FORMAT_ARGB8888);
		lv_display_set_flush_cb(display, FlushCallback);

		return display;
	}

	static void Process() {
		if (!_visible) return;
		lv_timer_handler();
	}

	static void Resize(int windowWidth, int windowHeight) {
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

	static bool HandleEvent(const SDL_Event& event) {
		if (!_visible) return false;

		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
			Resize(event.window.data1, event.window.data2);
			return true;
		}

		return (UIInputManager::HandleEvent(event));
	}

	static void Draw() {
		if (!_visible) return;
		SDL_RenderCopy(_renderer, _texture, NULL, NULL);
	}

	static void Show() {
		GameState::Pause();
		_visible = true;
	}

	static void Show(Menu* menu) {
		menu->Activate();
		Show();
	}

	static void Hide() {
		GameState::Unpause();
		_visible = false;
	}

	static void Toggle() {
		if (!_visible) {
			Show();
		} else {
			Hide();
		}
	}

	static lv_group_t* GetMainGroup() { return UIInputManager::GetMainGroup(); }
};