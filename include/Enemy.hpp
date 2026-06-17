//
// Created by webbe on 2026/4/15.
//

#ifndef MARIO_ENEMY_HPP
#define MARIO_ENEMY_HPP
#pragma once

#include "Character.hpp"
#include <memory>
#include "Definitions.hpp"
#include "Util/Logger.hpp"
#include "SFXManager.hpp"

class Mario;
class LevelManager;

class Enemy : public Character {
public:
    Enemy(const std::string& ImagePath);
    virtual ~Enemy() = default;

    virtual void Start() = 0;
    virtual void Update() = 0;

    virtual void IfCollidesTile(std::shared_ptr<LevelManager> levelManager);
    virtual void popDead(int direction, bool addScore = true);

    virtual void OnHit() = 0;
    virtual void OnHit(std::shared_ptr<Mario> mario) = 0;

    EnemyType Gettype() const { return type; }

    bool IsDead() const { return isDead; }
    bool IsActive() const { return isActive; }
    void SetDead(bool state) { isDead = state; }
    int GetmoveDirection() const { return moveDirection; }
    float GetTimer() const {return timer; }

protected:
    glm::vec2 velocity = {0, 0};
    bool isOnGround = false;
    bool isDead = false;
    bool isActive = false;
    bool isPopup = false;
    float deathTimer = 0;
    EnemyType type = EnemyType::None;

    int moveDirection = -1; // -1: 左, 1: 右
    float moveSpeed = 1.0f;
    float timer = 1.0f;
};

#endif //MARIO_ENEMY_HPP