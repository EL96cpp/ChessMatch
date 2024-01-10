#include "pawn.h"

Pawn::Pawn(const int &y, const int &x, const FigureColor& figure_color, const FigureOwner& figure_owner, QObject *parent)
    : ChessFigure{y, x, parent}
{
    this->figure_color = figure_color;
    this->figure_owner = figure_owner;
    figure_type = FigureType::PAWN;

    if (figure_color == FigureColor::WHITE) {

        setPixmap(QPixmap(":/ChessFigures/chess_figures/white_pawn.png"));

    } else {

        setPixmap(QPixmap(":/ChessFigures/chess_figures/black_pawn.png"));

    }

}

QVector<std::pair<int, int> > Pawn::CalculateMoves(QVector<QVector<ChessFigure*>>& figures)
{
    QVector<std::pair<int, int>> possible_moves;

    if (figure_owner == FigureOwner::PLAYER) {

        if (GetY() > 0 && figures[GetY() - 1][GetX()]->GetColor() == FigureColor::EMPTY) {

            possible_moves.push_back(std::make_pair(GetY() - 1, GetX()));

            if (GetY() > 1 && !MadeFirstStep() && figures[GetY() - 2][GetX()]->GetColor() == FigureColor::EMPTY) {

                possible_moves.push_back(std::make_pair(GetY() - 2, GetX()));

            }

        }

        if (GetY() > 0 && GetX() > 0 && figures[GetY() - 1][GetX() - 1]->GetColor() != FigureColor::EMPTY &&
            figures[GetY() - 1][GetX() - 1]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(GetY() - 1, GetX() - 1));

        }

        if (GetY() > 0 && GetX() < 7 && figures[GetY() - 1][GetX() + 1]->GetColor() != FigureColor::EMPTY &&
            figures[GetY() - 1][GetX() + 1]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(GetY() - 1, GetX() + 1));

        }


    } else {

        if (GetY() < 7 && figures[GetY() + 1][GetX()]->GetColor() == FigureColor::EMPTY) {

            possible_moves.push_back(std::make_pair(GetY() + 1, GetX()));

            if (GetY() < 6 && !MadeFirstStep() && figures[GetY() + 2][GetX()]->GetColor() == FigureColor::EMPTY) {

                possible_moves.push_back(std::make_pair(GetY() + 2, GetX()));

            }

        }

        if (GetY() < 7 && GetX() > 0 && figures[GetY() + 1][GetX() - 1]->GetColor() != FigureColor::EMPTY &&
            figures[GetY() + 1][GetX() - 1]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(GetY() + 1, GetX() - 1));

        }

        if (GetY() < 7 && GetX() < 7 && figures[GetY() + 1][GetX() + 1]->GetColor() != FigureColor::EMPTY &&
            figures[GetY() + 1][GetX() + 1]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(GetY() + 1, GetX() + 1));

        }

    }

    return possible_moves;
}

