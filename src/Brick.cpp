//
// Created by webbe on 2026/4/14.
//
#include "Brick.hpp"
#include "EffectManager.hpp"

Brick::Brick(float x, float y, bool isSurface) : Tile(x, y) {
    m_isSurface = isSurface;
    if (m_isSurface)
        SetImage(RESOURCE_DIR"/Image/Tile/Brick.png");
    else
        SetImage(RESOURCE_DIR"/Image/Tile/Brick_underground.png");

}

void Brick::SetContent(int content) {
    this->content = content;
    if (content == 160 || content == 161) {
        capacity = 6;
    }
    else {
        capacity = 1;
    }
}


void Brick::onHit(MarioType marioType) {
    if (marioType == MarioType::SMALL && content == 0) {
        BounceUp();
        SFXManager::GetInstance()->Play("bump");
    }
    else {
        if (content == 0) {
            EffectManager::GetInstance()->PlayBrickBreak(m_Transform.translation, m_isSurface);
            SFXManager::GetInstance()->Play("breakblock");
            BounceUp(20, true);
            // if (m_Visible == true) SetVisible(false);
        }
        else {
            if (capacity != 0) {
                capacity -= 1;
                BounceUp();
                SFXManager::GetInstance()->Play("bump");

                ItemManager::GetInstance()->SpawnItem(content, m_Transform.translation);

                if (capacity == 0) {
                    if (m_isSurface)
                        SetImage(RESOURCE_DIR"/Image/Tile/EmptyBlock.png");
                    else
                        SetImage(RESOURCE_DIR"/Image/Tile/EmptyBlock_underground.png");
                }
            }
        }
    }
}
