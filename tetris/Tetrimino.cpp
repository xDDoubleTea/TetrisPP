#include "Tetrimino.h"
#include "../Utils.h"
#include "../data/DataCenter.h"
#include "../data/SoundCenter.h"
#include "Board.h"
#include "TetriminoDefinitions.h"
#include <allegro5/allegro_audio.h>
// #include <cmath>

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

bool Tetrimino::update(Board& board)
{
    DataCenter* DC = DataCenter::get_instance();

    // --- Horizontal Movement (DAS/ARR) ---
    int dx = 0;
    if (DC->key_state[ALLEGRO_KEY_LEFT]) {
        playMovingSound(type);
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
        playMovingSound(type);
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
    bool moved = false;
    if (dx != 0) {
        moved = tryMove(dx, 0);
    }

    // --- Rotation ---
    bool rotated = false;
    if (DC->key_state[ALLEGRO_KEY_UP] && !DC->prev_key_state[ALLEGRO_KEY_UP]) {
        rotated = rotate(1); // Clockwise
    }
    if (DC->key_state[ALLEGRO_KEY_Z] && !DC->prev_key_state[ALLEGRO_KEY_Z]) {
        rotated = rotate(-1); // Counter-Clockwise
    }
    if (DC->key_state[ALLEGRO_KEY_A] && !DC->prev_key_state[ALLEGRO_KEY_A]) {
        rotated = rotate(2); // 180-degree
    }
    lockTimer = rotated ? 0 : lockTimer;
    // --- Soft Drop ---
    if (DC->key_state[ALLEGRO_KEY_DOWN]) {
        if ((moved |= tryMove(0, 1))) {
            lockTimer = 0; // Reset lock timer if we moved down
        }
    }

    // --- Hard Drop ---
    bool lockPiece = true;
    if (DC->key_state[ALLEGRO_KEY_SPACE] && !DC->prev_key_state[ALLEGRO_KEY_SPACE]) {
        hardDrop();
        lockPiece = DC->board->lockPiece(*this);
    }
    DC->board->updateGravityTimer(rotated, moved);
    if (rotated) {
        AllSpin = isAllSpin();
        TSpin = isTSpin();
        SoundCenter* SC = SoundCenter::get_instance();
        if (TSpin || AllSpin)
            SC->play(DataSettings::spin_sound_path, ALLEGRO_PLAYMODE_ONCE);
    } else if (moved) {
        AllSpin = false;
        TSpin = false;
    }
    return lockPiece;
}

void Tetrimino::playMovingSound(TetriminoType type)
{
    SoundCenter* SC = SoundCenter::get_instance();
    switch (type) {
    case TetriminoType::I:
        if (!SC->is_playing(DataSettings::i_sound_path))
            SC->play(DataSettings::i_sound_path, ALLEGRO_PLAYMODE_ONCE);
        break;
    case TetriminoType::J:
        if (!SC->is_playing(DataSettings::j_sound_path))
            SC->play(DataSettings::j_sound_path, ALLEGRO_PLAYMODE_ONCE);
        break;
    case TetriminoType::L:
        if (!SC->is_playing(DataSettings::l_sound_path))
            SC->play(DataSettings::l_sound_path, ALLEGRO_PLAYMODE_ONCE);
        break;
    case TetriminoType::O:
        if (!SC->is_playing(DataSettings::o_sound_path))
            SC->play(DataSettings::o_sound_path, ALLEGRO_PLAYMODE_ONCE);
        break;
    case TetriminoType::S:
        if (!SC->is_playing(DataSettings::s_sound_path))
            SC->play(DataSettings::s_sound_path, ALLEGRO_PLAYMODE_ONCE);
        break;
    case TetriminoType::T:
        if (!SC->is_playing(DataSettings::t_sound_path))
            SC->play(DataSettings::t_sound_path, ALLEGRO_PLAYMODE_ONCE);
        break;
    case TetriminoType::Z:
        if (!SC->is_playing(DataSettings::z_sound_path))
            SC->play(DataSettings::z_sound_path, ALLEGRO_PLAYMODE_ONCE);
        break;
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
bool Tetrimino::tryDryMove(int dx, int dy)
{
    DataCenter* DC = DataCenter::get_instance();
    return !DC->board->checkCollision(type, rotation, gridX + dx, gridY + dy);
}

bool Tetrimino::rotate(int direction)
{
    // TODO: Implement SRS Wall Kicks
    int oldRot = rotation;
    int newRot = (rotation + direction + 4) % 4; // Wrap around 0-3

    DataCenter* DC = DataCenter::get_instance();
    SoundCenter* SC = SoundCenter::get_instance();
    // Basic Rotation

    // Wall Kicks (SRS)

    SC->play(DataSettings::rotate_sound_path, ALLEGRO_PLAYMODE_ONCE);
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

bool Tetrimino::isTSpin()
{
    return type == TetriminoType::T && isAllSpin();
}

bool Tetrimino::isAllSpin()
{
    int occupied_corners = 0;
    // Try moving the piece up, left, right to see if it's blocked on 3 sides
    const std::vector<Point> check_offsets = {
        Point { 0, -1 }, // Up
        Point { -1, 0 }, // Left
        Point { 1, 0 } // Right
    };
    for (const auto& offset : check_offsets) {
        if (!tryDryMove(offset.x, offset.y))
            occupied_corners++;
    }
    // debug_log("All-Spin check: %d corners occupied.\n", occupied_corners);
    return occupied_corners >= 3;
}

void Tetrimino::hardDrop()
{
    while (tryMove(0, 1))
        ;
    SoundCenter* SC = SoundCenter::get_instance();
    SC->play(DataSettings::harddrop_sound_path, ALLEGRO_PLAYMODE_ONCE);
}

bool Tetrimino::collision(int testX, int testY)
{
    DataCenter* DC = DataCenter::get_instance();
    return DC->board->checkCollision(type, rotation, testX, testY);
}

size_t Tetrimino::damageDealt(size_t linesCleared, bool isPerfectClear, bool isB2B, bool isTSpin, bool isAllSpin, size_t combo) const
{
    SoundCenter* SC = SoundCenter::get_instance();
    size_t damage = 0;
    if (!linesCleared)
        return 0;
    if (isPerfectClear) {
        damage += 10; // Perfect Clear bonus
    }
    if (isTSpin) {
        SC->play(DataSettings::clearspin_sound_path, ALLEGRO_PLAYMODE_ONCE);
        switch (linesCleared) {
        case 1:
            damage += 2;
            break;
        case 2:
            damage += 4;
            break;
        case 3:
            damage += 6;
            break;
        default:
            break;
        }
    } else if (isAllSpin) {
        SC->play(DataSettings::clearspin_sound_path, ALLEGRO_PLAYMODE_ONCE);
        switch (linesCleared) {
        case 1:
            damage += 1;
            break;
        case 2:
            damage += 2;
            break;
        case 3:
            damage += 4;
            break;
        default:
            break;
        }
    } else {
        switch (linesCleared) {
        case 1:
            SC->play(DataSettings::clearline_sound_path, ALLEGRO_PLAYMODE_ONCE);
            damage += 0;
            break;
        case 2:
            SC->play(DataSettings::clearline_sound_path, ALLEGRO_PLAYMODE_ONCE);
            damage += 1;
            break;
        case 3:
            damage += 2;
            SC->play(DataSettings::clearline_sound_path, ALLEGRO_PLAYMODE_ONCE);
            break;
        case 4:
            damage += 4;
            SC->play(DataSettings::clearquad_sound_path, ALLEGRO_PLAYMODE_ONCE);
            break;
        default:
            break;
        }
    }
    // Combo bonus
    if (linesCleared == 1 && combo >= 2) {
        damage += log(1 + 1.25 * combo);
    }
    damage += (linesCleared - 1) * (1 + combo / 4);
    DataCenter* DC = DataCenter::get_instance();

    bool out_enabled = false;
    DC->board->getLineClearTypeDrawEnabled(out_enabled);
    if (!out_enabled) {
        DC->board->toggleDrawLineClearTypes();
    }
    size_t b2b = DC->stat->getBackToBackCount();
    if (b2b == 0 || b2b == 1) {
        // No B2B bonus
    } else if (b2b > 1 && b2b <= 3) {
        damage += 1;
    } else {
        damage += 2;
    }
    DC->board->setLastLineClearInfo(type, linesCleared, isTSpin || isAllSpin, b2b);

    return damage;
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
