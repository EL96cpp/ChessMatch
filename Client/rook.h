#ifndef ROOK_H
#define ROOK_H

#include <QObject>

#include "chessfigure.h"

class Rook : public ChessFigure
{
public:
    Rook(const int &y, const int &x, const FigureColor& figure_color, const FigureOwner& figure_owner, QObject *parent = nullptr);
    QVector<std::pair<int, int>> CalculateMoves(QVector<QVector<ChessFigure*>>& figures) override;

};

#endif // ROOK_H
