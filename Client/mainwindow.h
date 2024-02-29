#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QColor>
#include <QBrush>
#include <QDebug>
#include <QFontDatabase>
#include <QTimer>
#include <chrono>

#include <QGraphicsEffect>

#include "boardcell.h"
#include "chessfigure.h"
#include "board.h"
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

public slots:
    void SetPlayerTurn(const QString& turn);
    void ShowTransformPawnChoice(const QString& pawn_color);
    void PawnTransformFigureClicked(ChessFigure* figure);
    void GameOver(const QString& winner_color);

private slots:
    void on_BoardStyleComboBox_currentTextChanged(const QString &arg1);

    void on_PlayerColorComboBox_currentTextChanged(const QString &arg1);

    void on_NewGameButton_clicked();

    void on_loginButton_clicked();

    void on_registerButton_clicked();

    void on_exitButton_clicked();

    void on_registerRegButton_clicked();

    void on_regReturnButton_clicked();

    void on_logReturnButton_clicked();

    void on_logLoginButton_clicked();

    void on_startGameButton_clicked();

    void updateWaitingLabel();

private:
    void SetFont();
    void SetWhitePawnTransformChoice();
    void SetBlackPawnTransformChoice();

private:
    Ui::MainWindow *ui;
    Board* board;
    TakenFiguresManager* taken_figures_manager;
    QVector<ChessFigure*> pawn_transform_figures;

    QTimer waiting_dots_timer;
    QTimer waiting_rectangles_timer;

    QGraphicsScene* board_scene;
    QGraphicsScene* player_taken_figures_scene;
    QGraphicsScene* opponent_taken_figures_scene;
    QGraphicsScene* pawn_transform_scene;

};
#endif // MAINWINDOW_H
