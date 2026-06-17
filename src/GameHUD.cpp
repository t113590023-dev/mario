//
// Created by webbe on 2026/5/28.
//

#include "GameHUD.hpp"

#include "LevelManager.hpp"
#include "SFXManager.hpp"
#include "Util/Animation.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"

std::shared_ptr<GameHUD> GameHUD::s_Instance = nullptr;

void GameHUD::AddScore(int score) {
    if (score == 10000) {
        m_life += 1;
        SFXManager::GetInstance()->Play("1-up");
        return;
    }
    m_score += score;
}

void GameHUD::loseLife() {
    m_life -= 1;
}

void GameHUD::AddCoin(int coin) {
    m_score += 200;
    if (m_coin == 99) {
        m_life += 1;
        SFXManager::GetInstance()->Play("1-up");
        m_coin = 0;
        return;
    }
    m_coin += coin;
}

void GameHUD::AddLife() {
    m_life += 1;
}

void GameHUD::SetLife(int life) {
    m_life = life;
}

void GameHUD::SetScore(int score) {
    m_score = score;
}

void GameHUD::SetCoin(int coin) {
    m_coin = coin;
}

void GameHUD::SetTime(int time) {
    m_time = time;
    m_timeStop = false;
}

void GameHUD::SetLevel(std::string level) {
    level.erase(0, 5);
    level[1] = '-';
    if (level.length() > 3) {
        level.erase(level.length() - 2);
    }
    m_level = level;
}


void GameHUD::SetText(std::string text) {
    m_scoreText->SetText(text);
}

void GameHUD::PlayTitleScreen() {
    m_state = HUDState::TITLESCREEN;
    m_screenText->SetText(fmt::format("\n \n \n \n \n \n \n \n \n 1 PLAYER GAME \n \n 2 PLAYER GAME \n \n   TOP-{:06}", m_highScore));
    std::string text = fmt::format("MARIO            WORLD   TIME\n{:06}    ×{:02}     {}     {:03}", m_score, m_coin, m_level, m_time);
    if (text != m_scoreText->GetText()) {
        m_scoreText->SetText(text);
    }
    timer = 4000;
}

void GameHUD::PlayLevelIntro() {
    m_state = HUDState::LEVELINTRO;
    SetTime(400);
    m_screenText->SetText(fmt::format("WORLD {}\n \n \n    × {:02}", m_level, m_life));
    std::string text = fmt::format("MARIO            WORLD   TIME\n{:06}    ×{:02}     {}     {:03}", m_score, m_coin, m_level, m_time);
    if (text != m_scoreText->GetText()) {
        m_scoreText->SetText(text);
    }
    timer = 3000;
}

void GameHUD::FinishLevel() {
    m_state = HUDState::FINISHLEVEL;
}

void GameHUD::PlayGameover() {
    m_state = HUDState::GAMEOVER;
    SetTime(400);
    m_screenText->SetText("\n \nGAME OVER");
    SFXManager::GetInstance()->Play("gameover");
    SetLife(3);
    timer = 4000;
}

void GameHUD::PlayTimeup() {
    m_state = HUDState::TIMEUP;
    SetTime(400);
    m_screenText->SetText("\n \nTIME UP");
    timer = 4000;
}

void GameHUD::Reset() {
    LevelManager::GetInstance()->SetCurrentLevel("Level1_1");
    LevelManager::GetInstance()->SetNeedReload();
    LevelManager::GetInstance()->SetMarioType(MarioType::SMALL);
    SetTime(400);
    SetCoin(0);
    SetLife(3);
    if (GetScore() > m_highScore) m_highScore = GetScore();
    SetScore(0);
}

