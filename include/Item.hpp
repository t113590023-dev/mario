//
// Created by webbe on 2026/4/14.
//

#ifndef MARIO_ITEM_HPP
#define MARIO_ITEM_HPP
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "LevelManager.hpp"
#include "Definitions.hpp"

class Item : public Util::GameObject {
public:
    virtual void Update(float dt) = 0;
    bool IsDead() const { return m_IsDead; }
    void SetIsDead(bool isDead) {m_IsDead = isDead; }
    ItemType GetItemType() const {return m_itemType; }
protected:
    bool m_IsDead = false;
    ItemType m_itemType = ItemType::NONE;
};

class Mushroom : public Item {
public:
    Mushroom(glm::vec2 startPos, bool is1UP = false);
    void Update(float dt) override;
    bool IsActive() const {return m_State == State::MOVING; }
    bool is1UP() const {return m_is1UP; }
private:
    enum class State { POPPING_UP, MOVING, POP };
    bool m_is1UP;
    State m_State = State::POPPING_UP;
    float m_OriginY;
    glm::vec2 m_Velocity = {0, 0};
};

class Starman : public Item {
public:
    Starman(glm::vec2 startPos);
    void Update(float dt) override;
    bool IsActive() const {return m_State == State::MOVING; }
private:
    enum class State { POPPING_UP, MOVING };
    State m_State = State::POPPING_UP;
    float m_OriginY;
    glm::vec2 m_Velocity = {0, 0};
};

class FireFlower : public Item {
public:
    FireFlower(glm::vec2 startPos);
    void Update(float dt) override;
    bool IsActive() const {return m_State == State::MOVING; }
private:
    enum class State { POPPING_UP, MOVING };
    State m_State = State::POPPING_UP;
    float m_OriginY;
    glm::vec2 m_Velocity = {0, 0};
};

class Fireball : public Item {
public:
    Fireball(glm::vec2 startPos, int direction);
    void Update(float dt) override;
private:
    glm::vec2 m_Velocity = {0, 0};
};

class Coin : public Item {
public:
    Coin(glm::vec2 startPos, bool popup = true, bool isSurface = true);
    void Update(float dt) override;
    bool IsActive() const {return !m_popup; }
private:
    float m_OriginY;
    float m_Timer = 0;
    glm::vec2 m_Velocity = {0, 0};
    bool m_popup, m_isSurface;
};
#endif //MARIO_ITEM_HPP