#include "VideoWorker.hpp"
#include <QThread>
#include <QDebug>

VideoWorker::VideoWorker(QString videoPath, QObject* parent)
    : QObject(parent)
    , m_videoPath(videoPath)
    , m_stop(false)
    , m_paused(false)
    , m_seeking(false)
    , m_playbackSpeed(1.0)
    , m_fps(30.0)
    , m_duration(0.0)
{
}

VideoWorker::~VideoWorker() {
    stop();
}

void VideoWorker::openVideo() {
    // Convert QString to std::string for OpenCV (path encoding might be an issue on Windows, check later)
    // On Linux it's fine.
    m_cap.open(m_videoPath.toStdString());
    
    if (m_cap.isOpened()) {
        m_fps = m_cap.get(cv::CAP_PROP_FPS);
        if (m_fps <= 0) m_fps = 30.0;
        
        double frameCount = m_cap.get(cv::CAP_PROP_FRAME_COUNT);
        m_duration = frameCount / m_fps;
        int width = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_WIDTH));
        int height = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_HEIGHT));
        
        emit videoOpened(m_duration, m_fps, width, height);
    } else {
        emit errorOccurred("Failed to open video file: " + m_videoPath);
        m_stop = true;
    }
}

void VideoWorker::stop() {
    m_stop = true;
}

void VideoWorker::setPaused(bool paused) {
    m_paused = paused;
}

void VideoWorker::seek(double positionSeconds) {
    m_seekTarget = positionSeconds;
    m_seeking = true;
}

void VideoWorker::setSpeed(double speed) {
    m_playbackSpeed = speed;
}

void VideoWorker::process() {
    openVideo();
    
    // Simple pacing loop
    while (!m_stop) {
        // 1. Handle Seeking
        if (m_seeking) {
            m_bufferMutex.lock();
            m_buffer.clear();
            m_bufferMutex.unlock();
            
            double target = m_seekTarget.load();
            // Seek OpenCV
            m_cap.set(cv::CAP_PROP_POS_MSEC, target * 1000.0);
            m_seeking = false;
        }
        
        // 2. Decode / Buffer Filling
        m_bufferMutex.lock();
        bool bufferNeedsData = m_buffer.size() < BUFFER_SIZE;
        m_bufferMutex.unlock();
        
        if (bufferNeedsData) {
            cv::Mat frame;
            if (m_cap.read(frame)) {
                if (!frame.empty()) {
                    // Color Conversion
                    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
                    
                    // Deep copy to QImage
                    // Note: QImage constructor with raw data does NOT copy, so we must call .copy()
                    QImage qImg(frame.data, frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_RGB888);
                    QImage deepCopy = qImg.copy();
                    
                    double pos = m_cap.get(cv::CAP_PROP_POS_MSEC) / 1000.0;
                    
                    m_bufferMutex.lock();
                    m_buffer.push_back({pos, deepCopy});
                    m_bufferMutex.unlock();
                }
            } else {
                // Loop video
                m_cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            }
        }
        
        // 3. Playback / Emission
        if (m_paused) {
            QThread::msleep(50); // Sleep longer when paused
            continue;
        }
        
        m_bufferMutex.lock();
        bool hasFrames = !m_buffer.empty();
        std::pair<double, QImage> nextFrame;
        if (hasFrames) nextFrame = m_buffer.front();
        m_bufferMutex.unlock();
        
        if (hasFrames) {
            emit frameReady(nextFrame.second);
            emit positionChanged(nextFrame.first);
            
            m_bufferMutex.lock();
            m_buffer.pop_front();
            m_bufferMutex.unlock();
            
            // Pacing
            // Calculate desired frame time
            double frameTimeMs = 1000.0 / (m_fps * m_playbackSpeed);
            
            // In a real app, we would measure elapsed time since last emit
            // For now, a simple sleep is robust enough for v1
            QThread::msleep(static_cast<unsigned long>(frameTimeMs));
        } else {
            // Waiting for decoder
            QThread::msleep(5);
        }
    }
    
    m_cap.release();
    emit finished();
}

