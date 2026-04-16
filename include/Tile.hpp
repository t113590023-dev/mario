#ifndef TILE_HPP
#define TILE_HPP

#include <string>

#include "Definitions.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "ItemManager.hpp"

class Mario;

class Tile : public Util::GameObject {
public:
    Tile(const std::string& ImagePath, float x, float y);
    virtual void onHit(MarioType marioType);
    virtual void Update();
    void BounceUp();
protected:
    void SetImage(const std::string& ImagePath);
private:
    bool is_bounceUp;
    float timer;
    glm::vec2 orgTranslation;
};


#endif //TILE_HPP