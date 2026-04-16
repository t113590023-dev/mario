//
// Created by webbe on 2026/4/14.
//
#include "ItemManager.hpp"
#include "Item.hpp"
#include "Mario.hpp"

std::shared_ptr<ItemManager> ItemManager::s_Instance = nullptr;

void ItemManager::SpawnItem(int itemID, glm::vec2 position) {
    std::shared_ptr<Item> newItem = nullptr;

    // 假設 Tiled 中：10 是香菇，11 是金幣
    if (itemID == 158) {
        newItem = std::make_shared<Mushroom>(position);
    } else if (itemID == 160) {
        newItem = std::make_shared<Coin>(position);
    }

    if (newItem) {
        this->AddChild(newItem);        // 加入渲染樹
        m_Items.push_back(newItem);     // 加入邏輯陣列
    }
}

void ItemManager::UpdateItems(float dt, std::shared_ptr<Mario> mario) {
    dt = dt/1000;
    for (auto it = m_Items.begin(); it != m_Items.end(); ) {
        (*it)->Update(dt);

        if (IsIntersecting(mario->GetPosition(), mario->GetSize(), (*it)->GetTransform().translation, (*it)->GetScaledSize())) {
            switch ((*it)->GetItemType()) {
                case ItemType::MUSHROOM: {
                    std::shared_ptr<Mushroom> mushroom = std::dynamic_pointer_cast<Mushroom>(*it);
                    if (mushroom->IsActive()) {
                        mario->ChangeType(MarioType::BIG);
                        (*it)->SetIsDead(true);
                    }
                    break;
                }
                default:
                    break;
            }

        }

        if ((*it)->IsDead()) {
            this->RemoveChild(*it);
            it = m_Items.erase(it);
        } else {
            ++it;
        }
    }
}