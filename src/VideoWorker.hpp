#pragma once

#include <QObject>
#include <QImage>
#include <QMutex>
#include <QWaitCondition>
#include <opencv2/opencv.hpp>
#include <atomic>
#include <deque>

class VideoWorker : public QObject {
    Q_OBJECT

public:
    explicit VideoWorker(QString videoPath, QObject* parent = nullptr);
    ~VideoWorker() override;

public slots:
    // Main loop to start processing
    void process();
    
    // Controls
    void stop();
    void setPaused(bool paused);
    void seek(double positionSeconds);
    void setSpeed(double speed);

signals:
    // Emitted when a frame is ready for display
    void frameReady(const QImage& frame);
    
    // Metadata signals
    void videoOpened(double duration, double fps, int width, int height);
    void positionChanged(double timestamp);
    void finished();
    void errorOccurred(QString message);

private:
    void openVideo();
    void readFramesIntoBuffer();
    
    QString m_videoPath;
    cv::VideoCapture m_cap;
    
    // State
    std::atomic<bool> m_stop;
    std::atomic<bool> m_paused;
    std::atomic<bool> m_seeking;
    std::atomic<double> m_seekTarget;
    double m_playbackSpeed;
    double m_fps;
    double m_duration;
    
    // Buffer
    static const int BUFFER_SIZE = 10;
    std::deque<std::pair<double, QImage>> m_buffer; // Pair of timestamp, image
    QMutex m_bufferMutex;
};

