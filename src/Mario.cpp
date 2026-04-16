#include "Mario.hpp"

#include <cmath>

#include "imgui_internal.h"
#include "Util/Time.hpp"
#include "Util/Transform.hpp"


Mario::Mario(const std::string& ImagePath, std::shared_ptr<EffectManager> effectManager) : Character(ImagePath) {
    this->effectManager = effectManager;
}

void Mario::Start() {
    SetPosition({-449.f, -0.f});
    SetZIndex(50);
    velocity = glm::vec2(0, 0);
    isOnGround = false;
    isMoving = false;
    jumpTimer = 0;
    m_Transform.scale = glm::vec2(2, 2);
    ChangeType(MarioType::BIG);
}

bool Mario::IfCollidesTile(std::shared_ptr<LevelManager> levelManager) {
    float posX = GetPosition().x , posY = GetPosition().y;
    glm::vec2 size = GetSize();
    isCollidesTile = false;
    int collidesY = (m_Type == MarioType::SMALL)? 16: 0;

    // 左側碰撞
    int collideTileID = levelManager->CheckCollision(posX + velocity.x, posY + collidesY, size.x - 2 ,size.y - 8, Direction::LEFT);
    if (collideTileID != 0) {
        LOG_DEBUG("hit left");
        isCollidesTile = true;
        SetPosition({levelManager->GetTileById(collideTileID)->m_Transform.translation.x + 32, posY});
    }
    // 右側碰撞
    collideTileID = levelManager->CheckCollision(posX + velocity.x, posY + collidesY, size.x - 2 ,size.y - 8, Direction::RIGHT);
    if (collideTileID != 0) {
        LOG_DEBUG("hit right");
        isCollidesTile = true;
        SetPosition({levelManager->GetTileById(collideTileID)->m_Transform.translation.x - 32, posY});
    }
    // 頂方塊檢測
    collideTileID = levelManager->CheckCollision(posX, posY + velocity.y + collidesY, size.x - 16, size.y - 2, Direction::TOP);
    if (collideTileID != 0) {
        velocity.y = 0;
        LOG_DEBUG("hit" + std::to_string(collideTileID));
        std::shared_ptr<Tile> tile = levelManager->GetTileById(collideTileID);
        SetPosition({posX, tile->m_Transform.translation.y - 32});
        tile->onHit(m_Type);
        effectManager->AddTileEffect(tile);
    }
    // 落地檢測
    collideTileID = levelManager->CheckCollision(posX, posY + velocity.y, size.x - 4, size.y, Direction::BOTTOM);
    if (collideTileID != 0) {
        LOG_DEBUG("hit Ground");
        LOG_DEBUG(velocity.y);
        SetPosition({posX,levelManager->GetTileById(collideTileID)->m_Transform.translation.y + 16 + (size.y/2) + 1});
        isOnGround = true;
    }
    else {
        isOnGround = false;
    }
    return isOnGround || isCollidesTile;
}

void Mario::TakeDamage() {
    switch (m_Type) {
        case MarioType::SMALL:
            break;
        case MarioType::BIG:
            ChangeType(MarioType::SMALL);
            break;
        case MarioType::FIRE:
            ChangeType(MarioType::BIG);
            break;
    }
}


void Mario::ChangeState(State newState) {
    if (m_CurrentState == newState) return;

    m_CurrentState = newState;

    // 根據新狀態切換元件
    switch (m_CurrentState) {
        case State::IDLE:
            SetDrawable(m_IdleImage);
            break;
        case State::RUNNING:
            SetDrawable(m_RunAnimation);
            break;
        case State::JUMPING:
            SetDrawable(m_JumpImage);
            break;
        case State::SLIDEING:
            SetDrawable(m_SlideImage);
            break;
    }
}

