//
// Created by webbe on 2026/4/10.
//

#ifndef MARIO_QUESTIONBLOCK_HPP
#define MARIO_QUESTIONBLOCK_HPP
#include "Tile.hpp"
#include "Util/Logger.hpp"

class QuestionBlock : public Tile {
public:
    QuestionBlock(const std::string& ImagePath, float x, float y) : Tile(ImagePath, x, y) {this->content = 157; }
    QuestionBlock(const std::string& ImagePath, float x, float y, int contain) : Tile(ImagePath, x, y) { this->content = contain; }
    void onHit(MarioType marioType) override;
    void SetContent(int content) {this->content = content; }
private:
    int content;
    bool m_IsUsed;
};


#endif //MARIO_QUESTIONBLOCK_HPP