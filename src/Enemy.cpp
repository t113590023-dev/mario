//
// Created by webbe on 2026/4/15.
//
#include "Enemy.hpp"

#include "EffectManager.hpp"
#include "GameHUD.hpp"
#include "LevelManager.hpp"

Enemy::Enemy(const std::string& ImagePath) : Character(ImagePath) {

}

void Enemy::popDead(int direction, bool addScore) {
    isPopup = true;
    isDead = true;
    SFXManager::GetInstance()->Play("kick");
    velocity = {direction, 8};
    m_Transform.scale = {2, -2};
    if (addScore) {
        EffectManager::GetInstance()->ShowScoreText(GetTransform().translation, 100);
        GameHUD::GetInstance()->AddScore(100);
    }
}

void Enemy::IfCollidesTile(std::shared_ptr<LevelManager> levelManager) {
    if (isDead) {
        if (timer > 0)
            timer -= Util::Time::GetDeltaTimeMs() / 1000;
        return;
    }

    if (levelManager->IsOutsideCamera(GetPosition())) {
        SetDead(true);
        timer = 0;
    }

    float posX = GetPosition().x, posY = GetPosition().y + (GetScaledSize().y - 32)/2;
    glm::vec2 size = {32, 32}; //GetScaledSize();

    // 左右碰撞反彈
    int collideTileID = levelManager->CheckCollision(posX + velocity.x, posY, size.x, 0,
        (moveDirection == -1) ? Direction::LEFT : Direction::RIGHT);

    if (collideTileID != -1) {
        moveDirection *= -1; // 反轉方向
        velocity.x = moveSpeed * moveDirection;
    }

    // 落地檢測
    collideTileID = levelManager->CheckCollision(posX, posY + velocity.y, size.x/2, size.y, Direction::BOTTOM);
    if (collideTileID != -1) {
        SetPosition({posX, levelManager->GetTilePosition(levelManager->GetTileID(posX, posY)).y + 1 + (GetScaledSize().y - 32)/2});
        isOnGround = true;
        velocity.y = 0;
    } else {
        isOnGround = false;
    }
}