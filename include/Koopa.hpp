#ifndef MARIO_KOOPA_HPP
#define MARIO_KOOPA_HPP

#include "Enemy.hpp"
#include "Util/Animation.hpp"

enum class KoopaState {
    FLYING,
    WALKING,
    SHELL_IDLE,
    SHELL_MOVING
};

class Koopa : public Enemy {
public:
    Koopa(float startX, float startY, bool isUnderGround = false);

    void Start() override;
    void Update() override;
    void OnHit() override;
    void OnHit(std::shared_ptr<Mario> mario) override;
    bool isShellMoving() { return m_State == KoopaState::SHELL_MOVING; }
    void SetComboCount(const int comboCount) { m_comboCount = comboCount; }
    void AddComboCount() { m_comboCount += 1; }
    int GetComboCount() const { return m_comboCount; }
    void ChangeColorRed();
    void SetFlying();

private:
    KoopaState m_State;
    int m_comboCount;
    glm::vec2 m_direction, m_orgPosition;
    int m_distance;
    float m_curDistance = 0;
    bool m_reverse = true;
    bool m_isUnderGround = false;

    std::shared_ptr<Util::Animation> m_WalkAnimation;
    std::shared_ptr<Util::Animation> m_FlyingAnimation;
    std::shared_ptr<Util::Image> m_ShellImage;
};

#endif