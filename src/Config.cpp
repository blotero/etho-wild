#include "Config.hpp"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

Config& Config::instance() {
    static Config instance;
    return instance;
}

bool Config::loadFromDefaultPath() {
    // Try multiple locations in order:
    // 1. Next to executable
    // 2. Current working directory
    // 3. One level up from executable (for build directories)
    
    QStringList searchPaths = {
        QCoreApplication::applicationDirPath() + "/behaviors.json",
        QDir::currentPath() + "/behaviors.json",
        QCoreApplication::applicationDirPath() + "/../behaviors.json",
        QCoreApplication::applicationDirPath() + "/../cpp/behaviors.json"
    };
    
    for (const QString& path : searchPaths) {
        if (QFile::exists(path)) {
            qDebug() << "Loading config from:" << path;
            return load(path);
        }
    }
    
    m_lastError = "Could not find behaviors.json in any of the search paths";
    return false;
}

bool Config::load(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        m_lastError = QString("Could not open config file: %1").arg(path);
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        m_lastError = QString("JSON parse error: %1").arg(parseError.errorString());
        return false;
    }
    
    if (!doc.isObject()) {
        m_lastError = "Config file root must be an object";
        return false;
    }
    
    QJsonObject root = doc.object();
    
    // Parse behaviors
    m_behaviorCategories.clear();
    if (root.contains("behaviors") && root["behaviors"].isObject()) {
        QJsonObject behaviors = root["behaviors"].toObject();
        for (auto it = behaviors.begin(); it != behaviors.end(); ++it) {
            BehaviorCategory category;
            category.name = it.key();
            
            if (it.value().isObject()) {
                QJsonObject categoryBehaviors = it.value().toObject();
                for (auto bit = categoryBehaviors.begin(); bit != categoryBehaviors.end(); ++bit) {
                    BehaviorInfo info;
                    info.name = bit.key();
                    info.type = bit.value().toString();
                    category.behaviors.append(info);
                }
            }
            m_behaviorCategories.append(category);
        }
    }
    
    // Parse roles
    m_roles.clear();
    if (root.contains("roles") && root["roles"].isArray()) {
        QJsonArray roles = root["roles"].toArray();
        for (const QJsonValue& v : roles) {
            m_roles.append(v.toString());
        }
    }
    
    // Parse sexes
    m_sexes.clear();
    if (root.contains("sexes") && root["sexes"].isArray()) {
        QJsonArray sexes = root["sexes"].toArray();
        for (const QJsonValue& v : sexes) {
            m_sexes.append(v.toString());
        }
    }
    
    // Parse stages
    m_stages.clear();
    if (root.contains("stages") && root["stages"].isArray()) {
        QJsonArray stages = root["stages"].toArray();
        for (const QJsonValue& v : stages) {
            m_stages.append(v.toString());
        }
    }
    
    // Parse group types
    m_groupTypes.clear();
    if (root.contains("group_types") && root["group_types"].isArray()) {
        QJsonArray groupTypes = root["group_types"].toArray();
        for (const QJsonValue& v : groupTypes) {
            m_groupTypes.append(v.toString());
        }
    }
    
    qDebug() << "Loaded" << m_behaviorCategories.size() << "behavior categories";
    return true;
}

