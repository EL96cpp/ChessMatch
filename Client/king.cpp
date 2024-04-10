#include "king.h"

King::King(const int &y, const int &x, const FigureColor& figure_color, const FigureOwner& figure_owner, QObject *parent)
    : ChessFigure{y, x, parent} {

    this->figure_color = figure_color;
    this->figure_owner = figure_owner;
    figure_type = FigureType::KING;

    if (figure_color == FigureColor::WHITE) {

        setPixmap(QPixmap(":/ChessFigures/chess_figures/white_king.png"));

    } else {

        setPixmap(QPixmap(":/ChessFigures/chess_figures/black_king.png"));

    }

}

QVector<std::pair<int, int> > King::CalculateMoves(QVector<QVector<ChessFigure*>>& figures) {

    qDebug() << "Calculate king possible moves";

    QVector<std::pair<int, int>> possible_moves;

    int x = GetX() - 1, y = GetY() - 1;
    if (y >= 0) {

        while (x < GetX() + 2 && x >= 0 && x < 8) {

            if (figures[y][x]->GetColor() != GetColor()) {

                possible_moves.push_back(std::make_pair(y, x));

            }

            ++x;

        }

    }

    x = GetX() - 1, y = GetY() + 1;
    if (y < 8) {

        while (x < GetX() + 2 && x >= 0 && x < 8) {

            if (figures[y][x]->GetColor() != GetColor()) {

                possible_moves.push_back(std::make_pair(y, x));

            }
            ++x;

        }

    }

    x = GetX() - 1, y = GetY();
    if (x >= 0) {

        if (figures[y][x]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(y, x));

        }

    }

    x = GetX() + 1;
    if (x < 8) {

        if (figures[y][x]->GetColor() != GetColor()) {

            possible_moves.push_back(std::make_pair(y, x));

        }

    }

    if (!made_castling && !made_first_step) {

        if (y_board_index == 0) {

            if (figure_color == FigureColor::WHITE) {

                if (figures[0][x_board_index - 1]->GetColor() == FigureColor::EMPTY &&
                    figures[0][x_board_index - 2]->GetColor() == FigureColor::EMPTY &&
                    figures[0][x_board_index - 3]->GetType() == FigureType::ROOK &&
                    !figures[0][x_board_index - 3]->MadeFirstStep()) {

                    possible_moves.push_back(std::make_pair(y_board_index, x_board_index - 2));

                }

                if (figures[0][x_board_index + 1]->GetColor() == FigureColor::EMPTY &&
                    figures[0][x_board_index + 2]->GetColor() == FigureColor::EMPTY &&
                    figures[0][x_board_index + 3]->GetColor() == FigureColor::EMPTY &&
                    figures[0][x_board_index + 4]->GetType() == FigureType::ROOK &&
                    !figures[0][x_board_index + 4]->MadeFirstStep()) {

                    possible_moves.push_back(std::make_pair(y_board_index, x_board_index + 2));

                }


            } else if (figure_color == FigureColor::BLACK) {

                if (figures[0][x_board_index - 1]->GetColor() == FigureColor::EMPTY &&
                    figures[0][x_board_index - 2]->GetColor() == FigureColor::EMPTY &&
                    figures[0][x_board_index - 3]->GetColor() == FigureColor::EMPTY &&
                    figures[0][x_board_index - 4]->GetType() == FigureType::ROOK &&
                    !figures[0][x_board_index - 4]->MadeFirstStep()) {

                    possible_moves.push_back(std::make_pair(y_board_index, x_board_index - 2));

                }

                if (figures[0][x_board_index + 1]->GetColor() == FigureColor::EMPTY &&
                    figures[0][x_board_index + 2]->GetColor() == FigureColor::EMPTY &&
                    figures[0][x_board_index + 3]->GetType() == FigureType::ROOK &&
                    !figures[0][x_board_index + 3]->MadeFirstStep()) {

                    possible_moves.push_back(std::make_pair(y_board_index, x_board_index + 2));

                }

            }

        }

        if (y_board_index == 7) {

            if (figure_color == FigureColor::WHITE) {

                if (figures[7][x_board_index - 1]->GetColor() == FigureColor::EMPTY &&
                    figures[7][x_board_index - 2]->GetColor() == FigureColor::EMPTY &&
                    figures[7][x_board_index - 3]->GetColor() == FigureColor::EMPTY &&
                    figures[7][x_board_index - 4]->GetType() == FigureType::ROOK &&
                    !figures[7][x_board_index - 4]->MadeFirstStep()) {

                    possible_moves.push_back(std::make_pair(y_board_index, x_board_index - 2));

                }

                if (figures[7][x_board_index + 1]->GetColor() == FigureColor::EMPTY &&
                    figures[7][x_board_index + 2]->GetColor() == FigureColor::EMPTY &&
                    figures[7][x_board_index + 3]->GetType() == FigureType::ROOK &&
                    !figures[7][x_board_index + 3]->MadeFirstStep()) {

                    possible_moves.push_back(std::make_pair(y_board_index, x_board_index + 2));

                }

            } else if (figure_color == FigureColor::BLACK) {

                if (figures[7][x_board_index - 1]->GetColor() == FigureColor::EMPTY &&
                    figures[7][x_board_index - 2]->GetColor() == FigureColor::EMPTY &&
                    figures[7][x_board_index - 3]->GetType() == FigureType::ROOK &&
                    !figures[7][x_board_index - 3]->MadeFirstStep()) {

                    possible_moves.push_back(std::make_pair(y_board_index, x_board_index - 2));

                }

                if (figures[7][x_board_index + 1]->GetColor() == FigureColor::EMPTY &&
                    figures[7][x_board_index + 2]->GetColor() == FigureColor::EMPTY &&
                    figures[7][x_board_index + 3]->GetColor() == FigureColor::EMPTY &&
                    figures[7][x_board_index + 4]->GetType() == FigureType::ROOK &&
                    !figures[7][x_board_index + 4]->MadeFirstStep()) {

                    possible_moves.push_back(std::make_pair(y_board_index, x_board_index + 2));

                }


            }

        }

    }


    return possible_moves;

}

bool King::MadeCastling() {

    return made_castling;

}
