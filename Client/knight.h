#ifndef KNIGHT_H
#define KNIGHT_H

#include "chessfigure.h"

class Knight : public ChessFigure
{
public:
    explicit Knight(const int &y, const int &x,
                    const FigureColor& figure_color, const FigureOwner& figure_owner,
                    QObject *parent = nullptr, const bool& is_transform_pawn_choice_figure = false);
    QVector<std::pair<int, int>> CalculateMoves(QVector<QVector<ChessFigure*>>& figures) override;
};

#endif // KNIGHT_H
