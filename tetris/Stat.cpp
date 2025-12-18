#include "Stat.h"

#include "../data/DataCenter.h"
#include "../data/FontCenter.h"
#include <allegro5/allegro_font.h>

using namespace Tetris;

Stat::Stat()
    : frames_played(0)
    , total_lines_cleared(0)
    , time_played_seconds(0)
    , pieces_placed(0)
    , t_spins(0)
    , all_spins(0)
    , perfect_clears(0)
    , back_to_back_count(0)
    , APM(0.0)
    , PPS(0.0)
    , combo_count(0.0)
    , attacks_sent(0)
{
}

void Stat::init()
{
    frames_played = 0;
    total_lines_cleared = 0;
    time_played_seconds = 0;
    pieces_placed = 0;
    attacks_sent = 0;
    t_spins = 0;
    all_spins = 0;
    perfect_clears = 0;
    back_to_back_count = 0;
    APM = 0.0;
    PPS = 0.0;
    combo_count = 0.0;
}

void Stat::update()
{
    frames_played++;
    time_played_seconds = frames_played / DataCenter::get_instance()->FPS;
    APM = (attacks_sent / static_cast<double>(frames_played)) * DataCenter::get_instance()->FPS * 60.0;
    PPS = (pieces_placed / static_cast<double>(frames_played)) * DataCenter::get_instance()->FPS;
    APP = (attacks_sent / static_cast<double>(pieces_placed));
}

void Stat::updatePieceStat(size_t lines_cleared, bool is_t_spin, bool is_pc, bool is_b2b, bool is_all_spin)
{
    total_lines_cleared += lines_cleared;
    all_spins += static_cast<int>(is_all_spin);
    t_spins += static_cast<int>(is_t_spin);
    perfect_clears += static_cast<int>(is_pc);
    if (is_b2b)
        back_to_back_count++;
    else
        back_to_back_count = 0;
    if (combo_count > 0 && lines_cleared > 0)
        combo_count++;
    else
        combo_count = 0;
}

void Stat::draw()
{
    FontCenter* FC = FontCenter::get_instance();

    al_draw_textf(FC->courier_new[FontSize::MEDIUM], al_map_rgb(STAT_TEXT_COLOR.r, STAT_TEXT_COLOR.g, STAT_TEXT_COLOR.b),
        Stat::STAT_OFFSET_X, Stat::STAT_OFFSET_Y,
        ALLEGRO_ALIGN_CENTER,
        "Lines Cleared: %u", static_cast<unsigned>(total_lines_cleared));
    al_draw_textf(FC->courier_new[FontSize::MEDIUM], al_map_rgb(STAT_TEXT_COLOR.r, STAT_TEXT_COLOR.g, STAT_TEXT_COLOR.b),
        Stat::STAT_OFFSET_X, Stat::STAT_OFFSET_Y + Stat::STAT_SPACING_Y,
        ALLEGRO_ALIGN_CENTER,
        "Time Played: %u s", static_cast<unsigned>(time_played_seconds));
    al_draw_textf(FC->courier_new[FontSize::MEDIUM], al_map_rgb(STAT_TEXT_COLOR.r, STAT_TEXT_COLOR.g, STAT_TEXT_COLOR.b),
        Stat::STAT_OFFSET_X, Stat::STAT_OFFSET_Y + 2 * Stat::STAT_SPACING_Y,
        ALLEGRO_ALIGN_CENTER,
        "Pieces Placed: %u", static_cast<unsigned>(pieces_placed));
    al_draw_textf(FC->courier_new[FontSize::MEDIUM], al_map_rgb(STAT_TEXT_COLOR.r, STAT_TEXT_COLOR.g, STAT_TEXT_COLOR.b),
        Stat::STAT_OFFSET_X, Stat::STAT_OFFSET_Y + 3 * Stat::STAT_SPACING_Y,
        ALLEGRO_ALIGN_CENTER,
        "APM: %.2f", APM);
    al_draw_textf(FC->courier_new[FontSize::MEDIUM], al_map_rgb(STAT_TEXT_COLOR.r, STAT_TEXT_COLOR.g, STAT_TEXT_COLOR.b),
        Stat::STAT_OFFSET_X, Stat::STAT_OFFSET_Y + 4 * Stat::STAT_SPACING_Y,
        ALLEGRO_ALIGN_CENTER,
        "PPS: %.2f", PPS);
    al_draw_textf(FC->courier_new[FontSize::MEDIUM], al_map_rgb(STAT_TEXT_COLOR.r, STAT_TEXT_COLOR.g, STAT_TEXT_COLOR.b),
        Stat::STAT_OFFSET_X, Stat::STAT_OFFSET_Y + 5 * Stat::STAT_SPACING_Y,
        ALLEGRO_ALIGN_CENTER,
        "Attacks Sent: %u", static_cast<unsigned>(attacks_sent));
    al_draw_textf(FC->courier_new[FontSize::MEDIUM], al_map_rgb(STAT_TEXT_COLOR.r, STAT_TEXT_COLOR.g, STAT_TEXT_COLOR.b),
        Stat::STAT_OFFSET_X, Stat::STAT_OFFSET_Y + 6 * Stat::STAT_SPACING_Y,
        ALLEGRO_ALIGN_CENTER,
        "APP: %.2f", APP);
}
