#pragma once

#include <QString>
#include <QStringList>
#include <QMap>
#include <QVector>

struct BehaviorInfo {
    QString name;
    QString type; // "EVENT" or "STATE"
};

struct BehaviorCategory {
    QString name;
    QVector<BehaviorInfo> behaviors;
};

class Config {
public:
    static Config& instance();
    
    bool load(const QString& path);
    bool loadFromDefaultPath();
    
    const QVector<BehaviorCategory>& behaviorCategories() const { return m_behaviorCategories; }
    const QStringList& roles() const { return m_roles; }
    const QStringList& sexes() const { return m_sexes; }
    const QStringList& stages() const { return m_stages; }
    const QStringList& groupTypes() const { return m_groupTypes; }
    
    QString lastError() const { return m_lastError; }

private:
    Config() = default;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    
    QVector<BehaviorCategory> m_behaviorCategories;
    QStringList m_roles;
    QStringList m_sexes;
    QStringList m_stages;
    QStringList m_groupTypes;
    QString m_lastError;
};

