//
// Created by webbe on 2026/4/14.
//
// Mushroom.cpp
#include "Item.hpp"
#include "LevelManager.hpp"

Mushroom::Mushroom(glm::vec2 startPos, bool is1UP) {
    m_Transform.translation = startPos;
    m_Transform.translation.y += 8;
    m_OriginY = startPos.y;
    m_Transform.scale = {2, 2};
    if (is1UP)
        SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Item/1upMushroom.png"));
    else
        SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Item/Mushroom.png"));
    m_itemType = ItemType::MUSHROOM;
    m_ZIndex = -1;
    m_is1UP = is1UP;
}

void Mushroom::Update(float dt) {
    if (m_State == State::POPPING_UP) {
        // 緩慢往上升
        m_Transform.translation.y += 50.0f * dt;

        if (m_Transform.translation.y >= m_OriginY + 34.0f) {
            m_Transform.translation.y = m_OriginY + 34.0f;
            m_State = State::MOVING;
            m_Velocity = {2.5f, 0};
            m_ZIndex = 15;
            m_OriginY = m_Transform.translation.y;
        }
    }
    else if (m_State == State::MOVING || m_State == State::POP) {
        float posX = m_Transform.translation.x, posY = m_Transform.translation.y;
        glm::vec2 size = GetScaledSize();
        m_Velocity.y -= 0.45f;
        if (LevelManager::GetInstance()->IsOutsideMap(m_Transform.translation)) {
            m_IsDead = true;
        }

        int collideTileID = LevelManager::GetInstance()->CheckCollision(posX, posY + m_Velocity.y, size.x, size.y, Direction::BOTTOM);
        if (collideTileID != -1) {
            m_Transform.translation = {posX,LevelManager::GetInstance()->GetTileById(collideTileID)->m_Transform.translation.y + size.y + 2};
            m_State = State::MOVING;
            m_Velocity.y = 0;
        }
        collideTileID = LevelManager::GetInstance()->CheckCollision(posX + m_Velocity.x, posY, size.x, size.y/2 - 8, Direction::BOTHSIDE);
        if (collideTileID != -1) {
            m_Transform.translation = {LevelManager::GetInstance()->GetTileById(collideTileID)->m_Transform.translation.x - (32 * m_Velocity.x/2.5f), posY};
            m_Velocity.x *= -1;
        }
        if (m_OriginY < m_Transform.translation.y && m_State == State::MOVING) {
            m_Velocity.y = 7;
            m_Velocity.x *= -1;
            m_State = State::POP;
        }
        else {
            m_OriginY = m_Transform.translation.y;
        }
        m_Transform.translation += m_Velocity;
    }
}