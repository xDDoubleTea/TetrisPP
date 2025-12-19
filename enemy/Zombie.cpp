#include "Zombie.h"
#include "../Utils.h"
#include "../algif5/algif.h"
#include "../data/DataCenter.h"
#include "../data/FontCenter.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/bitmap.h>
#include <string>
Zombie::Zombie()
    : attacktimer(0)
    , attackdamage(1)
    , hp(5)
    , stop_walk(false)
    , walktimer(0)
{
    Zombie::init();
};

void Zombie::init()
{
    FontCenter* FC = FontCenter::get_instance();
    DataCenter* DC = DataCenter::get_instance();
    font = FC->courier_new[FontSize::MEDIUM];
    attackspeed = DC->FPS * 3; // Attack every 2 seconds
    walk_animation = algif_load_animation("assets/gifs/plants-vs-zombies-zombie.gif");
    attack_animation = algif_load_animation("assets/gifs/pvz-zombieatt.gif");
    w = 100;
    h = 200;
    x1 = DC->window_width - 100;
    x2 = DC->window_width + w;
    y1 = DC->window_height / 2.0 - h / 2.0;
    y2 = DC->window_height / 2.0 + h / 2.0;
    hitbox = new Rectangle(x1, y1, x2, y2);
    state = STATE::WALKING;
    walkspeed = 10;
}

bool Zombie::update()
{
    if (state == STATE::WALKING && !stop_walk) {
        walktimer++;
        if (walktimer % 5 == 0) {
            x1 -= walkspeed;
            x2 -= walkspeed;
        }
    }
    DataCenter* DC = DataCenter::get_instance();
    if (hitbox->overlap(*(DC->peaShooter->get_hitbox()))) {
        state = STATE::ATTACKING;
    }
    if (state == STATE::ATTACKING && !stop_walk) {
        stop_walk = true;
    }
    if (state == STATE::ATTACKING) {
        attacktimer++;
        if (attacktimer >= attackspeed) {
            attacktimer = 0;
            // Deal damage to PeaShooter
            DataCenter* DC = DataCenter::get_instance();
            DC->peaShooter->recieve_attack(attackdamage);
        }
    }

    hitbox->update_center_x((x1 + x2) / 2.0);
    return true;
}

void Zombie::draw()
{
    if (state == STATE::WALKING) {
        ALLEGRO_BITMAP* frame = algif_get_bitmap(walk_animation, al_get_time());
        if (frame == NULL)
            return;
        al_draw_scaled_bitmap(frame, 0, 0, al_get_bitmap_width(frame), al_get_bitmap_height(frame), x1,
            y1, x2 - x1, y2 - y1, 0);
        // Display HP

    } else if (state == STATE::ATTACKING) {
        ALLEGRO_BITMAP* frame = algif_get_bitmap(attack_animation, al_get_time());
        if (frame == NULL)
            return;
        al_draw_scaled_bitmap(frame, 0, 0, al_get_bitmap_width(frame), al_get_bitmap_height(frame), x1,
            y1, x2 - x1, y2 - y1, 0);
    }
    al_draw_text(font, al_map_rgb(255, 0, 0), (x1 + x2) / 2.0, y1 - 20, ALLEGRO_ALIGN_CENTRE,
        ("HP: " + std::to_string(hp)).c_str());
    // Draw hitbox for debugging
    // hitbox->draw(x1, y1, x2, y2, ALLEGRO_COLOR{1, 0, 0, 0.5});
    // al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(0, 255, 0), 2);
    // al_draw_rectangle(hitbox->x1, hitbox->y1, hitbox->x2, hitbox->y2, al_map_rgb(255, 0, 0), 2);
}

Zombie::~Zombie()
{
    algif_destroy_animation(walk_animation);
    algif_destroy_animation(attack_animation);
    delete hitbox;
}
