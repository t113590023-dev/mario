#include "Item.hpp"
#include "Util/Image.hpp"

Fireball::Fireball(glm::vec2 startPos, int direction) {
    m_Transform.translation = startPos;
    m_Transform.scale = {2, 2};
    m_Velocity = {direction*7, -0.0f};
    std::vector<std::string> path = {};
    for (int i = 1; i <= 4; i++) {
        path.push_back(std::string(RESOURCE_DIR "/Image/Item/Fireball") + std::to_string(i) + ".png");
    }
    SetDrawable(std::make_shared<Util::Animation>(path, true, 100));
    m_itemType = ItemType::FIREBALL;
    m_ZIndex = 15;
    LOG_DEBUG("Fireball create finish");
}

void Fireball::Update(float dt) {
    float posX = m_Transform.translation.x, posY = m_Transform.translation.y;
    glm::vec2 size = GetScaledSize();
    m_Velocity.y -= 0.45f;
    if (LevelManager::GetInstance()->IsOutsideMap(m_Transform.translation) || LevelManager::GetInstance()->IsOutsideCamera(m_Transform.translation)) {
        SetIsDead(true);
    }

    int collideTileID = LevelManager::GetInstance()->CheckCollision(posX, posY + m_Velocity.y, size.x, size.y, Direction::BOTTOM);
    if (collideTileID != -1 && LevelManager::GetInstance()->GetTileById(collideTileID)->isCollidable()) {
        m_Transform.translation = {posX,LevelManager::GetInstance()->GetTileById(collideTileID)->m_Transform.translation.y + size.y + 16};
        m_Velocity.y = 5;
    }
    collideTileID = LevelManager::GetInstance()->CheckCollision(posX + m_Velocity.x, posY, size.x, size.y/2 - 8, Direction::BOTHSIDE);
    if (collideTileID != -1 && LevelManager::GetInstance()->GetTileById(collideTileID)->isCollidable()) {
        SetIsDead(true);
    }
    collideTileID = LevelManager::GetInstance()->CheckCollision(posX + m_Velocity.x, posY, size.x, size.y/2 - 8, Direction::TOP);
    if (collideTileID != -1 && LevelManager::GetInstance()->GetTileById(collideTileID)->isCollidable()) {
        SetIsDead(true);
    }
    m_Transform.translation += m_Velocity;

}