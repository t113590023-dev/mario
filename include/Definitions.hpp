//
// Created by webbe on 2026/4/14.
//

#ifndef MARIO_DEFINITIONS_HPP
#define MARIO_DEFINITIONS_HPP
#include "glm/vec2.hpp"

enum class MarioType {
    SMALL,
    BIG,
    FIRE
};

enum class Direction {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    BOTHSIDE
};

enum class ItemType {
    NONE,
    MUSHROOM,
    FIREFLOWER,
    UP1MUSHROOM,
    STARMAN,
    COIN
};

inline bool IsIntersecting(const glm::vec2& pos1, const glm::vec2& size1,
                           const glm::vec2& pos2, const glm::vec2& size2) {

    return (pos1.x - size1.x/2 < pos2.x + size2.x/2 &&
            pos1.x + size1.x/2 > pos2.x - size2.x/2 &&
            pos1.y - size1.y/2 < pos2.y + size2.y/2 &&
            pos1.y + size1.y/2 > pos2.y - size2.y/2);
}

#endif //MARIO_DEFINITIONS_HPP