#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include "./shapes/Point.h"
#include "Button.h"
#include <allegro5/bitmap.h>
#include <map>
#include <tuple>

class UI {
public:
    UI() { }
    void init();
    void update();
    void draw();
    void drawStartScreen();

    void updateStartScreen();
    void addButton(const std::string& key, Button* button)
    {
        button_map[key] = button;
    }

private:
    std::map<std::string, Button*> button_map;
};

#endif
