#ifndef MARIO_LIFT_HPP
#define MARIO_LIFT_HPP

#include "Enemy.hpp"
#include "Util/Animation.hpp"

class Lift : public Enemy {
public:
    Lift(float startX, float startY);

    void Start() override;
    void Update() override;
    void OnHit() override;
    void OnHit(std::shared_ptr<Mario> mario) override;
    void SetDirection (glm::vec2 direction);
    void SetDistance (int distance) { m_distance = distance; }

private:
    glm::vec2 m_direction, m_orgPosition;
    int m_distance;
    float m_curDistance;
    bool m_reverse;
};

#endif //MARIO_LIFT_HPP