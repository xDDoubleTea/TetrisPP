#include "FontCenter.h"
#include "../Utils.h"
#include <allegro5/allegro_ttf.h>

// fixed settings
namespace FontSetting {
const char ui_font_path[] = "./assets/font/Inter-Bold.ttf";
const char mono_font_path[] = "./assets/font/JetBrainsMono-Bold.ttf";
}

/**
 * @brief Load a face at the given size, falling back to Allegro's builtin font.
 * @details A missing or unreadable font file would otherwise leave a null
 * ALLEGRO_FONT* in the map, which crashes at the first al_draw_text call.
 */
static ALLEGRO_FONT* load_or_builtin(const char* path, int size)
{
    ALLEGRO_FONT* font = al_load_ttf_font(path, size, 0);
    if (!font) {
        debug_log("<FontCenter> cannot load %s at size %d, using builtin font.\n", path, size);
        font = al_create_builtin_font();
    }
    return font;
}

void FontCenter::init()
{
    for (const int& fs : FontSize::list) {
        ui[fs] = load_or_builtin(FontSetting::ui_font_path, fs);
        mono[fs] = load_or_builtin(FontSetting::mono_font_path, fs);
    }
}

FontCenter::~FontCenter()
{
    for (auto& [size, font] : ui)
        al_destroy_font(font);
    for (auto& [size, font] : mono)
        al_destroy_font(font);
}
