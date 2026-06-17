#include "Item.hpp"
#include "LevelManager.hpp"

FireFlower::FireFlower(glm::vec2 startPos) {
    m_Transform.translation = startPos;
    m_Transform.translation.y += 8;
    m_OriginY = startPos.y;
    m_Transform.scale = {2, 2};
    std::vector<std::string> path = {};
    for (int i = 1; i <= 4; i++) {
        path.push_back(std::string(RESOURCE_DIR "/Image/Item/FireFlower") + std::to_string(i) + ".png");
    }
    SetDrawable(std::make_shared<Util::Animation>(path, true, 100));
    m_itemType = ItemType::FIREFLOWER;
    m_ZIndex = -1;
}

void FireFlower::Update(float dt) {
    if (m_State == State::POPPING_UP) {
        // 緩慢往上升
        m_Transform.translation.y += 50.0f * dt;

        if (m_Transform.translation.y >= m_OriginY + 34.0f) {
            m_Transform.translation.y = m_OriginY + 34.0f;
            m_State = State::MOVING;
            m_Velocity = {3.0f, 0};
            m_ZIndex = 15;
        }
    }
}