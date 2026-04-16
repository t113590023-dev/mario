#include "LevelManager.hpp"
#include "Mario.hpp"
#include "Tile.hpp"

std::shared_ptr<LevelManager> LevelManager::s_Instance = nullptr;

void LevelManager::SetMapWidth(int width) {
    this->m_MapWidth = width;
}

void LevelManager::Update(std::shared_ptr<Mario> mario) {
    for (int i = 0; i < m_DynamicEntities.size(); i++) {
        auto mobA = m_DynamicEntities[i];
        if (IsIntersecting(mario->GetPosition(), mario->GetSize(), mobA->GetTransform().translation, mobA->GetScaledSize())) {
            mobA->OnHit(mario);
        }
        if (mobA->IsDead()) {
            if (mobA->GetTimer() < 0) {
                m_DynamicEntities.erase(m_DynamicEntities.begin() + i);
                RemoveChild(mobA);
                i--;
            }
            continue;
        }
        for (int j = i+1; j < m_DynamicEntities.size(); j++) {
            auto mobB = m_DynamicEntities[j];
            if (IsIntersecting(mobA->GetPosition(), mobA->GetScaledSize(), mobB->GetPosition(), mobB->GetScaledSize())) {
                mobA->OnHit();
                mobB->OnHit();
            }
        }
    }
    for (auto& enemy : m_DynamicEntities) {
        enemy->Update();
    }
}

void LevelManager::ParseTilesets(const json& mapData) {
    for (const auto& tileset : mapData.at("tilesets")) {
        int firstgid = tileset.at("firstgid");

        if (tileset.contains("tiles")) {
            for (const auto& tile : tileset.at("tiles")) {
                int id = tile.at("id");
                std::string imagePath = tile.at("image");

                m_TileDictionary[firstgid + id] = imagePath;
            }
        }
    }
}

bool LevelManager::LoadLevelJSON(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    try {
        json mapData;
        file >> mapData; // 如果檔案內容不是標準 JSON，這裡會拋出 parse_error
        m_MapWidth = mapData.at("width");   // 建議用 .at() 代替 []，找不到 Key 時會明確報錯
        m_MapHeight = mapData.at("height");
        m_TileSize = mapData.at("tilewidth");
        m_TileSize *= 2.0f;

        ParseTilesets(mapData);

        std::vector<int> itemData;
        for (const auto& layer : mapData.at("layers")) {
            if (layer.at("type") == "tilelayer" && layer.at("name") == "items") {
                itemData = layer["data"].get<std::vector<int>>();
                break;
            }
        }

        for (const auto& layer : mapData.at("layers")) {
            std::string layerType = layer.at("type");

            if (layerType == "tilelayer") {
                if (layer.at("name") == "collision")
                    ParseStaticTiles(layer, true, itemData);
                else if (layer.at("name") == "bg")
                    ParseStaticTiles(layer, false, itemData);
                else if (layer.at("name") == "enemy")
                    ParseDynamicObjects(layer);
            }
        }
        return true;
    }
    catch (const std::exception& e) {
        LOG_ERROR("[Error]: ", e.what());
    }

    return false;
}

int LevelManager::GetTileID(float worldX, float worldY) {
    worldX = worldX + 540 + 64 + 16;
    worldY = worldY + (WINDOW_HEIGHT - m_TileSize)/2;
    int gridX = static_cast<int>(worldX / m_TileSize);
    int gridY = m_MapHeight - static_cast<int>(worldY / m_TileSize);


    if (gridX < 0 || gridX >= m_MapWidth || gridY < 0 || gridY >= m_MapHeight) {
        return false;
    }

    return gridY * m_MapWidth + gridX;
}

std::shared_ptr<Tile> LevelManager::GetTileByIdx(int idx) {
    return m_StaticTiles[m_CollisionData[idx]];
}

std::shared_ptr<Tile> LevelManager::GetTileById(int id) {
    return m_StaticTiles[id];
}

