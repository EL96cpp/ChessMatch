#ifndef EMPTYFIGURE_H
#define EMPTYFIGURE_H

#include "chessfigure.h"

class EmptyFigure : public ChessFigure
{
public:
    EmptyFigure(const int &y, const int &x, QObject *parent);
    QVector<std::pair<int, int>> CalculateMoves(QVector<QVector<ChessFigure*>>& figures) override;

};

#endif // EMPTYFIGURE_H
