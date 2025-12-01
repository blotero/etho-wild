#pragma once

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QThread>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDockWidget>
#include <QTreeWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QTableWidget>

#include "VideoWorker.hpp"
#include "BehaviorRecord.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

public slots:
    // Received from Worker
    void updateFrame(const QImage& frame);
    void onVideoOpened(double duration, double fps, int width, int height);
    void onPositionChanged(double pos);
    void onVideoError(const QString& message);
    
    // User Actions
    void openVideo();
    void openVideoDirectory();
    void togglePlayPause();
    void onSliderPressed();
    void onSliderReleased();
    void onSliderMoved(int value);
    void onSpeedChanged(int index);
    
    // Behavior recording
    void onBehaviorDoubleClicked(QTreeWidgetItem* item, int column);
    void toggleBehavior(const QString& parentCategory, const QString& behavior, const QString& type);
    void deleteRecord(int index);
    void saveRecords();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void setupUi();
    void setupDockWidgets();
    void setupBehaviorTree();
    void setupControlsDock();
    void setupRecordsDock();
    void startWorker(const QString& path);
    void loadNextVideo();
    void loadPrevVideo();
    void updateRecordsDisplay();
    void clearActiveState();
    
    double currentPosition() const { return m_currentPosition; }

    // UI Elements - Main
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;
    QGraphicsPixmapItem* m_pixmapItem;
    
    QPushButton* m_playButton;
    QSlider* m_seekSlider;
    QLabel* m_timeLabel;
    QComboBox* m_speedCombo;
    QPushButton* m_prevButton;
    QPushButton* m_nextButton;
    
    // Dock Widgets
    QDockWidget* m_behaviorDock;
    QDockWidget* m_controlsDock;
    QDockWidget* m_recordsDock;
    
    // Behavior Tree
    QTreeWidget* m_behaviorTree;
    
    // Controls
    QLineEdit* m_tagEdit;
    QComboBox* m_roleCombo;
    QComboBox* m_groupTypeCombo;
    QComboBox* m_sexCombo;
    QComboBox* m_stageCombo;
    QSpinBox* m_groupSizeSpin;
    QSpinBox* m_motherCalvesSpin;
    QSpinBox* m_calvesSpin;
    QLineEdit* m_observationsEdit;
    QLabel* m_stateFeedbackLabel;
    
    // Records
    QTableWidget* m_recordsTable;
    QPushButton* m_saveButton;
    
    // Threading
    QThread* m_workerThread;
    VideoWorker* m_worker;
    
    // Video state
    bool m_isSliderPressed;
    double m_duration;
    double m_currentPosition;
    
    // Video directory navigation
    QString m_videoDir;
    QStringList m_videoFiles;
    int m_currentVideoIndex;
    
    // Behavior recording state
    QVector<BehaviorRecord> m_records;
    QString m_currentStateBehavior;
    QString m_currentStateParent;
    double m_stateStartTime;
    bool m_stateActive;
};
