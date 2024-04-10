#ifndef QUEEN_H
#define QUEEN_H

#include "chessfigure.h"

class Queen : public ChessFigure
{
public:
    explicit Queen(const int &y,
                   const int &x,
                   const FigureColor& figure_color,
                   const FigureOwner& figure_owner,
                   QObject *parent = nullptr,
                   const bool& is_transform_pawn_choice_figure = false);

    QVector<std::pair<int, int>> CalculateMoves(QVector<QVector<ChessFigure*>>& figures) override;

};

#endif // QUEEN_H
