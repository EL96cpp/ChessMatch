#ifndef BISHOP_H
#define BISHOP_H

#include "chessfigure.h"

class Bishop : public ChessFigure
{
public:
    explicit Bishop(const int &y, const int &x, const FigureColor& figure_color, const FigureOwner& figure_owner, QObject *parent);
    QVector<std::pair<int, int>> CalculateMoves(QVector<QVector<ChessFigure*>>& figures) override;
};

#endif // BISHOP_H
