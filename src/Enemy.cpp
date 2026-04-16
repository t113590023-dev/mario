//
// Created by webbe on 2026/4/15.
//
#include "Enemy.hpp"
#include "LevelManager.hpp"

Enemy::Enemy(const std::string& ImagePath) : Character(ImagePath) {

}

void Enemy::IfCollidesTile(std::shared_ptr<LevelManager> levelManager) {
    if (isDead) {
        if (timer > 0)
            timer -= Util::Time::GetDeltaTimeMs() / 1000;
        return;
    }

    if (levelManager->IsOutsideCamera(GetPosition()))
        SetDead(true);

    float posX = GetPosition().x, posY = GetPosition().y;
    glm::vec2 size = GetScaledSize();

    // 左右碰撞反彈
    int collideTileID = levelManager->CheckCollision(posX + velocity.x, posY, size.x, 0,
        (moveDirection == -1) ? Direction::LEFT : Direction::RIGHT);

    if (collideTileID != 0) {
        moveDirection *= -1; // 反轉方向
        velocity.x = moveSpeed * moveDirection;
    }

    // 落地檢測
    collideTileID = levelManager->CheckCollision(posX, posY + velocity.y, size.x/2, size.y, Direction::BOTTOM);
    if (collideTileID != 0) {
        SetPosition({posX, levelManager->GetTilePosition(levelManager->GetTileID(posX, posY)).y + 1});
        isOnGround = true;
        velocity.y = 0;
    } else {
        isOnGround = false;
    }
}