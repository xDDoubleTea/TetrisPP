#include "Button.h"
#include "data/DataCenter.h"

#include "Utils.h"
#include <iostream>
Button::Button(float x, float y, float w, float h, const std::string& text)
    : x(x)
    , y(y)
    , w(w)
    , h(h)
    , text(text)
{
    // Default Styling
    bodyColor = al_map_rgb(60, 60, 60);
    hoverColor = al_map_rgb(90, 90, 90);
    textColor = al_map_rgb(255, 255, 255);
    outlineColor = al_map_rgb(200, 200, 200);
}

void Button::init()
{
    // Initialization if necessary
}

void Button::update()
{

    DataCenter* DC = DataCenter::get_instance();
    // Check collision with mouse
    isHovered = DC->mouse.x >= x && DC->mouse.x <= x + w && DC->mouse.y >= y && DC->mouse.y <= y + h;
    // std::cout << "Button '" << text << "' hover state: " << (isHovered ? "true" : "false") << std::endl;
    if (isHovered) {
        if (DC->mouse_state[1] && !DC->prev_mouse_state[1]) {
            // Clicked
            if (onClick) {
                onClick();
                // std::cout << "Button '" << text << "' clicked." << std::endl;
            }
        }
    }
}

void Button::draw()
{
    // Determine background color based on state
    ALLEGRO_COLOR currentColor = isHovered ? hoverColor : bodyColor;

    // Draw Body
    al_draw_filled_rectangle(x, y, x + w, y + h, currentColor);

    // Draw Outline
    al_draw_rectangle(x, y, x + w, y + h, outlineColor, 2.0);

    // Draw Text Centered
    if (font) {
        int textW = al_get_text_width(font, text.c_str());
        int textH = al_get_font_line_height(font);

        float textX = x + (w - textW) / 2.0f;
        float textY = y + (h - textH) / 2.0f;

        al_draw_text(font, textColor, textX, textY, 0, text.c_str());
    }
}

void Button::setOnClickCallback(std::function<void()> callback)
{
    onClick = callback;
}

void Button::setFont(ALLEGRO_FONT* f)
{
    font = f;
}

void Button::setCustomColor(ALLEGRO_COLOR body, ALLEGRO_COLOR hover, ALLEGRO_COLOR text)
{
    bodyColor = body;
    hoverColor = hover;
    textColor = text;
}

void Button::setText(const std::string& t)
{
    text = t;
}
