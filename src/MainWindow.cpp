#include "MainWindow.hpp"
#include "Config.hpp"
#include "CsvExporter.hpp"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTime>
#include <QMessageBox>
#include <QWheelEvent>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QDir>
#include <QFileInfo>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_workerThread(nullptr)
    , m_worker(nullptr)
    , m_isSliderPressed(false)
    , m_duration(0.0)
    , m_currentPosition(0.0)
    , m_currentVideoIndex(0)
    , m_stateStartTime(0.0)
    , m_stateActive(false)
{
    // Load configuration
    if (!Config::instance().loadFromDefaultPath()) {
        QMessageBox::warning(this, "Configuration Error", 
            Config::instance().lastError() + "\n\nUsing empty configuration.");
    }
    
    setupUi();
    setupDockWidgets();
    
    resize(1280, 720);
    setWindowTitle("Behaviour Labeling (C++ Port)");
}

MainWindow::~MainWindow() {
    // Clean shutdown of thread
    if (m_worker) {
        m_worker->stop();
    }
    if (m_workerThread) {
        m_workerThread->quit();
        m_workerThread->wait();
    }
}

void MainWindow::setupUi() {
    // Central Widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // 1. Video Area (QGraphicsView)
    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene);
    m_view->setRenderHint(QPainter::SmoothPixmapTransform);
    m_view->setDragMode(QGraphicsView::ScrollHandDrag); // Native Panning
    m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_view->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setBackgroundBrush(Qt::black);
    
    // Install event filter for Zooming
    m_view->viewport()->installEventFilter(this);
    
    m_pixmapItem = new QGraphicsPixmapItem();
    m_scene->addItem(m_pixmapItem);
    
    mainLayout->addWidget(m_view, 1); // Stretch factor 1
    
    // 2. Controls
    QHBoxLayout* controlsLayout = new QHBoxLayout();
    
    m_prevButton = new QPushButton("⏮");
    m_prevButton->setFixedWidth(40);
    connect(m_prevButton, &QPushButton::clicked, this, &MainWindow::loadPrevVideo);
    
    m_playButton = new QPushButton("▶");
    m_playButton->setFixedWidth(40);
    connect(m_playButton, &QPushButton::clicked, this, &MainWindow::togglePlayPause);
    
    m_nextButton = new QPushButton("⏭");
    m_nextButton->setFixedWidth(40);
    connect(m_nextButton, &QPushButton::clicked, this, &MainWindow::loadNextVideo);
    
    m_timeLabel = new QLabel("00:00 / 00:00");
    m_timeLabel->setMinimumWidth(100);
    
    m_seekSlider = new QSlider(Qt::Horizontal);
    m_seekSlider->setRange(0, 1000); // Precision 0.1%
    connect(m_seekSlider, &QSlider::sliderPressed, this, &MainWindow::onSliderPressed);
    connect(m_seekSlider, &QSlider::sliderReleased, this, &MainWindow::onSliderReleased);
    connect(m_seekSlider, &QSlider::valueChanged, this, &MainWindow::onSliderMoved);
    
    m_speedCombo = new QComboBox();
    m_speedCombo->addItems({"0.5x", "0.75x", "1.0x", "1.25x", "1.5x", "2.0x"});
    m_speedCombo->setCurrentIndex(2); // 1.0x default
    connect(m_speedCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onSpeedChanged);
    
    controlsLayout->addWidget(m_prevButton);
    controlsLayout->addWidget(m_playButton);
    controlsLayout->addWidget(m_nextButton);
    controlsLayout->addWidget(m_timeLabel);
    controlsLayout->addWidget(m_seekSlider, 1);
    controlsLayout->addWidget(m_speedCombo);
    
    mainLayout->addLayout(controlsLayout);
    
    // Menu
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* openAction = fileMenu->addAction("Open Video...");
    connect(openAction, &QAction::triggered, this, &MainWindow::openVideo);
    
    QAction* openDirAction = fileMenu->addAction("Open Video Directory...");
    connect(openDirAction, &QAction::triggered, this, &MainWindow::openVideoDirectory);
    
    fileMenu->addSeparator();
    
    QAction* saveAction = fileMenu->addAction("Save Records...");
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveRecords);
}

