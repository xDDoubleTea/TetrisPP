#ifndef ENEMY_H
#define ENEMY_H

#include "../algif5/algif.h"
#include "../shapes/Shape.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

namespace Enemy {

class Enemy {
public:
    Enemy();
    void init();
    void update();
    void draw();

private:
    // Enemy properties
    ALGIF_ANIMATION* animation;
    int x1, y1, x2, y2;
    int w, h;
    ALLEGRO_COLOR color;
    ALGIF_ANIMATION* walk_animation;
    int walkspeed;
    int hp;
    bool stop_walk;
    ALLEGRO_FONT* font;
    int walktimer;
    Shape* hitbox;
};
}

#endif
