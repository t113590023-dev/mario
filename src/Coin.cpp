//
// Created by webbe on 2026/4/14.
//
#include "Item.hpp" // 假設你把 Coin 寫在 Item.hpp 或 Coin.hpp
#include "Util/Image.hpp"

Coin::Coin(glm::vec2 startPos) {
    m_Transform.translation = startPos;
    m_OriginY = startPos.y;
    m_Transform.scale = {2, 2};
    m_Velocity = {0, 10.0f};

    SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Item/Coin.png"));

    m_ZIndex = 5;
}

void Coin::Update(float dt) {
    m_Velocity.y -= 0.4f;

    m_Transform.translation.y += m_Velocity.y;

    if (m_Transform.translation.y < m_OriginY) {
        m_IsDead = true;
    }
}