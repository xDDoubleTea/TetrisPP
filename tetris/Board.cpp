#include "Board.h"
#include "../Utils.h"
#include "../data/DataCenter.h"
#include "Tetrimino.h"
#include <algorithm>
#include <random>

using namespace Tetris;

// Constants
constexpr int GRID_W = 10;
constexpr int GRID_H = 20;
constexpr int BLOCK_SIZE = 30; // Pixel size
constexpr int BOARD_OFFSET_X = 200;
constexpr int BOARD_OFFSET_Y = 50;

Board::Board()
    : activePiece(nullptr)
    , holdPiece(nullptr)
    , gravitySpeed(60)
    , gravityTimer(0)
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
    spawnPiece();
}

void Board::update()
{
    activePiece->update(*this);
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

void Board::lockPiece(const Tetrimino& t)
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

    delete activePiece;
    activePiece = nullptr;

    clearLines();
    spawnPiece();
}

void Board::clearLines()
{
    // Standard naive line clear implementation
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
}

void Board::spawnPiece()
{
    // Simple random spawn for now (implement 7-bag later)
    activePiece = new Tetrimino(nextQueue.front()->getType());
    nextQueue.pop();
}

void Board::draw()
{
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
        }
    }

    if (activePiece) {
        activePiece->draw(BOARD_OFFSET_X, BOARD_OFFSET_Y, BLOCK_SIZE);
    }
}
bool Board::isOccupied(int x, int y) const
{
    debug_log("Checking occupancy at (%d, %d)\n", x, y);
    if (x < 0 || x >= GRID_W || y < 0 || y >= GRID_H)
        return true;
    return grid[y][x] != 0;
}

void Board::generate7Bag()
{
    for (int i = 0; i < 7; ++i) {
        // TODO: Generate random order
        nextQueue.push(new Tetrimino(static_cast<TetriminoType>(i)));
    }
    debug_log("Generated 7-bag of Tetriminos.\n");
}
