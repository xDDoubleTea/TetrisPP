#include "PeaShooter.h"
#include "../Utils.h"
#include "../data/DataCenter.h"
#include "../tetris/Board.h"

PeaShooter::PeaShooter()
{
    init();
}

void PeaShooter::init()
{
    DataCenter* DC = DataCenter::get_instance();
    x1 = Tetris::Board::BOARD_OFFSET_X + Tetris::Board::BLOCK_SIZE * 10 + 300;
    y1 = DC->window_height / 2.0;
    w = 200;
    h = 200;
    x2 = x1 + w;
    y2 = y1 + h;

    // debug_log("Loading PeaShooter idle animation...\n");
    idle_animation = algif_load_animation("assets/gifs/PeaShooter_Idle1.gif");
    // debug_log("PeaShooter idle animation loaded.\n");
    shoot_animation = algif_load_animation("assets/gifs/PeaShooter_Spitnew.gif");
    GAME_ASSERT(shoot_animation != nullptr, "Failed to load PeaShooter shooting animation.");
    // debug_log("PeaShooter animations loaded.\n");
    shoot_animation->loop = 1;
    state = STATE::IDLE;
    hitbox = new Rectangle(x1, y1, x2, y2);
}

void PeaShooter::recieve_attack(int damage)
{
    recieved_damage += damage;
    // debug_log("PeaShooter received %d damage (no effect).\n", damage);
}

bool PeaShooter::update()
{
    if (state == STATE::SHOOTING) {
        ALLEGRO_BITMAP* frame = algif_get_bitmap(shoot_animation, al_get_time());
        if (frame == NULL) {
            // Shooting animation ended
            state = STATE::IDLE;
            shoot_animation->start_time = 0;
        }
    }
    if (recieved_damage > 0) {
        recieve_timer++;
        if (recieve_timer >= 60) {
            DataCenter* DC = DataCenter::get_instance();
            DC->board->receiveGarbage(recieved_damage);
            recieved_damage = 0;
            recieve_timer = 0;
        }
    }
    return true;
}

void PeaShooter::draw()
{
    if (state == STATE::IDLE) {
        ALLEGRO_BITMAP* frame = algif_get_bitmap(idle_animation, al_get_time());
        if (frame == NULL)
            return;
        al_draw_scaled_bitmap(frame, 0, 0, al_get_bitmap_width(frame), al_get_bitmap_height(frame), x1,
            y1, x2 - x1, y2 - y1, 0);
    } else if (state == STATE::SHOOTING) {
        ALLEGRO_BITMAP* frame = algif_get_bitmap(shoot_animation, al_get_time());
        if (frame == NULL) {
            state = STATE::IDLE;
            return;
        }
        al_draw_scaled_bitmap(frame, 0, 0, al_get_bitmap_width(frame), al_get_bitmap_height(frame), x1,
            y1, x2 - x1, y2 - y1, 0);
    }
}
PeaShooter::~PeaShooter()
{
    if (idle_animation)
        algif_destroy_animation(idle_animation);
    if (shoot_animation)
        algif_destroy_animation(shoot_animation);
    if (hitbox)
        delete hitbox;
}
