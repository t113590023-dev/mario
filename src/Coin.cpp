//
// Created by webbe on 2026/4/14.
//
#include "GameHUD.hpp"
#include "Item.hpp" // 假設你把 Coin 寫在 Item.hpp 或 Coin.hpp
#include "Mario.hpp"
#include "Util/Image.hpp"

Coin::Coin(glm::vec2 startPos, bool popup, bool isSurface) {
    startPos += glm::vec2{0, 8};
    m_Transform.translation = startPos;
    m_OriginY = startPos.y;
    m_Transform.scale = {2, 2};
    m_Velocity = {0, 9.0f};
    m_popup = popup;
    m_isSurface = isSurface;
    m_itemType = ItemType::COIN;
    if (m_popup) {
        std::vector<std::string> path = {};
        for (int i = 1; i <= 4; i++) {
            if (m_isSurface)
                path.push_back(std::string(RESOURCE_DIR "/Image/Effects/Coin") + std::to_string(i) + ".png");
            else
                path.push_back(std::string(RESOURCE_DIR "/Image/Effects/Coin") + std::to_string(i) + ".png");
        }
        SetDrawable(std::make_shared<Util::Animation>(path, true, 100));
    }
    else {
        std::vector<std::string> path = {};
        for (int i = 1; i <= 6; i++) {
            if (m_isSurface)
                path.push_back(std::string(RESOURCE_DIR "/Image/Item/Coin") + std::to_string(i) + ".png");
            else
                path.push_back(std::string(RESOURCE_DIR "/Image/Item/Coin_Underground") + std::to_string(i) + ".png");
        }
        SetDrawable(std::make_shared<Util::Animation>(path, true, 100));
    }
    m_ZIndex = 15;
}

void Coin::Update(float dt) {
    if (m_popup) {
        m_Velocity.y -= 0.4f;

        m_Transform.translation.y += m_Velocity.y;

        if (m_Transform.translation.y < m_OriginY) {
            EffectManager::GetInstance()->ShowScoreText(m_Transform.translation, 200);
            GameHUD::GetInstance()->AddCoin();
            m_IsDead = true;
        }
    }
}