#include "UI.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/FontCenter.h"
#include "data/ImageCenter.h"
#include "shapes/Point.h"
#include "shapes/Rectangle.h"
#include <algorithm>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

// fixed settings
constexpr char love_img_path[] = "./assets/image/love.png";
constexpr int love_img_padding = 5;
constexpr int tower_img_left_padding = 30;
constexpr int tower_img_top_padding = 30;

void UI::init()
{
}

void UI::update()
{
}

void UI::draw()
{
}
void UI::drawStartScreen()
{
    DataCenter* DC = DataCenter::get_instance();
    FontCenter* FC = FontCenter::get_instance();
    ImageCenter* IC = ImageCenter::get_instance();

    // Draw background
    ALLEGRO_COLOR bg_color = al_map_rgb(20, 20, 20);
    al_clear_to_color(bg_color);

    // Draw title
    ALLEGRO_FONT* title_font = FC->courier_new[FontSize::LARGE];
    const char* title_text = "TETRIS GAME";
    int title_width = al_get_text_width(title_font, title_text);
    al_draw_text(title_font, al_map_rgb(255, 255, 255),
        DC->window_width / 2, 100,
        ALLEGRO_ALIGN_CENTRE, title_text);
    al_draw_text(FC->courier_new[FontSize::MEDIUM], al_map_rgb(200, 200, 200),
        DC->window_width / 2, 200,
        ALLEGRO_ALIGN_CENTRE, "Press ENTER to Start");
}
