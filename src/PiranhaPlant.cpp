#include "PiranhaPlant.hpp"
#include "GameHUD.hpp"
#include "Mario.hpp"
#include "Util/Time.hpp"

PiranhaPlant::PiranhaPlant(float startX, float startY, bool isUnderGround) : Enemy(RESOURCE_DIR"/Image/Enemy/PiranhaPlant_Bite1.png") {
    startY = startY - 38;
    startX = startX + 16;
    SetPosition({startX, startY});
    m_isUnderGround = isUnderGround;
    m_StartY = startY;

    m_TargetY = startY + 48.0f;

    PiranhaPlant::Start();
}

void PiranhaPlant::Start() {
    m_Transform.scale = {2, 2};

    SetZIndex(3);

    moveSpeed = 1.0f;
    type = EnemyType::PiranhaPlant;
    if (m_isUnderGround) {
        m_BiteAnimation = std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR"/Image/Enemy/PiranhaPlant_Under1.png",
                RESOURCE_DIR"/Image/Enemy/PiranhaPlant_Under2.png"
            }, true, 200, true, 0);
    }
    else {
        m_BiteAnimation = std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR"/Image/Enemy/PiranhaPlant1.png",
                RESOURCE_DIR"/Image/Enemy/PiranhaPlant2.png"
            }, true, 200, true, 0);
    }

    SetDrawable(m_BiteAnimation);
}

void PiranhaPlant::Update() {
    if (LevelManager::GetInstance()->IsInsideCamera(GetPosition())) {
        isActive = true;
    }
    if (!isActive) return;

    if (isDead) {
        deathTimer += Util::Time::GetDeltaTimeMs();
        if (isPopup) {
            SetPosition(GetPosition() + velocity);
            velocity.y -= 0.55f;
        }
        if (deathTimer > 500.0f) {
            SetVisible(false);
        }
        return;
    }

    switch (m_State) {
        case State::Hiding:
            m_WaitTimer += Util::Time::GetDeltaTimeMs();
            if (m_WaitTimer > 2000.0f) {
                m_State = State::MovingUp;
                m_WaitTimer = 0.0f;
            }
            break;

        case State::MovingUp:
            SetPosition({GetPosition().x, GetPosition().y + moveSpeed});
            if (GetPosition().y >= m_TargetY) {
                SetPosition({GetPosition().x, m_TargetY});
                m_State = State::Exposed;
            }
            break;

        case State::Exposed:
            m_WaitTimer += Util::Time::GetDeltaTimeMs();
            if (m_WaitTimer > 2000.0f) {
                m_State = State::MovingDown;
                m_WaitTimer = 0.0f;
            }
            break;

        case State::MovingDown:
            SetPosition({GetPosition().x, GetPosition().y - moveSpeed});
            if (GetPosition().y <= m_StartY) {
                SetPosition({GetPosition().x, m_StartY});
                m_State = State::Hiding;
            }
            break;
    }
}

void PiranhaPlant::OnHit() {
}

void PiranhaPlant::OnHit(std::shared_ptr<Mario> mario) {
    if (isDead || !mario->isCollidable()) return;

    if (m_State == State::Hiding && m_WaitTimer > 1500) {
        m_WaitTimer = 1500;
    }

    if (mario->GetInvincible()) {
        int direction = (mario->GetPosition().x < GetPosition().x) ? 1 : -1;
        popDead(direction * 2);

        EffectManager::GetInstance()->ShowScoreText(GetTransform().translation, 100);
        GameHUD::GetInstance()->AddScore(100);
        return;
    }
    if (m_State != State::Hiding) {
        mario->TakeDamage();
    }
}