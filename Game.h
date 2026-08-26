#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "UI.h"
#include "tetris/Board.h"
#include "tetris/Stat.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
/**
 * @brief Main class that runs the whole game.
 * @details All game procedures must be processed through this class.
 */

class Game {
public:
    void execute();

public:
    Game(bool testMode = false);
    ~Game();
    enum class STATE {
        START, // -> LEVEL
        LEVEL, // -> PAUSE, END
        PAUSE, // -> LEVEL
        END
    };
    void game_init();
    bool game_update();
    void game_draw();
    /**
     * @brief Tear down a finished run and build a fresh one.
     * @details Rebuilds the board, statistics and field entities, and silences
     * the run's audio, so the game can return to the menu and be played again
     * without restarting the process.
     */
    void reset_level();

private:
    /**
     * @brief States of the game process in game_update.
     * @see Game::game_update()
     */
    STATE state = STATE::START;
    ALLEGRO_EVENT event;
    ALLEGRO_BITMAP* game_icon = nullptr;
    ALLEGRO_BITMAP* background = nullptr;
    /**
     * @brief Audio for the current run.
     * @details Held so a run that ends can stop and release its own instances;
     * these are cleared to null once handed back to SoundCenter for reaping.
     */
    ALLEGRO_SAMPLE_INSTANCE* menu_bgm = nullptr;
    ALLEGRO_SAMPLE_INSTANCE* level_bgm = nullptr;
    bool menu_bgm_playing = false;
    bool level_bgm_playing = false;

private:
    /**
     * @brief Allegro objects used in the game.
     * @see Game::game_init()
     */
    ALLEGRO_DISPLAY* display = nullptr;
    ALLEGRO_TIMER* timer = nullptr;
    ALLEGRO_EVENT_QUEUE* event_queue = nullptr;
    UI* ui = nullptr;
    Tetris::Board* board = nullptr;
    Tetris::Stat* stat = nullptr;
    Button* startButton = nullptr;
};

#endif
