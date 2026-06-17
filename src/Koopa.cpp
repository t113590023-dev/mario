#include "Koopa.hpp"

#include "GameHUD.hpp"
#include "Mario.hpp"

Koopa::Koopa(float startX, float startY, bool isUnderGround) : Enemy(RESOURCE_DIR"/Image/Enemy/Koopa_Walk1.png") {
    SetPosition({startX, startY});
    m_isUnderGround = isUnderGround;
    this->Koopa::Start();
}

void Koopa::Start() {
    m_Transform.scale = {2, 2};
    SetZIndex(40);
    moveSpeed = 1.5f;
    moveDirection = -1;
    m_orgPosition = GetPosition();

    m_direction = {0, 1.0f};
    m_distance = 64;
    SetComboCount(0);
    velocity = glm::vec2(moveSpeed * moveDirection, 0);
    m_State = KoopaState::WALKING;
    type = EnemyType::Koopa;

    if (m_isUnderGround) {
        m_FlyingAnimation = std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR"/Image/Enemy/Koopa_Under_Flying1.png",
                RESOURCE_DIR"/Image/Enemy/Koopa_Under_Flying2.png"
            }, true, 200, true, 0);

        m_WalkAnimation = std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR"/Image/Enemy/Koopa_Under_Walk1.png",
                RESOURCE_DIR"/Image/Enemy/Koopa_Under_Walk2.png"
            }, true, 200, true, 0);

        m_ShellImage = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Enemy/Koopa_Under_Shell.png");
    }else {
        m_FlyingAnimation = std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR"/Image/Enemy/Koopa_Flying1.png",
                RESOURCE_DIR"/Image/Enemy/Koopa_Flying2.png"
            }, true, 200, true, 0);

        m_WalkAnimation = std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR"/Image/Enemy/Koopa_Walk1.png",
                RESOURCE_DIR"/Image/Enemy/Koopa_Walk2.png"
            }, true, 200, true, 0);

        m_ShellImage = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Enemy/Koopa_Shell.png");
    }

    SetDrawable(m_WalkAnimation);
}

void Koopa::SetFlying() {
    SetDrawable(m_FlyingAnimation);
    m_State = KoopaState::FLYING;
}

void Koopa::Update() {
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

    if (!isOnGround && m_State != KoopaState::FLYING) {
        velocity.y -= 0.4f;
    }

    if (m_State == KoopaState::WALKING) {
        velocity.x = moveSpeed * moveDirection;
        m_Transform.scale.x = (moveDirection == 1) ? -2 : 2;
    } else if (m_State == KoopaState::SHELL_IDLE) {
        velocity.x = 0;
    } else if (m_State == KoopaState::SHELL_MOVING) {
        velocity.x = moveSpeed * 3.0f * moveDirection;
    } else if (m_State == KoopaState::FLYING) {
        int factor = m_distance - m_curDistance;
        if (factor < m_distance*0.2) {
            velocity = m_direction * moveSpeed * (0.1f + 0.9f*factor/(m_distance*0.2f));
        }
        else if (factor > m_distance*0.8) {
            velocity = m_direction * moveSpeed * (0.1f + 0.9f*m_curDistance/(m_distance*0.2f));
        }
        else {
            velocity = m_direction * moveSpeed;
        }
        SetPosition(GetPosition() + velocity);
        m_curDistance += glm::max(std::abs(velocity.x), std::abs(velocity.y));
        if (m_curDistance >= m_distance) {
            m_direction = -m_direction;
            m_curDistance = 0;
            m_reverse = !m_reverse;
            if (m_reverse) SetPosition(m_orgPosition);
        }
    }


    SetPosition(glm::round(GetPosition() + velocity));
}

void Koopa::OnHit() {
    moveDirection *= -1;
}

