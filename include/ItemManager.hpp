//
// Created by webbe on 2026/4/14.
//

#ifndef MARIO_ITEMMANAGER_HPP
#define MARIO_ITEMMANAGER_HPP
#include <vector>
#include <memory>
#include "Util/GameObject.hpp"
#include "Definitions.hpp"

class Item;
class Mario;

class ItemManager : public Util::GameObject {
public:
    static std::shared_ptr<ItemManager> GetInstance() { return s_Instance; }
    static void Init(const std::shared_ptr<ItemManager> &instance) { s_Instance = instance; }

    // 供方塊呼叫的生成介面
    void SpawnItem(int itemID, glm::vec2 position);

    // 在 App::Update 中呼叫
    void UpdateItems(float dt, std::shared_ptr<Mario> mario);

private:
    static std::shared_ptr<ItemManager> s_Instance;
    std::vector<std::shared_ptr<Item>> m_Items;
};
#endif //MARIO_ITEMMANAGER_HPP