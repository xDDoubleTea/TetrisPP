#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "UI.h"
#include "tetris/Board.h"
#include "tetris/Stat.h"
#include <allegro5/allegro.h>
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

private:
    /**
     * @brief States of the game process in game_update.
     * @see Game::game_update()
     */
    STATE state;
    ALLEGRO_EVENT event;
    ALLEGRO_BITMAP* game_icon;
    ALLEGRO_BITMAP* background;

private:
    /**
     * @brief Allegro objects used in the game.
     * @see Game::game_init()
     */
    ALLEGRO_DISPLAY* display;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* event_queue;
    UI* ui;
    Tetris::Board* board;
    Tetris::Stat* stat;
    Button* startButton;
};

#endif
