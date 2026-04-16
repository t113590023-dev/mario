//
// Created by webbe on 2026/4/14.
//
#include "QuestionBlock.hpp"

void QuestionBlock::onHit(MarioType marioType) {
    LOG_INFO("Hit Question Block content: " + std::to_string(content));
    if (!m_IsUsed) {
        m_IsUsed = true;
        BounceUp();

        ItemManager::GetInstance()->SpawnItem(content, m_Transform.translation);

        // 變成空方塊的圖片
        SetImage(RESOURCE_DIR"/Image/Tile/EmptyBlock.png");
    }
}
