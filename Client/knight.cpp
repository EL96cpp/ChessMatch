#include "knight.h"

Knight::Knight(const int &y,
               const int &x,
               const FigureColor& figure_color,
               const FigureOwner& figure_owner,
               QObject *parent,
               const bool& is_transform_pawn_choice_figure)
    : ChessFigure{y, x, parent, is_transform_pawn_choice_figure}
{
    this->figure_color = figure_color;
    this->figure_owner = figure_owner;
    figure_type = FigureType::KNIGHT;

    if (figure_color == FigureColor::WHITE) {

        setPixmap(QPixmap(":/ChessFigures/chess_figures/white_knight.png"));

    } else {

        setPixmap(QPixmap(":/ChessFigures/chess_figures/black_knight.png"));

    }

}

QVector<std::pair<int, int> > Knight::CalculateMoves(QVector<QVector<ChessFigure*>>& figures)
{
    QVector<std::pair<int, int>> possible_moves;

    if (GetX() - 2 >= 0 && GetY() - 1 >= 0) {

        if (figures[GetY() - 1][GetX() - 2]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(GetY() - 1, GetX() - 2));

        }

    }

    if (GetX() - 1 >= 0 && GetY() - 2 >= 0) {

        if (figures[GetY() - 2][GetX() - 1]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(GetY() - 2, GetX() - 1));

        }

    }

    if (GetX() + 1 < 8 && GetY() - 2 >= 0) {

        if (figures[GetY() - 2][GetX() + 1]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(GetY() - 2, GetX() + 1));

        }

    }

    if (GetX() + 2 < 8 && GetY() - 1 >= 0) {

        if (figures[GetY() - 1][GetX() + 2]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(GetY() - 1, GetX() + 2));

        }

    }

    if (GetX() + 2 < 8 && GetY() + 1 < 8) {

        if (figures[GetY() + 1][GetX() + 2]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(GetY() + 1, GetX() + 2));

        }

    }

    if (GetX() + 1 < 8 && GetY() + 2 < 8) {

        if (figures[GetY() + 2][GetX() + 1]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(GetY() + 2, GetX() + 1));

        }

    }

    if (GetX() - 1 >= 0 && GetY() + 2 < 8) {

        if (figures[GetY() + 2][GetX() - 1]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(GetY() + 2, GetX() - 1));

        }

    }

    if (GetX() - 2 >= 0 && GetY() + 1 < 8) {

        if (figures[GetY() + 1][GetX() - 2]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(GetY() + 1, GetX() - 2));

        }

    }

    return possible_moves;

}
