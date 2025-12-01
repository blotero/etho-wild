#pragma once

#include "BehaviorRecord.hpp"
#include <QString>
#include <QVector>

class CsvExporter {
public:
    static bool exportRecords(const QString& filePath, 
                              const QVector<BehaviorRecord>& records);
    
    // Generate a unique filename (auto-increment if exists)
    static QString generateUniqueFilePath(const QString& directory, 
                                          const QString& baseName);
};

