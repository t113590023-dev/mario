//
// Created by webbe on 2026/4/15.
//

#include "Goomba.hpp"
#include "Mario.hpp"
#include "Util/Time.hpp"

Goomba::Goomba(float startX, float startY) : Enemy("C:/Users/webbe/Documents/code/mario/Resources/Image/Enemy/Goomba_Walk1.png") {
    SetPosition({startX, startY});
    Start();
}

void Goomba::Start() {
    m_Transform.scale = {2, 2};
    SetZIndex(40);
    moveSpeed = 1.5f;
    moveDirection = -1;
    velocity = glm::vec2(moveSpeed * moveDirection, 0);

    m_WalkAnimation = std::make_shared<Util::Animation>(
        std::vector<std::string>{
            "C:/Users/webbe/Documents/code/mario/Resources/Image/Enemy/Goomba_Walk1.png",
            "C:/Users/webbe/Documents/code/mario/Resources/Image/Enemy/Goomba_Walk2.png"
        }, true, 200, true, 0);

    m_DeadImage = std::make_shared<Util::Image>("C:/Users/webbe/Documents/code/mario/Resources/Image/Enemy/Goomba_Dead.png");

    SetDrawable(m_WalkAnimation);
}

void Goomba::Update() {
    if (LevelManager::GetInstance()->IsInsideCamera(GetPosition())) {
        isActive = true;
    }
    if (!isActive) return;

    if (isDead) {
        deathTimer += Util::Time::GetDeltaTimeMs();
        if (deathTimer > 500.0f) {
            SetVisible(false);
        }
        return;
    }
    IfCollidesTile(LevelManager::GetInstance());

    if (!isOnGround) {
        velocity.y -= 0.4f;
    }
    velocity.x = moveSpeed * moveDirection;
    SetPosition(GetPosition() + velocity);
}

void Goomba::OnHit() {
    moveDirection *= -1;
}

void Goomba::OnHit(std::shared_ptr<Mario> mario) {
    if (isDead) return;

    if (mario->GetPosition().y > GetPosition().y + 5.0f && mario->GetVelocity().y < 0) {
        isDead = true;
        SetDrawable(m_DeadImage); // 換成扁掉的圖

        mario->SetVelocity({mario->GetVelocity().x, 8.0f});
    } else {
        mario->TakeDamage();
        LOG_DEBUG("Mario gets hurt by Goomba!");
    }
}