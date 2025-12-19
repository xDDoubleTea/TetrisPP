#include "OperationCenter.h"
#include "../Utils.h"
#include "DataCenter.h"

void OperationCenter::update()
{
    _update_pea();
    _update_zombie();
    DataCenter* DC = DataCenter::get_instance();
    DC->peaShooter->update();
    zombie_spawn_timer++;
    if (zombie_spawn_timer >= DC->FPS * 15) {
        DC->add_zombie(new Zombie());
        // debug_log("Zombie spawned.\n");
        zombie_spawn_timer = 0;
    }
}

void OperationCenter::_update_pea()
{
    DataCenter* DC = DataCenter::get_instance();
    // Update all peas
    for (size_t i = 0; i < DC->peas.size();) {
        Pea* pea = DC->peas[i];
        if (pea) {
            if (!pea->update()) {
                // Remove pea if it goes out of bounds or collides
                delete pea;
                DC->peas.erase(DC->peas.begin() + i);
                // debug_log("Pea removed.\n");
            } else {
                ++i;
            }
        } else {
            ++i;
        }
    }
}

void OperationCenter::_update_zombie()
{
    DataCenter* DC = DataCenter::get_instance();
    // Update all zombies
    for (size_t i = 0; i < DC->zombies.size();) {
        Zombie* zombie = DC->zombies[i];
        if (zombie) {
            if (!zombie->update() || zombie->is_dead()) {
                // Remove zombie if dead
                delete zombie;
                DC->zombies.erase(DC->zombies.begin() + i);
                // debug_log("Zombie removed.\n");
            } else {
                ++i;
            }
        } else {
            ++i;
        }
    }
}

void OperationCenter::draw()
{
    _draw_pea();
    _draw_zombie();
    DataCenter* DC = DataCenter::get_instance();
    DC->peaShooter->draw();
}

void OperationCenter::_draw_pea()
{
    DataCenter* DC = DataCenter::get_instance();
    for (auto& pea : DC->peas) {
        if (pea)
            pea->draw();
    }
}

void OperationCenter::_draw_zombie()
{
    DataCenter* DC = DataCenter::get_instance();
    for (auto& zombie : DC->zombies) {
        if (zombie)
            zombie->draw();
    }
}
