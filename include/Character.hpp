#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>

#include "Util/GameObject.hpp"

class Character : public Util::GameObject {
public:
    explicit Character(const std::string& ImagePath);

    Character(const Character&) = delete;

    Character(Character&&) = delete;

    Character& operator=(const Character&) = delete;

    Character& operator=(Character&&) = delete;

    [[nodiscard]] const std::string& GetImagePath() const { return m_ImagePath; }

    [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] bool GetVisibility() const { return m_Visible; }

    void SetImage(const std::string& ImagePath);

    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }

    [[nodiscard]] bool IfCollides(const std::shared_ptr<Character>& other) const {
        if (!other || !other->m_Drawable || !this->m_Drawable) {
            return false;
        }

        auto pos1 = this->GetPosition();
        auto size1 = this->m_Drawable->GetSize();

        auto pos2 = other->GetPosition();
        auto size2 = other->m_Drawable->GetSize();

        bool isColliding =
            pos1.x < pos2.x + size2.x &&  // A 的左邊界小於 B 的右邊界
            pos1.x + size1.x > pos2.x &&  // A 的右邊界大於 B 的左邊界
            pos1.y < pos2.y + size2.y &&  // A 的上邊界小於 B 的下邊界
            pos1.y + size1.y > pos2.y;    // A 的下邊界大於 B 的上邊界

        return isColliding;
    }

    // TODO: Add and implement more methods and properties as needed to finish Giraffe Adventure.

private:
    void ResetPosition() { m_Transform.translation = {0, 0}; }

    std::string m_ImagePath;
};


#endif //CHARACTER_HPP
