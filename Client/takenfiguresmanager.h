#ifndef TAKENFIGURESMANAGER_H
#define TAKENFIGURESMANAGER_H

#include <QObject>
#include <QGraphicsScene>
#include <QVector>

#include "chessfigure.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"


class TakenFiguresManager : public QObject
{
    Q_OBJECT
public:
    explicit TakenFiguresManager(QGraphicsScene* opponent_taken_scene, QGraphicsScene* player_taken_scene, QObject *parent = nullptr);

signals:

public slots:
    void StartNewGame();
    void AddPlayerTakenFigure(const FigureType& figure_type, const FigureColor& figure_color);
    void AddOpponentTakenFigure(const FigureType& figure_type, const FigureColor& figure_color);

private:
    ChessFigure *CreateTakenFigure(const FigureType& figure_type, const FigureColor& figure_color);

private:
    QVector<ChessFigure*> opponent_taken_figures;
    QVector<ChessFigure*> player_taken_figures;
    QGraphicsScene* opponent_taken_scene;
    QGraphicsScene* player_taken_scene;
    int x_opponent_taken = -130;
    int y_opponent_taken = -100;
    int x_player_taken = -130;
    int y_player_taken = -100;

};

#endif // TAKENFIGURESMANAGER_H
