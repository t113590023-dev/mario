#include "Mario.hpp"

#include <cmath>
#include <complex>

#include "GameHUD.hpp"
#include "imgui_internal.h"
#include "SFXManager.hpp"
#include "Util/Time.hpp"
#include "Util/Transform.hpp"


Mario::Mario(const std::string& ImagePath, std::shared_ptr<EffectManager> effectManager) : Character(ImagePath) {
    this->effectManager = effectManager;
}

void Mario::Start() {
    SetPosition({-449.f, -48.f});
    SetZIndex(50);
    m_CurrentState = State::IDLE;
    velocity = glm::vec2(0, 0);
    isOnGround = false;
    m_isDead = false;
    isMoving = false;
    isControllable = true;
    m_isCollidable = true;
    m_isDamageable = true;
    m_isInvincible = false;
    m_isPullDownTheFlag = false;
    isMovable = true;
    jumpTimer = 0;
    invincibleTimer = 0;
    drillTimer = 0;
    m_ComboCount = 0;
    m_maxMoveSpeed = 5;
    m_Transform.scale = glm::vec2(2, 2);

    std::vector<std::string> typeNames = {"Small", "Big", "Fire"};
    for (int i=0; i<typeNames.size(); i++) {
        std::string typeName = typeNames[i];
        m_IdleImages.push_back(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Mario_" + typeName + "_Idle.png"));
        m_JumpImages.push_back(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Mario_"+ typeName + "_Jump.png"));
        m_SlideImages.push_back(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Mario_" + typeName + "_Slide.png"));
        m_RunImage = {RESOURCE_DIR"/Image/Character/Mario_" + typeName + "_Run1.png",
            RESOURCE_DIR"/Image/Character/Mario_" + typeName + "_Run2.png",
            RESOURCE_DIR"/Image/Character/Mario_" + typeName + "_Run3.png"};
        m_RunAnimations.push_back(std::make_shared<Util::Animation>(m_RunImage, true, 100, true, 0));
    }

    m_DeathImage = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Mario_Small_Death.png");
    std::vector<std::string> growImage {RESOURCE_DIR"/Image/Character/Mario_Small_Idle.png",
                                        RESOURCE_DIR"/Image/Character/Mario_Grow_Idle.png",
                                        RESOURCE_DIR"/Image/Character/Mario_Small_Idle.png",
                                        RESOURCE_DIR"/Image/Character/Mario_Grow_Idle.png",
                                        RESOURCE_DIR"/Image/Character/Mario_Big_Idle.png",
                                        RESOURCE_DIR"/Image/Character/Mario_Grow_Idle.png",
                                        RESOURCE_DIR"/Image/Character/Mario_Big_Idle.png"};

    m_InvincibleIdleImage = { std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Small_Idle1.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Small_Idle2.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Small_Idle3.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Small_Idle4.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Big_Idle1.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Big_Idle2.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Big_Idle3.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Big_Idle4.png")
    };
    m_InvincibleJumpImage = { std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Small_Jump1.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Small_Jump2.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Small_Jump3.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Small_Jump4.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Big_Jump1.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Big_Jump2.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Big_Jump3.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Big_Jump4.png"),
    };
    m_InvincibleSlideImage = { std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Small_Slide1.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Small_Slide2.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Small_Slide3.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Small_Slide4.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Big_Slide1.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Big_Slide2.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Big_Slide3.png"),
                              std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Invincible/Mario_Big_Slide4.png"),
    };
    m_InvincibleRunImage.clear();
    for (int i=0; i<=1; i++) {
        for (int j=1; j<=3; j++) {
            for (int k=1; k<=4; k++) {
                std::string size = (i==0)? "Small": "Big";
                std::string path = RESOURCE_DIR "/Image/Character/Invincible/Mario_"
                                       + size + "_Run"
                                       + std::to_string(j) + "_"
                                       + std::to_string(k) + ".png";
                m_InvincibleRunImage.emplace_back(std::make_shared<Util::Image>(path));
            }
        }
    }
    m_GrowAnimation = std::make_shared<Util::Animation>(growImage, false, 100, false, 0);
    std::reverse(growImage.begin(), growImage.end());
    m_HurtAnimation = std::make_shared<Util::Animation>(growImage, false, 100, false, 0);
    ChangeType(MarioType::SMALL, false);
    m_size = GetSize();
}

bool Mario::IfCollidesTile(std::shared_ptr<LevelManager> levelManager) {
    float posX = GetPosition().x , posY = GetPosition().y;
    glm::vec2 size = GetSize();
    isCollidesTile = false;
    int collidesY = (m_Type == MarioType::SMALL)? 16: 0;

    // 落地檢測
    int collideTileID = levelManager->CheckCollision(posX, posY + velocity.y, size.x - 4, size.y - 4, Direction::BOTTOM);
    if (collideTileID != -1 && LevelManager::GetInstance()->GetTileById(collideTileID)->isCollidable()) {
        SetPosition({posX,levelManager->GetTileById(collideTileID)->m_Transform.translation.y + 16 + (size.y/2) + 1});
        posX = GetPosition().x , posY = GetPosition().y;
        m_ComboCount = 0;
        isOnGround = true;
    }
    else {
        isOnGround = false;
    }
    // 左側碰撞
    collideTileID = levelManager->CheckCollision(posX + velocity.x, posY + collidesY, size.x - 3 ,size.y - 4, Direction::LEFT);
    if (collideTileID != -1 && LevelManager::GetInstance()->GetTileById(collideTileID)->isCollidable()) {
        LOG_DEBUG("hit left");
        isCollidesTile = true;
        SetPosition({levelManager->GetTileById(collideTileID)->m_Transform.translation.x + 32 + 1, posY});
        posX = GetPosition().x , posY = GetPosition().y;
    }
    // 右側碰撞
    collideTileID = levelManager->CheckCollision(posX + velocity.x, posY + collidesY, size.x - 3 ,size.y - 4, Direction::RIGHT);
    if (collideTileID != -1 && LevelManager::GetInstance()->GetTileById(collideTileID)->isCollidable()) {
        LOG_DEBUG("hit right");
        isCollidesTile = true;
        SetPosition({levelManager->GetTileById(collideTileID)->m_Transform.translation.x - 32 - 1, posY});
        posX = GetPosition().x , posY = GetPosition().y;
    }
    // 頂方塊檢測
    collideTileID = levelManager->CheckCollision(posX, posY + velocity.y + collidesY, size.x - 16, size.y - 4, Direction::TOP);
    if (collideTileID != -1) {
        velocity.y = 0;
        LOG_DEBUG("hit" + std::to_string(collideTileID));
        std::shared_ptr<Tile> tile = levelManager->GetTileById(collideTileID);
        SetPosition({posX, tile->m_Transform.translation.y - 32});
        tile->onHit(m_Type);
        effectManager->AddTileEffect(tile);
    }
    return isOnGround || isCollidesTile;
}

void Mario::TakeDamage() {
    if (!m_isDamageable) return;
    invincibleTimer = 3000;
    switch (m_Type) {
        case MarioType::SMALL:
            Dead();
            break;
        case MarioType::BIG:
            ChangeType(MarioType::SMALL);
            SFXManager::GetInstance()->Play("pipe");
            break;
        case MarioType::FIRE:
            ChangeType(MarioType::SMALL);
            SFXManager::GetInstance()->Play("pipe");
            break;
    }
}

void Mario::Dead() {
    if (m_isDead) return;

    this->velocity = {0, 10};
    SetDrawable(m_DeathImage);
    deathTimer = 2000;
    m_isDead = true;
    m_isCollidable = false;
    GameHUD::GetInstance()->loseLife();
    GameHUD::GetInstance()->TimeStop();
    SFXManager::GetInstance()->Pause();
    SFXManager::GetInstance()->Play("mariodie");
}

void Mario::Drill(Direction d, float detance) {
    isControllable = false, isMovable = false, m_isCollidable = false;
    drillTimer = 2000;
    m_drillDistance = detance;
    SetZIndex(-20);
    SetVisible(true);
    SFXManager::GetInstance()->Play("pipe");
    if (d == Direction::BOTTOM) {
        ChangeState(State::DRILLING);
        SetDrawable(m_IdleImage);
        m_drillDirection = {0, -2};
        return;
    }
    if (d == Direction::RIGHT || d == Direction::LEFT) {
        ChangeState(State::DRILLING);
        SetDrawable(m_RunAnimation);
        float direction = (d == Direction::RIGHT)? 2: -2;
        m_Transform.scale.x = direction;
        m_drillDirection = {direction, 0};
    }
}

void Mario::FinishLevel() {
    LOG_DEBUG("FinishLevel");
    isControllable = false, isMovable = false, m_isCollidable = false;
    SFXManager::GetInstance()->Pause();
    SFXManager::GetInstance()->Play("flagpole");
    GameHUD::GetInstance()->TimeStop();
    drillTimer = 3000;
    m_drillDistance = 32 * 5;
    SetVisible(true);
    ChangeState(State::FINISHING);
    SetDrawable(m_JumpImage);
    float direction = 2;
    m_Transform.scale.x = direction;
    m_drillDirection = {direction, 0};
    int y = GetPosition().y, score;
    if (y > 150)
        score = 5000;
    else if (y > 16)
        score = 2000;
    else if (y > -48)
        score = 800;
    else if (y > -80)
        score = 400;
    else
        score = 100;

    EffectManager::GetInstance()->ShowScoreText(GetTransform().translation + glm::vec2{16, 0}, score);
    GameHUD::GetInstance()->AddScore(score);
}

void Mario::SetInvincible() {
    m_isInvincible = true;
    invincibleStarTimer = 12000;
}

void Mario::ChangeState(State newState) {
    if (m_isInvincible) {
        m_CurrentState = newState;
        int type = 0;
        switch (GetType()) {
            case MarioType::SMALL:
                type = 0;
                break;
            case MarioType::BIG:
                type = 1;
                break;
            case MarioType::FIRE:
                type = 1;
                break;
        }

        int invIndex = static_cast<int>(floor(invincibleStarTimer / 30)) % 4;
        if (invincibleStarTimer < 2700) invIndex = static_cast<int>(floor(invincibleStarTimer / 120)) % 4;
        int runIndex = static_cast<int>(floor(invincibleStarTimer / 100)) % 3;
        switch (m_CurrentState) {
            case State::IDLE:
                SetDrawable(m_InvincibleIdleImage[type*4 + invIndex]);
                break;
            case State::RUNNING:
                SetDrawable(m_InvincibleRunImage[type*12 + runIndex*4 + invIndex]);
                break;
            case State::JUMPING:
                SetDrawable(m_InvincibleJumpImage[type*4 + invIndex]);
                break;
            case State::SLIDEING:
                SetDrawable(m_InvincibleSlideImage[type*4 + invIndex]);
                break;
            default:
                break;
        }
        SetVisible(true);
        return;
    }

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
        default:
            break;
    }
}

void Mario::ChangeType(MarioType newType, bool playAnimation) {
    if (m_Type == newType && playAnimation) return;
    m_Type = newType;
    int typeIndex;

    switch (newType) {
        case MarioType::SMALL:
            typeIndex = 0;
            break;
        case MarioType::BIG:
            typeIndex = 1;
            break;
        case MarioType::FIRE:
            typeIndex = 2;
            break;
        default:
            typeIndex = 0;
    }

    m_IdleImage = m_IdleImages[typeIndex];
    m_JumpImage = m_JumpImages[typeIndex];
    m_SlideImage = m_SlideImages[typeIndex];
    m_RunAnimation = m_RunAnimations[typeIndex];
    if (newType == MarioType::BIG && playAnimation) {
        SetDrawable(m_GrowAnimation);
        m_GrowAnimation->Play();
    }
    else if (newType == MarioType::SMALL && playAnimation) {
        SetDrawable(m_HurtAnimation);
        m_HurtAnimation->Play();
    }
    else
        m_Drawable = m_IdleImage;
}

glm::vec2 Mario::GetVelocity() {
    return velocity;
}

void Mario::SetVelocity(glm::vec2 velocity) {
    this->velocity = velocity;
}

void Mario::SetisOnGround(bool onGround) {
    isOnGround = onGround;
    if (!isOnGround && !isAnimating()) {
        if (velocity.y > -10)
            velocity.y -= 0.765;
    }
    else {
        if (velocity.y < 8)
            velocity.y = 0;
    }
}


void Mario::Update() {
    if (m_isDead == true) {
        velocity.y -= 0.42;
        SetPosition(glm::round(GetPosition() + velocity));
        if (deathTimer > 0) {
            deathTimer -= Util::Time::GetDeltaTimeMs();
            if (deathTimer <= 0) {
                ChangeState(State::DEAD);
                LevelManager::GetInstance()->SetNeedReload();
                LevelManager::GetInstance()->SetMarioType(MarioType::SMALL);
            }
        }
        return;
    }

    if (m_size != GetSize()) {
        SetPosition(GetPosition() + glm::vec2{0, (GetSize().y - m_size.y)/2.0f});
        m_size = GetSize();
    }

    if (m_CurrentState == State::DRILLING) {
        if (m_drillDistance >= 0) {
            SetPosition(glm::round(GetPosition() + m_drillDirection));
            float d = m_drillDirection.x == 0? std::abs(m_drillDirection.y) : std::abs(m_drillDirection.x);
            m_drillDistance -= d;
            if (m_drillDistance < 0) {
                SetDrawable(m_IdleImage);
                SetZIndex(-20);
            }
        }
        if (drillTimer <= 0) {
            LevelManager::GetInstance()->SetNeedReload();
            LevelManager::GetInstance()->SetMarioType(GetType());
        }
        else
            drillTimer -= Util::Time::GetDeltaTimeMs();
        return;
    }

    if (m_CurrentState == State::FINISHING) {
        if (GetPosition().y - GetSize().y/2 >= -112 && !m_isPullDownTheFlag) {
            SetPosition(glm::round(GetPosition() + glm::vec2{0, -3}));
            if (GetPosition().y - GetSize().y/2 < -112) {
                SetPosition(glm::round(GetPosition() + glm::vec2{48, 0}));
                m_Transform.scale.x = -2;
                GameHUD::GetInstance()->FinishLevel();
                SFXManager::GetInstance()->Play("stage_clear");
                m_isPullDownTheFlag = true;
            }
            return;
        }
        if (m_drillDistance >= 0 && drillTimer <= 2000) {
            IfCollidesTile(LevelManager::GetInstance());
            if (!isOnGround && m_drillDirection.y > -10) m_drillDirection.y -= 0.765;
            else m_drillDirection.y = 0;
            SetDrawable(m_RunAnimation);
            m_Transform.scale.x = 2;
            SetPosition(glm::round(GetPosition() + m_drillDirection));
            float d = m_drillDirection.x == 0? std::abs(m_drillDirection.y) : std::abs(m_drillDirection.x);
            m_drillDistance -= d;
            if (m_drillDistance < 0) {
                SetDrawable(m_IdleImage);
                SetZIndex(-20);
            }
        }
        if (drillTimer > 0)
            drillTimer -= Util::Time::GetDeltaTimeMs();
        return;
    }

    if (m_isInvincible) {
        if (invincibleStarTimer == 12000) {
            SFXManager::GetInstance()->PlayInvincibilityBGM();
        }
        invincibleStarTimer -= Util::Time::GetDeltaTimeMs();
        if (invincibleStarTimer <= 0) {
            m_isInvincible = false;
            SFXManager::GetInstance()->PlayCurrentBGM();
        }
    }

    if (m_GrowAnimation->GetState() == Util::Animation::State::PLAY || m_HurtAnimation->GetState() == Util::Animation::State::PLAY) {
        isControllable = false, isMovable = false;
        SetVisible(true);
        m_CurrentState = State::ANIMATING;
    }
    else {
        isControllable = true, isMovable = true;
        ChangeState(State::IDLE);
    }

    if (invincibleTimer > 0 && !isAnimating()) {
        m_isDamageable = false;
        bool isVisible = (static_cast<int>(Util::Time::GetElapsedTimeMs()) % 2) == 0;
        SetVisible(isVisible);
        invincibleTimer -= Util::Time::GetDeltaTimeMs();
        if (invincibleTimer <= 0) {
            m_isDamageable = true;
            SetVisible(true);
        }
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::LSHIFT) && isControllable) {
        m_maxMoveSpeed = 6.0f;
    } else {
        m_maxMoveSpeed = 4.5f;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::LSHIFT) && isControllable) {
        if (GetType() == MarioType::FIRE && ItemManager::GetInstance()->GetFireballCount() < 2) {
            ItemManager::GetInstance()->ShootFireball(GetPosition() + glm::vec2{0, GetSize().y/2}, m_Transform.scale.x / 2);
            SFXManager::GetInstance()->Play("fireball");
        }
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::D) && isControllable) {
        isMoving = true;
        (velocity.x < -3)? velocity.x += 0.55: velocity.x += 0;
        (velocity.x < m_maxMoveSpeed)? velocity.x += 0.10: velocity.x = m_maxMoveSpeed;
        if (isOnGround && isControllable) m_Transform.scale.x = 2;

    }
    else if (Util::Input::IsKeyPressed(Util::Keycode::A) && isControllable) {
        isMoving = true;
        (velocity.x > 3)? velocity.x -= 0.55: velocity.x += 0;
        (velocity.x > -m_maxMoveSpeed)? velocity.x -= 0.10: velocity.x = -m_maxMoveSpeed;
        if (isOnGround && isControllable) m_Transform.scale.x = -2;
    }
    else {
        isMoving = false;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::SPACE) && isOnGround && isControllable) {
        isOnGround = false;
        velocity.y += 8.5;
        jumpTimer = 350;
        if (velocity.x > 4.5 || velocity.x < -4.5)
            SFXManager::GetInstance()->Play("jump-super");
        else
            SFXManager::GetInstance()->Play("jump-small");
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::SPACE) && jumpTimer > 0) {
        jumpTimer -= Util::Time::GetDeltaTimeMs();
        velocity.y += 0.5;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::SPACE) && jumpTimer > 0) {
        jumpTimer = 0;
    }

    if (!isOnGround && !isAnimating()) {
        ChangeState(State::JUMPING);
    }
    else if (isMoving && !isAnimating()) {
        m_RunAnimation->SetInterval(100/(abs(velocity.x/4) + 1));
        ChangeState(State::RUNNING);
        if (velocity.x > 0 && Util::Input::IsKeyPressed(Util::Keycode::A) && !Util::Input::IsKeyPressed(Util::Keycode::D))
            ChangeState(State::SLIDEING);
        if (velocity.x < 0 && Util::Input::IsKeyPressed(Util::Keycode::D) && !Util::Input::IsKeyPressed(Util::Keycode::A))
            ChangeState(State::SLIDEING);
    }
    else if (!isAnimating()) {
        ChangeState(State::IDLE);
    }

    if (!isMoving && !isAnimating()) {
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

    if (!isOnGround && !isAnimating()) {
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
    if (GetPosition().y <= -(WINDOW_WIDTH/2.f)) {
        Dead();
    }

    //碰撞
    if (isCollidesTile) {
        velocity.x = 0;
    }

    if (isMovable) SetPosition(glm::round(GetPosition() + velocity));
}

