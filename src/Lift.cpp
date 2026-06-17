#include "Lift.hpp"

#include "GameHUD.hpp"
#include "Mario.hpp"
#include "Util/Time.hpp"

Lift::Lift(float startX, float startY) : Enemy(RESOURCE_DIR"/Image/Enemy/Lift_6.png") {
    SetPosition({startX, startY});
    m_orgPosition = GetPosition();
    m_direction = {0, 1};
    m_distance = 0;
    m_reverse = true;
    Lift::Start();
}

void Lift::Start() {
    m_Transform.scale = {2, 2};
    SetZIndex(40);
    type = EnemyType::LIFT;
    moveSpeed = 2;
    velocity = m_direction * moveSpeed;
    m_curDistance = 0;
}

void Lift::OnHit() {
}

void Lift::SetDirection(glm::vec2 direction) {
    m_direction = direction;
    velocity = m_direction * moveSpeed;
}


void Lift::Update() {
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

    if (m_distance == 0) {
        if (velocity.x != 0) return;

        SetPosition(GetPosition() + velocity);
        if (std::abs(GetPosition().y) > WINDOW_HEIGHT/2 + 64) {
            if (velocity.y != 0)
                SetPosition({GetPosition().x, -m_direction.y * (WINDOW_HEIGHT/2 + 32)});
        }
    }
    else {
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
}

void Lift::OnHit(std::shared_ptr<Mario> mario) {
    if (isDead) return;
    if (mario->GetPosition().y - mario->GetSize().y/2 < GetPosition().y)
        return;
    mario->SetPosition({mario->GetPosition().x + velocity.x, GetPosition().y + GetScaledSize().y/2 + mario->GetSize().y/2});
    mario->SetisOnGround(true);

}
