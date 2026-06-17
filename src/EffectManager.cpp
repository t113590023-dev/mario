//
// Created by webbe on 2026/4/14.
//

#include "EffectManager.hpp"

#include "Util/Logger.hpp"
#include "Util/Text.hpp"

std::shared_ptr<EffectManager> EffectManager::s_Instance = nullptr;

void EffectManager::Reset() {
    RemoveAllChildren();
    tileEffects.clear();
    m_ScoreText.clear();
    m_Shards.clear();
}

void EffectManager::ShowScoreText(glm::vec2 position, int score) {
    auto Obj = std::make_shared<Util::GameObject>();
    Obj->SetZIndex(80);
    if (score == 10000)
        Obj->SetDrawable(std::make_shared<Util::Text>(RESOURCE_DIR"/fonts/Super Mario Bros. NES.ttf", 16, "1up", Util::Color{255, 255, 255}));
    else
        Obj->SetDrawable(std::make_shared<Util::Text>(RESOURCE_DIR"/fonts/smb1-score.ttf", 16, std::to_string(score), Util::Color{255, 255, 255}));

    Obj->m_Transform.translation = position;

    this->AddChild(Obj);

    m_ScoreText.push_back({Obj, {0, 1}, 1000.0f});
}


void EffectManager::PlayBrickBreak(glm::vec2 position, bool isSurface) {
    // 噴發方向 (左上、右上、左下、右下)
    std::vector<glm::vec2> velocities = {
        {-2, 5}, {2, 5},
        {-1.5, 3}, {1.5, 3}
    };

    for (const auto& vel : velocities) {
        auto shardObj = std::make_shared<Util::GameObject>();
        if (isSurface)
            shardObj->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Effects/Brick_piece.png"));
        else
            shardObj->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Effects/Brick_piece_underground.png"));
        shardObj->m_Transform.translation = position;
        shardObj->m_Transform.scale = {2, 2};

        this->AddChild(shardObj);

        m_Shards.push_back({shardObj, vel, 10.0f});
    }
}

void EffectManager::AddTileEffect(std::shared_ptr<Tile> tile) {
    tileEffects.push_back(tile);
}

void EffectManager::Update(float dt) {
    for (auto it = tileEffects.begin(); it != tileEffects.end();) {
        if (*it != nullptr && !(*it)->isFinishEffect()) {
            (*it)->Update();
            ++it;
        } else {
            it = tileEffects.erase(it);
        }
    }
    for (auto it = m_Shards.begin(); it != m_Shards.end(); ) {
        it->velocity.y -= 0.37;

        it->gameObject->m_Transform.translation += it->velocity;

        it->gameObject->m_Transform.rotation += it->rotationSpeed;

        it->lifeTime -= dt;
        if (it->lifeTime <= 0) {
            this->RemoveChild(it->gameObject);
            it = m_Shards.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = m_ScoreText.begin(); it != m_ScoreText.end(); ) {
        it->gameObject->m_Transform.translation += it->velocity;

        it->lifeTime -= dt;
        if (it->lifeTime <= 0) {
            this->RemoveChild(it->gameObject);
            it = m_ScoreText.erase(it);
        } else {
            ++it;
        }
    }
}