void MainWindow::setupDockWidgets() {
    // Allow docks on all sides
    setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
    
    // Behavior Tree Dock (Right side)
    m_behaviorDock = new QDockWidget("Behaviors", this);
    m_behaviorDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setupBehaviorTree();
    addDockWidget(Qt::RightDockWidgetArea, m_behaviorDock);
    
    // Controls Dock (Right side, below behavior tree)
    m_controlsDock = new QDockWidget("Controls", this);
    m_controlsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    setupControlsDock();
    addDockWidget(Qt::RightDockWidgetArea, m_controlsDock);
    
    // Records Dock (Bottom)
    m_recordsDock = new QDockWidget("Records", this);
    m_recordsDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setupRecordsDock();
    addDockWidget(Qt::BottomDockWidgetArea, m_recordsDock);
    
    // Add view menu for dock visibility
    QMenu* viewMenu = menuBar()->addMenu("View");
    viewMenu->addAction(m_behaviorDock->toggleViewAction());
    viewMenu->addAction(m_controlsDock->toggleViewAction());
    viewMenu->addAction(m_recordsDock->toggleViewAction());
}

void MainWindow::setupBehaviorTree() {
    m_behaviorTree = new QTreeWidget();
    m_behaviorTree->setHeaderLabels({"Behavior", "Type"});
    m_behaviorTree->setColumnWidth(0, 300);
    m_behaviorTree->setColumnWidth(1, 80);
    m_behaviorTree->setAlternatingRowColors(true);
    
    // Populate from config
    const auto& categories = Config::instance().behaviorCategories();
    for (const auto& category : categories) {
        QTreeWidgetItem* parentItem = new QTreeWidgetItem(m_behaviorTree);
        parentItem->setText(0, category.name);
        parentItem->setExpanded(true);
        
        for (const auto& behavior : category.behaviors) {
            QTreeWidgetItem* childItem = new QTreeWidgetItem(parentItem);
            childItem->setText(0, behavior.name);
            childItem->setText(1, behavior.type);
            
            // Color code by type
            if (behavior.type == "STATE") {
                childItem->setForeground(1, QBrush(QColor(0, 128, 0))); // Green for STATE
            } else {
                childItem->setForeground(1, QBrush(QColor(0, 0, 200))); // Blue for EVENT
            }
        }
    }
    
    connect(m_behaviorTree, &QTreeWidget::itemDoubleClicked, 
            this, &MainWindow::onBehaviorDoubleClicked);
    
    m_behaviorDock->setWidget(m_behaviorTree);
}

