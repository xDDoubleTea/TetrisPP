#include "GIFCenter.h"
#include "../Utils.h"
#include <allegro5/bitmap_io.h>

GIFCenter::~GIFCenter()
{
    for (auto& [path, gif] : gifs) {
        if (gif)
            algif_destroy_animation(gif);
    }
}

/**
 * @brief The getter function searches if a bitmap is loaded and return the bitmap. If not loaded, it will try to load the GIF and return.
 * @details If the GIF cannot be loaded, nullptr is returned and cached. Callers must null-check before drawing; a missing animation is not fatal.
 * @param path the GIF path.
 * @return The curresponding loaded ALGIF_ANIMATION* instance.
 */
ALGIF_ANIMATION*
GIFCenter::get(const std::string& path)
{
    std::map<std::string, ALGIF_ANIMATION*>::iterator it = gifs.find(path);
    if (it == gifs.end()) {
        ALGIF_ANIMATION* gif = algif_load_animation(path.c_str());
        if (!gif) {
            debug_log("<GIFCenter> cannot find GIF: %s, skipping animation.\n", path.c_str());
        }
        gifs[path] = gif;
        return gif;
    } else {
        return it->second;
    }
}

/**
 * @brief Remove a bitmap.
 * @param path the GIF path.
 * @return True if the bitmap of the path is removed. False if the bitmap does not exist.
 */
bool GIFCenter::erase(const std::string& path)
{
    std::map<std::string, ALGIF_ANIMATION*>::iterator it = gifs.find(path);
    if (it == gifs.end()) {
        return false;
    }
    ALGIF_ANIMATION* bitmap = it->second;
    if (bitmap)
        algif_destroy_animation(bitmap);
    gifs.erase(it);
    return true;
}
