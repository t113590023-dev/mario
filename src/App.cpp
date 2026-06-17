#include "App.hpp"

#include "Core/Context.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
    m_LevelManager = std::make_shared<LevelManager>();
    m_EffectManager = std::make_shared<EffectManager>();
    m_ItemManager = std::make_shared<ItemManager>();
    m_SFXManager = std::make_shared<SFXManager>();
    m_GameHUD = std::make_shared<GameHUD>();
    LevelManager::Init(m_LevelManager);
    EffectManager::Init(m_EffectManager);
    ItemManager::Init(m_ItemManager);
    SFXManager::Init(m_SFXManager);
    GameHUD::Init(m_GameHUD);
    m_c = std::make_shared<Util::GameObject>();
    m_Mario = std::make_shared<Mario>(RESOURCE_DIR"/Image/Character/Mario_Big_Idle.png", m_EffectManager);
    m_c->m_Transform.translation = {234, -32};
    m_c->AddChild(m_GameHUD);
    m_c->AddChild(m_Mario);
    m_c->AddChild(m_EffectManager);
    m_c->AddChild(m_ItemManager);
    m_c->AddChild(m_LevelManager);
    m_Root.AddChild(m_c);
    m_Mario->Start();
    m_LevelManager->LoadLevelJSON(RESOURCE_DIR"/Level1_1.json");
    m_c->AddChildren(m_LevelManager->GetStaticTiles());
}

void App::Update() {
    float dt = Util::Time::GetDeltaTimeMs();
    if (-m_Mario->GetPosition().x < m_c->m_Transform.translation.x && m_LevelManager->isCameraMove())
        m_c->m_Transform.translation = {-m_Mario->GetPosition().x, m_c->m_Transform.translation.y};

    m_Mario->SetCameraPos(-m_c->m_Transform.translation.x);
    m_LevelManager->SetCameraPosition(m_c->m_Transform.translation);
    m_EffectManager->m_Transform.translation = m_c->m_Transform.translation;
    m_ItemManager->m_Transform.translation = m_c->m_Transform.translation;
    m_LevelManager->m_Transform.translation = m_c->m_Transform.translation;
    m_GameHUD->Update(m_Mario, Util::Time::GetDeltaTimeMs());
    if (m_GameHUD->GetState() == HUDState::TITLESCREEN) {
        m_Mario->SetVisible(false);
        if (Util::Input::IsKeyUp(Util::Keycode::RETURN)) {
            m_GameHUD->PlayLevelIntro();
            m_Mario->SetVisible(true);
        }
    }
    if (m_GameHUD->GetState() == HUDState::HUD || m_GameHUD->GetState() == HUDState::FINISHLEVEL) {
        m_Mario->Update();
        if (!(Util::Input::IsKeyPressed(Util::Keycode::P) || m_Mario->isAnimating())) {
            m_EffectManager->Update(dt);
            m_ItemManager->UpdateItems(dt, m_Mario);
            m_LevelManager->Update(m_Mario);
            if (m_GameHUD->GetTime() == 0 && m_GameHUD->GetState() == HUDState::HUD) {
                m_Mario->Dead();
            }
        }
    }
    m_Root.Update();
    if (Util::Input::IsKeyUp(Util::Keycode::R)) {
        LOG_DEBUG(Util::Input::GetCursorPosition() - m_c->m_Transform.translation);
        m_Mario->SetPosition(Util::Input::GetCursorPosition() - m_c->m_Transform.translation);
    }
    if (Util::Input::IsKeyUp(Util::Keycode::V)) {
        m_c->m_Transform.translation = {220, -32};
        m_Mario->SetPosition({-449.f, -0.f});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::H)) {
        m_Mario->TakeDamage();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::G)) {
        m_SFXManager->Play("powerup");
        if (m_Mario->GetType() == MarioType::BIG)
            m_Mario->ChangeType(MarioType::FIRE);

        if (m_Mario->GetType() == MarioType::SMALL)
            m_Mario->ChangeType(MarioType::BIG);
    }
    if (Util::Input::IsKeyUp(Util::Keycode::L)) {
        m_GameHUD->AddScore(10000);
        m_EffectManager->ShowScoreText(m_Mario->GetPosition(), 10000);
    }
    if (Util::Input::IsKeyUp(Util::Keycode::F)) {
        m_Mario->SetInvincible();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_1) || Util::Input::IsKeyUp(Util::Keycode::NUM_2) || Util::Input::IsKeyUp(Util::Keycode::NUM_3) || m_LevelManager->isNeedReload()) {
        m_c->RemoveAllChildren();
        m_LevelManager->Reset();
        m_EffectManager->Reset();
        m_ItemManager->Reset();
        m_c->m_Transform.translation = {234, -32};
        m_c->AddChild(m_Mario);
        m_c->AddChild(m_EffectManager);
        m_c->AddChild(m_ItemManager);
        m_c->AddChild(m_LevelManager);
        m_c->AddChild(m_GameHUD);
        if (m_GameHUD->GetTime() <= 0 && m_Mario->GetState() != Mario::State::FINISHING) {
            m_GameHUD->PlayTimeup();
        } else if (m_GameHUD->GetLife() < 0) {
            m_GameHUD->PlayGameover();
            m_LevelManager->SetNeedReload(false);
        } else {
            m_Mario->Start();
            m_Mario->ChangeType(m_LevelManager->GetMarioType(), false);
            if (Util::Input::IsKeyUp(Util::Keycode::NUM_1)) {
                m_LevelManager->SetCurrentLevel("Level1_1");
            }
            if (Util::Input::IsKeyUp(Util::Keycode::NUM_2)) {
                m_LevelManager->SetCurrentLevel("Level1_2");
            }
            if (Util::Input::IsKeyUp(Util::Keycode::NUM_3)) {
                m_LevelManager->SetCurrentLevel("Level1_3");
            }
            m_LevelManager->LoadLevelJSON(RESOURCE_DIR"/" + m_LevelManager->GetCurrentLevel() + ".json");
            m_c->AddChildren(m_LevelManager->GetStaticTiles());
            m_GameHUD->SetLevel(m_LevelManager->GetCurrentLevel());
            m_Mario->SetPosition(m_LevelManager->GetPlayerLoadPosition());
            m_LevelManager->ReflashCurrentLevel();
            m_Mario->SetVisible(true);
            if (m_GameHUD->GetState() == HUDState::GAMEOVER || m_GameHUD->IsFinishGame()) {
                m_GameHUD->Reset();
                m_GameHUD->SetIsFinishGame(false);
                m_Mario->ChangeType(MarioType::SMALL, false);
                LevelManager::GetInstance()->SetNeedReload(false);
                m_GameHUD->PlayTitleScreen();
            } else {
                if (m_LevelManager->isNeedPlayLevelScreen()) {
                    m_GameHUD->PlayLevelIntro();
                }
                else {
                    if (LevelManager::GetInstance()->IsSurface())
                        SFXManager::GetInstance()->PlayBGM("Ground Theme");
                    else
                        SFXManager::GetInstance()->PlayBGM("Underground Theme");
                }
            }
        }
    }
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
