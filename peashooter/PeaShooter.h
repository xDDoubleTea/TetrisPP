#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include "../algif5/algif.h"
#include "../shapes/Rectangle.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

class PeaShooter {
public:
    PeaShooter();
    ~PeaShooter();
    void init();
    bool update();
    void draw();
    void set_state_shooting() { state = STATE::SHOOTING; }

    void recieve_attack(int damage);
    Rectangle* get_hitbox() const { return hitbox; }

private:
    // PeaShooter properties
    int x1, y1, x2, y2;
    int w, h;
    enum class STATE {
        IDLE,
        SHOOTING,
    };
    STATE state;
    ALGIF_ANIMATION* idle_animation;
    ALGIF_ANIMATION* shoot_animation;

    int recieve_timer;
    int recieved_damage;
    Rectangle* hitbox;
};

#endif
