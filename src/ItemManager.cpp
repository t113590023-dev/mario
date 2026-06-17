//
// Created by webbe on 2026/4/14.
//
#include "ItemManager.hpp"

#include "GameHUD.hpp"
#include "Item.hpp"
#include "Mario.hpp"
#include "SFXManager.hpp"

std::shared_ptr<ItemManager> ItemManager::s_Instance = nullptr;

void ItemManager::Reset() {
    RemoveAllChildren();
    m_Items.clear();
}

void ItemManager::ShootFireball(glm::vec2 position, int direction) {
    std::shared_ptr<Item> newItem = std::make_shared<Fireball>(position, direction);
    this->AddChild(newItem);
    m_Items.push_back(newItem);
    m_fireballCount += 1;
}

void ItemManager::SpawnItem(int itemID, glm::vec2 position, bool popup) {
    std::shared_ptr<Item> newItem = nullptr;

    if (itemID == 158 || itemID == 159) {
        newItem = std::make_shared<Mushroom>(position, itemID == 159);
        SFXManager::GetInstance()->Play("powerup_appears");
    } else if (itemID == 160 || itemID == 161) {
        newItem = std::make_shared<Coin>(position, popup, itemID == 160);
        if (popup) SFXManager::GetInstance()->Play("coin");
    } else if (itemID == 162) {
        newItem = std::make_shared<Starman>(position);
        SFXManager::GetInstance()->Play("powerup_appears");
    } else if (itemID == 163) {
        newItem = std::make_shared<FireFlower>(position);
        SFXManager::GetInstance()->Play("powerup_appears");
    }

    if (newItem) {
        this->AddChild(newItem);
        m_Items.push_back(newItem);
    }
}

void ItemManager::UpdateItems(float dt, std::shared_ptr<Mario> mario) {
    dt = dt/1000;
    for (auto it = m_Items.begin(); it != m_Items.end(); ) {
        (*it)->Update(dt);

        if (IsIntersecting(mario->GetPosition(), mario->GetSize(), (*it)->GetTransform().translation, (*it)->GetScaledSize())) {
            if (mario->GetM_isDead()) return;
            switch ((*it)->GetItemType()) {
                case ItemType::MUSHROOM: {
                    std::shared_ptr<Mushroom> mushroom = std::dynamic_pointer_cast<Mushroom>(*it);
                    if (mushroom->IsActive()) {
                        if (mushroom->is1UP()) {
                            EffectManager::GetInstance()->ShowScoreText(mushroom->GetTransform().translation, 10000);
                            SFXManager::GetInstance()->Play("1-up");
                            GameHUD::GetInstance()->AddLife();
                        }
                        else {
                            mario->ChangeType(MarioType::BIG);
                            EffectManager::GetInstance()->ShowScoreText(mushroom->GetTransform().translation, 1000);
                            SFXManager::GetInstance()->Play("powerup");
                            GameHUD::GetInstance()->AddScore(1000);
                        }
                        (*it)->SetIsDead(true);
                    }
                    break;
                }
                case ItemType::STARMAN: {
                    std::shared_ptr<Starman> starman = std::dynamic_pointer_cast<Starman>(*it);
                    if (starman->IsActive()) {
                        mario->SetInvincible();
                        EffectManager::GetInstance()->ShowScoreText(starman->GetTransform().translation, 1000);
                        SFXManager::GetInstance()->Play("powerup");
                        GameHUD::GetInstance()->AddScore(1000);
                        (*it)->SetIsDead(true);
                    }
                    break;
                }
                case ItemType::COIN: {
                    std::shared_ptr<Coin> coin = std::dynamic_pointer_cast<Coin>(*it);
                    if (coin->IsActive()) {
                        GameHUD::GetInstance()->AddCoin();
                        SFXManager::GetInstance()->Play("coin");
                        (*it)->SetIsDead(true);
                    }
                    break;
                }
                case ItemType::FIREFLOWER: {
                    std::shared_ptr<FireFlower> fireFlower = std::dynamic_pointer_cast<FireFlower>(*it);
                    if (fireFlower->IsActive()) {
                        mario->ChangeType(MarioType::FIRE);
                        EffectManager::GetInstance()->ShowScoreText(fireFlower->GetTransform().translation, 1000);
                        SFXManager::GetInstance()->Play("powerup");
                        GameHUD::GetInstance()->AddScore(1000);
                        (*it)->SetIsDead(true);
                    }
                    break;
                }
                default:
                    break;
            }

        }

        if ((*it)->IsDead()) {
            if ((*it)->GetItemType() == ItemType::FIREBALL) m_fireballCount -= 1;
            this->RemoveChild(*it);
            it = m_Items.erase(it);
        } else {
            ++it;
        }
    }
}