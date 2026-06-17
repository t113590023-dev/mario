//
// Created by webbe on 2026/5/28.
//

#ifndef MARIO_GAMEHUD_HPP
#define MARIO_GAMEHUD_HPP

#include "Definitions.hpp"
#include "Mario.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include "Util/GameObject.hpp"

class GameHUD : public Util::GameObject {
public:
    static std::shared_ptr<GameHUD> GetInstance() { return s_Instance; }
    GameHUD() {
        if (!s_Instance) s_Instance = std::shared_ptr<GameHUD>(this, [](GameHUD*){});
        Start();
    }
    static void Init(const std::shared_ptr<GameHUD> &instance) { s_Instance = instance; }
    void Start();
    void Update(std::shared_ptr<Mario> mario, float deltaTimeMs);
    void SetText(std::string text);
    void SetScore(int score);
    void SetLife(int life);
    void SetCoin(int coin);
    void SetTime(int time);
    void SetLevel(std::string level);
    void AddScore(int score);
    void loseLife();
    void AddCoin(int coin = 1);
    void AddLife();
    int GetScore() const { return m_score; }
    int GetLife() const { return m_life; }
    int GetCoin() const { return m_coin; }
    int GetTime() const { return m_time; }
    void PlayLevelIntro();
    void PlayTitleScreen();
    void PlayGameover();
    void PlayTimeup();
    void FinishLevel();
    void TimeStop() { m_timeStop = true; }
    void SetIsFinishGame(bool isFinishGame) { m_isFinishGame = isFinishGame; }
    bool IsFinishGame() { return m_isFinishGame; }
    void Reset();
    HUDState GetState() { return m_state; }
private:
    static std::shared_ptr<GameHUD> s_Instance;
    HUDState m_state;
    std::shared_ptr<Util::Text> m_scoreText, m_screenText;
    int m_score, m_life, m_coin, m_time, m_highScore;
    float timer, finishTimer = 0;
    bool m_timeStop = false, m_isFinishGame = false;
    std::string m_level;
    std::shared_ptr<GameObject> m_marioIcon;
    std::shared_ptr<GameObject> m_CoinIcon;
    std::shared_ptr<GameObject> m_screenTextObj;
    std::shared_ptr<GameObject> m_Text;
    std::shared_ptr<GameObject> m_BlackScreen;
    std::shared_ptr<GameObject> m_gameTitle;
};

#endif //MARIO_GAMEHUD_HPP