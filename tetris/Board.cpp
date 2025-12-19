#include "Board.h"
#include "../Utils.h"
#include "../data/DataCenter.h"
#include "../data/FontCenter.h"
#include "../data/SoundCenter.h"
#include "Tetrimino.h"
#include "TetriminoDefinitions.h"
#include <algorithm>
#include <allegro5/allegro_font.h>
#include <allegro5/bitmap_draw.h>
#include <random>

using namespace Tetris;

// Constants

Board::Board()
    : activePiece(nullptr)
    , holdPiece(nullptr)
    , gravityTimer(0)
    , gravitySpeed(GRAVITY_SPEED)
{
    init();
}

void Board::init()
{
    // Clear grid
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            grid[y][x] = 0; // 0 means empty
        }
    }
    // Initial spawn
    holdPiece = nullptr;
    debug_log("Trying to spawn initial piece...\n");
    spawnPiece();
    garbageQueue = 0;
    background = al_load_bitmap("assets/background/pvz_playfield.png");
    // Test Garbage
    // garbageQueue = 5;
}

bool Board::update()
{
    bool activePieceUpdated = false;
    if (activePiece)
        activePieceUpdated = activePiece->update(*this);
    else
        debug_log("No active piece to update.\n");

    // debug_log("Active piece updated: %s\n", activePieceUpdated ? "true" : "false");
    if (!activePieceUpdated) {
        return false; // Game over or error
    }
    DataCenter* DC = DataCenter::get_instance();
    if (DC->key_state[ALLEGRO_KEY_C] && !DC->prev_key_state[ALLEGRO_KEY_C]) {
        // Hold
        if (!holdPiece) {
            holdPiece = activePiece;
            holdPiece->setHold(true);
            holdPiece->resetRotation();
            spawnPiece();
        } else {
            holdPiece->setHold(true);
            holdPiece->resetRotation();
            std::swap(holdPiece, activePiece);
        }
    }
    return true;
}

void Board::updateGravityTimer(bool rotated, bool moved)
{
    if (rotated) {
        gravityTimer = 0;
        return;
    }
    // gravityTimer++;
    if (gravityTimer >= gravitySpeed) {
        gravityTimer = 0;
        if (activePiece) {
            if (moved) {
                gravityTimer = 0;
                return;
            }
            if (!activePiece->tryMove(0, 1)) {
                lockPiece(*activePiece);
            }
        }
    }
}
bool Board::checkCollision(TetriminoType type, int rotation, int x, int y)
{
    int typeIdx = static_cast<int>(type);

    for (const auto& block : tetrimino_shapes[typeIdx][rotation]) {
        int bx = x + block.x;
        int by = y + block.y;

        // Wall collision
        if (bx < 0 || bx >= GRID_W || by >= GRID_H)
            return true;

        // Floor/Ceiling check (by < 0 is usually allowed for spawning, but check your rules)
        if (by >= 0 && grid[by][bx] != 0)
            return true;
    }
    return false;
}

void Board::sendAttack(size_t damage)
{
    DataCenter* DC = DataCenter::get_instance();
    if (damage > 0) {
        DC->add_pea(new Pea(damage));
        DC->peaShooter->set_state_shooting();
    }
}
bool Board::lockPiece(const Tetrimino& t)
{
    int typeIdx = static_cast<int>(t.getType());
    int rotation = t.getRotation();
    int tx = t.getX();
    int ty = t.getY();

    // Lock into grid
    for (const auto& block : tetrimino_shapes[typeIdx][rotation]) {
        int bx = tx + block.x;
        int by = ty + block.y;
        if (bx >= 0 && bx < GRID_W && by >= 0 && by < GRID_H) {
            grid[by][bx] = typeIdx + 1; // Store type ID (1-based)
        }
    }

    size_t linesCleared = clearLines();
    DataCenter* DC = DataCenter::get_instance();
    DC->stat->updatePieceStat(linesCleared, activePiece->isTSpin(), isPerfectClear(), (linesCleared == 4 || activePiece->TSpin || activePiece->AllSpin), activePiece->isAllSpin());
    DC->stat->increasePiecesPlaced();
    size_t damage = t.damageDealt(linesCleared, isPerfectClear(), DC->stat->getBackToBackCount(), activePiece->TSpin, activePiece->AllSpin, DC->stat->getComboCount());
    // debug_log("Damage dealt: %zu\n", damage);

    sendAttack(damage);
    DC->stat->increaseAttacksSent(damage);
    garbageQueue -= std::min(garbageQueue, damage);

    if (garbageQueue > 0) {
        addGarbageLines(std::min(garbageQueue, (size_t)8));
        // debug_log("Added garbage lines from queue. Remaining garbage: %zu\n", garbageQueue);
    }

    delete activePiece;
    activePiece = nullptr;

    // Spawn new piece, return false if game over
    return spawnPiece();
}

