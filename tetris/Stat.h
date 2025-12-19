#ifndef STAT_H
#define STAT_H

#include "Board.h"
#include "TetriminoDefinitions.h"

namespace Tetris {
class Tetrimino;
class Board;
class Stat {
private:
    constexpr static int STAT_OFFSET_X = Board::BOARD_OFFSET_X + Board::GRID_W / 2 * Board::BLOCK_SIZE;
    constexpr static int STAT_OFFSET_Y = Board::BOARD_OFFSET_Y + Board::GRID_H * Board::BLOCK_SIZE + 20;
    constexpr static int STAT_SPACING_Y = 30;
    constexpr static ColorRGB STAT_TEXT_COLOR = { 123, 255, 100 };

public:
    Stat();
    void init();
    void update();
    void draw();
    void updatePieceStat(size_t lines_cleared, bool is_t_spin, bool is_pc, bool is_b2b, bool is_all_spin);
    size_t getComboCount() const { return combo_count; }
    size_t getBackToBackCount() const { return back_to_back_count; }
    void increasePiecesPlaced() { pieces_placed++; }
    void increaseAttacksSent(size_t amount) { attacks_sent += amount; }

private:
    size_t frames_played;

private:
    // Piece statistics
    size_t total_lines_cleared;
    size_t time_played_seconds;
    size_t pieces_placed;
    size_t t_spins;
    size_t all_spins;
    size_t perfect_clears;
    size_t back_to_back_count;
    double APM;
    double PPS;
    double APP;
    size_t combo_count;

    // Attack and Damage statistics
    size_t attacks_sent;
    size_t attacks_received;
};

}

#endif
