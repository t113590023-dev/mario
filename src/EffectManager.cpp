//
// Created by webbe on 2026/4/14.
//

#include "EffectManager.hpp"

#include "Util/Logger.hpp"

std::shared_ptr<EffectManager> EffectManager::s_Instance = nullptr;

void EffectManager::PlayBrickBreak(glm::vec2 position) {
    // 噴發方向 (左上、右上、左下、右下)
    std::vector<glm::vec2> velocities = {
        {-2, 5}, {2, 5},
        {-1.5, 3}, {1.5, 3}
    };

    for (const auto& vel : velocities) {
        LOG_DEBUG("Create piece");
        auto shardObj = std::make_shared<Util::GameObject>();
        shardObj->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Effects/Brick_piece.png"));
        shardObj->m_Transform.translation = position;
        shardObj->m_Transform.scale = {2, 2};

        this->AddChild(shardObj);

        m_Shards.push_back({shardObj, vel, 10.0f});
    }
}

void EffectManager::AddTileEffect(std::shared_ptr<Tile> tile) {
    tileEffect = tile;
}

void EffectManager::Update(float dt) {

    if (tileEffect != nullptr) {
        tileEffect->Update();
    }

    for (auto it = m_Shards.begin(); it != m_Shards.end(); ) {
        // 1. 套用重力 (假設重力為 -1000)
        it->velocity.y -= 0.37;

        // 2. 更新位置
        it->gameObject->m_Transform.translation += it->velocity;

        // 3. 更新旋轉 (增加動態感)
        it->gameObject->m_Transform.rotation += it->rotationSpeed;

        // 4. 生命週期管理
        it->lifeTime -= dt;
        if (it->lifeTime <= 0) {
            this->RemoveChild(it->gameObject); // 從渲染樹移除
            it = m_Shards.erase(it);           // 從物理列表移除
            LOG_DEBUG("Delete piece");
        } else {
            ++it;
        }
    }
}
