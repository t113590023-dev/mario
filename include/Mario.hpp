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
#include "Util/GameObject.hpp"

class Mario : public Character {
public:
    enum class State {
        IDLE,
        RUNNING,
        JUMPING,
        SLIDEING
    };

    explicit Mario(const std::string& ImagePath, std::shared_ptr<EffectManager> effectManager);

    void Start();
    void Update();
    bool IfCollidesTile(std::shared_ptr<LevelManager> levelManager);
    void ResetCameraPos() {cameraPosition = 0; }
    void SetCameraPos(float x) {cameraPosition = x; }
    void TakeDamage();
    MarioType GetType() const { return m_Type; }
    void ChangeType(MarioType newType);
    glm::vec2 GetVelocity();
    glm::vec2 GetSize() {return  m_Drawable->GetSize() * glm::abs(m_Transform.scale); }
    void SetVelocity(glm::vec2 velocity);
private:
    void ChangeState(State newState);
private:
    State m_CurrentState = State::IDLE;
    MarioType m_Type = MarioType::SMALL;
    std::shared_ptr<Util::Image> m_IdleImage;
    std::shared_ptr<Util::Image> m_JumpImage;
    std::shared_ptr<Util::Image> m_SlideImage;
    std::vector<std::string> m_RunImage;
    std::shared_ptr<Util::Animation> m_RunAnimation;
    bool isOnGround, isMoving, isCollidesTile;
    glm::vec2 velocity;
    float jumpTimer, cameraPosition = 0;
    std::shared_ptr<EffectManager> effectManager;
};

#endif