glm::vec2 LevelManager::GetTilePosition(int id) {
    int x = id % m_MapWidth;
    int y = m_MapHeight - id / m_MapWidth;
    float posX = std::round(x * m_TileSize - 540 - 64 - 16);
    float posY = std::round(y * m_TileSize - (WINDOW_HEIGHT - m_TileSize)/2);
    return {posX, posY};
}

int LevelManager::IsSolid(float worldX, float worldY) {
    worldX = worldX + 544 + 64 + 32 - 4;
    worldY = worldY + (WINDOW_HEIGHT - m_TileSize)/2;
    int gridX = static_cast<int>(worldX / m_TileSize);
    int gridY = m_MapHeight - static_cast<int>(worldY / m_TileSize);


    if (gridX < 0 || gridX >= m_MapWidth || gridY < 0 || gridY >= m_MapHeight) {
        LOG_DEBUG("OutSide Map");
        return false;
    }

    int tileID = m_CollisionData[gridY * m_MapWidth + gridX];
    if (m_StaticTiles[tileID]->GetVisible() == false) return 0;
    return (tileID);
}

bool LevelManager::IsOutsideMap(glm::vec2 position) {
    float worldX = position.x + 540 + 64;
    float worldY = position.y + (WINDOW_HEIGHT - m_TileSize)/2;
    int gridX = static_cast<int>(worldX / m_TileSize);
    int gridY = m_MapHeight - static_cast<int>(worldY / m_TileSize);

    if (gridX < 0 || gridX >= m_MapWidth || gridY < 0 || gridY >= m_MapHeight) {
        return true;
    }
    return false;
}

bool LevelManager::IsOutsideCamera(glm::vec2 position) {
    return  position.x <= -(WINDOW_WIDTH/2.f) - 16 - m_Transform.translation.x ||
            position.x >= WINDOW_WIDTH/2.f + 16 - m_Transform.translation.x ||
            position.y <= -(WINDOW_HEIGHT/2.f);
}

bool LevelManager::IsInsideCamera(glm::vec2 position) {
    return  position.x > -(WINDOW_WIDTH/2.f) - 15 - m_Transform.translation.x &&
            position.x < WINDOW_WIDTH/2.f + 15 - m_Transform.translation.x &&
            position.y > -(WINDOW_HEIGHT/2.f);
}

int LevelManager::CheckCollision(float x, float y, float w, float h) {
    int tileID;
    x = x + w;
    w /= 2;
    tileID = IsSolid(x , y - h);         //正下
    if (tileID != 0) return tileID;
    tileID = IsSolid(x - w, y - h);      //左下
    if (tileID != 0) return tileID;
    tileID = IsSolid(x + w, y - h);      //右下
    if (tileID != 0) return tileID;
    tileID = IsSolid(x, y + h + m_TileSize);        //正上
    if (tileID != 0) return tileID;
    tileID = IsSolid(x - w, y + h + m_TileSize);    //左上
    if (tileID != 0) return tileID;
    tileID = IsSolid(x + w, y + h + m_TileSize);    //右上
    if (tileID != 0) return tileID;
    tileID = IsSolid(x - w , y - h/2);       //左中
    if (tileID != 0) return tileID;
    tileID = IsSolid(x - w , y + h);         //左中
    if (tileID != 0) return tileID;
    tileID = IsSolid(x + w, y - h/2);    //右中
    if (tileID != 0) return tileID;
    tileID = IsSolid(x + w, y + h);      //右中
    if (tileID != 0) return tileID;
    return 0;
}

