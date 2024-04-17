#include "King.h"


King::King(const Color& figure_color, 
           const size_t& y_index, 
           const size_t& x_index) : ChessFigure(figure_color, FigureType::KING,
                                                y_index, x_index) {}


std::vector<std::pair<size_t, size_t>> King::CalculatePossibleMoves(std::vector<std::vector<std::shared_ptr<ChessFigure>>>& board_cells) {


    std::vector<std::pair<size_t, size_t>> possible_moves;

    //Upper line

    if (y_index > 0) {

        if (x_index > 0 && board_cells[y_index-1][x_index-1]->GetColor() != figure_color) {

            possible_moves.emplace_back(y_index-1, x_index-1);

        }

        if (board_cells[y_index-1][x_index]->GetColor() != figure_color) {

            possible_moves.emplace_back(y_index-1, x_index);

        }

        if (x_index < 7 && board_cells[y_index-1][x_index+1]->GetColor() != figure_color) {

            possible_moves.emplace_back(y_index-1, x_index+1);

        }

    }


    //Right cell
    
    if (x_index > 0 && board_cells[y_index][x_index-1]->GetColor() != figure_color) {

        possible_moves.emplace_back(y_index, x_index-1);

    }

    //Left cell

    if (x_index < 7 && board_cells[y_index][x_index+1]->GetColor() != figure_color) {

        possible_moves.emplace_back(y_index, x_index+1);

    }


    //Lower line

    if (y_index < 7) {

        if (x_index > 0 && board_cells[y_index+1][x_index-1]->GetColor() != figure_color) {

            possible_moves.emplace_back(y_index+1, x_index-1);

        }

        if (board_cells[y_index+1][x_index]->GetColor() != figure_color) {

            possible_moves.emplace_back(y_index+1, x_index);

        }

        if (x_index < 7 && board_cells[y_index+1][x_index+1]->GetColor() != figure_color) {

            possible_moves.emplace_back(y_index+1, x_index+1);

        }

    }

    return possible_moves;

}



