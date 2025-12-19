#include "Pea.h"
#include "../Utils.h"
#include "../data/DataCenter.h"
#include <allegro5/allegro_primitives.h>
#include <vector>

Pea::Pea(int damage)
    : damage(damage)
{
    init();
}

void Pea::init()
{
    // Initialize pea properties
    DataCenter* DC = DataCenter::get_instance();
    x1 = Tetris::Board::BOARD_OFFSET_X + Tetris::Board::BLOCK_SIZE * 10 + 300;
    y1 = DC->window_height / 2.0;
    w = 60;
    h = 60;
    x2 = x1 + w;
    y2 = y1 + h;
    flyspeed = 5;
    flytimer = 0;
    // debug_log("Loading pea bitmap...\n");
    pea_bitmap = al_load_bitmap("assets/gifs/ProjectilePea.png");
    GAME_ASSERT(pea_bitmap != nullptr, "Failed to load pea bitmap.");
    hitbox = new Rectangle(x1, y1, x2, y2);
    GAME_ASSERT(hitbox != nullptr, "Failed to create pea hitbox.");
}

bool Pea::update()
{
    // Update pea position
    x1 += flyspeed;
    x2 = x1 + w;
    hitbox->update_center_x((x1 + x2) / 2.0);
    DataCenter* DC = DataCenter::get_instance();
    // Remove pea if it goes out of bounds
    if (x1 > DC->window_width) {
        return false;
    } else if (collide_with_zombie()) {
        // debug_log("Collided with zombie.\n");
        return false;
    }
    return true;
}

bool Pea::collide_with_zombie()
{
    DataCenter* DC = DataCenter::get_instance();
    std::vector<Zombie*> out_zombies;
    DC->get_zombies(out_zombies);
    for (auto& zombie : out_zombies) {
        if (hitbox->overlap(*(zombie->get_hitbox()))) {
            zombie->take_damage(damage);
            return true;
        }
    }
    return false;
}

void Pea::draw()
{
    // Draw pea
    al_draw_scaled_bitmap(pea_bitmap, 0, 0, al_get_bitmap_width(pea_bitmap),
        al_get_bitmap_height(pea_bitmap), x1, y1, w, h, 0);
    // al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(0, 255, 0), 2);
    // al_draw_rectangle(hitbox->x1, hitbox->y1, hitbox->x2, hitbox->y2, al_map_rgb(255, 0, 0), 2);
}

Pea::~Pea()
{
    if (pea_bitmap)
        al_destroy_bitmap(pea_bitmap);
    if (hitbox)
        delete hitbox;
}
