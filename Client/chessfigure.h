#ifndef CHESSFIGURE_H
#define CHESSFIGURE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QVector>
#include <QDebug>
#include <map>


enum class FigureType {
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    KING,
    QUEEN,
    EMPTY
};

enum class FigureColor {
    WHITE,
    BLACK,
    EMPTY
};


enum class FigureOwner {
    OPPONENT,
    PLAYER
};


class ChessFigure : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    ChessFigure(const int& y_index, const int& x_index,
                QObject *parent = nullptr, const bool& is_transform_pawn_choice_figure = false);
    ChessFigure(ChessFigure& figure);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    int GetX();
    int GetY();
    void SetX(const int& x);
    void SetY(const int& y);
    void SetMadeFirstStep(const bool& value);
    bool MadeFirstStep();
    FigureType GetType();
    FigureColor GetColor();
    FigureOwner GetOwner();
    void Move(const int& y_coordinate, const int& x_coordinate);
    void SwapCoordinatesAndMovePixmaps(ChessFigure* figure);

    void Debug();

    virtual QVector<std::pair<int, int>> CalculateMoves(QVector<QVector<ChessFigure*>>& figures) = 0;

signals:
    void FigureClicked(ChessFigure* figure);
    void TransformFigureClicked(ChessFigure* figure);

protected:
    int y_board_index;
    int x_board_index;
    FigureType figure_type;
    FigureColor figure_color;
    FigureOwner figure_owner;
    bool is_transform_pawn_choice_figure;
    bool made_first_step = false;

};

#endif // CHESSFIGURE_H
