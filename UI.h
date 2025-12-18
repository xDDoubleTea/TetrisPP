#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include "./shapes/Point.h"
#include <allegro5/bitmap.h>
#include <tuple>
#include <vector>

class UI {
public:
    UI() { }
    void init();
    void update();
    void draw();
    void drawStartScreen();

private:
};

#endif
