#include "configs/loaders/LevelConfigLoader.h"
#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

USING_NS_CC;

LevelConfig LevelConfigLoader::loadFromFile(const std::string& filename) {
    CCLOG("========== LevelConfigLoader::loadFromFile ==========");

    // 添加路径前缀
    std::string fullFilename = "res/levels/" + filename;
    CCLOG("Loading level config:  %s", fullFilename.c_str());

    LevelConfig config;

    // 获取完整文件路径
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(fullFilename);
    CCLOG("Full path: %s", fullPath.c_str());

    // 读取文件内容
    std::string content = FileUtils::getInstance()->getStringFromFile(fullPath);
    if (content.empty()) {
        CCLOG("ERROR: Failed to load file or file is empty");
        return config;
    }

    CCLOG("File content length: %zu", content.size());

    // 解析JSON
    rapidjson::Document doc;
    doc.Parse(content.c_str());

    if (doc.HasParseError()) {
        CCLOG("ERROR:  JSON parse error at offset %zu:  %d",
            doc.GetErrorOffset(), doc.GetParseError());
        return config;
    }

    // 解析 Playfield（桌面牌区）
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const auto& playfieldArray = doc["Playfield"];
        CCLOG("Playfield cards:  %u", playfieldArray.Size());

        for (rapidjson::SizeType i = 0; i < playfieldArray.Size(); ++i) {
            const auto& cardObj = playfieldArray[i];

            if (!cardObj.HasMember("CardFace") || !cardObj.HasMember("CardSuit") ||
                !cardObj.HasMember("Position")) {
                CCLOG("WARNING: Card %u missing required fields", i);
                continue;
            }

            CardConfig card;
            card.face = cardObj["CardFace"].GetInt();
            card.suit = cardObj["CardSuit"].GetInt();
            card.position.x = cardObj["Position"]["x"].GetFloat();
            card.position.y = cardObj["Position"]["y"].GetFloat();

            config.playfieldCards.push_back(card);
            CCLOG("  Card %u: face=%d, suit=%d, pos=(%.1f, %.1f)",
                i, card.face, card.suit, card.position.x, card.position.y);
        }
    }

    // 解析 Stack（备用牌堆）
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const auto& stackArray = doc["Stack"];
        CCLOG("Stack cards:  %u", stackArray.Size());

        for (rapidjson::SizeType i = 0; i < stackArray.Size(); ++i) {
            const auto& cardObj = stackArray[i];

            if (!cardObj.HasMember("CardFace") || !cardObj.HasMember("CardSuit") ||
                !cardObj.HasMember("Position")) {
                CCLOG("WARNING: Card %u missing required fields", i);
                continue;
            }

            CardConfig card;
            card.face = cardObj["CardFace"].GetInt();
            card.suit = cardObj["CardSuit"].GetInt();
            card.position.x = cardObj["Position"]["x"].GetFloat();
            card.position.y = cardObj["Position"]["y"].GetFloat();

            config.stackCards.push_back(card);
            CCLOG("  Card %u: face=%d, suit=%d, pos=(%. 1f, %.1f)",
                i, card.face, card.suit, card.position.x, card.position.y);
        }
    }

    CCLOG("Level config loaded successfully");
    CCLOG("======================================================");

    return config;
}