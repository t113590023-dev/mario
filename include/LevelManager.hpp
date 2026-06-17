#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include "nlohmann/json.hpp"
#include "Core/Context.hpp"
#include "Util/Logger.hpp"
#include "Brick.hpp"
#include "QuestionBlock.hpp"
#include "Enemy.hpp"
#include "Goomba.hpp"
#include "Koopa.hpp"
#include "Lift.hpp"
#include "PiranhaPlant.hpp"

class Tile;

using json = nlohmann::json;

class LevelManager : public Util::GameObject{
public:
    struct PipeData {
        float x, y, width, height;
        std::string direction;
        std::string target_map;
        bool playLevelScreen;
        float target_x;
        float target_y;
    };
    static std::shared_ptr<LevelManager> GetInstance() { return s_Instance; }
    static void Init(const std::shared_ptr<LevelManager> &instance) { s_Instance = instance; }
    bool LoadLevelJSON(const std::string& filepath);
    std::vector<PipeData> LoadPipesData(json map_json);
    int GetTileID(float worldX, float worldY);
    std::shared_ptr<Tile> GetTileByIdx(int idx);
    std::shared_ptr<Tile> GetTileById(int id);
    glm::vec2 GetTilePosition(int id);
    int IsSolid(float worldX, float worldY);
    bool IsOutsideMap(glm::vec2 position);
    bool IsOutsideCamera(glm::vec2 position);
    bool IsInsideCamera(glm::vec2 position);
    int CheckCollision(float x, float y, float w, float h);
    int CheckCollision(float x, float y, float w, float h, Direction d);
    void SetMapWidth(int width);
    void SetCameraPosition(glm::vec2 position) {cameraPosition = position; }
    void SetCurrentLevel(const std::string& newLevel) { m_currentLevel = newLevel; }
    void SetNeedReload(bool needReload = true) { m_isNeedReload = needReload; }
    bool isNeedReload() const { return m_isNeedReload; }
    bool isNeedPlayLevelScreen();
    bool isCameraMove() const { return m_cameraMove; }
    bool IsSurface() const { return m_isSurface; }
    glm::vec2 GetPlayerLoadPosition() const { return m_playerLoadPosition; }
    void SetPlayerLoadPosition(glm::vec2 position) { m_playerLoadPosition = position; }
    std::string GetCurrentLevel() { return m_currentLevel; }
    MarioType GetMarioType() { return m_MarioType; }
    void SetMarioType(MarioType type) { m_MarioType = type; }
    void Update(std::shared_ptr<Mario> mario);
    void RemoveTile(int id);
    void ReflashCurrentLevel();
    void Reset();
    std::vector<std::shared_ptr<Util::GameObject>> GetStaticTiles() {
        std::vector<std::shared_ptr<Util::GameObject>> tiles(m_StaticTiles.begin(), m_StaticTiles.end());
        return tiles;
    };
private:
    std::string m_currentLevel = "Level1_1";
    static std::shared_ptr<LevelManager> s_Instance;
    std::map<int, std::string> m_TileDictionary;
    std::vector<std::shared_ptr<Tile>> m_StaticTiles;
    std::vector<PipeData> m_pipesData;
    std::vector<std::shared_ptr<Enemy>> m_DynamicEntities;
    std::vector<int> m_CollisionData;
    MarioType m_MarioType = MarioType::SMALL;

    int m_MapWidth = 0;
    int m_MapHeight = 0;
    float m_TileSize = 48.0f;
    glm::vec2 cameraPosition = {0, 0};
    glm::vec2 m_playerLoadPosition = {-449.f, -0.f};
    bool m_isNeedReload = false, m_playLevelScreen = true, m_isSurface = true, m_cameraMove = true;

    void ParseTilesets(const json& mapData);

    void ParseStaticTiles(const json& layer, bool is_collision, const std::vector<int>& itemData);

    void ParseDynamicObjects(const json& layer);

    std::string GetTexturePathByID(int id);
};

#endif //LEVELMANAGER_HPP