void Koopa::ChangeColorRed() {
    m_FlyingAnimation = std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR"/Image/Enemy/Koopa_Red_Flying1.png",
                RESOURCE_DIR"/Image/Enemy/Koopa_Red_Flying2.png"
            }, true, 200, true, 0);

    m_WalkAnimation = std::make_shared<Util::Animation>(
        std::vector<std::string>{
            RESOURCE_DIR"/Image/Enemy/Koopa_Red_Walk1.png",
            RESOURCE_DIR"/Image/Enemy/Koopa_Red_Walk2.png"
        }, true, 200, true, 0);

    m_ShellImage = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Enemy/Koopa_Red_Shell.png");
}

void Koopa::OnHit(std::shared_ptr<Mario> mario) {
    if (isDead || !mario->isCollidable()) return;

    if (mario->GetInvincible()) {
        popDead(-moveDirection*2);
        return;
    }
    bool isMarioFalling = (mario->GetVelocity().y < 0);
    bool hitFromTop = (mario->GetPosition().y > GetPosition().y + 10.0f);

    if (m_State == KoopaState::WALKING) {
        if (hitFromTop && isMarioFalling) {
            m_State = KoopaState::SHELL_IDLE;
            SetDrawable(m_ShellImage);
            mario->SetVelocity({mario->GetVelocity().x, 8.0f});
            mario->SetPosition({mario->GetPosition().x, GetPosition().y + GetScaledSize().y/2 + mario->GetSize().y/2 + 2});
            SFXManager::GetInstance()->Play("stomp");
            int score = comboScore[mario->GetComboCount()];
            EffectManager::GetInstance()->ShowScoreText(GetTransform().translation, score);
            GameHUD::GetInstance()->AddScore(score);
            SetComboCount(0);
        } else {
            mario->TakeDamage();
        }
    }
    else if (m_State == KoopaState::SHELL_IDLE) {
        m_State = KoopaState::SHELL_MOVING;
        SetComboCount(0);

        if (mario->GetPosition().x < GetPosition().x) {
            moveDirection = 1;
        } else {
            moveDirection = -1;
        }

        if (hitFromTop && isMarioFalling) {
            mario->SetVelocity({mario->GetVelocity().x, 8.0f});
            mario->SetPosition({mario->GetPosition().x, GetPosition().y + GetScaledSize().y/2 + mario->GetSize().y/2 + 2});
            SFXManager::GetInstance()->Play("stomp");
            int score = comboScore[mario->GetComboCount()];
            EffectManager::GetInstance()->ShowScoreText(GetTransform().translation, score);
            GameHUD::GetInstance()->AddScore(score);
        }
    }
    else if (m_State == KoopaState::SHELL_MOVING) {
        if (hitFromTop && isMarioFalling) {
            m_State = KoopaState::SHELL_IDLE;
            mario->SetVelocity({mario->GetVelocity().x, 8.0f});
            mario->SetPosition({mario->GetPosition().x, GetPosition().y + GetScaledSize().y/2 + mario->GetSize().y/2 + 2});
            SFXManager::GetInstance()->Play("stomp");
            int score = comboScore[mario->GetComboCount()];
            EffectManager::GetInstance()->ShowScoreText(GetTransform().translation, score);
            GameHUD::GetInstance()->AddScore(score);
        } else {
            mario->TakeDamage();
        }
    }
    else if (m_State == KoopaState::FLYING) {
        if (hitFromTop && isMarioFalling) {
            m_State = KoopaState::WALKING;
            SetDrawable(m_WalkAnimation);
            mario->SetVelocity({mario->GetVelocity().x, 8.0f});
            mario->SetPosition({mario->GetPosition().x, GetPosition().y + GetScaledSize().y/2 + mario->GetSize().y/2 + 2});
            SFXManager::GetInstance()->Play("stomp");
            int score = comboScore[mario->GetComboCount()];
            EffectManager::GetInstance()->ShowScoreText(GetTransform().translation, score);
            GameHUD::GetInstance()->AddScore(score);
            SetComboCount(0);
        } else {
            mario->TakeDamage();
        }
    }
}