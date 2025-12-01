#include "CsvExporter.hpp"

#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDir>

bool CsvExporter::exportRecords(const QString& filePath, 
                                const QVector<BehaviorRecord>& records) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream out(&file);
    
    // Write header
    out << "session,role,behaviour,parent_behaviour,start_time,end_time,"
        << "duration,record_type,tag,group_type,sex,observations,stage,"
        << "group_size,mother_and_calf,calves,start_time_str,end_time_str\n";
    
    // Write records
    for (const BehaviorRecord& r : records) {
        // Helper to escape CSV fields
        auto escapeField = [](const QString& s) -> QString {
            if (s.contains(',') || s.contains('"') || s.contains('\n')) {
                QString escaped = s;
                escaped.replace("\"", "\"\"");
                return "\"" + escaped + "\"";
            }
            return s;
        };
        
        auto optIntToStr = [](const std::optional<int>& opt) -> QString {
            return opt.has_value() ? QString::number(opt.value()) : "";
        };
        
        auto optDoubleToStr = [](const std::optional<double>& opt) -> QString {
            return opt.has_value() ? QString::number(opt.value(), 'f', 3) : "";
        };
        
        out << r.session << ","
            << escapeField(r.role) << ","
            << escapeField(r.behaviour) << ","
            << escapeField(r.parentBehaviour) << ","
            << QString::number(r.startTime, 'f', 3) << ","
            << optDoubleToStr(r.endTime) << ","
            << QString::number(r.duration, 'f', 3) << ","
            << escapeField(r.recordType) << ","
            << escapeField(r.tag) << ","
            << escapeField(r.groupType) << ","
            << escapeField(r.sex) << ","
            << escapeField(r.observations) << ","
            << escapeField(r.stage) << ","
            << optIntToStr(r.groupSize) << ","
            << optIntToStr(r.motherAndCalf) << ","
            << optIntToStr(r.calves) << ","
            << r.startTimeStr() << ","
            << r.endTimeStr() << "\n";
    }
    
    file.close();
    return true;
}

QString CsvExporter::generateUniqueFilePath(const QString& directory, 
                                            const QString& baseName) {
    QString path = QDir(directory).filePath(baseName + ".csv");
    
    if (!QFile::exists(path)) {
        return path;
    }
    
    int counter = 1;
    while (true) {
        path = QDir(directory).filePath(QString("%1_%2.csv").arg(baseName).arg(counter));
        if (!QFile::exists(path)) {
            return path;
        }
        counter++;
    }
}

