#include "ImageCenter.h"
#include "../Utils.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/bitmap_io.h>

ImageCenter::~ImageCenter()
{
    for (auto& [path, bitmap] : bitmaps) {
        // The shared placeholder appears under many keys; destroy it once below.
        if (bitmap && bitmap != placeholder_bitmap)
            al_destroy_bitmap(bitmap);
    }
    if (placeholder_bitmap)
        al_destroy_bitmap(placeholder_bitmap);
}


namespace {
constexpr int PLACEHOLDER_SIZE = 64;
constexpr int PLACEHOLDER_CELL = 8;
}

/**
 * @brief Build the magenta/black checkerboard shown in place of a missing image.
 * @details Keeps the game running when an asset is absent, which matters because
 * the repository intentionally ships no artwork.
 */
ALLEGRO_BITMAP* ImageCenter::placeholder()
{
    if (placeholder_bitmap)
        return placeholder_bitmap;

    ALLEGRO_BITMAP* previous = al_get_target_bitmap();
    ALLEGRO_BITMAP* bitmap = al_create_bitmap(PLACEHOLDER_SIZE, PLACEHOLDER_SIZE);
    if (!bitmap)
        return nullptr;

    al_set_target_bitmap(bitmap);
    for (int y = 0; y < PLACEHOLDER_SIZE; y += PLACEHOLDER_CELL) {
        for (int x = 0; x < PLACEHOLDER_SIZE; x += PLACEHOLDER_CELL) {
            bool magenta = ((x / PLACEHOLDER_CELL) + (y / PLACEHOLDER_CELL)) % 2 == 0;
            al_draw_filled_rectangle(x, y, x + PLACEHOLDER_CELL, y + PLACEHOLDER_CELL,
                magenta ? al_map_rgb(255, 0, 255) : al_map_rgb(0, 0, 0));
        }
    }
    al_set_target_bitmap(previous);
    placeholder_bitmap = bitmap;
    return placeholder_bitmap;
}

/**
 * @brief Whether a bitmap is the stand-in returned for a missing file.
 * @details Call sites that would rather draw nothing (or a flat fill) than a
 * checkerboard use this to tell the difference.
 */
bool ImageCenter::is_placeholder(const ALLEGRO_BITMAP* bitmap) const
{
    return bitmap != nullptr && bitmap == placeholder_bitmap;
}

/**
 * @brief The getter function searches if a bitmap is loaded and return the bitmap. If not loaded, it will try to load the image and return.
 * @details If the image cannot be loaded, a magenta/black checkerboard placeholder is cached and returned instead, so a missing asset degrades the visuals rather than terminating the game.
 * @param path the image path.
 * @return The curresponding loaded ALLEGRO_BITMAP* instance.
 */
ALLEGRO_BITMAP*
ImageCenter::get(const std::string& path)
{
    std::map<std::string, ALLEGRO_BITMAP*>::iterator it = bitmaps.find(path);
    if (it != bitmaps.end())
        return it->second;

    ALLEGRO_BITMAP* bitmap = al_load_bitmap(path.c_str());
    if (!bitmap) {
        debug_log("<ImageCenter> cannot find image: %s, using placeholder.\n", path.c_str());
        bitmap = placeholder();
    }
    bitmaps[path] = bitmap;
    return bitmap;
}

/**
 * @brief Remove a bitmap.
 * @param path the image path.
 * @return True if the bitmap of the path is removed. False if the bitmap does not exist.
 */
bool ImageCenter::erase(const std::string& path)
{
    std::map<std::string, ALLEGRO_BITMAP*>::iterator it = bitmaps.find(path);
    if (it == bitmaps.end()) {
        return false;
    }
    ALLEGRO_BITMAP* bitmap = it->second;
    al_destroy_bitmap(bitmap);
    bitmaps.erase(it);
    return true;
}
