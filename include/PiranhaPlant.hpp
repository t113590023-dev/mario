//
// Created by webbe on 2026/4/15.
//

#ifndef MARIO_PIRANHAPLANT_HPP
#define MARIO_PIRANHAPLANT_HPP

#pragma once
#include "Enemy.hpp"
#include "Util/Animation.hpp"

class PiranhaPlant : public Enemy {
public:
    enum class State {
        Hiding,
        MovingUp,
        Exposed,
        MovingDown
    };

    PiranhaPlant(float startX, float startY, bool isUnderGround = false);

    void Start() override;
    void Update() override;
    void OnHit() override;
    void OnHit(std::shared_ptr<Mario> mario) override;

private:
    std::shared_ptr<Util::Animation> m_BiteAnimation;

    State m_State = State::Hiding;
    float m_StartY;
    float m_TargetY;
    float m_WaitTimer = 0.0f;
    bool m_isUnderGround;
};

#endif //MARIO_PIRANHAPLANT_HPP