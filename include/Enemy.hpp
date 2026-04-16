//
// Created by webbe on 2026/4/15.
//

#ifndef MARIO_ENEMY_HPP
#define MARIO_ENEMY_HPP
#pragma once

#include "Character.hpp"
#include <memory>
#include "Definitions.hpp"

class Mario;
class LevelManager;

class Enemy : public Character {
public:
    Enemy(const std::string& ImagePath);
    virtual ~Enemy() = default;

    virtual void Start() = 0;
    virtual void Update() = 0;

    virtual void IfCollidesTile(std::shared_ptr<LevelManager> levelManager);

    virtual void OnHit() = 0;
    virtual void OnHit(std::shared_ptr<Mario> mario) = 0;

    bool IsDead() const { return isDead; }
    void SetDead(bool state) { isDead = state; }
    float GetTimer() const {return timer; }

protected:
    glm::vec2 velocity = {0, 0};
    bool isOnGround = false;
    bool isDead = false;
    bool isActive = false;

    int moveDirection = -1; // -1: 左, 1: 右
    float moveSpeed = 1.0f;
    float timer = 1.0f;
};

#endif //MARIO_ENEMY_HPP