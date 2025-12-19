#ifndef DATACENTER_H_INCLUDED
#define DATACENTER_H_INCLUDED

#include "../enemy/Zombie.h"
#include "../peashooter/Pea.h"
#include "../peashooter/PeaShooter.h"
#include "../shapes/Point.h"
#include "../tetris/Board.h"
#include "../tetris/Stat.h"
#include <allegro5/keycodes.h>
#include <allegro5/mouse.h>

class Player;
class Level;
namespace DataSettings {
constexpr static char btb1_sound_path[] = "./assets/sfx/btb1.ogg";
constexpr static char btb2_sound_path[] = "./assets/sfx/btb2.ogg";
constexpr static char btb3_sound_path[] = "./assets/sfx/btb3.ogg";
constexpr static char btb_breaak_sound_path[] = "./assets/sfx/btb_break.ogg";
constexpr static char clearbtb_sound_path[] = "./assets/sfx/clearbtb.ogg";
constexpr static char clearline_sound_path[] = "./assets/sfx/clearline.ogg";
constexpr static char clearquad_sound_path[] = "./assets/sfx/clearquad.ogg";
constexpr static char clearspin_sound_path[] = "./assets/sfx/clearspin.ogg";
constexpr static char combo_1_sound_path[] = "./assets/sfx/combo_1.ogg";
constexpr static char combo_2_sound_path[] = "./assets/sfx/combo_2.ogg";
constexpr static char combo_3_sound_path[] = "./assets/sfx/combo_3.ogg";
constexpr static char combo_4_sound_path[] = "./assets/sfx/combo_4.ogg";
constexpr static char combo_5_sound_path[] = "./assets/sfx/combo_5.ogg";
constexpr static char combo_6_sound_path[] = "./assets/sfx/combo_6.ogg";
constexpr static char combo_7_sound_path[] = "./assets/sfx/combo_7.ogg";
constexpr static char combo_8_sound_path[] = "./assets/sfx/combo_8.ogg";
constexpr static char combo_9_sound_path[] = "./assets/sfx/combo_9.ogg";
constexpr static char combo_10_sound_path[] = "./assets/sfx/combo_10.ogg";
constexpr static char combo_11_sound_path[] = "./assets/sfx/combo_11.ogg";
constexpr static char combo_12_sound_path[] = "./assets/sfx/combo_12.ogg";
constexpr static char combo_13_sound_path[] = "./assets/sfx/combo_13.ogg";
constexpr static char combo_14_sound_path[] = "./assets/sfx/combo_14.ogg";
constexpr static char combo_15_sound_path[] = "./assets/sfx/combo_15.ogg";
constexpr static char combo_16_sound_path[] = "./assets/sfx/combo_16.ogg";
constexpr static char combobreak_sound_path[] = "./assets/sfx/combobreak.ogg";
constexpr static char damage_small_sound_path[] = "./assets/sfx/damage_small.ogg";
constexpr static char garbage_in_small_sound_path[] = "./assets/sfx/garbage_in_small.ogg";
constexpr static char harddrop_sound_path[] = "./assets/sfx/harddrop.ogg";
constexpr static char rotate_sound_path[] = "./assets/sfx/rotate.ogg";
constexpr static char spin_sound_path[] = "./assets/sfx/spin.ogg";
constexpr static char spinend_sound_path[] = "./assets/sfx/spinend.ogg";
constexpr static char i_sound_path[] = "./assets/sfx/i.ogg";
constexpr static char j_sound_path[] = "./assets/sfx/j.ogg";
constexpr static char l_sound_path[] = "./assets/sfx/l.ogg";
constexpr static char o_sound_path[] = "./assets/sfx/o.ogg";
constexpr static char s_sound_path[] = "./assets/sfx/s.ogg";
constexpr static char t_sound_path[] = "./assets/sfx/t.ogg";
constexpr static char z_sound_path[] = "./assets/sfx/z.ogg";

}
/**
 * @brief Stores generic global data and relatively small data structures.
 * @details The globally used data such as FPS (frames per second), windows size, game region, and states of input devices (mouse and keyboard).
 * Player and Level are stored here mainly because the objects are small enough that do not require complex management.
 * Other specific data like game objects (towers, monsters ... etc) or ALLEGRO_BITMAP will be managed by other center objects.
 */
class DataCenter {
public:
    static DataCenter* get_instance()
    {
        static DataCenter DC;
        return &DC;
    }
    void add_zombie(Zombie* zombie)
    {
        zombies.push_back(zombie);
    }
    void add_pea(Pea* pea)
    {
        peas.push_back(pea);
    }

    void get_zombies(std::vector<Zombie*>& out_zombies) const
    {
        out_zombies = zombies;
    }
    void get_peas(std::vector<Pea*>& out_peas) const
    {
        out_peas = peas;
    }

    ~DataCenter();

public:
    double FPS;
    int window_width, window_height;
    /**
     * @brief The width and height of game area (not window size). That is, the region excludes menu region.
     * @details The game area is sticked to the top-left of the display window.
     */
    int game_field_length;
    /**
     * @brief Stores the keyboard state whether a key is being pressed.
     * @details The states will be updated once a key is pressed, asynchronously with frame update.
     * @see Game::execute()
     */
    bool key_state[ALLEGRO_KEY_MAX];
    /**
     * @brief Stores the keyboard states of the previous frame.
     * @details The states will be updated every frame.
     * @see Game::game_update()
     */
    bool prev_key_state[ALLEGRO_KEY_MAX];
    /**
     * @brief Stores the mouse position relative to the game window.
     * @details The state will be update once the mouse is moved.
     * @see Game::execute()
     */
    Point mouse;
    /**
     * @brief Stores the state of every mouse button.
     * @details The state will be update once a mouse button is clicked.
     * @see Game::execute()
     */
    bool mouse_state[ALLEGRO_MOUSE_MAX_EXTRA_AXES];
    /**
     * @brief Stores the state of every mouse button of the previous frame.
     * @details The state will be update every frame.
     * @see Game::game_update()
     */
    bool prev_mouse_state[ALLEGRO_MOUSE_MAX_EXTRA_AXES];

public:
    /**
     * @brief Stores the basic information that a player should have.
     * @details For a tower-defense game, coin and health point is enough to represent a player.
     * @see Player
     */
    Player* player;
    /**
     * @brief Loads and stores the information of a level.
     * @see Level
     */
    Level* level;
    Tetris::Board* board;
    Tetris::Stat* stat;
    PeaShooter* peaShooter;
    std::vector<Zombie*> zombies;
    std::vector<Pea*> peas;

private:
    DataCenter();
};

#endif
