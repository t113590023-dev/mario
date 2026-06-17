#include "Item.hpp"
#include "Util/Image.hpp"

Starman::Starman(glm::vec2 startPos) {
    m_Transform.translation = startPos;
    m_OriginY = startPos.y;
    m_Transform.scale = {2, 2};
    m_Velocity = {0, 7.0f};
    std::vector<std::string> path = {};
    for (int i=1; i<=4; i++) path.push_back(RESOURCE_DIR"/Image/Item/Starman" + std::to_string(i) + ".png");
    SetDrawable(std::make_shared<Util::Animation>(path, true, 100));
    m_itemType = ItemType::STARMAN;
    m_ZIndex = -1;
}

void Starman::Update(float dt) {
    if (m_State == State::POPPING_UP) {
        m_Transform.translation.y += 50.0f * dt;

        if (m_Transform.translation.y >= m_OriginY + 34.0f) {
            m_Transform.translation.y = m_OriginY + 34.0f;
            m_State = State::MOVING;
            m_Velocity = {2.0f, 0};
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
        if (collideTileID != -1) {
            m_Transform.translation = {posX,LevelManager::GetInstance()->GetTileById(collideTileID)->m_Transform.translation.y + size.y + 2};
            m_Velocity.y = 8;
        }
        collideTileID = LevelManager::GetInstance()->CheckCollision(posX + m_Velocity.x, posY, size.x, size.y/2 - 8, Direction::BOTHSIDE);
        if (collideTileID != -1) {
            m_Transform.translation = {LevelManager::GetInstance()->GetTileById(collideTileID)->m_Transform.translation.x - (34 * m_Velocity.x/3), posY};
            m_Velocity.x *= -1;
        }
        collideTileID = LevelManager::GetInstance()->CheckCollision(posX + m_Velocity.x, posY, size.x, size.y/2 - 8, Direction::TOP);
        if (collideTileID != -1) {
            m_Transform.translation = {posX,LevelManager::GetInstance()->GetTileById(collideTileID)->m_Transform.translation.y - size.y - 20};
            // m_Velocity.y = -1;
        }
        m_Transform.translation += m_Velocity;

    }
}