//
// Created by webbe on 2026/4/14.
//
#include "QuestionBlock.hpp"


QuestionBlock::QuestionBlock(const std::string& ImagePath, float x, float y, bool isSurface) : Tile(ImagePath, x, y) {
    std::vector<std::string> path = {};
    for (int i = 1; i <= 4; i++) {
        path.push_back(std::string(RESOURCE_DIR "/Image/Item/QuestionBlock") + std::to_string(i) + ".png");
    }
    SetDrawable(std::make_shared<Util::Animation>(path, true, 100));
    this->content = 160; m_IsUsed = false;
    m_isSurface = isSurface;
}

QuestionBlock::QuestionBlock(const std::string& ImagePath, float x, float y, int contain, bool isSurface) : Tile(ImagePath, x, y) {
    this->content = contain; m_IsUsed = false;
    m_isSurface = isSurface;
}

void QuestionBlock::SetContent(int content) {
    if (content == 159) {
        SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/Image/Item/HiddenBlock.png"));
        SetIsCollidable(false);
    }
    this->content = content;
}


void QuestionBlock::onHit(MarioType marioType) {
    // LOG_INFO("Hit Question Block content: " + std::to_string(content));
    SFXManager::GetInstance()->Play("bump");
    if (!m_IsUsed) {
        m_IsUsed = true;
        BounceUp();
        SetIsCollidable(true);

        if (content == 158 && marioType != MarioType::SMALL)
            ItemManager::GetInstance()->SpawnItem(163, m_Transform.translation);
        else
            ItemManager::GetInstance()->SpawnItem(content, m_Transform.translation);

        if (m_isSurface)
            SetImage(RESOURCE_DIR"/Image/Tile/EmptyBlock.png");
        else
            SetImage(RESOURCE_DIR"/Image/Tile/EmptyBlock_underground.png");
    }
}
