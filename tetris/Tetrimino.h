#ifndef TETRIMINO_H
#define TETRIMINO_H

#include "TetriminoDefinitions.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

// Forward declaration to avoid circular dependency
namespace Tetris {
enum class TetriminoType;

class Board;

class Tetrimino {
public:
    Tetrimino(Tetris::TetriminoType type);

    // Core game loop methods
    void update(Board& board);
    void draw(int startX, int startY, int blockSize);

    // Getters for collision checks
    int getX() const { return gridX; }
    int getY() const { return gridY; }
    int getRotation() const { return rotation; }
    Tetris::TetriminoType getType() const { return type; }

private:
    // Properties
    Tetris::TetriminoType type;
    int rotation; // 0, 1, 2, 3
    int gridX, gridY;

    // Movement Timers (DAS/ARR handling)
    int dasTimer;
    int arrTimer;
    int lockTimer; // Time before piece locks after touching ground

    // Movement Settings (Constants)
    static constexpr int DAS_DELAY = 10; // Frames before auto-repeat starts
    static constexpr int ARR_DELAY = 2; // Frames between auto-repeats
    static constexpr int LOCK_DELAY = 30; // Frames before locking

    // Helpers
    enum Direction { LEFT = -1,
        RIGHT = 1 };
    bool rotate(int direction); // 1 = CW, -1 = CCW
    void hardDrop();

public:
    bool tryMove(int dx, int dy);
    bool collision(int testX, int testY);
};

}
#endif
