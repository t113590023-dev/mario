#include "LevelManager.hpp"

#include "GameHUD.hpp"
#include "Item.hpp"
#include "Mario.hpp"
#include "Tile.hpp"

std::shared_ptr<LevelManager> LevelManager::s_Instance = nullptr;

template <typename T>
T get_custom_property(const json& props_json, const std::string& name, T default_val) {
    if (!props_json.is_array()) return default_val;
    for (const auto& prop : props_json) {
        if (prop["name"] == name) {
            return prop["value"].get<T>();
        }
    }
    return default_val;
}

void LevelManager::SetMapWidth(int width) {
    this->m_MapWidth = width;
}

bool LevelManager::isNeedPlayLevelScreen() {
    const bool result = m_playLevelScreen;
    m_playLevelScreen = true;
    return result;
}


void LevelManager::Reset() {
    m_isNeedReload = false;
    this->RemoveAllChildren();
    m_TileDictionary.clear();
    m_StaticTiles.clear();
    m_DynamicEntities.clear();
    m_CollisionData.clear();
    m_pipesData.clear();
    m_MapWidth = 0;
    m_MapHeight = 0;
    m_TileSize = 48.0f;
    cameraPosition = {0, 0};
}

void LevelManager::ReflashCurrentLevel() {
    std::string currLevel = GetCurrentLevel();
    if (currLevel.length() >= 2) {
        char secondToLast = currLevel[currLevel.length() - 2];
        char last = currLevel[currLevel.length() - 1];

        if (secondToLast == '_' && std::islower(static_cast<unsigned char>(last))) {
            currLevel.pop_back();
            currLevel.pop_back();
            SetCurrentLevel(currLevel);
        }
    }
    SetPlayerLoadPosition({-449.f, -0.f});
}



