#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaDevices>
#include <QAudioDevice>


class AudioManager : public QObject
{
    Q_OBJECT
public:
    explicit AudioManager(QObject *parent = nullptr);

public slots:
    void PlayMoveSound();
    void PlayEatSound();
    void PlayCastlingSound();
    void PlayButtonClickedSound();

signals:

private:
    QMediaPlayer player;
    QString move_sound_path;
    QString eat_sound_path;
    QString castling_sound_path;
    QString button_click_sound_path;

};

#endif // AUDIOMANAGER_H
