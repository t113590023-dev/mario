//
// Created by webbe on 2026/4/15.
//

#include "Goomba.hpp"

#include "GameHUD.hpp"
#include "Mario.hpp"
#include "Util/Time.hpp"

Goomba::Goomba(float startX, float startY, bool isUnderGround) : Enemy(RESOURCE_DIR"/Image/Enemy/Goomba_Walk1.png") {
    SetPosition({startX, startY});
    m_isUnderGround = isUnderGround;
    Goomba::Start();
}

void Goomba::Start() {
    m_Transform.scale = {2, 2};
    SetZIndex(40);
    moveSpeed = 1.5f;
    moveDirection = -1;
    type = EnemyType::Goomba;
    velocity = glm::vec2(moveSpeed * moveDirection, 0);

    if (m_isUnderGround) {
        m_WalkAnimation = std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR"/Image/Enemy/Goomba_Under_Walk1.png",
                RESOURCE_DIR"/Image/Enemy/Goomba_Under_Walk2.png"
            }, true, 200, true, 0);

        m_DeadImage = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Enemy/Goomba_Under_Dead.png");
    }else {
        m_WalkAnimation = std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR"/Image/Enemy/Goomba_Walk1.png",
                RESOURCE_DIR"/Image/Enemy/Goomba_Walk2.png"
            }, true, 200, true, 0);

        m_DeadImage = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Enemy/Goomba_Dead.png");
    }

    SetDrawable(m_WalkAnimation);
}

void Goomba::Update() {
    if (LevelManager::GetInstance()->IsInsideCamera(GetPosition())) {
        isActive = true;
    }
    if (!isActive) return;

    if (isDead) {
        deathTimer += Util::Time::GetDeltaTimeMs();
        if (isPopup) {
            SetPosition(GetPosition() + velocity);
            velocity.y -= 0.55;
        }
        if (deathTimer > 500.0f) {
            SetVisible(false);
        }
        return;
    }
    IfCollidesTile(LevelManager::GetInstance());
    int id = LevelManager::GetInstance()->CheckCollision(GetPosition().x, GetPosition().y, GetScaledSize().x, GetScaledSize().y, Direction::BOTTOM);
    if (id != -1 && !LevelManager::GetInstance()->GetTileById(id)->isFinishEffect()) popDead(-velocity.x);
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
    if (isDead || !mario->isCollidable()) return;

    if (mario->GetInvincible()) {
        popDead(-moveDirection*2);
        return;
    }

    if (mario->GetPosition().y > GetPosition().y + 5.0f && mario->GetVelocity().y < 0) {
        isDead = true;
        isPopup = false;
        SetDrawable(m_DeadImage);
        int score = comboScore[mario->GetComboCount()];
        EffectManager::GetInstance()->ShowScoreText(GetTransform().translation, score);
        GameHUD::GetInstance()->AddScore(score);
        mario->SetPosition({mario->GetPosition().x, GetPosition().y + GetScaledSize().y/2 + mario->GetSize().y/2 + 2});

        mario->SetVelocity({mario->GetVelocity().x, 8.0f});
        SFXManager::GetInstance()->Play("stomp");
    } else {
        mario->TakeDamage();
    }
}