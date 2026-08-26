#ifndef FONTCENTER_H_INCLUDED
#define FONTCENTER_H_INCLUDED

#include <allegro5/allegro_font.h>
#include <array>
#include <map>

// fixed settings
namespace FontSize {
static constexpr int SMALL = 12, MEDIUM = 24, LARGE = 36, TITLE = 108;
static constexpr std::array<int, 4> list({ FontSize::SMALL, FontSize::MEDIUM, FontSize::LARGE, FontSize::TITLE });
};

/**
 * @brief Stores and manages fonts.
 * @details While FontCenter is initializing, it will use the fixed settings to create ALLEGRO_FONT* instances and store them. The created font instances will be stored in map and use font size as the key.
 */
class FontCenter {
public:
    static FontCenter* get_instance()
    {
        static FontCenter FC;
        return &FC;
    }
    ~FontCenter();
    void init();

public:
    /**
     * @brief Proportional face, used for headings and menu text.
     */
    std::map<int, ALLEGRO_FONT*> ui;
    /**
     * @brief Monospaced face, used for the board overlay and statistics.
     */
    std::map<int, ALLEGRO_FONT*> mono;

private:
    FontCenter() { }
};

#endif
