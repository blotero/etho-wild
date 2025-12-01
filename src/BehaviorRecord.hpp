#pragma once

#include <QString>
#include <optional>

struct BehaviorRecord {
    int session = 1;
    QString role;
    QString behaviour;
    QString parentBehaviour;
    double startTime = 0.0;
    std::optional<double> endTime;
    double duration = 0.0;
    QString recordType; // "EVENT" or "STATE"
    QString tag;
    QString groupType;
    QString sex;
    QString observations;
    QString stage;
    std::optional<int> groupSize;
    std::optional<int> motherAndCalf;
    std::optional<int> calves;
    
    // Format time as MM:SS
    static QString formatTime(double seconds) {
        int totalSeconds = static_cast<int>(seconds);
        int minutes = totalSeconds / 60;
        int secs = totalSeconds % 60;
        return QString("%1:%2")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(secs, 2, 10, QChar('0'));
    }
    
    QString startTimeStr() const {
        return formatTime(startTime);
    }
    
    QString endTimeStr() const {
        return endTime.has_value() ? formatTime(endTime.value()) : QString();
    }
    
    QString asDisplayString() const {
        QString base = QString(">> %1 [%2] -- Rol: %3 - Comportamiento: %4 - Tag: %5 - Tipo grupo: %6 - Sexo: %7 - Estadio: %8 - Tamaño grupal: %9")
            .arg(startTimeStr())
            .arg(recordType)
            .arg(role)
            .arg(behaviour)
            .arg(tag)
            .arg(groupType)
            .arg(sex)
            .arg(stage.isEmpty() ? "N/A" : stage)
            .arg(groupSize.has_value() ? QString::number(groupSize.value()) : "N/A");
        
        if (!observations.isEmpty()) {
            base += QString(" - Observaciones: %1").arg(observations);
        }
        
        base += QString(" - Madres con cría: %1 - Crías: %2")
            .arg(motherAndCalf.has_value() ? QString::number(motherAndCalf.value()) : "N/A")
            .arg(calves.has_value() ? QString::number(calves.value()) : "N/A");
        
        if (recordType == "STATE" && endTime.has_value()) {
            base += QString(" - Fin: %1 (Duración: %2s)")
                .arg(endTimeStr())
                .arg(QString::number(duration, 'f', 2));
        }
        
        return base;
    }
};