void Mario::ChangeType(MarioType newType) {
    if (m_Type == newType) return;
    m_Type = newType;
    std::string typeName;

    switch (newType) {
        case MarioType::SMALL:
            typeName = "Small";
            break;
        case MarioType::BIG:
            typeName = "Big";
            break;
        case MarioType::FIRE:
            typeName = "Fire";
            break;
    }

    m_IdleImage = std::make_shared<Util::Image>("C:/Users/webbe/Documents/code/mario/Resources/Image/Character/Mario_" + typeName + "_Idle.png");
    m_JumpImage = std::make_shared<Util::Image>("C:/Users/webbe/Documents/code/mario/Resources/Image/Character/Mario_"+ typeName + "_Jump.png");
    m_SlideImage = std::make_shared<Util::Image>("C:/Users/webbe/Documents/code/mario/Resources/Image/Character/Mario_" + typeName + "_Slide.png");
    m_RunImage = {"C:/Users/webbe/Documents/code/mario/Resources/Image/Character/Mario_" + typeName + "_Run1.png",
        "C:/Users/webbe/Documents/code/mario/Resources/Image/Character/Mario_" + typeName + "_Run2.png",
        "C:/Users/webbe/Documents/code/mario/Resources/Image/Character/Mario_" + typeName + "_Run3.png"};
    m_RunAnimation = std::make_shared<Util::Animation>(m_RunImage, true, 100, true, 0);
}

glm::vec2 Mario::GetVelocity() {
    return velocity;
}

void Mario::SetVelocity(glm::vec2 velocity) {
    this->velocity = velocity;
}

void Mario::Update() {
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        isMoving = true;
        (velocity.x < -3)? velocity.x += 0.55: velocity.x += 0;
        (velocity.x < 5)? velocity.x += 0.10: velocity.x = 5;
        m_Transform.scale.x = 2;

    }
    else if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        isMoving = true;
        (velocity.x > 3)? velocity.x -= 0.55: velocity.x += 0;
        (velocity.x > -5)? velocity.x -= 0.10: velocity.x = -5;
        m_Transform.scale.x = -2;
    }
    else {
        isMoving = false;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::SPACE) && isOnGround) {
        isOnGround = false;
        velocity.y += 8.5;
        jumpTimer = 350;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::SPACE) && jumpTimer > 0) {
        jumpTimer -= Util::Time::GetDeltaTimeMs();
        velocity.y += 0.5;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::SPACE) && jumpTimer > 0) {
        jumpTimer = 0;
    }

    if (!isOnGround) {
        ChangeState(State::JUMPING);
    }
    else if (isMoving) {
        m_RunAnimation->SetInterval(100/(abs(velocity.x/4) + 1));
        ChangeState(State::RUNNING);
        if (velocity.x > 0 && Util::Input::IsKeyPressed(Util::Keycode::A) && !Util::Input::IsKeyPressed(Util::Keycode::D))
            ChangeState(State::SLIDEING);
        if (velocity.x < 0 && Util::Input::IsKeyPressed(Util::Keycode::D) && !Util::Input::IsKeyPressed(Util::Keycode::A))
            ChangeState(State::SLIDEING);
    }
    else {
        ChangeState(State::IDLE);
    }

    if (!isMoving) {
        if (velocity.x > 0) {
            velocity.x -= 0.18;
            if (velocity.x < 0) velocity.x = 0;
        }
        if (velocity.x < 0) {
            velocity.x += 0.18;
            if (velocity.x > 0) velocity.x = 0;
        }
    }

    IfCollidesTile(LevelManager::GetInstance());

    if (!isOnGround) {
        if (velocity.y > -10)
            velocity.y -= 0.765;
    }
    else {
        if (velocity.y < 8)
            velocity.y = 0;
    }
    //移動限制
    if (GetPosition().x <= -(WINDOW_WIDTH/2.f) + 16 + cameraPosition) {
        velocity.x = velocity.x < 0? 0: velocity.x;
    }

    //碰撞
    if (isCollidesTile) {
        velocity.x = 0;
    }

    SetPosition(glm::round(GetPosition() + velocity));
}