void GameHUD::Start() {
    m_scoreText = std::make_shared<Util::Text>(
        RESOURCE_DIR"/fonts/Super Mario Bros. NES.ttf",
        20,
        "MARIO            WORLD   TIME\n000000    ×00     1-1     400",
        Util::Color(255, 255, 255, 255)
    );
    m_BlackScreen = std::make_shared<GameObject>();
    m_Text = std::make_shared<GameObject>();
    m_marioIcon = std::make_shared<GameObject>();
    m_CoinIcon = std::make_shared<GameObject>();
    m_screenTextObj = std::make_shared<GameObject>();
    m_gameTitle = std::make_shared<GameObject>();

    m_BlackScreen->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/HUD/black.png"));
    m_BlackScreen->m_Transform.scale = {2, 2};
    m_BlackScreen->SetVisible(false);
    m_BlackScreen->SetZIndex(80);

    m_gameTitle->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/HUD/titleScreen.png"));
    m_gameTitle->m_Transform.scale = {2, 2};
    m_gameTitle->SetVisible(false);
    m_gameTitle->SetZIndex(100);

    m_Text->SetDrawable(m_scoreText);
    m_Text->m_Transform.translation = glm::vec2{0, 208};
    m_Text->SetZIndex(90);

    m_screenText = std::make_shared<Util::Text>(
        RESOURCE_DIR"/fonts/Super Mario Bros. NES.ttf",
        20,
        "WORLD 1-1\n \n \n    × 03",
        Util::Color(255, 255, 255, 255
    ));
    m_screenTextObj->SetDrawable(m_screenText);
    m_screenTextObj->SetZIndex(90);
    m_screenTextObj->SetVisible(false);

    m_marioIcon->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/Character/Mario_Small_Idle.png"));
    m_marioIcon->m_Transform.scale = {2, 2};
    m_marioIcon->SetZIndex(90);
    m_marioIcon->SetVisible(false);

    std::vector<std::string> imagesPath = {
        RESOURCE_DIR"/Image/HUD/Coin1.png", RESOURCE_DIR"/Image/HUD/Coin2.png", RESOURCE_DIR"/Image/HUD/Coin3.png"
    , RESOURCE_DIR"/Image/HUD/Coin2.png" , RESOURCE_DIR"/Image/HUD/Coin1.png"};
    m_CoinIcon->SetDrawable(std::make_shared<Util::Animation>(imagesPath, true, 200, true));
    m_CoinIcon->m_Transform.scale = {2, 2};

    this->SetZIndex(100);
    this->AddChild(m_Text);
    this->AddChild(m_CoinIcon);
    this->AddChild(m_marioIcon);
    this->AddChild(m_screenTextObj);
    this->AddChild(m_BlackScreen);
    this->AddChild(m_gameTitle);
    m_CoinIcon->m_Transform.translation = glm::vec2{-112, 198};
    m_marioIcon->m_Transform.translation = glm::vec2{-60, 0};
    m_screenTextObj->m_Transform.translation = glm::vec2{0, 30};
    m_BlackScreen->m_Transform.translation = glm::vec2{0, 0};
    m_gameTitle->m_Transform.translation = glm::vec2{0, 100};
    m_coin = 0, m_time = 400,m_score = 0, m_highScore = 0;
    SetLife(3);
    m_level = "1-1";
    PlayTitleScreen();
    timer = 5000;
}

void GameHUD::Update(std::shared_ptr<Mario> mario, float deltaTimeMs) {
    switch (m_state) {
        case HUDState::TITLESCREEN: {
            m_gameTitle->SetVisible(true);
            m_screenTextObj->SetVisible(true);
            m_BlackScreen->SetVisible(false);
            break;
        }
        case HUDState::LEVELINTRO:
            m_gameTitle->SetVisible(false);
            m_BlackScreen->SetVisible(true);
            m_marioIcon->SetVisible(true);
            m_screenTextObj->SetVisible(true);
            timer -= deltaTimeMs;
            if (timer <= 0) {
                m_BlackScreen->SetVisible(false);
                m_marioIcon->SetVisible(false);
                m_screenTextObj->SetVisible(false);
                m_state = HUDState::HUD;

                if (LevelManager::GetInstance()->IsSurface())
                    SFXManager::GetInstance()->PlayBGM("Ground Theme");
                else
                    SFXManager::GetInstance()->PlayBGM("Underground Theme");
            }
            break;
        case HUDState::GAMEOVER:
            m_BlackScreen->SetVisible(true);
            m_screenTextObj->SetVisible(true);
            timer -= deltaTimeMs;
            if (timer <= 0) {
                Reset();
            }
            break;
        case HUDState::TIMEUP:
            m_BlackScreen->SetVisible(true);
            m_screenTextObj->SetVisible(true);
            timer -= deltaTimeMs;
            if (timer <= 0) {
                LevelManager::GetInstance()->SetNeedReload();
            }
            break;

        case HUDState::FINISHLEVEL: {
            timer += deltaTimeMs;
            finishTimer += deltaTimeMs;
            if (timer >= 15 && m_time > 0) {
                m_time -= 1;
                timer = 0;
                AddScore(50);
                SFXManager::GetInstance()->Play("beep");
            }
            std::string text = fmt::format("MARIO            WORLD   TIME\n{:06}    ×{:02}     {}     {:03}", m_score, m_coin, m_level, m_time);
            if (text != m_scoreText->GetText()) {
                m_scoreText->SetText(text);
            }
            if (m_time == 0) {
                if (timer >= 1500 && finishTimer >= 4000) {
                    finishTimer = 0;
                    LevelManager::GetInstance()->SetNeedReload();
                    LevelManager::GetInstance()->SetMarioType(mario->GetType());
                }
            }
            break;
        }
        case HUDState::HUD: {
            timer += deltaTimeMs;
            if (timer >= 500 && !m_timeStop) {
                m_time -= 1;
                timer = 0;
            }
            if (m_time == 100)
                SFXManager::GetInstance()->PlayHurryVersionBGM();
            std::string text = fmt::format("MARIO            WORLD   TIME\n{:06}    ×{:02}     {}     {:03}", m_score, m_coin, m_level, m_time);
            if (text != m_scoreText->GetText()) {
                m_scoreText->SetText(text);
            }
            break;
        }
    }
}