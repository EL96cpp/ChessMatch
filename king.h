#ifndef KING_H
#define KING_H

#include "chessfigure.h"

class King : public ChessFigure
{
public:
    King(const int &y, const int &x, const FigureColor& figure_color, const FigureOwner& figure_owner, QObject *parent);
    QVector<std::pair<int, int>> CalculateMoves(QVector<QVector<ChessFigure*>>& figures) override;
    bool MadeCastling();

private:
    bool made_castling = false;

};

#endif // KING_H