bool Board::isPerfectClear()
{
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            if (grid[y][x] != 0)
                return false;
        }
    }
    return true;
}

void Board::addGarbageLines(size_t count)
{
    // Shift grid up
    for (size_t y = 0; y < GRID_H - count; y++) {
        for (int x = 0; x < GRID_W; x++) {
            grid[y][x] = grid[y + count][x];
        }
    }
    // Add garbage lines at the bottom
    std::random_device rd;
    std::mt19937 gen(rd());
    int hole = gen() % GRID_W;
    for (size_t i = 0; i < count; i++) {
        for (int x = 0; x < GRID_W; x++) {
            if (x == hole)
                grid[GRID_H - count + i][x] = 0; // Hole
            else
                grid[GRID_H - count + i][x] = 8; // Garbage block (use 8 as garbage ID)
        }
    }
    garbageQueue -= (count > garbageQueue) ? garbageQueue : count;
}

size_t Board::clearLines()
{
    // Standard naive line clear implementation
    size_t linesCleared = 0;
    for (int y = GRID_H - 1; y >= 0; y--) {
        bool full = true;
        for (int x = 0; x < GRID_W; x++) {
            if (grid[y][x] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            // Remove row and shift down
            ++linesCleared;
            for (int ky = y; ky > 0; ky--) {
                for (int kx = 0; kx < GRID_W; kx++) {
                    grid[ky][kx] = grid[ky - 1][kx];
                }
            }

            for (int kx = 0; kx < GRID_W; kx++)
                grid[0][kx] = 0;
            y++;
        }
    }
    return linesCleared;
}

bool Board::spawnPiece()
{
    // Check game over
    if (nextQueue.size() < 7 + 1) {
        generate7Bag();
    }

    if (checkCollision(nextQueue.front()->getType(), 0, 4, 0)) {
        debug_log("Game Over triggered.\n");
        return false;
    }
    activePiece = new Tetrimino(nextQueue.front()->getType());
    nextQueue.pop();
    return true;
}

void Board::draw()
{
    drawDecorations();
    al_draw_rectangle(BOARD_OFFSET_X, BOARD_OFFSET_Y,
        BOARD_OFFSET_X + GRID_W * BLOCK_SIZE,
        BOARD_OFFSET_Y + GRID_H * BLOCK_SIZE,
        al_map_rgb(255, 255, 255), 2);

    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            if (grid[y][x] != 0) {
                // Get color based on ID stored in grid
                ColorRGB c = tetrimino_colors[grid[y][x] - 1];
                al_draw_filled_rectangle(
                    BOARD_OFFSET_X + x * BLOCK_SIZE,
                    BOARD_OFFSET_Y + y * BLOCK_SIZE,
                    BOARD_OFFSET_X + (x + 1) * BLOCK_SIZE,
                    BOARD_OFFSET_Y + (y + 1) * BLOCK_SIZE,
                    al_map_rgb(c.r, c.g, c.b));
            }
            al_draw_rectangle(
                BOARD_OFFSET_X + x * BLOCK_SIZE,
                BOARD_OFFSET_Y + y * BLOCK_SIZE,
                BOARD_OFFSET_X + (x + 1) * BLOCK_SIZE,
                BOARD_OFFSET_Y + (y + 1) * BLOCK_SIZE,
                al_map_rgb(0, 0, 0), 1);
        }
    }

    if (activePiece) {
        activePiece->draw(BOARD_OFFSET_X, BOARD_OFFSET_Y, BLOCK_SIZE);
    }
    if (holdPiece) {
        drawHoldPiece(holdPiece->getType());
    }
    if (!nextQueue.empty()) {
        drawNextQueue(NEXT_QUEUE_COUNT);
    }
    drawGarbageQueue();
    drawLineClearTypes();
}

