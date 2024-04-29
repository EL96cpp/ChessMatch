#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QMessageBox>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QColor>
#include <QBrush>
#include <QDebug>
#include <QFontDatabase>
#include <QTimer>
#include <QStandardItemModel>
#include <chrono>
#include <boost/asio.hpp>

#include <QGraphicsEffect>

#include "ratingproxymodel.h"
#include "boardcell.h"
#include "chessfigure.h"
#include "board.h"
#include "audiomanager.h"
#include "client.h"
#include "takenfiguresmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void SetPawnTransformChoice(const FigureType& figure_type);
    void Login(const QString& nickname, const QString& password);
    void Logout(const QString& nickname);
    void Register(const QString& nickname, const QString& password);
    void StartWaitingForOpponent();
    void StopWaitingForOpponent();
    void OfferDraw();
    void Resign();
    void AcceptDraw();

public slots:
    void SetPlayerTurn(const QString& turn);
    void ShowTransformPawnChoice(const FigureColor& pawn_color);
    void HideTransformPawnChoice();
    void PawnTransformFigureClicked(ChessFigure* figure);
    void GameOver(const QString& winner_color);
    void OnLoggedIn(const QString& nickname, const QString& rating, const QString& games_played, const QList<QPair<QString, QString>>& rating_values);
    void OnLoggedout();
    void OnRegistered(const QString& nickname);
    void OnStartWaitingForOpponentAccepted();
    void OnStopWaitingForOpponentAccepted();
    void OnGameStarted(const QString& player_color, const QString& player_nickname, const QString& opponent_nickname);
    void OnShowErrorMessage(const QString& title, const QString& error_description);
    void OnGameOver(const QString& game_result);
    void OnUpdatePlayerRatingAndGamesPlayed(const QString& new_rating);
    void OnUpdateTopPlayersRating(const QList<QPair<QString, QString>>& rating_map);
    void OnDrawOffered();

private slots:
    void on_BoardStyleComboBox_currentTextChanged(const QString &arg1);

    void on_loginButton_clicked();

    void on_registerButton_clicked();

    void on_exitButton_clicked();

    void on_registerRegButton_clicked();

    void on_regReturnButton_clicked();

    void on_logReturnButton_clicked();

    void on_logLoginButton_clicked();

    void on_startGameButton_clicked();

    void updateWaitingLabel();

    void on_waitingExitButton_clicked();

    void on_exitProfileButton_clicked();

    void on_logoutButton_clicked();

    void on_gameExitButton_clicked();

    void on_OfferDrawButton_clicked();

    void on_ResignButton_clicked();

private:
    void SetFont();
    void SetWhitePawnTransformChoice();
    void SetBlackPawnTransformChoice();

private:
    Ui::MainWindow *ui;
    Client* client;
    Board* board;
    AudioManager* audio_manager;
    TakenFiguresManager* taken_figures_manager;
    QVector<ChessFigure*> pawn_transform_figures;

    QStandardItemModel* rating_model;
    RatingProxyModel* rating_proxy_model;

    QTimer waiting_dots_timer;
    QTimer waiting_rectangles_timer;

    QString typing_font_family;
    QString logo_font_family;

    QGraphicsScene* board_scene;
    QGraphicsScene* player_taken_figures_scene;
    QGraphicsScene* opponent_taken_figures_scene;
    QGraphicsScene* pawn_transform_scene;

};
#endif // MAINWINDOW_H
