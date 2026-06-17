//
// Created by webbe on 2026/4/10.
//

#ifndef MARIO_QUESTIONBLOCK_HPP
#define MARIO_QUESTIONBLOCK_HPP
#include "Tile.hpp"
#include "Util/Logger.hpp"
#include "Util/Animation.hpp"

class QuestionBlock : public Tile {
public:
    QuestionBlock(const std::string& ImagePath, float x, float y, bool isSurface = true);
    QuestionBlock(const std::string& ImagePath, float x, float y, int contain, bool isSurface = true);
    void onHit(MarioType marioType) override;
    void SetContent(int content);
private:
    int content;
    bool m_IsUsed = false;
    bool m_isSurface = true;
};


#endif //MARIO_QUESTIONBLOCK_HPP