#include "ThemeManager.hpp"

#include <QApplication>
#include <QSettings>
#include <QFont>

ThemeManager& ThemeManager::instance() {
    static ThemeManager instance;
    return instance;
}

ThemeManager::ThemeManager()
    : m_currentTheme(Theme::Light)
{
    loadSettings();
}

void ThemeManager::loadSettings() {
    QSettings settings("EthoWild", "EthoWild");
    int themeValue = settings.value("theme", static_cast<int>(Theme::Light)).toInt();
    m_currentTheme = static_cast<Theme>(themeValue);
}

void ThemeManager::saveSettings() {
    QSettings settings("EthoWild", "EthoWild");
    settings.setValue("theme", static_cast<int>(m_currentTheme));
}

void ThemeManager::setTheme(Theme theme) {
    if (m_currentTheme != theme) {
        m_currentTheme = theme;
        saveSettings();
        applyTheme();
        emit themeChanged(theme);
    }
}

void ThemeManager::applyTheme() {
    QString styleSheet = (m_currentTheme == Theme::Light) 
        ? lightStyleSheet() 
        : darkStyleSheet();
    qApp->setStyleSheet(styleSheet);
}

QString ThemeManager::lightStyleSheet() const {
    return QStringLiteral(R"(
/* ========== Light Theme - Teal/Amber Palette ========== */

/* Color Variables (used in comments for reference):
   Primary Teal: #009688
   Primary Dark: #00796B
   Primary Light: #4DB6AC
   Accent Amber: #FFC107
   Accent Dark: #FFB300
   Background: #FAFAFA
   Surface: #FFFFFF
   Text Primary: #212121
   Text Secondary: #757575
   Border: #E0E0E0
*/

/* Global */
QWidget {
    background-color: #FAFAFA;
    color: #212121;
    font-family: "Segoe UI", "Roboto", "Noto Sans", sans-serif;
    font-size: 13px;
}

QMainWindow {
    background-color: #FAFAFA;
}

/* Menu Bar */
QMenuBar {
    background-color: #009688;
    color: #FFFFFF;
    padding: 4px 0px;
    border: none;
}

QMenuBar::item {
    background-color: transparent;
    padding: 6px 12px;
    border-radius: 4px;
    margin: 0px 2px;
}

QMenuBar::item:selected {
    background-color: #00796B;
}

QMenuBar::item:pressed {
    background-color: #00695C;
}

/* Menus */
QMenu {
    background-color: #FFFFFF;
    border: 1px solid #E0E0E0;
    border-radius: 8px;
    padding: 8px 0px;
}

QMenu::item {
    padding: 8px 32px 8px 16px;
    margin: 2px 8px;
    border-radius: 4px;
}

QMenu::item:selected {
    background-color: #E0F2F1;
    color: #00796B;
}

QMenu::separator {
    height: 1px;
    background-color: #E0E0E0;
    margin: 8px 16px;
}

QMenu::indicator {
    width: 18px;
    height: 18px;
    margin-left: 8px;
}

/* Push Buttons */
QPushButton {
    background-color: #009688;
    color: #FFFFFF;
    border: none;
    border-radius: 6px;
    padding: 8px 16px;
    font-weight: 500;
    min-height: 20px;
}

QPushButton:hover {
    background-color: #00897B;
}

QPushButton:pressed {
    background-color: #00796B;
}

QPushButton:disabled {
    background-color: #BDBDBD;
    color: #757575;
}

/* Flat/Icon Buttons (for control buttons) */
QPushButton[flat="true"], 
QPushButton#controlButton {
    background-color: transparent;
    color: #009688;
    border: 2px solid #009688;
}

QPushButton[flat="true"]:hover,
QPushButton#controlButton:hover {
    background-color: #E0F2F1;
}

/* Delete Button in Tables */
QPushButton#deleteButton {
    background-color: transparent;
    color: #D32F2F;
    border: 1px solid #E0E0E0;
    border-radius: 4px;
    font-size: 16px;
    font-weight: bold;
    padding: 0px;
    min-height: 24px;
    min-width: 24px;
}

QPushButton#deleteButton:hover {
    background-color: #FFEBEE;
    border-color: #D32F2F;
}

QPushButton#deleteButton:pressed {
    background-color: #FFCDD2;
}

