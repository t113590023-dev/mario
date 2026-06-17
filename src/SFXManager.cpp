#include "SFXManager.hpp"

#include "Util/Logger.hpp"

std::shared_ptr<SFXManager> SFXManager::s_Instance = nullptr;

SFXManager::SFXManager() {
    if (!s_Instance) s_Instance = std::shared_ptr<SFXManager>(this, [](SFXManager*){});

    LoadSFX("1-up", RESOURCE_DIR"/Audio/smb_1-up.wav");
    LoadSFX("beep", RESOURCE_DIR"/Audio/smb_beep.wav");
    LoadSFX("breakblock", RESOURCE_DIR"/Audio/smb_breakblock.wav");
    LoadSFX("bump", RESOURCE_DIR"/Audio/smb_bump.wav");
    LoadSFX("coin", RESOURCE_DIR"/Audio/smb_coin.wav");
    LoadSFX("fireball", RESOURCE_DIR"/Audio/smb_fireball.wav");
    LoadSFX("fireworks", RESOURCE_DIR"/Audio/smb_fireworks.wav");
    LoadSFX("flagpole", RESOURCE_DIR"/Audio/smb_flagpole.wav");
    LoadSFX("gameover", RESOURCE_DIR"/Audio/smb_gameover.wav");
    LoadSFX("jump-small", RESOURCE_DIR"/Audio/smb_jump-small.wav");
    LoadSFX("jump-super", RESOURCE_DIR"/Audio/smb_jump-super.wav");
    LoadSFX("kick", RESOURCE_DIR"/Audio/smb_kick.wav");
    LoadSFX("mariodie", RESOURCE_DIR"/Audio/smb_mariodie.wav");
    LoadSFX("pause", RESOURCE_DIR"/Audio/smb_pause.wav");
    LoadSFX("pipe", RESOURCE_DIR"/Audio/smb_pipe.wav");
    LoadSFX("powerup", RESOURCE_DIR"/Audio/smb_powerup.wav");
    LoadSFX("powerup_appears", RESOURCE_DIR"/Audio/smb_powerup_appears.wav");
    LoadSFX("stage_clear", RESOURCE_DIR"/Audio/smb_stage_clear.wav");
    LoadSFX("stomp", RESOURCE_DIR"/Audio/smb_stomp.wav");
    LoadSFX("warning", RESOURCE_DIR"/Audio/smb_warning.wav");
    LoadSFX("world_clear", RESOURCE_DIR"/Audio/smb_world_clear.wav");

    m_bgm = std::make_shared<Util::BGM>(RESOURCE_DIR"/Audio/GroundTheme.mp3");
    m_bgm->Pause();
    LoadBGM("Ground Theme (Hurry Up!)", RESOURCE_DIR"/Audio/Ground Theme (Hurry Up!).mp3");
    LoadBGM("Ground Theme", RESOURCE_DIR"/Audio/Ground Theme.mp3");
    LoadBGM("Invincibility Theme", RESOURCE_DIR"/Audio/Invincibility Theme.mp3");
    LoadBGM("Underground Theme (Hurry Up!)", RESOURCE_DIR"/Audio/Underground Theme (Hurry Up!).mp3");
    LoadBGM("Underground Theme", RESOURCE_DIR"/Audio/Underground Theme.mp3");
}


void SFXManager::LoadSFX(const std::string& name, const std::string& filepath) {
    if (m_SFXMap.find(name) == m_SFXMap.end()) {
        m_SFXMap[name] = std::make_shared<Util::SFX>(filepath);
        if (name == "beep")
            m_SFXMap[name]->SetVolume(70);
        else
            m_SFXMap[name]->SetVolume(92);
    }
}

void SFXManager::Play(const std::string& name) {
    auto it = m_SFXMap.find(name);
    if (it != m_SFXMap.end()) {
        it->second->Play();
    }
}

void SFXManager::LoadBGM(const std::string &name, const std::string &filepath) {
    if (m_BGMMap.find(name) == m_BGMMap.end()) {
        m_BGMMap[name] = filepath;
    }
}

void SFXManager::PlayBGM(const std::string &name) {
    auto it = m_BGMMap.find(name);
    if (it != m_BGMMap.end()) {
        m_currentBGM = name;
        m_bgm->LoadMedia(it->second);
        m_bgm->Play();
    }
}

void SFXManager::PlayCurrentBGM() {
    auto it = m_BGMMap.find(m_currentBGM);
    if (it != m_BGMMap.end()) {
        m_bgm->LoadMedia(it->second);
        m_bgm->Play();
    }
}

void SFXManager::PlayHurryVersionBGM() {
    auto it = m_BGMMap.find(m_currentBGM+" (Hurry Up!)");
    if (it != m_BGMMap.end()) {
        m_bgm->LoadMedia(it->second);
        m_bgm->Play();
    }
}

void SFXManager::PlayInvincibilityBGM() {
    auto it = m_BGMMap.find("Invincibility Theme");
    if (it != m_BGMMap.end()) {
        m_bgm->LoadMedia(it->second);
        m_bgm->Play(1);
    }
}

