#ifndef BOARD_H
#define BOARD_H

#include "../shapes/Shape.h"
#include "TetriminoDefinitions.h"
#include <queue>

namespace Tetris {
class Tetrimino; // Forward declaration
class Board {
public:
    constexpr static int GRID_W = 10;
    constexpr static int GRID_H = 20;
    constexpr static int BLOCK_SIZE = 30; // Pixel size
    constexpr static int BOARD_OFFSET_X = 200;
    constexpr static int BOARD_OFFSET_Y = 50;
    constexpr static int GRAVITY_SPEED = 60; // Frames per drop
    constexpr static int HOLD_PIECE_OFFSET_X = BOARD_OFFSET_X - 50;
    constexpr static int HOLD_PIECE_OFFSET_Y = BOARD_OFFSET_Y;
    constexpr static int NEXT_QUEUE_OFFSET_X = BOARD_OFFSET_X + GRID_W * BLOCK_SIZE + 50;
    constexpr static int NEXT_QUEUE_OFFSET_Y = BOARD_OFFSET_Y;
    constexpr static int NEXT_QUEUE_SPACING = 120;
    constexpr static int NEXT_QUEUE_COUNT = 5;

public:
    Board();
    ~Board();
    void init();
    bool update();
    void draw();

    // Game Logic
    bool checkCollision(Tetris::TetriminoType type, int rotation, int x, int y);
    bool lockPiece(const Tetrimino& t);

    // Getters for Tetrimino ghost calculations
    bool isOccupied(int x, int y) const;
    void drawHoldPiece(TetriminoType type);

    // Gravity
    void updateGravityTimer(bool rotated, bool moved);

    // Next Queue
    void drawNextQueue(int count);

    void drawGarbageQueue();
    // Draw decorations
    void drawDecorations();

    void drawLineClearTypes();
    void toggleDrawLineClearTypes() { drawLineClearTypes_enabled = !drawLineClearTypes_enabled; }
    void getLineClearTypeDrawEnabled(bool& out_enabled) const { out_enabled = drawLineClearTypes_enabled; }

    void setLastLineClearInfo(TetriminoType type, size_t lines, bool isSpin, bool isB2B)
    {
        lastClearedType = type;
        lastClearedLines = lines;
        lastClearWasSpin = isSpin;
        lastClearWasB2B = isB2B;
    }

    void sendAttack(size_t damage);

    void receiveGarbage(size_t damage) { garbageQueue += damage; }
    void addGarbageLines(size_t count);
    bool gameOver() const;

private:
    // The main grid: 0 = empty, 1-7 = colors/types
    int grid[20][10];

    // Active piece management
    Tetrimino* activePiece;
    Tetrimino* holdPiece;
    std::queue<Tetrimino*> nextQueue;

    // Gravity
    int gravityTimer;
    int gravitySpeed; // Frames per drop (lower is faster)

    bool spawnPiece();
    size_t clearLines();
    bool isPerfectClear();
    void generate7Bag(); // Fills nextQueue

    // Garbage
    size_t garbageQueue = 0;
    Shape* hitbox;

    bool drawLineClearTypes_enabled = false;

    // Decorations
    ALLEGRO_BITMAP* background;
    TetriminoType lastClearedType;
    size_t lastClearedLines;
    bool lastClearWasSpin;
    bool lastClearWasB2B;

    ColorRGB font_color = { 123, 255, 100 };
};
}

#endif
