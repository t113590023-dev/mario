//
// Created by webbe on 2026/4/14.
//

#ifndef MARIO_BRICK_HPP
#define MARIO_BRICK_HPP
#include "Tile.hpp"

class Brick : public Tile {
public:
    Brick(float x, float y, bool isSurface);
    Brick(const std::string& ImagePath, float x, float y) : Tile(ImagePath, x, y) {}
    void onHit(MarioType marioType) override;
    void SetContent(int content);
private:
    int content, capacity = 6;
    bool m_isSurface;
};

#endif //MARIO_BRICK_HPP