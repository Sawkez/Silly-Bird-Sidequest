#pragma once

#include "lvgl/lvgl.h"
#include "engine/ui/Menu.hpp"
#include "engine/ui/UIManager.hpp"

class TitleScreenMenu : public Menu {

    lv_obj_t* _buttons;
    static inline const char* _buttonMap[] = {"New game", "Load game", "\n", "Settings", "\n", "Quit", ""};

    public:
    TitleScreenMenu() : Menu() {
        lv_obj_set_style_bg_opa(_screen, LV_OPA_TRANSP, LV_PART_MAIN);

        _buttons = lv_buttonmatrix_create(_screen);
        lv_buttonmatrix_set_map(_buttons, _buttonMap);
        lv_obj_align(_buttons, LV_ALIGN_LEFT_MID, 0, 0);


    }
};