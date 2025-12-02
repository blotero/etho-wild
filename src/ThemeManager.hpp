#pragma once

#include <QObject>
#include <QString>

class ThemeManager : public QObject {
    Q_OBJECT

public:
    enum class Theme {
        Light,
        Dark
    };
    Q_ENUM(Theme)

    static ThemeManager& instance();

    Theme currentTheme() const { return m_currentTheme; }
    void setTheme(Theme theme);
    void applyTheme();

    QString lightStyleSheet() const;
    QString darkStyleSheet() const;

signals:
    void themeChanged(Theme newTheme);

private:
    ThemeManager();
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;

    void loadSettings();
    void saveSettings();

    Theme m_currentTheme;
};