void Board::drawLineClearTypes()
{
    if (!drawLineClearTypes_enabled)
        return;

    if (lastClearedLines == 0)
        return;

    FontCenter* FC = FontCenter::get_instance();
    DataCenter* DC = DataCenter::get_instance();
    switch (lastClearedLines) {
    case 1:
        al_draw_textf(FC->courier_new[FontSize::LARGE], al_map_rgb(font_color.r, font_color.g, font_color.b),
            BOARD_OFFSET_X - 30,
            BOARD_OFFSET_Y + HOLD_PIECE_OFFSET_Y + BLOCK_SIZE * 4,
            ALLEGRO_ALIGN_RIGHT,
            "SINGLE");
        break;
    case 2:
        al_draw_textf(FC->courier_new[FontSize::LARGE], al_map_rgb(font_color.r, font_color.g, font_color.b),
            BOARD_OFFSET_X - 30,
            BOARD_OFFSET_Y + HOLD_PIECE_OFFSET_Y + BLOCK_SIZE * 4,
            ALLEGRO_ALIGN_RIGHT,
            "DOUBLE");
        break;
    case 3:
        al_draw_textf(FC->courier_new[FontSize::LARGE], al_map_rgb(font_color.r, font_color.g, font_color.b),
            BOARD_OFFSET_X - 30,
            BOARD_OFFSET_Y + HOLD_PIECE_OFFSET_Y + BLOCK_SIZE * 4,
            ALLEGRO_ALIGN_RIGHT,
            "TRIPLE");
        break;
    case 4:
        al_draw_textf(FC->courier_new[FontSize::LARGE], al_map_rgb(font_color.r, font_color.g, font_color.b),
            BOARD_OFFSET_X - 30,
            BOARD_OFFSET_Y + HOLD_PIECE_OFFSET_Y + BLOCK_SIZE * 4,
            ALLEGRO_ALIGN_RIGHT,
            "TETRIS");
        break;
    }
    std::string lastClearedTypeToString;
    switch (lastClearedType) {
    case TetriminoType::I:
        lastClearedTypeToString = "I";
        break;
    case TetriminoType::J:
        lastClearedTypeToString = "J";
        break;
    case TetriminoType::L:
        lastClearedTypeToString = "L";
        break;
    case TetriminoType::O:
        lastClearedTypeToString = "O";
        break;
    case TetriminoType::S:
        lastClearedTypeToString = "S";
        break;
    case TetriminoType::T:
        lastClearedTypeToString = "T";
        break;
    case TetriminoType::Z:
        lastClearedTypeToString = "Z";
        break;
    }
    ColorRGB lastclearedColor = tetrimino_colors[static_cast<int>(lastClearedType)];
    if (lastClearWasSpin) {
        al_draw_textf(FC->courier_new[FontSize::LARGE], al_map_rgb(lastclearedColor.r, lastclearedColor.g, lastclearedColor.b),
            BOARD_OFFSET_X - 30,
            BOARD_OFFSET_Y + HOLD_PIECE_OFFSET_Y + BLOCK_SIZE * 3,
            ALLEGRO_ALIGN_RIGHT,
            "%s-SPIN", lastClearedTypeToString.c_str());
    }
    if (lastClearWasB2B) {
        al_draw_textf(FC->courier_new[FontSize::LARGE], al_map_rgb(255, 215, 0),
            BOARD_OFFSET_X - 30,
            BOARD_OFFSET_Y + HOLD_PIECE_OFFSET_Y + BLOCK_SIZE * 5,
            ALLEGRO_ALIGN_RIGHT,
            "B2B x%zu", DC->stat->getBackToBackCount());
    }
}

void Board::drawNextQueue(int count)
{
    int offsetX = NEXT_QUEUE_OFFSET_X;
    int offsetY = NEXT_QUEUE_OFFSET_Y;
    std::queue<Tetrimino*> tempQueue = nextQueue;
    for (int i = 0; i < count && !tempQueue.empty(); ++i) {
        Tetrimino* t = tempQueue.front();
        tempQueue.pop();
        int typeIdx = static_cast<int>(t->getType());
        ColorRGB c = tetrimino_colors[typeIdx];
        ALLEGRO_COLOR color = al_map_rgb(c.r, c.g, c.b);
        int pieceOffsetY = offsetY + i * NEXT_QUEUE_SPACING;
        for (const auto& block : tetrimino_shapes[typeIdx][0]) {
            int drawX = offsetX + (block.x + 1) * BLOCK_SIZE; // Centering
            int drawY = pieceOffsetY + (block.y + 1) * BLOCK_SIZE;
            al_draw_filled_rectangle(drawX, drawY,
                drawX + BLOCK_SIZE, drawY + BLOCK_SIZE,
                color);
            al_draw_rectangle(drawX, drawY,
                drawX + BLOCK_SIZE, drawY + BLOCK_SIZE,
                al_map_rgb(0, 0, 0), 2);
        }
    }
}

