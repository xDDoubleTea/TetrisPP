#include "Tetrimino.h"
#include "../data/DataCenter.h"
#include "Board.h"
#include <cmath>

using namespace Tetris;

Tetrimino::Tetrimino(TetriminoType t)
    : type(t)
    , rotation(0)
    , gridX(3)
    , gridY(0)
    , dasTimer(0)
    , arrTimer(0)
    , lockTimer(0)
{
    // Spawn logic (usually center top)
    // Adjust Y if you want it to spawn hidden above the board
}

void Tetrimino::update(Board& board)
{
    DataCenter* DC = DataCenter::get_instance();

    // --- Horizontal Movement (DAS/ARR) ---
    int dx = 0;
    if (DC->key_state[ALLEGRO_KEY_LEFT]) {
        if (dasTimer == 0 || dasTimer >= DAS_DELAY) {
            if (arrTimer == 0) {
                dx = -1;
                arrTimer = ARR_DELAY;
            } else {
                arrTimer--;
            }
        }
        dasTimer++;
    } else if (DC->key_state[ALLEGRO_KEY_RIGHT]) {
        if (dasTimer == 0 || dasTimer >= DAS_DELAY) {
            if (arrTimer == 0) {
                dx = 1;
                arrTimer = ARR_DELAY;
            } else {
                arrTimer--;
            }
        }
        dasTimer++;
    } else {
        dasTimer = 0;
        arrTimer = 0;
    }

    if (dx != 0) {
        tryMove(board, dx, 0);
    }

    // --- Rotation ---
    // Note: You need a way to detect "Key Pressed" (single frame) vs "Key Down"
    // Using previous key state from DataCenter
    if (DC->key_state[ALLEGRO_KEY_UP] && !DC->prev_key_state[ALLEGRO_KEY_UP]) {
        rotate(board, 1); // Clockwise
    }
    if (DC->key_state[ALLEGRO_KEY_Z] && !DC->prev_key_state[ALLEGRO_KEY_Z]) {
        rotate(board, -1); // Counter-Clockwise
    }

    // --- Soft Drop ---
    if (DC->key_state[ALLEGRO_KEY_DOWN]) {
        if (tryMove(board, 0, 1)) {
            lockTimer = 0; // Reset lock timer if we moved down
        }
    }

    // --- Hard Drop ---
    if (DC->key_state[ALLEGRO_KEY_SPACE] && !DC->prev_key_state[ALLEGRO_KEY_SPACE]) {
        hardDrop(board);
    }
}

bool Tetrimino::tryMove(Board& board, int dx, int dy)
{
    if (!board.checkCollision(type, rotation, gridX + dx, gridY + dy)) {
        gridX += dx;
        gridY += dy;
        return true;
    }
    return false;
}

void Tetrimino::rotate(Board& board, int direction)
{
    // TODO: Implement SRS Wall Kicks
    int oldRot = rotation;
    int newRot = (rotation + direction + 4) % 4; // Wrap around 0-3

    // 1. Basic Rotation
    if (!board.checkCollision(type, newRot, gridX, gridY)) {
        rotation = newRot;
        return;
    }

    // 2. Wall Kicks (SRS)
    // This assumes you implemented the lookup in TetriminoDefinitions.h
    // Iterate through the 5 tests for this rotation transition
    int tableIdx = (type == TetriminoType::I) ? 1 : 0;

    // Logic to find correct kick offset from definitions...
    // For simplicity here, pseudo-code:
    // for (offset : getKickOffsets(tableIdx, oldRot, direction)) {
    //    if (!board.checkCollision(type, newRot, gridX + offset.x, gridY + offset.y)) {
    //        gridX += offset.x;
    //        gridY += offset.y;
    //        rotation = newRot;
    //        return;
    //    }
    // }
}

void Tetrimino::hardDrop(Board& board)
{
    while (tryMove(board, 0, 1))
        ;
    board.lockPiece(*this);
}

void Tetrimino::draw(int startX, int startY, int blockSize)
{
    // 1. Draw Ghost Block
    int ghostY = gridY;
    // Calculate ghost position
    // while (!collision(x, ghostY + 1)) ghostY++;
    // Draw ghost at (gridX, ghostY) using ghost colors

    // 2. Draw Actual Block
    int typeIdx = static_cast<int>(type);
    ColorRGB c = tetrimino_colors[typeIdx];
    ALLEGRO_COLOR color = al_map_rgb(c.r, c.g, c.b);

    for (const auto& block : tetrimino_shapes[typeIdx][rotation]) {
        int drawX = startX + (gridX + block.x) * blockSize;
        int drawY = startY + (gridY + block.y) * blockSize;

        al_draw_filled_rectangle(drawX, drawY,
            drawX + blockSize, drawY + blockSize,
            color);
        // Optional: Draw outline
        al_draw_rectangle(drawX, drawY,
            drawX + blockSize, drawY + blockSize,
            al_map_rgb(0, 0, 0), 2);
    }
}
