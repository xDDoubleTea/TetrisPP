#ifndef BUTTON_H
#define BUTTON_H

#include "Object.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <functional>
#include <string>

class Button : public Object {
public:
    /**
     * @brief Construct a new Button object
     * @param x X position
     * @param y Y position
     * @param w Width
     * @param h Height
     * @param text Label text
     */
    Button(float x, float y, float w, float h, const std::string& text);
    virtual ~Button() { }

    // Method names updated to camelCase and 'override' removed
    void init();
    void update();
    void draw();

    /**
     * @brief Set the function to call when clicked
     * @param callback A void function or lambda
     */
    void setOnClickCallback(std::function<void()> callback);

    /**
     * @brief Set the Font for the label
     * @param font Allegro font pointer
     */
    void setFont(ALLEGRO_FONT* font);

    /**
     * @brief Update button colors
     */
    void setCustomColor(ALLEGRO_COLOR body, ALLEGRO_COLOR hover, ALLEGRO_COLOR text);

    void setText(const std::string& text);

private:
    float x, y, w, h;
    std::string text;
    ALLEGRO_FONT* font = nullptr;
    std::function<void()> onClick;

    // Appearance
    ALLEGRO_COLOR bodyColor;
    ALLEGRO_COLOR hoverColor;
    ALLEGRO_COLOR textColor;
    ALLEGRO_COLOR outlineColor;

    // State
    bool isHovered = false;
    bool isPressed = false; // Latches the click so it triggers once per press
};

#endif