void Board::drawHoldPiece(TetriminoType type)
{
    int typeIdx = static_cast<int>(type);
    ColorRGB c = tetrimino_colors[typeIdx];
    ALLEGRO_COLOR color = al_map_rgb(c.r, c.g, c.b);

    int offsetX = BOARD_OFFSET_X - HOLD_PIECE_OFFSET_X;
    int offsetY = BOARD_OFFSET_Y + HOLD_PIECE_OFFSET_Y;

    for (const auto& block : tetrimino_shapes[typeIdx][0]) {
        int drawX = offsetX + (block.x + 1) * BLOCK_SIZE; // Centering
        int drawY = offsetY + (block.y + 1) * BLOCK_SIZE;

        al_draw_filled_rectangle(drawX, drawY,
            drawX + BLOCK_SIZE, drawY + BLOCK_SIZE,
            color);
        al_draw_rectangle(drawX, drawY,
            drawX + BLOCK_SIZE, drawY + BLOCK_SIZE,
            al_map_rgb(0, 0, 0), 2);
    }
}

void Board::drawGarbageQueue()
{
    int offsetX = BOARD_OFFSET_X;
    int offsetY = BOARD_OFFSET_Y + GRID_H * BLOCK_SIZE;
    ALLEGRO_COLOR color = al_map_rgb(255, 0, 0);
    for (size_t i = 0; i < garbageQueue; ++i)
        al_draw_filled_rectangle(
            offsetX, offsetY - BLOCK_SIZE * i, offsetX - BLOCK_SIZE, offsetY - BLOCK_SIZE * (i + 1),
            color);
}

bool Board::isOccupied(int x, int y) const
{
    if (x < 0 || x >= GRID_W || y < 0 || y >= GRID_H)
        return true;
    return grid[y][x] != 0;
}

void Board::drawDecorations()
{
    DataCenter* DC = DataCenter::get_instance();
    FontCenter* FC = FontCenter::get_instance();
    al_draw_tinted_scaled_bitmap(background, al_map_rgb(100, 100, 100), 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
        0, 0,
        DC->window_width, DC->window_height, 0);
    al_draw_text(FC->courier_new[FontSize::LARGE], al_map_rgb(font_color.r, font_color.g, font_color.b),
        BOARD_OFFSET_X - 100,
        BOARD_OFFSET_Y,
        ALLEGRO_ALIGN_CENTRE,
        "HOLD");
    al_draw_text(FC->courier_new[FontSize::LARGE], al_map_rgb(font_color.r, font_color.g, font_color.b),
        NEXT_QUEUE_OFFSET_X + BLOCK_SIZE * 2,
        NEXT_QUEUE_OFFSET_Y - 40,
        ALLEGRO_ALIGN_CENTRE,
        "NEXT");
}

void Board::generate7Bag()
{
    std::vector<int> indices = { 0, 1, 2, 3, 4, 5, 6 };
    std::shuffle(indices.begin(), indices.end(), std::default_random_engine(std::random_device {}()));
    for (int i = 0; i < 7; ++i) {
        nextQueue.push(new Tetrimino(static_cast<TetriminoType>(indices[i])));
    }

    // std::queue<Tetrimino*> tempQueue = nextQueue;
    // while (!tempQueue.empty()) {
    //     Tetrimino* t = tempQueue.front();
    //     tempQueue.pop();
    //     debug_log("Next Queue contains Tetrimino type: %d\n", static_cast<int>(t->getType()));
    // }

    // debug_log("Generated 7-bag of Tetriminos.\n");
}

Board::~Board()
{
    if (activePiece)
        delete activePiece;
    if (holdPiece)
        delete holdPiece;
    while (!nextQueue.empty()) {
        delete nextQueue.front();
        nextQueue.pop();
    }
    if (background)
        al_destroy_bitmap(background);
}
