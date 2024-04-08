#include "ChessFigure.h"
#include "Rook.h"

Rook::Rook(const Color& figure_color, 
           const size_t& y_index, 
           const size_t& x_index) : ChessFigure(figure_color, FigureType::ROOK, 
                                                y_index, x_index) {}

std::vector<std::pair<size_t, size_t>> Rook::CalculatePossibleMoves(std::vector<std::vector<std::shared_ptr<ChessFigure>>>& board_cells) {

    std::cout << "Calculate rook moves:\n";

    std::vector<std::pair<size_t, size_t>> possible_moves;


    int x_left = x_index - 1;

    while (x_left >= 0) {

        if (board_cells[y_index][x_left]->GetColor() == Color::EMPTY) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_index, x_left));
            --x_left;

        } else if (board_cells[y_index][x_left]->GetColor() != figure_color) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_index, x_left));
            break;

        } else {

            break;

        }

    }


    int x_right = x_index + 1;

    while (x_right < 8) {

        if (board_cells[y_index][x_right]->GetColor() == Color::EMPTY) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_index, x_right));
            ++x_right;

        } else if (board_cells[y_index][x_right]->GetColor() != figure_color) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_index, x_right));
            break;

        } else {

            break;

        }

    }

    
    int y_top = y_index - 1;

    while (y_top >= 0) {

        if (board_cells[y_top][x_index]->GetColor() == Color::EMPTY) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_top, x_index));
            --y_top;    
        
        } else if (board_cells[y_top][x_index]->GetColor() != figure_color) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_top, x_index));
            break;

        } else {

            break;

        }

    }


    int y_bottom = y_index + 1;

    while (y_bottom < 8) {

        if (board_cells[y_bottom][x_index]->GetColor() == Color::EMPTY) {
        
            possible_moves.push_back(std::pair<size_t, size_t>(y_bottom, x_index));
            ++y_bottom;    
        
        } else if (board_cells[y_bottom][x_index]->GetColor() != figure_color) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_bottom, x_index));
            break;

        } else {

            break;

        }

    }

    return possible_moves;

}
 
