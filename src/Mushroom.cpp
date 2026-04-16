//
// Created by webbe on 2026/4/14.
//
// Mushroom.cpp
#include "Item.hpp"
#include "LevelManager.hpp"

Mushroom::Mushroom(glm::vec2 startPos) {
    m_Transform.translation = startPos;
    m_Transform.translation.y += 8;
    m_OriginY = startPos.y;
    m_Transform.scale = {2, 2};
    SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Item/Mushroom.png"));
    m_itemType = ItemType::MUSHROOM;
    // 注意：香菇一開始的 Z-Index 應該要比方塊低，這樣才有從方塊「後面」冒出來的感覺
    m_ZIndex = -1;
}

void Mushroom::Update(float dt) {
    if (m_State == State::POPPING_UP) {
        // 緩慢往上升
        m_Transform.translation.y += 50.0f * dt;

        if (m_Transform.translation.y >= m_OriginY + 34.0f) {
            m_Transform.translation.y = m_OriginY + 34.0f;
            m_State = State::MOVING;
            m_Velocity = {3.0f, 0};
            m_ZIndex = 1;
        }
    }
    else if (m_State == State::MOVING) {
        float posX = m_Transform.translation.x, posY = m_Transform.translation.y;
        glm::vec2 size = GetScaledSize();
        m_Velocity.y -= 0.45f;
        if (LevelManager::GetInstance()->IsOutsideMap(m_Transform.translation)) {
            m_IsDead = true;
        }

        int collideTileID = LevelManager::GetInstance()->CheckCollision(posX, posY + m_Velocity.y, size.x, size.y, Direction::BOTTOM);
        if (collideTileID != 0) {
            m_Transform.translation = {posX,LevelManager::GetInstance()->GetTileById(collideTileID)->m_Transform.translation.y + size.y + 2};
            m_Velocity.y = 0;
        }
        collideTileID = LevelManager::GetInstance()->CheckCollision(posX + m_Velocity.x, posY, size.x, size.y/2 - 8, Direction::BOTHSIDE);
        if (collideTileID != 0) {
            m_Transform.translation = {LevelManager::GetInstance()->GetTileById(collideTileID)->m_Transform.translation.x - (32 * m_Velocity.x/3), posY};
            m_Velocity.x *= -1;
        }
        m_Transform.translation += m_Velocity;

    }
}