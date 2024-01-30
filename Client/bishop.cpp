#include "bishop.h"

Bishop::Bishop(const int &y, const int &x, const FigureColor& figure_color, const FigureOwner& figure_owner, QObject *parent)
    : ChessFigure{y, x, parent} {

    this->figure_color = figure_color;
    this->figure_owner = figure_owner;
    figure_type = FigureType::BISHOP;

    if (figure_color == FigureColor::WHITE) {

        setPixmap(QPixmap(":/ChessFigures/chess_figures/white_bishop.png"));

    } else {

        setPixmap(QPixmap(":/ChessFigures/chess_figures/black_bishop.png"));

    }

}

QVector<std::pair<int, int> > Bishop::CalculateMoves(QVector<QVector<ChessFigure*>>& figures) {

    QVector<std::pair<int, int>> possible_moves;

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

    return possible_moves;
}
