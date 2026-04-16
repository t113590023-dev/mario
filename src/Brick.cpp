//
// Created by webbe on 2026/4/14.
//
#include "Brick.hpp"

#include "EffectManager.hpp"

void Brick::onHit(MarioType marioType) {
    if (marioType == MarioType::SMALL) {
        BounceUp();
    }
    else {
        if (content == 0) {
            EffectManager::GetInstance()->PlayBrickBreak(m_Transform.translation);
            if (m_Visible == true) SetVisible(false);
        }
        else {
            if (capacity != 0) {
                capacity -= 1;
                BounceUp();

                ItemManager::GetInstance()->SpawnItem(content, m_Transform.translation);

                if (capacity == 0)
                    SetImage(RESOURCE_DIR"/Image/Tile/EmptyBlock.png");
            }
        }
    }
}