/* Line Edit */
QLineEdit {
    background-color: #FFFFFF;
    border: 2px solid #E0E0E0;
    border-radius: 6px;
    selection-background-color: #4DB6AC;
}

QLineEdit:focus {
    border-color: #009688;
}

QLineEdit:disabled {
    background-color: #F5F5F5;
    color: #9E9E9E;
}


/* Slider */
QSlider::groove:horizontal {
    height: 6px;
    background-color: #E0E0E0;
    border-radius: 3px;
}

QSlider::handle:horizontal {
    background-color: #009688;
    width: 18px;
    height: 18px;
    margin: -6px 0;
    border-radius: 9px;
}

QSlider::handle:horizontal:hover {
    background-color: #00897B;
}

QSlider::sub-page:horizontal {
    background-color: #4DB6AC;
    border-radius: 3px;
}

/* Dock Widgets */
QDockWidget {
    titlebar-close-icon: url(none);
    titlebar-normal-icon: url(none);
    font-weight: 600;
}

QDockWidget::title {
    background-color: #009688;
    color: #FFFFFF;
    padding: 10px 12px;
    text-align: left;
}

QDockWidget::close-button, QDockWidget::float-button {
    background-color: transparent;
    border: none;
    padding: 4px;
}

QDockWidget::close-button:hover, QDockWidget::float-button:hover {
    background-color: #00796B;
    border-radius: 4px;
}

/* Tree Widget */
QTreeWidget {
    background-color: #FFFFFF;
    border: 1px solid #E0E0E0;
    border-radius: 6px;
    alternate-background-color: #FAFAFA;
}

QTreeWidget::item {
    padding: 4px 6px;
    border-radius: 4px;
}

QTreeWidget::item:selected {
    background-color: #E0F2F1;
    color: #00796B;
}

QTreeWidget::item:hover {
    background-color: #F5F5F5;
}


QHeaderView::section {
    background-color: #F5F5F5;
    color: #212121;
    padding: 10px 12px;
    border: none;
    border-bottom: 2px solid #009688;
    font-weight: 600;
}

/* Table Widget */
QTableWidget {
    background-color: #FFFFFF;
    border: 1px solid #E0E0E0;
    border-radius: 6px;
    alternate-background-color: #FAFAFA;
    gridline-color: #EEEEEE;
    outline: none;
}

QTableWidget::item {
    padding: 8px;
}

QTableWidget::item:selected {
    background-color: #E0F2F1;
    color: #00796B;
}

/* Scroll Bars */
QScrollBar:vertical {
    background-color: #F5F5F5;
    width: 12px;
    border-radius: 6px;
    margin: 0;
}

QScrollBar::handle:vertical {
    background-color: #BDBDBD;
    border-radius: 5px;
    min-height: 30px;
    margin: 2px;
}

QScrollBar::handle:vertical:hover {
    background-color: #4DB6AC;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0;
}

QScrollBar:horizontal {
    background-color: #F5F5F5;
    height: 12px;
    border-radius: 6px;
    margin: 0;
}

QScrollBar::handle:horizontal {
    background-color: #BDBDBD;
    border-radius: 5px;
    min-width: 30px;
    margin: 2px;
}

QScrollBar::handle:horizontal:hover {
    background-color: #4DB6AC;
}

QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0;
}

/* Graphics View (Video Area) */
QGraphicsView {
    border: 2px solid #E0E0E0;
    border-radius: 8px;
    background-color: #212121;
}

/* Labels */
QLabel {
    background-color: transparent;
    color: #212121;
}

/* Group Box */
QGroupBox {
    font-weight: 600;
    border: 2px solid #E0E0E0;
    border-radius: 8px;
    margin-top: 12px;
    padding-top: 12px;
}

QGroupBox::title {
    subcontrol-origin: margin;
    left: 12px;
    padding: 0 8px;
    color: #009688;
}

/* Tool Tips */
QToolTip {
    background-color: #424242;
    color: #FFFFFF;
    border: none;
    border-radius: 4px;
    padding: 6px 8px;
}

/* Message Box */
QMessageBox {
    background-color: #FFFFFF;
}

QMessageBox QPushButton {
    min-width: 80px;
}

