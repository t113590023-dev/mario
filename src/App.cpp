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
    LevelManager::Init(m_LevelManager);
    EffectManager::Init(m_EffectManager);
    ItemManager::Init(m_ItemManager);
    m_c = std::make_shared<Util::GameObject>();
    m_Mario = std::make_shared<Mario>(RESOURCE_DIR"/Image/Character/Mario_Big_Idle.png", m_EffectManager);
    m_c->m_Transform.translation = {220, -32};
    m_c->AddChild(m_Mario);
    m_c->AddChild(m_EffectManager);
    m_c->AddChild(m_ItemManager);
    m_c->AddChild(m_LevelManager);
    m_Root.AddChild(m_c);
    m_Mario->Start();
    m_LevelManager->LoadLevelJSON(RESOURCE_DIR"/Level1_1.json");
    m_c->AddChildren(m_LevelManager->GetStaticTiles());
    glClearColor(0.36f, 0.58f, 0.98f, 1.0f);

}

void App::Update() {
    float dt = Util::Time::GetDeltaTimeMs();
    if (-m_Mario->GetPosition().x < m_c->m_Transform.translation.x)
        m_c->m_Transform.translation = {-m_Mario->GetPosition().x, m_c->m_Transform.translation.y};
    m_Mario->SetCameraPos(-m_c->m_Transform.translation.x);
    m_LevelManager->SetCameraPosition(m_c->m_Transform.translation);
    m_EffectManager->m_Transform.translation = m_c->m_Transform.translation;
    m_ItemManager->m_Transform.translation = m_c->m_Transform.translation;
    m_LevelManager->m_Transform.translation = m_c->m_Transform.translation;
    m_Mario->Update();
    LOG_DEBUG(m_Mario->GetPosition());
    if (!Util::Input::IsKeyPressed(Util::Keycode::P)) {
        m_EffectManager->Update(dt);
        m_ItemManager->UpdateItems(dt, m_Mario);
        m_LevelManager->Update(m_Mario);
    }
    m_Root.Update();
    if (Util::Input::IsKeyUp(Util::Keycode::R)) {
        m_Mario->SetPosition(Util::Input::GetCursorPosition() - m_c->m_Transform.translation);
    }
    if (Util::Input::IsKeyUp(Util::Keycode::V)) {
        m_c->m_Transform.translation = {220, -32};
        m_Mario->SetPosition({-449.f, -0.f});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::H)) {
        m_Mario->ChangeType(MarioType::SMALL);
    }
    if (Util::Input::IsKeyUp(Util::Keycode::G)) {
        m_Mario->ChangeType(MarioType::BIG);
    }
    if (Util::Input::IsKeyUp(Util::Keycode::F)) {
        m_Mario->ChangeType(MarioType::FIRE);
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
