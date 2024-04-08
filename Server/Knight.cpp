#include "Knight.h"
#include "ChessFigure.h"

Knight::Knight(const Color& figure_color,
               const size_t& y_index,
               const size_t& x_index) : ChessFigure(figure_color, FigureType::KNIGHT,
                                                    y_index, x_index) {}


std::vector<std::pair<size_t, size_t>> Knight::CalculatePossibleMoves(std::vector<std::vector<std::shared_ptr<ChessFigure>>>& board_cells) {

    std::vector<std::pair<size_t, size_t>> possible_moves;

    std::cout << "Calculate knight moves:\n";


    if (y_index > 0 && x_index > 1 && board_cells[y_index-1][x_index-2]->GetColor() != figure_color) {

        possible_moves.push_back(std::pair<size_t, size_t>(y_index-1, x_index-2));

    }

    if (y_index > 1 && x_index > 0 && board_cells[y_index-2][x_index-1]->GetColor() != figure_color) {

        possible_moves.push_back(std::pair<size_t, size_t>(y_index-2, x_index-1));

    }

    if (y_index > 1 && x_index < 7 && board_cells[y_index-2][x_index+1]->GetColor() != figure_color) {

        possible_moves.push_back(std::pair<size_t, size_t>(y_index-2, x_index+1));

    }

    if (y_index > 0 && x_index < 6 && board_cells[y_index-1][x_index+2]->GetColor() != figure_color) {

        possible_moves.push_back(std::pair<size_t, size_t>(y_index-1, x_index+2));

    }

    if (y_index < 7 && x_index < 6 && board_cells[y_index+1][x_index+2]->GetColor() != figure_color) {

        possible_moves.push_back(std::pair<size_t, size_t>(y_index+1, x_index+2));

    }

    if (y_index < 6 && x_index < 7 && board_cells[y_index+2][x_index+1]->GetColor() != figure_color) {

        possible_moves.push_back(std::pair<size_t, size_t>(y_index+2, x_index+1));

    }

    if (y_index < 6 && x_index > 0 && board_cells[y_index+2][x_index-1]->GetColor() != figure_color) {

        possible_moves.push_back(std::pair<size_t, size_t>(y_index+2, x_index-1));

    }

    if (y_index < 7 && x_index > 1 && board_cells[y_index+1][x_index-2]->GetColor() != figure_color) {

        possible_moves.push_back(std::pair<size_t, size_t>(y_index+1, x_index-2));

    }

    return possible_moves;

}
 