void LevelManager::Update(std::shared_ptr<Mario> mario) {
    for (int i = 0; i < m_DynamicEntities.size(); i++) {
        auto mobA = m_DynamicEntities[i];

        if (mobA->IsDead()) {
            if (mobA->GetTimer() < 0) {
                m_DynamicEntities.erase(m_DynamicEntities.begin() + i);
                RemoveChild(mobA);
                i--;
            }
            continue;
        }

        std::vector<std::shared_ptr<Item>> items = ItemManager::GetInstance()->GetItems();
        for (int j = 0; j < items.size(); j++) {
            auto item = items[j];
            if (item->GetItemType() == ItemType::FIREBALL) {
                if (IsIntersecting(mobA->GetPosition(), mobA->GetScaledSize(), item->m_Transform.translation, item->GetScaledSize())
                    && mobA->Gettype() != EnemyType::LIFT && mobA->IsActive()) {
                    mobA->popDead(-mobA->GetmoveDirection());
                    item->SetIsDead(true);
                }
            }
        }
        if (mobA->IsDead()) {
            if (mobA->GetTimer() < 0) {
                m_DynamicEntities.erase(m_DynamicEntities.begin() + i);
                RemoveChild(mobA);
                i--;
            }
            continue;
        }
        if (mobA->Gettype() == EnemyType::LIFT && !Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
            if (!mario->GetM_isDead() && IsIntersecting(mario->GetPosition(), mario->GetSize(), mobA->GetTransform().translation, {mobA->GetScaledSize().x, 32.0f})) {
                mobA->OnHit(mario);
            }
        }
        else {
            if (!mario->GetM_isDead() && IsIntersecting(mario->GetPosition(), mario->GetSize(), mobA->GetTransform().translation, mobA->GetScaledSize())) {
                mobA->OnHit(mario);
            }
        }
        for (int j = i+1; j < m_DynamicEntities.size(); j++) {
            auto mobB = m_DynamicEntities[j];
            if (mobB->IsDead()) continue;
            if (IsIntersecting(mobA->GetPosition(), mobA->GetScaledSize(), mobB->GetPosition(), mobB->GetScaledSize())) {
                if (mobA->Gettype() == EnemyType::Koopa && std::dynamic_pointer_cast<Koopa>(mobA)->isShellMoving()) {
                    int direction = mobA->GetPosition().x <= mobB->GetPosition().x? 1 : -1;
                    int score = comboShellScore[std::dynamic_pointer_cast<Koopa>(mobA)->GetComboCount()];
                    std::dynamic_pointer_cast<Koopa>(mobA)->AddComboCount();
                    EffectManager::GetInstance()->ShowScoreText(mobA->GetTransform().translation, score);
                    GameHUD::GetInstance()->AddScore(score);
                    mobB->popDead(direction, false);
                }
                else if (mobB->Gettype() == EnemyType::Koopa && std::dynamic_pointer_cast<Koopa>(mobB)->isShellMoving()) {
                    int direction = mobA->GetPosition().x <= mobB->GetPosition().x? 1 : -1;
                    int score = comboShellScore[std::dynamic_pointer_cast<Koopa>(mobB)->GetComboCount()];
                    std::dynamic_pointer_cast<Koopa>(mobB)->AddComboCount();
                    EffectManager::GetInstance()->ShowScoreText(mobB->GetTransform().translation, score);
                    GameHUD::GetInstance()->AddScore(score);
                    mobA->popDead(direction, false);
                }
                else {
                    mobA->OnHit();
                    mobB->OnHit();
                }
            }
        }
        if (mobA->IsDead()) {
            if (mobA->GetTimer() < 0) {
                m_DynamicEntities.erase(m_DynamicEntities.begin() + i);
                RemoveChild(mobA);
                i--;
            }
            continue;
        }

        if (mobA->IsDead()) {
            if (mobA->GetTimer() < 0) {
                m_DynamicEntities.erase(m_DynamicEntities.begin() + i);
                RemoveChild(mobA);
                i--;
            }
        }
    }
    for (const auto& pipe : m_pipesData) {
        glm::vec2 pipePosition = {pipe.x, pipe.y - pipe.height};
        glm::vec2 pipeSize = {pipe.width, pipe.height};
        // if (pipe.direction == "none") LOG_DEBUG(pipePosition);
        if (IsIntersecting(mario->GetPosition(), mario->GetSize(), pipePosition, pipeSize) && mario->isCollidable()) {
            if (pipe.direction == "none") {
                mario->SetPosition({pipePosition.x -32, mario->GetPosition().y});
                mario->FinishLevel();
                if (m_currentLevel == "Level1_3") GameHUD::GetInstance()->SetIsFinishGame(true);
                m_currentLevel = pipe.target_map;
                m_playerLoadPosition = {pipe.target_x, pipe.target_y};
                m_playLevelScreen = true;
            }
            if (Util::Input::IsKeyPressed(Util::Keycode::S) && pipe.direction == "down") {
                mario->Drill(Direction::BOTTOM, 64); // 執行鑽水管 (Drill Down)
                m_currentLevel = pipe.target_map;
                m_playerLoadPosition = {pipe.target_x, pipe.target_y};
                m_playLevelScreen = false;
            }
            if (Util::Input::IsKeyPressed(Util::Keycode::D) && pipe.direction == "right") {
                mario->Drill(Direction::RIGHT, 48); // 執行鑽水管 (Drill Down)
                m_currentLevel = pipe.target_map;
                m_playerLoadPosition = {pipe.target_x, pipe.target_y};
                m_playLevelScreen = false;
            }
            break;

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
        m_pipesData = LoadPipesData(mapData);
        LOG_DEBUG(m_pipesData.size());
        m_MapWidth = mapData.at("width");   // 建議用 .at() 代替 []，找不到 Key 時會明確報錯
        m_MapHeight = mapData.at("height");
        m_TileSize = mapData.at("tilewidth");
        m_isSurface = mapData.at("properties").at(0).at("value").get<bool>();
        if (m_isSurface)
            glClearColor(0.36f, 0.58f, 0.98f, 1.0f);
        else
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        m_cameraMove = m_MapWidth > 40;
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

std::vector<LevelManager::PipeData> LevelManager::LoadPipesData(json map_json) {
    std::vector<PipeData> pipes;

    for (const auto& layer : map_json["layers"]) {
        if (layer["type"] == "objectgroup" && layer["name"] == "obj") {
            for (const auto& obj : layer["objects"]) {

                if (obj.value("type", "") == "Lift" || obj.value("class", "") == "Lift") {
                    float x = -610 + obj["x"].get<float>()*2;
                    float y = 240 -obj["y"].get<float>()*2;
                    auto props = obj["properties"];
                    auto mob = std::make_shared<Lift>(x, y);
                    mob->SetDirection({get_custom_property<float>(props, "direction_x", 0.0f), get_custom_property<float>(props, "direction_y", 1.0f)});
                    mob->SetDistance(get_custom_property<int>(props, "distance", 0)*2);
                    m_DynamicEntities.push_back(mob);
                    this->AddChild(mob);
                }
                if (obj.value("type", "") == "Pipe" || obj.value("class", "") == "Pipe") {
                    PipeData pipe;
                    pipe.x = -610 + obj["x"].get<float>()*2;
                    pipe.y = 240 -obj["y"].get<float>()*2;
                    pipe.width = obj["width"].get<float>();
                    pipe.height = obj["height"].get<float>();

                    // 提取自定義屬性 (Custom Properties)
                    auto props = obj["properties"];
                    pipe.direction = get_custom_property<std::string>(props, "direction", "none");
                    pipe.target_map = get_custom_property<std::string>(props, "target_map", "Level1_1");
                    pipe.playLevelScreen = get_custom_property<bool>(props, "PlayLevelScreen", false);
                    pipe.target_x = get_custom_property<float>(props, "target_x", 0.0f);
                    pipe.target_y = get_custom_property<float>(props, "target_y", 0.0f);

                    pipes.push_back(pipe);
                }
            }
        }
    }
    return pipes;
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
        return -1;
    }

    int tileID = m_CollisionData[gridY * m_MapWidth + gridX];
    if (tileID == -1 || m_StaticTiles[tileID]->GetVisible() == false) return -1;
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
    return  position.x <= -(WINDOW_WIDTH/2.f) - 32 - m_Transform.translation.x ||
            position.x >= WINDOW_WIDTH/2.f + 32 - m_Transform.translation.x ||
            position.y <= -(WINDOW_HEIGHT/2.f);
}

bool LevelManager::IsInsideCamera(glm::vec2 position) {
    return  position.x > -(WINDOW_WIDTH/2.f) - 15 - m_Transform.translation.x &&
            position.x < WINDOW_WIDTH/2.f + 15 - m_Transform.translation.x &&
            position.y > -(WINDOW_HEIGHT/2.f);
}

int LevelManager::CheckCollision(float x, float y, float w, float h) {
    return 0;
}

int LevelManager::CheckCollision(float x, float y, float w, float h, Direction d) {
    int tileID = -1;
    w /= 2;
    h /= 2;
    if (d == Direction::BOTTOM) {
        tileID = IsSolid(x , y - h);         //正下
        if (tileID != -1) return tileID;
        tileID = IsSolid(x - w, y - h);      //左下
        if (tileID != -1) return tileID;
        tileID = IsSolid(x + w, y - h);      //右下
        if (tileID != -1) return tileID;
    }
    if (d == Direction::TOP) {
        tileID = IsSolid(x, y + h);        //正上
        if (tileID != -1) return tileID;
        tileID = IsSolid(x - w, y + h);    //左上
        if (tileID != -1) return tileID;
        tileID = IsSolid(x + w, y + h);    //右上
        if (tileID != -1) return tileID;
    }
    if (d == Direction::LEFT || d == Direction::BOTHSIDE) {
        tileID = IsSolid(x - w, y - h/2);        //左中
        if (tileID != -1) return tileID;
        tileID = IsSolid(x - w, y + h/2);        //左中
        if (tileID != -1) return tileID;
        tileID = IsSolid(x - w, y + h);          //左上
        if (tileID != -1) return tileID;
    }
    if (d == Direction::RIGHT || d == Direction::BOTHSIDE) {
        tileID = IsSolid(x + w, y - h/2);      //右中
        if (tileID != -1) return tileID;
        tileID = IsSolid(x + w, y + h/2);      //右中
        if (tileID != -1) return tileID;
        tileID = IsSolid(x + w, y + h);        //右上
        if (tileID != -1) return tileID;
    }
    return -1;
}

void LevelManager::ParseStaticTiles(const json& layer, bool is_collision, const std::vector<int>& itemData) {
    const auto& data = layer["data"];
    int id = 0;
    for (int i = 0; i < data.size(); ++i) {
        int tileID = data[i];
        if (tileID == 0) {
            if (is_collision) m_CollisionData.push_back(-1);
            continue;
        }

        int x = i % m_MapWidth;
        int y = m_MapHeight - i / m_MapWidth;

        float posX = std::round(x * m_TileSize - 540 - 64 - 16);
        float posY = std::round(y * m_TileSize - (WINDOW_HEIGHT - m_TileSize)/2);

        std::string texPath = GetTexturePathByID(tileID);
        if (tileID == 144) { // Question Block
            auto qBlock = std::make_shared<QuestionBlock>(texPath, posX, posY, m_isSurface);
            qBlock->SetContent(itemData[i]);
            if (itemData[i] == 0) // 空方塊自動填充金幣
                qBlock->SetContent(160);

            m_StaticTiles.push_back(qBlock);
        }
        else if (tileID == 6 || tileID == 31) { // brick
            auto brick = std::make_shared<Brick>(posX, posY, tileID == 6);
            brick->SetContent(itemData[i]);
            m_StaticTiles.push_back(brick);
        }
        else if (tileID == 160 || tileID == 161) { // coin
            ItemManager::GetInstance()->SpawnItem(tileID, {posX, posY}, false);
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

        if (tileID == 163) { // Goomba
            auto mob = std::make_shared<Goomba>(posX, posY, !m_isSurface);
            m_DynamicEntities.push_back(mob);
            this->AddChild(mob);
        }
        if (tileID == 165) { // Koopa
            auto mob = std::make_shared<Koopa>(posX, posY, !m_isSurface);
            m_DynamicEntities.push_back(mob);
            this->AddChild(mob);
        }
        if (tileID == 168) { // Red Koopa
            auto mob = std::make_shared<Koopa>(posX, posY, !m_isSurface);
            m_DynamicEntities.push_back(mob);
            mob->ChangeColorRed();
            this->AddChild(mob);
        }
        if (tileID == 166) { // Fly Koopa
            auto mob = std::make_shared<Koopa>(posX, posY, !m_isSurface);
            m_DynamicEntities.push_back(mob);
            mob->SetFlying();
            this->AddChild(mob);
        }
        if (tileID == 167) { // Plant
            auto mob = std::make_shared<PiranhaPlant>(posX, posY);
            m_DynamicEntities.push_back(mob);
            this->AddChild(mob);
        }
    }
}


std::string LevelManager::GetTexturePathByID(int id) {
    if (m_TileDictionary.find(id) != m_TileDictionary.end()) {
        return RESOURCE_DIR"/" + m_TileDictionary[id];
    }
    return RESOURCE_DIR"/Image/Tile/Brick.png";
}