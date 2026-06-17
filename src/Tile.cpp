//
// Created by webbe on 2026/4/10.
//

#include "Tile.hpp"

Tile::Tile(float x, float y) {
    m_Transform.translation = {x, y};
    orgTranslation = {x, y};
    m_Transform.scale = {2, 2};
    m_isFinishEffect = true;
    SetZIndex(5);

    m_Drawable = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Tile/Brick.png");
}


Tile::Tile(const std::string &ImagePath, float x, float y) {
    m_Transform.translation = {x, y};
    orgTranslation = {x, y};
    m_Transform.scale = {2, 2};
    m_isFinishEffect = true;
    SetZIndex(5);

    m_Drawable = std::make_shared<Util::Image>(ImagePath);
}


void Tile::Update() {
    if (is_bounceUp) {
        is_bounceUp = false;
        m_isFinishEffect = false;
    }
    if (timer > 0) {
        if (timer > 50)
            m_Transform.translation = orgTranslation + glm::vec2{0, (100 - timer)*0.3};
        else
            m_Transform.translation = orgTranslation + glm::vec2{0, (timer)*0.3};
        timer -= Util::Time::GetDeltaTimeMs();
        if (timer <= 0) {
            timer = 0;
            m_Transform.translation = orgTranslation;
            SetVisible(!m_destory);
            m_isFinishEffect = true;
        }
    }
}

void Tile::onHit(MarioType marioType) {

}

void Tile::SetImage(const std::string &ImagePath) {
    m_Drawable = std::make_shared<Util::Image>(ImagePath);
}


void Tile::BounceUp() {
    is_bounceUp = true;
    m_isFinishEffect = false;
    timer = 100;
}

void Tile::BounceUp(int timer, bool destory) {
    is_bounceUp = true;
    m_isFinishEffect = false;
    this->timer = timer;
    m_destory = destory;
}
