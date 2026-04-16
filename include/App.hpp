#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Util/Renderer.hpp"
#include "Character.hpp"
#include "EffectManager.hpp"
#include "Mario.hpp"
#include "Tile.hpp"
#include "LevelManager.hpp"
#include "ItemManager.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();

    State m_CurrentState = State::START;
    Util::Renderer m_Root;
    std::shared_ptr<Util::GameObject> m_c;

    std::shared_ptr<LevelManager> m_LevelManager;
    std::shared_ptr<EffectManager> m_EffectManager;
    std::shared_ptr<ItemManager> m_ItemManager;

    std::shared_ptr<Mario> m_Mario;

    bool showDemoWindow = true;
};

#endif
