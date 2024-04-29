#include "audiomanager.h"

AudioManager::AudioManager(QObject *parent)
    : QObject{parent},
      move_sound_path("qrc:/Audio/move.mp3"),
      eat_sound_path("qrc:/Audio/eat.mp3"),
      castling_sound_path("qrc:/Audio/castling.mp3") {}

void AudioManager::PlayMoveSound() {

    auto audioOut = new QAudioOutput{};
    audioOut->setDevice(QMediaDevices::defaultAudioOutput());
    audioOut->setVolume(1.f);
    player.setAudioOutput(audioOut);

    player.setSource(QUrl(move_sound_path));
    player.setPosition(0);
    player.play();

}

void AudioManager::PlayEatSound() {

    auto audioOut = new QAudioOutput{};
    audioOut->setDevice(QMediaDevices::defaultAudioOutput());
    audioOut->setVolume(1.f);
    player.setAudioOutput(audioOut);

    player.setSource(QUrl(eat_sound_path));
    player.setPosition(0);
    player.play();

}

void AudioManager::PlayCastlingSound() {

    auto audioOut = new QAudioOutput{};
    audioOut->setDevice(QMediaDevices::defaultAudioOutput());
    audioOut->setVolume(1.f);
    player.setAudioOutput(audioOut);

    player.setSource(QUrl(castling_sound_path));
    player.setPosition(0);
    player.play();

}
