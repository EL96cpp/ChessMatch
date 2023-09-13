#include "emptyfigure.h"
#include "chessfigure.h"

EmptyFigure::EmptyFigure(const int &y, const int &x, QObject *parent)
    : ChessFigure{y, x, parent}
{
    figure_color = FigureColor::EMPTY;
    figure_type = FigureType::EMPTY;
}

QVector<std::pair<int, int> > EmptyFigure::CalculateMoves(QVector<QVector<ChessFigure*>>& figures)
{
    QVector<std::pair<int, int>> empty;
    return empty;
}
