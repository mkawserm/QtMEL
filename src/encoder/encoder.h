#ifndef ENCODER_H
#define ENCODER_H

#include "../qvacl_global.h"
#include "encoderglobal.h"
#include "videocodecsettings.h"
#include <QObject>
#include <QSize>
#include <QImage>

class EncoderPrivate;
class QThread;

class QVACLSHARED_EXPORT Encoder : public QObject
{
    Q_OBJECT

    friend class EncoderPrivate;

public:

     /*! This enum describes encoder's errors. */
    enum Error {
        NoError = 0, /*!< No error occurred. */
        InvalidVideoSizeError, /*!< Wrong video size. */
        InvalidFilePathError, /*!< Empty file path. */
        InvalidOutputFormatError, /*!< Could not get output format by filename. */
        InvalidVideoStreamError, /*!< Could not add video stream. */
        VideoEncoderNotFoundError, /*!< Requested video encoder was not found. */
        InvalidVideoCodecError, /*!< Could not open video codec. */
        InvalidAudioStreamError, /*!< Could not add audio stream. */
        AudioEncoderNotFoundError, /*!< Requested audio encoder was not found. */
        InvalidAudioCodecError, /*!< Could not open audio codec. */
        FileOpenError, /*!< Could not open a file. */
        InvalidConversionContext, /*!< Could not initialize conversion context. */
        InvalidInputPixelFormat /*!< Could not convert input pixel format to the ffmpeg's format. */
    };

    /*! This enum describes the state of the encoder. */
    enum State {
        ActiveState = 0, /*!< Encoder is active, this state is set after start() is called. */
        StoppedState /*!< Encoder is not active. */
    };

    explicit Encoder(QObject *parent = 0);
    ~Encoder();

    void setFilePath(const QString &filePath);
    QString filePath() const;

    void setVideoSize(const QSize &size);
    QSize videoSize() const;

    void setFixedFrameRate(int frameRate);
    int fixedFrameRate() const;

    void setEncodeAudio(bool encode);
    bool encodeAudio() const;

    void setOutputPixelFormat(EncoderGlobal::PixelFormat format);
    EncoderGlobal::PixelFormat outputPixelFormat() const;

    void setVideoCodec(EncoderGlobal::VideoCodec codec);
    EncoderGlobal::VideoCodec videoCodec() const;

    void setAudioCodec(EncoderGlobal::AudioCodec codec);
    EncoderGlobal::AudioCodec audioCodec() const;

    void setVideoCodecSettings(const VideoCodecSettings &settings);
    VideoCodecSettings videoCodecSettings() const;

    /*!
      Returns the state of the encoder.
      \sa stateChanged()
    */
    Encoder::State state() const;

    /*!
      Returns the last error that occurred. If no error occurred, returns Encoder::NoError.
      \sa errorString()
    */
    Encoder::Error error() const;

    /*!
      Returns a human-readable description of the last error that occurred.
      \sa error()
    */
    QString errorString() const;

public slots:
    void start();
    void stop();

    void encodeVideoFrame(const QImage &frame, int duration);

signals:
    void stateChanged(Encoder::State state);
    void error(Encoder::Error errorCode);

private:
    /*!
      Sets the encoder's state to the given state.
      \sa state()
    */
    void setState(Encoder::State state);

    void setError(Encoder::Error errorCode, const QString &errorString);

    EncoderPrivate *d_ptr;
    QThread *m_encoderThread;

    Encoder::State m_state;
    Encoder::Error m_error;
    QString m_errorString;
};

#endif // ENCODER_H