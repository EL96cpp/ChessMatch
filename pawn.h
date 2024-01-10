#ifndef PAWN_H
#define PAWN_H

#include <QObject>
#include <QVector>

#include "chessfigure.h"
#include "boardcell.h"

class Pawn : public ChessFigure
{

public:
    Pawn(const int &y, const int &x, const FigureColor& figure_color, const FigureOwner& figure_owner, QObject *parent = nullptr);
    QVector<std::pair<int, int>> CalculateMoves(QVector<QVector<ChessFigure*>>& figures) override;

signals:
    void TransformPawn(Pawn* pawn_figure);


};

#endif // PAWN_H
