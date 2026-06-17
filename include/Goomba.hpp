//
// Created by webbe on 2026/4/15.
//

#ifndef MARIO_GOOMBA_HPP
#define MARIO_GOOMBA_HPP

#pragma once
#include "Enemy.hpp"
#include "Util/Animation.hpp"

class Goomba : public Enemy {
public:
    Goomba(float startX, float startY, bool isUnderGround = false);

    void Start() override;
    void Update() override;
    void OnHit() override;
    void OnHit(std::shared_ptr<Mario> mario) override;

private:
    std::shared_ptr<Util::Animation> m_WalkAnimation;
    std::shared_ptr<Util::Image> m_DeadImage;
    bool m_isUnderGround;
};

#endif //MARIO_GOOMBA_HPP