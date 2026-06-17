#ifndef MARIO_HPP
#define MARIO_HPP

#include "Definitions.hpp"
#include "Character.hpp"
#include "EffectManager.hpp"
#include "LevelManager.hpp"
#include "pch.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

#include "Util/Animation.hpp"
#include "Util/Color.hpp"
#include "Util/GameObject.hpp"

class Mario : public Character {
public:
    enum class State {
        IDLE,
        RUNNING,
        JUMPING,
        SLIDEING,
        ANIMATING,
        DRILLING,
        FINISHING,
        DEAD
    };

    explicit Mario(const std::string& ImagePath, std::shared_ptr<EffectManager> effectManager);

    void Start();
    void Update();
    bool IfCollidesTile(std::shared_ptr<LevelManager> levelManager);
    void ResetCameraPos() {cameraPosition = 0; }
    void SetCameraPos(float x) {cameraPosition = x; }
    void TakeDamage();
    void Dead();
    void Drill(Direction d, float distance);
    void FinishLevel();
    void SetInvincible();
    bool GetInvincible() const { return m_isInvincible; }
    MarioType GetType() const { return m_Type; }
    void ChangeType(MarioType newType, bool playAnimation = true);
    glm::vec2 GetVelocity();
    glm::vec2 GetSize() {return  m_Drawable->GetSize() * glm::abs(m_Transform.scale); }
    void SetVelocity(glm::vec2 velocity);
    void SetisOnGround(bool onGround);
    bool isAnimating() const { return m_CurrentState == State::ANIMATING; }
    bool isCollidable() const { return m_isCollidable; }
    bool isDead() const { return m_CurrentState == State::DEAD ;}
    bool GetM_isDead() const { return m_isDead; }
    void AddComboCount() { m_ComboCount++; }
    int GetComboCount() { return m_ComboCount++; }
    State GetState() { return m_CurrentState; }
private:
    void ChangeState(State newState);
private:
    State m_CurrentState = State::IDLE;
    MarioType m_Type = MarioType::SMALL;
    bool isControllable, isMovable;
    std::vector<std::shared_ptr<Util::Image>> m_IdleImages;
    std::vector<std::shared_ptr<Util::Image>> m_JumpImages;
    std::vector<std::shared_ptr<Util::Image>> m_SlideImages;
    std::shared_ptr<Util::Image> m_IdleImage;
    std::shared_ptr<Util::Image> m_JumpImage;
    std::shared_ptr<Util::Image> m_SlideImage;
    std::vector<std::string> m_RunImage;
    std::vector<std::shared_ptr<Util::Animation>> m_RunAnimations;
    std::shared_ptr<Util::Animation> m_RunAnimation;
    std::shared_ptr<Util::Image> m_DeathImage;
    std::vector<std::shared_ptr<Util::Image>> m_InvincibleIdleImage;
    std::vector<std::shared_ptr<Util::Image>> m_InvincibleJumpImage;
    std::vector<std::shared_ptr<Util::Image>> m_InvincibleSlideImage;
    std::vector<std::shared_ptr<Util::Image>> m_InvincibleRunImage;
    std::shared_ptr<Util::Animation> m_GrowAnimation;
    std::shared_ptr<Util::Animation> m_HurtAnimation;
    bool isOnGround, isMoving, isCollidesTile, m_isDead, m_isCollidable, m_isDamageable, m_isInvincible, m_isPullDownTheFlag = false;
    glm::vec2 velocity, m_size;
    float jumpTimer, cameraPosition = 0, hurtTimer, deathTimer, invincibleTimer, drillTimer, m_drillDistance, invincibleStarTimer, m_maxMoveSpeed;
    int m_ComboCount;
    glm::vec2 m_drillDirection;
    std::shared_ptr<EffectManager> effectManager;
};

#endif
