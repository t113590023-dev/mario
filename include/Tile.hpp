#ifndef TILE_HPP
#define TILE_HPP

#include <string>

#include "Definitions.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "ItemManager.hpp"
#include "SFXManager.hpp"

class Mario;

class Tile : public Util::GameObject {
public:
    Tile(float x, float y);
    Tile(const std::string& ImagePath, float x, float y);
    virtual void onHit(MarioType marioType);
    virtual void Update();
    void BounceUp();
    void BounceUp(int timer, bool destory = false);
    bool isFinishEffect() const { return m_isFinishEffect; }
    bool isCollidable() const { return m_isCollidable; }
    void SetIsCollidable(bool isCollidable) {m_isCollidable = isCollidable; }
protected:
    void SetImage(const std::string& ImagePath);
private:
    bool is_bounceUp, m_isFinishEffect, m_destory = false, m_isCollidable = true;
    float timer;
    glm::vec2 orgTranslation;
};


#endif //TILE_HPP