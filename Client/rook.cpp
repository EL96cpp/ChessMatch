#include "rook.h"

Rook::Rook(const int &y,
           const int &x,
           const FigureColor& figure_color,
           const FigureOwner& figure_owner,
           QObject *parent,
           const bool& is_transform_pawn_choice_figure)
    : ChessFigure{y, x, parent, is_transform_pawn_choice_figure}
{
    this->figure_color = figure_color;
    this->figure_owner = figure_owner;
    figure_type = FigureType::ROOK;

    if (figure_color == FigureColor::WHITE) {

        setPixmap(QPixmap(":/ChessFigures/chess_figures/white_rook.png"));

    } else {

        setPixmap(QPixmap(":/ChessFigures/chess_figures/black_rook.png"));

    }

}

QVector<std::pair<int, int> > Rook::CalculateMoves(QVector<QVector<ChessFigure*>>& figures)
{
    QVector<std::pair<int, int>> possible_moves;

    int x = GetX() - 1, y = GetY();
    while (x >= 0) {

        if (figures[y][x]->GetColor() == FigureColor::EMPTY) {

            possible_moves.push_back(std::make_pair(y, x));
            --x;

        } else if (figures[y][x]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(y, x));
            break;

        } else {

            break;

        }
    }

    x = GetX(), y = GetY() - 1;
    while (y >= 0) {

        if (figures[y][x]->GetColor() == FigureColor::EMPTY) {

            possible_moves.push_back(std::make_pair(y, x));
            --y;

        } else if (figures[y][x]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(y, x));
            break;

        } else {

            break;

        }
    }

    x = GetX() + 1, y = GetY();
    while (x < 8) {

        if (figures[y][x]->GetColor() == FigureColor::EMPTY) {

            possible_moves.push_back(std::make_pair(y, x));
            ++x;

        } else if (figures[y][x]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(y, x));
            break;

        } else {

            break;

        }
    }

    x = GetX(), y = GetY() + 1;
    while (y < 8) {

        if (figures[y][x]->GetColor() == FigureColor::EMPTY) {

            possible_moves.push_back(std::make_pair(y, x));
            ++y;

        } else if (figures[y][x]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(y, x));
            break;

        } else {

            break;

        }
    }

    return possible_moves;
}