void MainWindow::setupControlsDock() {
    QWidget* container = new QWidget();
    QFormLayout* layout = new QFormLayout(container);
    layout->setSpacing(6);
    
    // Tag
    m_tagEdit = new QLineEdit();
    m_tagEdit->setPlaceholderText("Enter tag...");
    layout->addRow("Tag:", m_tagEdit);
    
    // Role
    m_roleCombo = new QComboBox();
    m_roleCombo->addItems(Config::instance().roles());
    if (m_roleCombo->count() > 0) {
        m_roleCombo->setCurrentIndex(m_roleCombo->count() - 1); // "indefinido"
    }
    layout->addRow("Role:", m_roleCombo);
    
    // Group Type
    m_groupTypeCombo = new QComboBox();
    m_groupTypeCombo->addItems(Config::instance().groupTypes());
    layout->addRow("Group Type:", m_groupTypeCombo);
    
    // Sex
    m_sexCombo = new QComboBox();
    m_sexCombo->addItems(Config::instance().sexes());
    if (m_sexCombo->count() > 0) {
        m_sexCombo->setCurrentIndex(m_sexCombo->count() - 1); // "indefinido"
    }
    layout->addRow("Sex:", m_sexCombo);
    
    // Stage
    m_stageCombo = new QComboBox();
    m_stageCombo->addItem(""); // Empty option
    m_stageCombo->addItems(Config::instance().stages());
    layout->addRow("Stage:", m_stageCombo);
    
    // Group Size
    m_groupSizeSpin = new QSpinBox();
    m_groupSizeSpin->setRange(0, 100);
    m_groupSizeSpin->setSpecialValueText("N/A");
    layout->addRow("Group Size:", m_groupSizeSpin);
    
    // Mother & Calves
    m_motherCalvesSpin = new QSpinBox();
    m_motherCalvesSpin->setRange(0, 100);
    m_motherCalvesSpin->setSpecialValueText("N/A");
    layout->addRow("Mother & Calves:", m_motherCalvesSpin);
    
    // Calves Only
    m_calvesSpin = new QSpinBox();
    m_calvesSpin->setRange(0, 100);
    m_calvesSpin->setSpecialValueText("N/A");
    layout->addRow("Calves:", m_calvesSpin);
    
    // Observations
    m_observationsEdit = new QLineEdit();
    m_observationsEdit->setPlaceholderText("Observations...");
    layout->addRow("Observations:", m_observationsEdit);
    
    // State feedback
    m_stateFeedbackLabel = new QLabel();
    m_stateFeedbackLabel->setStyleSheet("QLabel { color: green; font-weight: bold; }");
    layout->addRow("", m_stateFeedbackLabel);
    
    m_controlsDock->setWidget(container);
}

