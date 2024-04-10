#include "queen.h"

Queen::Queen(const int &y,
             const int &x,
             const FigureColor& figure_color,
             const FigureOwner& figure_owner,
             QObject *parent,
             const bool& is_transform_pawn_choice_figure)
    : ChessFigure{y, x, parent, is_transform_pawn_choice_figure}
{
    this->figure_color = figure_color;
    this->figure_owner = figure_owner;
    figure_type = FigureType::QUEEN;

    if (figure_color == FigureColor::WHITE) {

        setPixmap(QPixmap(":/ChessFigures/chess_figures/white_queen.png"));

    } else {

        setPixmap(QPixmap(":/ChessFigures/chess_figures/black_queen.png"));

    }

}

QVector<std::pair<int, int> > Queen::CalculateMoves(QVector<QVector<ChessFigure*>>& figures)
{
    QVector<std::pair<int, int>> possible_moves;

    //Calculate diagonal moves

    int x = GetX() - 1, y = GetY() - 1;
    while (x >= 0 && y >= 0) {

        if (figures[y][x]->GetColor() == FigureColor::EMPTY) {

            possible_moves.push_back(std::make_pair(y, x));
            --x, --y;

        } else if (figures[y][x]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(y, x));
            break;

        } else {

            break;

        }

    }

    x = GetX() + 1, y = GetY() - 1;
    while (x < 8 && y >= 0) {

        if (figures[y][x]->GetColor() == FigureColor::EMPTY) {

            possible_moves.push_back(std::make_pair(y, x));
            ++x, --y;

        } else if (figures[y][x]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(y, x));
            break;

        } else {

            break;

        }

    }

    x = GetX() + 1, y = GetY() + 1;
    while (x < 8 && y < 8) {

        if (figures[y][x]->GetColor() == FigureColor::EMPTY) {

            possible_moves.push_back(std::make_pair(y, x));
            ++x, ++y;

        } else if (figures[y][x]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(y, x));
            break;

        } else {

            break;

        }

    }

    x = GetX() - 1, y = GetY() + 1;
    while (x >= 0 && y < 8) {

        if (figures[y][x]->GetColor() == FigureColor::EMPTY) {

            possible_moves.push_back(std::make_pair(y, x));
            --x, ++y;

        } else if (figures[y][x]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(y, x));
            break;

        } else {

            break;

        }

    }

    // Calculate linear moves

    x = GetX() - 1, y = GetY();
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
