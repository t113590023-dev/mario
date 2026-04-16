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

class Tile;

using json = nlohmann::json;

class LevelManager : public Util::GameObject{
public:
    static std::shared_ptr<LevelManager> GetInstance() { return s_Instance; }
    static void Init(const std::shared_ptr<LevelManager> &instance) { s_Instance = instance; }
    bool LoadLevelJSON(const std::string& filepath);
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
    void Update(std::shared_ptr<Mario> mario);
    void RemoveTile(int id);
    std::vector<std::shared_ptr<Util::GameObject>> GetStaticTiles() {
        std::vector<std::shared_ptr<Util::GameObject>> tiles(m_StaticTiles.begin(), m_StaticTiles.end());
        return tiles;
    };
private:
    static std::shared_ptr<LevelManager> s_Instance;
    std::map<int, std::string> m_TileDictionary;
    std::vector<std::shared_ptr<Tile>> m_StaticTiles;
    std::vector<std::shared_ptr<Enemy>> m_DynamicEntities;
    std::vector<int> m_CollisionData;

    int m_MapWidth = 0;
    int m_MapHeight = 0;
    float m_TileSize = 48.0f;
    glm::vec2 cameraPosition = {0, 0};

    void ParseTilesets(const json& mapData);

    void ParseStaticTiles(const json& layer, bool is_collision, const std::vector<int>& itemData);

    void ParseDynamicObjects(const json& layer);

    std::string GetTexturePathByID(int id);
};

#endif //LEVELMANAGER_HPP