int LevelManager::CheckCollision(float x, float y, float w, float h, Direction d) {
    int tileID = 0;
    w /= 2;
    h /= 2;
    if (d == Direction::BOTTOM) {
        tileID = IsSolid(x , y - h);         //正下
        if (tileID != 0) return tileID;
        tileID = IsSolid(x - w, y - h);      //左下
        if (tileID != 0) return tileID;
        tileID = IsSolid(x + w, y - h);      //右下
        if (tileID != 0) return tileID;
    }
    if (d == Direction::TOP) {
        tileID = IsSolid(x, y + h);        //正上
        if (tileID != 0) return tileID;
        tileID = IsSolid(x - w, y + h);    //左上
        if (tileID != 0) return tileID;
        tileID = IsSolid(x + w, y + h);    //右上
        if (tileID != 0) return tileID;
    }
    if (d == Direction::LEFT || d == Direction::BOTHSIDE) {
        tileID = IsSolid(x - w, y - h/2);        //左中
        if (tileID != 0) return tileID;
        tileID = IsSolid(x - w, y + h/2);        //左中
        if (tileID != 0) return tileID;
        tileID = IsSolid(x - w, y + h);          //左上
        if (tileID != 0) return tileID;
    }
    if (d == Direction::RIGHT || d == Direction::BOTHSIDE) {
        tileID = IsSolid(x + w, y - h/2);      //右中
        if (tileID != 0) return tileID;
        tileID = IsSolid(x + w, y + h/2);      //右中
        if (tileID != 0) return tileID;
        tileID = IsSolid(x + w, y + h);        //右上
        if (tileID != 0) return tileID;
    }
    return 0;
}

void LevelManager::ParseStaticTiles(const json& layer, bool is_collision, const std::vector<int>& itemData) {
    const auto& data = layer["data"];
    int id = 0;
    for (int i = 0; i < data.size(); ++i) {
        int tileID = data[i];
        if (tileID == 0) {
            if (is_collision) m_CollisionData.push_back(0);
            continue;
        }

        int x = i % m_MapWidth;
        int y = m_MapHeight - i / m_MapWidth;

        float posX = std::round(x * m_TileSize - 540 - 64 - 16);
        float posY = std::round(y * m_TileSize - (WINDOW_HEIGHT - m_TileSize)/2);

        std::string texPath = GetTexturePathByID(tileID);
        if (tileID == 144) { //Question Block
            auto qBlock = std::make_shared<QuestionBlock>(texPath, posX, posY);
            qBlock->SetContent(itemData[i]);
            if (itemData[i] == 0) //空方塊自動填充金幣
                qBlock->SetContent(160);

            m_StaticTiles.push_back(qBlock);
        }
        else if (tileID == 6) { //brick
            auto brick = std::make_shared<Brick>(texPath, posX, posY);
            brick->SetContent(itemData[i]);
            m_StaticTiles.push_back(brick);
        }
        else {
            m_StaticTiles.push_back(std::make_shared<Tile>(texPath, posX, posY));
        }

        if (is_collision) {
            m_CollisionData.push_back(id);
            id++;
        }
    }
}

void LevelManager::ParseDynamicObjects(const json &layer) {
    const auto& data = layer["data"];
    for (int i = 0; i < data.size(); ++i) {
        int tileID = data[i];
        if (tileID == 0)
            continue;

        int x = i % m_MapWidth;
        int y = m_MapHeight - i / m_MapWidth;

        float posX = std::round(x * m_TileSize - 540 - 64 - 16);
        float posY = std::round(y * m_TileSize - (WINDOW_HEIGHT - m_TileSize)/2);

        if (tileID == 163) { //Goomba
            auto mob = std::make_shared<Goomba>(posX, posY);
            m_DynamicEntities.push_back(mob);
            this->AddChild(mob);
        }
    }
}


std::string LevelManager::GetTexturePathByID(int id) {
    if (m_TileDictionary.find(id) != m_TileDictionary.end()) {
        return "C:/Users/webbe/Documents/code/mario/Resources/" + m_TileDictionary[id];
    }
    return "C:/Users/webbe/Documents/code/mario/Resources/Image/Tile/Brick.png";
}