/* Form Layout Labels */
QFormLayout QLabel {
    font-weight: 500;
    color: #757575;
}

/* State Feedback Label - Accent styling */
QLabel#stateFeedbackLabel {
    color: #009688;
    font-weight: bold;
    font-size: 14px;
}

)");
}

QString ThemeManager::darkStyleSheet() const {
    return QStringLiteral(R"(
/* ========== Dark Theme - Slate/Teal Palette ========== */

/* Color Variables (used in comments for reference):
   Primary Teal: #4DB6AC
   Primary Light: #80CBC4
   Accent Amber: #FFD54F
   Background: #1E1E2E
   Surface: #2D2D3F
   Surface Light: #3D3D50
   Text Primary: #E0E0E0
   Text Secondary: #A0A0A0
   Border: #3D3D50
*/

/* Global */
QWidget {
    background-color: #1E1E2E;
    color: #E0E0E0;
    font-family: "Segoe UI", "Roboto", "Noto Sans", sans-serif;
    font-size: 13px;
}

QMainWindow {
    background-color: #1E1E2E;
}

/* Menu Bar */
QMenuBar {
    background-color: #2D2D3F;
    color: #E0E0E0;
    padding: 4px 0px;
    border: none;
    border-bottom: 1px solid #3D3D50;
}

QMenuBar::item {
    background-color: transparent;
    padding: 6px 12px;
    border-radius: 4px;
    margin: 0px 2px;
}

QMenuBar::item:selected {
    background-color: #4DB6AC;
    color: #1E1E2E;
}

QMenuBar::item:pressed {
    background-color: #00897B;
}

/* Menus */
QMenu {
    background-color: #2D2D3F;
    border: 1px solid #3D3D50;
    border-radius: 8px;
    padding: 8px 0px;
}

QMenu::item {
    padding: 8px 32px 8px 16px;
    margin: 2px 8px;
    border-radius: 4px;
}

QMenu::item:selected {
    background-color: #3D3D50;
    color: #80CBC4;
}

QMenu::separator {
    height: 1px;
    background-color: #3D3D50;
    margin: 8px 16px;
}

QMenu::indicator {
    width: 18px;
    height: 18px;
    margin-left: 8px;
}

/* Push Buttons */
QPushButton {
    background-color: #4DB6AC;
    color: #1E1E2E;
    border: none;
    border-radius: 6px;
    padding: 8px 16px;
    font-weight: 600;
    min-height: 20px;
}

QPushButton:hover {
    background-color: #80CBC4;
}

QPushButton:pressed {
    background-color: #00897B;
    color: #FFFFFF;
}

QPushButton:disabled {
    background-color: #3D3D50;
    color: #6E6E7E;
}

/* Flat/Icon Buttons */
QPushButton[flat="true"],
QPushButton#controlButton {
    background-color: transparent;
    color: #4DB6AC;
    border: 2px solid #4DB6AC;
}

QPushButton[flat="true"]:hover,
QPushButton#controlButton:hover {
    background-color: #3D3D50;
}

/* Delete Button in Tables */
QPushButton#deleteButton {
    background-color: transparent;
    color: #EF5350;
    border: 1px solid #3D3D50;
    border-radius: 4px;
    font-size: 16px;
    font-weight: bold;
    padding: 0px;
    min-height: 24px;
    min-width: 24px;
}

QPushButton#deleteButton:hover {
    background-color: #4A2020;
    border-color: #EF5350;
}

QPushButton#deleteButton:pressed {
    background-color: #5C2828;
}

/* Line Edit */
QLineEdit {
    background-color: #2D2D3F;
    border: 2px solid #3D3D50;
    border-radius: 6px;
    color: #E0E0E0;
    selection-background-color: #4DB6AC;
    selection-color: #1E1E2E;
}

QLineEdit:focus {
    border-color: #4DB6AC;
}

QLineEdit:disabled {
    background-color: #252535;
    color: #6E6E7E;
}

QLineEdit::placeholder {
    color: #6E6E7E;
}



/* Slider */
QSlider::groove:horizontal {
    height: 6px;
    background-color: #3D3D50;
    border-radius: 3px;
}

QSlider::handle:horizontal {
    background-color: #4DB6AC;
    width: 18px;
    height: 18px;
    margin: -6px 0;
    border-radius: 9px;
}

