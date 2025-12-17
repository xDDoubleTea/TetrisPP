#include "Tetrimino.h"
#include "../Utils.h"
#include "../data/DataCenter.h"
#include "Board.h"
#include "TetriminoDefinitions.h"
#include <cmath>

using namespace Tetris;

Tetrimino::Tetrimino(TetriminoType t)
    : type(t)
    , rotation(0)
    , gridX(4)
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
        tryMove(dx, 0);
    }

    // --- Rotation ---
    if (DC->key_state[ALLEGRO_KEY_UP] && !DC->prev_key_state[ALLEGRO_KEY_UP]) {
        rotate(1); // Clockwise
    }
    if (DC->key_state[ALLEGRO_KEY_Z] && !DC->prev_key_state[ALLEGRO_KEY_Z]) {
        rotate(-1); // Counter-Clockwise
    }
    if (DC->key_state[ALLEGRO_KEY_A] && !DC->prev_key_state[ALLEGRO_KEY_A]) {
        rotate(2); // 180-degree
    }

    // --- Soft Drop ---
    if (DC->key_state[ALLEGRO_KEY_DOWN]) {
        if (tryMove(0, 1)) {
            lockTimer = 0; // Reset lock timer if we moved down
        }
    }

    // --- Hard Drop ---
    if (DC->key_state[ALLEGRO_KEY_SPACE] && !DC->prev_key_state[ALLEGRO_KEY_SPACE]) {
        hardDrop();
    }
}

bool Tetrimino::tryMove(int dx, int dy)
{
    DataCenter* DC = DataCenter::get_instance();
    if (!DC->board->checkCollision(type, rotation, gridX + dx, gridY + dy)) {
        gridX += dx;
        gridY += dy;
        return true;
    }
    return false;
}

bool Tetrimino::rotate(int direction)
{
    // TODO: Implement SRS Wall Kicks
    int oldRot = rotation;
    int newRot = (rotation + direction + 4) % 4; // Wrap around 0-3

    DataCenter* DC = DataCenter::get_instance();
    // Basic Rotation

    // Wall Kicks (SRS)

    int tableIdx = (type == TetriminoType::I) ? 1 : 0;
    for (int i = 0; i < 5; ++i) {
        int temp_check_coord_x = gridX;
        int temp_check_coord_y = gridY;
        temp_check_coord_x += srs_offsets[tableIdx][oldRot][i].x - srs_offsets[tableIdx][newRot][i].x;
        temp_check_coord_y -= srs_offsets[tableIdx][oldRot][i].y - srs_offsets[tableIdx][newRot][i].y;
        if (!DC->board->checkCollision(type, newRot, temp_check_coord_x, temp_check_coord_y)) {
            gridX = temp_check_coord_x;
            gridY = temp_check_coord_y;
            rotation = newRot;
            return true;
        }
    }
    return false;
}

void Tetrimino::hardDrop()
{
    DataCenter* DC = DataCenter::get_instance();
    while (tryMove(0, 1))
        ;
    DC->board->lockPiece(*this);
}

bool Tetrimino::collision(int testX, int testY)
{
    DataCenter* DC = DataCenter::get_instance();
    return DC->board->checkCollision(type, rotation, testX, testY);
}

void Tetrimino::draw(int startX, int startY, int blockSize)
{
    // 1. Draw Ghost Block
    int ghostY = gridY;
    // Calculate ghost position
    while (!collision(gridX, ghostY + 1))
        ghostY++;
    // Draw ghost at (gridX, ghostY) using ghost colors
    int typeIdx = static_cast<int>(type);
    ColorRGB c = tetrimino_ghost_block_colors[typeIdx];
    ALLEGRO_COLOR ghostColor = al_map_rgba(c.r, c.g, c.b, 50); // Semi-transparent
    for (const auto& block : tetrimino_shapes[typeIdx][rotation]) {
        int drawX = startX + (gridX + block.x) * blockSize;
        int drawY = startY + (ghostY + block.y) * blockSize;
        al_draw_filled_rectangle(drawX, drawY,
            drawX + blockSize, drawY + blockSize,
            ghostColor);
        al_draw_rectangle(drawX, drawY,
            drawX + blockSize, drawY + blockSize,
            al_map_rgba(0, 0, 0, 100), 1);
    }

    // 2. Draw Actual Block
    typeIdx = static_cast<int>(type);
    c = tetrimino_colors[typeIdx];
    ALLEGRO_COLOR color = al_map_rgb(c.r, c.g, c.b);

    for (const auto& block : tetrimino_shapes[typeIdx][rotation]) {
        int drawX = startX + (gridX + block.x) * blockSize;
        int drawY = startY + (gridY + block.y) * blockSize;

        al_draw_filled_rectangle(drawX, drawY,
            drawX + blockSize, drawY + blockSize,
            color);
        al_draw_rectangle(drawX, drawY,
            drawX + blockSize, drawY + blockSize,
            al_map_rgb(0, 0, 0), 2);
    }
}
