#ifndef PEA_H
#define PEA_H

#include "../shapes/Rectangle.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/bitmap.h>
class Pea {
public:
    Pea(int damage);
    ~Pea();
    void init();
    bool update();
    void draw();

    bool collide_with_zombie();

private:
    // Pea properties

    int x1, y1, x2, y2;
    int w, h;
    ALLEGRO_BITMAP* pea_bitmap;
    int flyspeed;
    int flytimer;
    int damage;
    Rectangle* hitbox;
};

#endif
