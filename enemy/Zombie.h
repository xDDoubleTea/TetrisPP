#ifndef ENEMY_H
#define ENEMY_H

#include "../algif5/algif.h"
#include "../shapes/Rectangle.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

class Zombie {
public:
    Zombie();
    ~Zombie();
    void init();
    bool update();
    void draw();
    bool is_dead() const { return hp <= 0; }
    bool take_damage(int damage)
    {
        hp -= damage;
        if (hp <= 0)
            return true;
        return false;
    }
    Rectangle* get_hitbox() const { return hitbox; }
    void set_attacking() { state = STATE::ATTACKING; }
    void get_attacking() { stop_walk = true; }

private:
    // Enemy properties
    constexpr static int Y_OFFSET = 50;
    int x1, y1, x2, y2;
    int w, h;
    ALLEGRO_COLOR color;
    enum class STATE {
        WALKING,
        ATTACKING,
    };
    STATE state;
    ALGIF_ANIMATION* walk_animation;
    ALGIF_ANIMATION* attack_animation;
    int walkspeed;
    int attackspeed;
    int attacktimer;
    int attackdamage;
    int hp;
    bool stop_walk;
    ALLEGRO_FONT* font;
    int walktimer;
    Rectangle* hitbox;
};

#endif
