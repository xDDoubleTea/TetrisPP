#include "UI.h"
#include "Button.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

// fixed settings

void UI::init()
{
}

void UI::update()
{
}

void UI::draw()
{
}
void UI::updateStartScreen()
{
    for (auto& [key, button] : button_map) {
        button->update();
    }
}
void UI::drawStartScreen()
{
}