QSlider::handle:horizontal:hover {
    background-color: #80CBC4;
}

QSlider::sub-page:horizontal {
    background-color: #00897B;
    border-radius: 3px;
}

/* Dock Widgets */
QDockWidget {
    titlebar-close-icon: url(none);
    titlebar-normal-icon: url(none);
    font-weight: 600;
    color: #E0E0E0;
}

QDockWidget::title {
    background-color: #2D2D3F;
    color: #80CBC4;
    padding: 10px 12px;
    text-align: left;
    border-bottom: 2px solid #4DB6AC;
}

QDockWidget::close-button, QDockWidget::float-button {
    background-color: transparent;
    border: none;
    padding: 4px;
}

QDockWidget::close-button:hover, QDockWidget::float-button:hover {
    background-color: #3D3D50;
    border-radius: 4px;
}

/* Tree Widget */
QTreeWidget {
    background-color: #2D2D3F;
    border: 1px solid #3D3D50;
    border-radius: 6px;
    alternate-background-color: #252535;
}

QTreeWidget::item {
    padding: 4px 6px;
    border-radius: 4px;
    color: #E0E0E0;
}

QTreeWidget::item:selected {
    background-color: #3D3D50;
    color: #80CBC4;
}

QTreeWidget::item:hover {
    background-color: #353545;
}

QHeaderView::section {
    background-color: #252535;
    color: #E0E0E0;
    padding: 10px 12px;
    border: none;
    border-bottom: 2px solid #4DB6AC;
    font-weight: 600;
}

/* Table Widget */
QTableWidget {
    background-color: #2D2D3F;
    border: 1px solid #3D3D50;
    border-radius: 6px;
    alternate-background-color: #252535;
    gridline-color: #3D3D50;
    outline: none;
}

QTableWidget::item {
    padding: 8px;
    color: #E0E0E0;
}

QTableWidget::item:selected {
    background-color: #3D3D50;
    color: #80CBC4;
}

/* Scroll Bars */
QScrollBar:vertical {
    background-color: #252535;
    width: 12px;
    border-radius: 6px;
    margin: 0;
}

QScrollBar::handle:vertical {
    background-color: #4D4D60;
    border-radius: 5px;
    min-height: 30px;
    margin: 2px;
}

QScrollBar::handle:vertical:hover {
    background-color: #4DB6AC;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0;
}

QScrollBar:horizontal {
    background-color: #252535;
    height: 12px;
    border-radius: 6px;
    margin: 0;
}

QScrollBar::handle:horizontal {
    background-color: #4D4D60;
    border-radius: 5px;
    min-width: 30px;
    margin: 2px;
}

QScrollBar::handle:horizontal:hover {
    background-color: #4DB6AC;
}

QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0;
}

/* Graphics View (Video Area) */
QGraphicsView {
    border: 2px solid #3D3D50;
    border-radius: 8px;
    background-color: #000000;
}

/* Labels */
QLabel {
    background-color: transparent;
    color: #E0E0E0;
}

/* Group Box */
QGroupBox {
    font-weight: 600;
    border: 2px solid #3D3D50;
    border-radius: 8px;
    margin-top: 12px;
    padding-top: 12px;
    color: #E0E0E0;
}

QGroupBox::title {
    subcontrol-origin: margin;
    left: 12px;
    padding: 0 8px;
    color: #4DB6AC;
}

/* Tool Tips */
QToolTip {
    background-color: #3D3D50;
    color: #FFFFFF;
    border: 1px solid #4DB6AC;
    border-radius: 4px;
    padding: 6px 8px;
}

/* Message Box */
QMessageBox {
    background-color: #2D2D3F;
}

QMessageBox QLabel {
    color: #E0E0E0;
}

QMessageBox QPushButton {
    min-width: 80px;
}

/* Form Layout Labels */
QFormLayout QLabel {
    font-weight: 500;
    color: #A0A0A0;
}

/* State Feedback Label - Accent styling */
QLabel#stateFeedbackLabel {
    color: #4DB6AC;
    font-weight: bold;
    font-size: 14px;
}

/* File Dialog */
QFileDialog {
    background-color: #2D2D3F;
}

)");
}

