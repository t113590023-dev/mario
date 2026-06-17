#ifndef MARIO_SFXMANAGER_HPP
#define MARIO_SFXMANAGER_HPP

#include "Util/SFX.hpp"
#include <map>
#include <string>
#include <memory>

#include "Util/BGM.hpp"

class SFXManager {
public:
    static std::shared_ptr<SFXManager> GetInstance() { return s_Instance; }
    SFXManager();
    static void Init(std::shared_ptr<SFXManager> instance) { s_Instance = instance; }

    void LoadSFX(const std::string& name, const std::string& filepath);
    void LoadBGM(const std::string& name, const std::string& filepath);
    void Play(const std::string& name);
    void PlayCurrentBGM();
    void PlayHurryVersionBGM();
    void PlayBGM(const std::string& name);
    void PlayInvincibilityBGM();
    void Pause() { if (m_bgm) m_bgm->Pause(); }
    void Resume() { if (m_bgm) m_bgm->Resume(); }
private:
    static std::shared_ptr<SFXManager> s_Instance;
    std::map<std::string, std::shared_ptr<Util::SFX>> m_SFXMap;
    std::map<std::string, std::string> m_BGMMap;
    std::shared_ptr<Util::BGM> m_bgm;
    std::string m_currentBGM;
};


#endif //MARIO_SFXMANAGER_HPP