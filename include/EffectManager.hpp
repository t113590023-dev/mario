//
// Created by webbe on 2026/4/10.
//

#ifndef MARIO_EFFECTMAMAGER_HPP
#define MARIO_EFFECTMAMAGER_HPP
#include <vector>
#include <memory>
#include "Util/GameObject.hpp"
#include "Tile.hpp"


struct BrickShard {
    std::shared_ptr<Util::GameObject> gameObject;
    glm::vec2 velocity;
    float rotationSpeed;
    float lifeTime = 2000.f;
};
struct ScoreText {
    std::shared_ptr<Util::GameObject> gameObject;
    glm::vec2 velocity;
    float lifeTime = 2000.f;
};

class EffectManager : public Util::GameObject {
public:
    static std::shared_ptr<EffectManager> GetInstance() { return s_Instance; }
    EffectManager() {
        if (!s_Instance) s_Instance = std::shared_ptr<EffectManager>(this, [](EffectManager*){});
    }
    static void Init(std::shared_ptr<EffectManager> instance) { s_Instance = instance; }
    void ShowScoreText(glm::vec2 position, int score);
    void PlayBrickBreak(glm::vec2 position, bool isSurface = true);
    void AddTileEffect(std::shared_ptr<Tile> tile);
    void Reset();
    void Update(float dt);

private:
    static std::shared_ptr<EffectManager> s_Instance;
    std::vector<BrickShard> m_Shards;
    std::vector<ScoreText> m_ScoreText;
    std::vector<std::shared_ptr<Tile>> tileEffects;

};

#endif //MARIO_EFFECTMAMAGER_HPP