void MainWindow::setupRecordsDock() {
    QWidget* container = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(container);
    
    m_recordsTable = new QTableWidget();
    m_recordsTable->setColumnCount(4);
    m_recordsTable->setHorizontalHeaderLabels({"Time", "Behavior", "Type", ""});
    m_recordsTable->horizontalHeader()->setStretchLastSection(false);
    m_recordsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_recordsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_recordsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_recordsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    m_recordsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_recordsTable->setAlternatingRowColors(true);
    
    layout->addWidget(m_recordsTable);
    
    // Save button
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    m_saveButton = new QPushButton("💾 Save Records");
    connect(m_saveButton, &QPushButton::clicked, this, &MainWindow::saveRecords);
    buttonLayout->addWidget(m_saveButton);
    layout->addLayout(buttonLayout);
    
    m_recordsDock->setWidget(container);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (obj == m_view->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        if (wheelEvent->modifiers() & Qt::ControlModifier) {
            // Zoom Logic
            double scaleFactor = 1.15;
            if (wheelEvent->angleDelta().y() < 0) {
                scaleFactor = 1.0 / scaleFactor;
            }
            m_view->scale(scaleFactor, scaleFactor);
            return true; // Consume the event
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::openVideo() {
    QString path = QFileDialog::getOpenFileName(this, "Open Video", "", 
        "Video Files (*.mp4 *.avi *.mkv *.mov *.wmv)");
    if (!path.isEmpty()) {
        m_videoDir.clear();
        m_videoFiles.clear();
        m_currentVideoIndex = 0;
        startWorker(path);
    }
}

void MainWindow::openVideoDirectory() {
    QString dir = QFileDialog::getExistingDirectory(this, "Open Video Directory");
    if (!dir.isEmpty()) {
        m_videoDir = dir;
        
        // Find video files
        QDir qdir(dir);
        QStringList filters = {"*.mp4", "*.avi", "*.mkv", "*.mov", "*.wmv"};
        m_videoFiles = qdir.entryList(filters, QDir::Files, QDir::Name);
        
        if (m_videoFiles.isEmpty()) {
            QMessageBox::information(this, "No Videos", "No video files found in directory.");
            return;
        }
        
        m_currentVideoIndex = 0;
        m_records.clear();
        updateRecordsDisplay();
        clearActiveState();
        
        QString videoPath = qdir.filePath(m_videoFiles[m_currentVideoIndex]);
        startWorker(videoPath);
    }
}

void MainWindow::startWorker(const QString& path) {
    // Clean up previous
    if (m_workerThread) {
        m_worker->stop();
        m_workerThread->quit();
        m_workerThread->wait();
        delete m_worker;
        delete m_workerThread;
    }
    
    m_workerThread = new QThread;
    m_worker = new VideoWorker(path);
    m_worker->moveToThread(m_workerThread);
    
    // Connections
    connect(m_workerThread, &QThread::started, m_worker, &VideoWorker::process);
    connect(m_worker, &VideoWorker::frameReady, this, &MainWindow::updateFrame);
    connect(m_worker, &VideoWorker::videoOpened, this, &MainWindow::onVideoOpened);
    connect(m_worker, &VideoWorker::positionChanged, this, &MainWindow::onPositionChanged);
    connect(m_worker, &VideoWorker::errorOccurred, this, &MainWindow::onVideoError);
    connect(m_worker, &VideoWorker::finished, m_workerThread, &QThread::quit);
    
    // Start
    m_workerThread->start();
    m_playButton->setText("⏸");
    
    // Update window title
    QFileInfo fileInfo(path);
    setWindowTitle(QString("Behaviour Labeling - %1").arg(fileInfo.fileName()));
}

void MainWindow::loadNextVideo() {
    if (m_videoFiles.isEmpty()) return;
    
    m_currentVideoIndex = (m_currentVideoIndex + 1) % m_videoFiles.size();
    QString videoPath = QDir(m_videoDir).filePath(m_videoFiles[m_currentVideoIndex]);
    
    m_records.clear();
    updateRecordsDisplay();
    clearActiveState();
    
    startWorker(videoPath);
}

void MainWindow::loadPrevVideo() {
    if (m_videoFiles.isEmpty()) return;
    
    m_currentVideoIndex = (m_currentVideoIndex - 1 + m_videoFiles.size()) % m_videoFiles.size();
    QString videoPath = QDir(m_videoDir).filePath(m_videoFiles[m_currentVideoIndex]);
    
    m_records.clear();
    updateRecordsDisplay();
    clearActiveState();
    
    startWorker(videoPath);
}

void MainWindow::updateFrame(const QImage& frame) {
    // Convert to Pixmap (must be done in GUI thread)
    m_pixmapItem->setPixmap(QPixmap::fromImage(frame));
}

void MainWindow::onVideoOpened(double duration, double fps, int width, int height) {
    m_duration = duration;
    m_scene->setSceneRect(0, 0, width, height);
    m_view->fitInView(m_pixmapItem, Qt::KeepAspectRatio);
}

void MainWindow::onPositionChanged(double pos) {
    m_currentPosition = pos;
    
    if (!m_isSliderPressed) {
        int sliderVal = static_cast<int>((pos / m_duration) * 1000.0);
        m_seekSlider->blockSignals(true);
        m_seekSlider->setValue(sliderVal);
        m_seekSlider->blockSignals(false);
        
        // Update label
        QTime c(0,0);
        c = c.addSecs(static_cast<int>(pos));
        QTime t(0,0);
        t = t.addSecs(static_cast<int>(m_duration));
        m_timeLabel->setText(c.toString("mm:ss") + " / " + t.toString("mm:ss"));
    }
}

void MainWindow::onVideoError(const QString& message) {
    QMessageBox::critical(this, "Video Error", message);
}

void MainWindow::togglePlayPause() {
    if (m_worker) {
        bool isPaused = (m_playButton->text() == "▶");
        m_worker->setPaused(!isPaused);
        m_playButton->setText(isPaused ? "⏸" : "▶");
    }
}

void MainWindow::onSliderPressed() {
    m_isSliderPressed = true;
    if (m_worker) m_worker->setPaused(true);
}

void MainWindow::onSliderReleased() {
    m_isSliderPressed = false;
    if (m_worker) {
        // Seek to position
        double pct = m_seekSlider->value() / 1000.0;
        m_worker->seek(pct * m_duration);
        m_worker->setPaused(false);
        m_playButton->setText("⏸");
    }
}

void MainWindow::onSliderMoved(int value) {
    // Update time label during drag
    if (m_isSliderPressed && m_duration > 0) {
        double pos = (value / 1000.0) * m_duration;
        QTime c(0,0);
        c = c.addSecs(static_cast<int>(pos));
        QTime t(0,0);
        t = t.addSecs(static_cast<int>(m_duration));
        m_timeLabel->setText(c.toString("mm:ss") + " / " + t.toString("mm:ss"));
    }
}

void MainWindow::onSpeedChanged(int index) {
    if (!m_worker) return;
    
    static const double speeds[] = {0.5, 0.75, 1.0, 1.25, 1.5, 2.0};
    if (index >= 0 && index < 6) {
        m_worker->setSpeed(speeds[index]);
    }
}

void MainWindow::onBehaviorDoubleClicked(QTreeWidgetItem* item, int column) {
    Q_UNUSED(column);
    
    // Only handle leaf nodes (actual behaviors, not categories)
    if (item->childCount() > 0) return;
    
    QTreeWidgetItem* parent = item->parent();
    if (!parent) return;
    
    QString parentCategory = parent->text(0);
    QString behavior = item->text(0);
    QString type = item->text(1);
    
    toggleBehavior(parentCategory, behavior, type);
}

void MainWindow::toggleBehavior(const QString& parentCategory, const QString& behavior, const QString& type) {
    // Get current values from controls
    QString role = m_roleCombo->currentText();
    QString groupType = m_groupTypeCombo->currentText();
    QString sex = m_sexCombo->currentText();
    QString stage = m_stageCombo->currentText();
    QString tag = m_tagEdit->text();
    QString observations = m_observationsEdit->text();
    
    std::optional<int> groupSize = m_groupSizeSpin->value() > 0 ? 
        std::optional<int>(m_groupSizeSpin->value()) : std::nullopt;
    std::optional<int> motherCalves = m_motherCalvesSpin->value() > 0 ? 
        std::optional<int>(m_motherCalvesSpin->value()) : std::nullopt;
    std::optional<int> calves = m_calvesSpin->value() > 0 ? 
        std::optional<int>(m_calvesSpin->value()) : std::nullopt;
    
    if (type == "EVENT") {
        // Instant recording
        BehaviorRecord record;
        record.session = 1;
        record.role = role;
        record.behaviour = behavior;
        record.parentBehaviour = parentCategory;
        record.startTime = m_currentPosition;
        record.duration = 0.0;
        record.recordType = "EVENT";
        record.tag = tag;
        record.groupType = groupType;
        record.sex = sex;
        record.stage = stage;
        record.observations = observations;
        record.groupSize = groupSize;
        record.motherAndCalf = motherCalves;
        record.calves = calves;
        
        m_records.append(record);
        updateRecordsDisplay();
        
    } else if (type == "STATE") {
        if (!m_stateActive) {
            // Start state
            m_stateActive = true;
            m_currentStateBehavior = behavior;
            m_currentStateParent = parentCategory;
            m_stateStartTime = m_currentPosition;
            
            m_stateFeedbackLabel->setText(QString("🔄 Active: %1").arg(behavior));
            
            // Disable other behaviors in tree
            for (int i = 0; i < m_behaviorTree->topLevelItemCount(); ++i) {
                QTreeWidgetItem* category = m_behaviorTree->topLevelItem(i);
                for (int j = 0; j < category->childCount(); ++j) {
                    QTreeWidgetItem* child = category->child(j);
                    if (child->text(0) != behavior) {
                        child->setDisabled(true);
                    }
                }
            }
            
        } else {
            // End state
            double endTime = m_currentPosition;
            double duration = endTime - m_stateStartTime;
            
            BehaviorRecord record;
            record.session = 1;
            record.role = role;
            record.behaviour = m_currentStateBehavior;
            record.parentBehaviour = m_currentStateParent;
            record.startTime = m_stateStartTime;
            record.endTime = endTime;
            record.duration = duration;
            record.recordType = "STATE";
            record.tag = tag;
            record.groupType = groupType;
            record.sex = sex;
            record.stage = stage;
            record.observations = observations;
            record.groupSize = groupSize;
            record.motherAndCalf = motherCalves;
            record.calves = calves;
            
            m_records.append(record);
            updateRecordsDisplay();
            
            clearActiveState();
        }
    }
}

void MainWindow::clearActiveState() {
    m_stateActive = false;
    m_currentStateBehavior.clear();
    m_currentStateParent.clear();
    m_stateStartTime = 0.0;
    m_stateFeedbackLabel->clear();
    
    // Re-enable all behaviors
    for (int i = 0; i < m_behaviorTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem* category = m_behaviorTree->topLevelItem(i);
        for (int j = 0; j < category->childCount(); ++j) {
            category->child(j)->setDisabled(false);
        }
    }
}

void MainWindow::updateRecordsDisplay() {
    m_recordsTable->setRowCount(m_records.size());
    
    for (int i = 0; i < m_records.size(); ++i) {
        const BehaviorRecord& r = m_records[i];
        
        // Time column
        QString timeStr = r.startTimeStr();
        if (r.endTime.has_value()) {
            timeStr += " - " + r.endTimeStr();
        }
        QTableWidgetItem* timeItem = new QTableWidgetItem(timeStr);
        timeItem->setFlags(timeItem->flags() & ~Qt::ItemIsEditable);
        m_recordsTable->setItem(i, 0, timeItem);
        
        // Behavior column
        QString behaviorStr = r.parentBehaviour + " / " + r.behaviour;
        QTableWidgetItem* behaviorItem = new QTableWidgetItem(behaviorStr);
        behaviorItem->setFlags(behaviorItem->flags() & ~Qt::ItemIsEditable);
        behaviorItem->setToolTip(r.asDisplayString());
        m_recordsTable->setItem(i, 1, behaviorItem);
        
        // Type column
        QTableWidgetItem* typeItem = new QTableWidgetItem(r.recordType);
        typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
        m_recordsTable->setItem(i, 2, typeItem);
        
        // Delete button
        QPushButton* deleteBtn = new QPushButton("🗑");
        deleteBtn->setFixedWidth(30);
        connect(deleteBtn, &QPushButton::clicked, this, [this, i]() {
            deleteRecord(i);
        });
        m_recordsTable->setCellWidget(i, 3, deleteBtn);
    }
}

void MainWindow::deleteRecord(int index) {
    if (index >= 0 && index < m_records.size()) {
        m_records.removeAt(index);
        updateRecordsDisplay();
    }
}

void MainWindow::saveRecords() {
    if (m_records.isEmpty()) {
        QMessageBox::information(this, "No Records", "There are no records to save.");
        return;
    }
    
    QString suggestedPath;
    if (!m_videoFiles.isEmpty() && !m_videoDir.isEmpty()) {
        QString baseName = QFileInfo(m_videoFiles[m_currentVideoIndex]).completeBaseName();
        suggestedPath = CsvExporter::generateUniqueFilePath(m_videoDir, baseName);
    } else {
        suggestedPath = QDir::homePath() + "/behavior_records.csv";
    }
    
    QString filePath = QFileDialog::getSaveFileName(this, "Save Records", 
        suggestedPath, "CSV Files (*.csv)");
    
    if (filePath.isEmpty()) return;
    
    if (CsvExporter::exportRecords(filePath, m_records)) {
        QMessageBox::information(this, "Saved", 
            QString("Saved %1 records to:\n%2").arg(m_records.size()).arg(filePath));
        m_records.clear();
        updateRecordsDisplay();
    } else {
        QMessageBox::critical(this, "Error", "Failed to save records.");
    }
}
