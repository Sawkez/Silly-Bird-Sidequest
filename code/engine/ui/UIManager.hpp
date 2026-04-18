#pragma once

#include <SDL.h>

#include <queue>
#include <vector>

#include "3rdparty/lvgl/lvgl.h"
#include "engine/input/UIInputManager.hpp"
#include "engine/ui/Menu.hpp"
#include "game/ui/Styles.hpp"

class UIManager {
   public:
	enum MenuID { MENU_TEST, MENU_PAUSE, MENU_TITLE, MENU_MODS, _MENU_COUNT };

   private:
	UIManager() = delete;
	static inline lv_display_t* _display = NULL;
	static inline std::vector<uint8_t> _buf;

	static inline SDL_Texture* _texture = NULL;
	static inline SDL_Renderer* _renderer;

	static inline std::vector<Menu*> _stack;
	static inline int _stackTop = -1;

	static Menu* _menus[_MENU_COUNT];

	static void FlushCallback(lv_display_t* display, const lv_area_t* area, uint8_t* pixelData) {
		void* outPixels;
		int pitch;

		int w = (area->x2 - area->x1) + 1;
		int h = (area->y2 - area->y1) + 1;

		SDL_Rect rect = {area->x1, area->y1, w, h};

		size_t bytesPerPixel = LV_COLOR_DEPTH / 8;
		size_t bytesPerRow = w * bytesPerPixel;

		SDL_UpdateTexture(_texture, &rect, pixelData, bytesPerRow);
		lv_display_flush_ready(display);
	}

	static SDL_Point GetWindowSize(SDL_Window* window) {
		int w, h;
		SDL_GetWindowSizeInPixels(window, &w, &h);
		return {w, h};
	}

   public:
	static void Init(SDL_Renderer* renderer, SDL_Window* window) { Init(renderer, GetWindowSize(window)); }

	static void Init(SDL_Renderer* renderer, SDL_Point windowSize) {
		_display = InitLVGL(windowSize);
		UIInputManager::Init();
		Styles::Init();

		for (int i = 0; i < _MENU_COUNT; i++) {
			_menus[i]->Init();
		}

		_renderer = renderer;

#if __PSP__
		_buf.resize(windowSize.x * windowSize.y * 4);  // use fullscreen buffer on PSP to save 4 fps during animations
#endif

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

	static void Process() { lv_timer_handler(); }

	static void PushAsync(void* data) {
		auto menu = (Menu*)data;

		if (_stackTop >= 0) _stack[_stackTop]->Deactivate();

		_stack.push_back(menu);
		_stackTop++;
		menu->Activate();
	}

	static void PopAsync(void* data) {
		_stack[_stackTop]->Deactivate();
		_stackTop--;

		if (_stackTop >= 0) _stack[_stackTop]->Activate();

		_stack.pop_back();
	}

	static void ClearStackAsync(void* data) {
		while (_stackTop >= 0) {
			_stack[_stackTop]->Deactivate();
			_stack.pop_back();
			_stackTop--;
		}
	}

	static void ClearStackAndPushAsync(void* data) {
		ClearStackAsync(data);
		PushAsync(data);
	}

	static void PopAndPushAsync(void* data) {
		PopAsync(data);
		PushAsync(data);
	}

	static void Resize(int windowWidth, int windowHeight) {
#if !__PSP__
		_buf.resize(windowWidth * windowHeight * 2);
#endif

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
		if (_stackTop < 0) return false;

		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_EVENT_WINDOW_RESIZED) {
			Resize(event.window.data1, event.window.data2);
			return true;
		}

		return (UIInputManager::HandleEvent(event));
	}

	static void Draw() {
		if (_stackTop < 0) return;
		SDL_RenderTexture(_renderer, _texture, NULL, NULL);
	}

	static void Push(MenuID menu) { Push(_menus[menu]); }
	static void Push(Menu* menu) { lv_async_call(PushAsync, menu); }
	static void Pop() { lv_async_call(PopAsync, NULL); }
	static void ClearStack() { lv_async_call(ClearStackAsync, nullptr); }
	static void ClearStackAndPush(MenuID menu) { ClearStackAndPush(_menus[menu]); }
	static void ClearStackAndPush(Menu* menu) { lv_async_call(ClearStackAndPushAsync, menu); }
	static void PopAndPush(MenuID menu) { PopAndPush(_menus[menu]); }
	static void PopAndPush(Menu* menu) { lv_async_call(PopAndPushAsync, menu); }

	static lv_group_t* GetMainGroup() { return UIInputManager::GetMainGroup(); }
};