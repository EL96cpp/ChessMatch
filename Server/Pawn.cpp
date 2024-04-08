#include "ChessFigure.h"
#include "Pawn.h"

Pawn::Pawn(const Color& figure_color,  
           const size_t& y_index, 
           const size_t& x_index) : ChessFigure(figure_color, FigureType::PAWN,
                                                y_index, x_index) {} 


std::vector<std::pair<size_t, size_t>> Pawn::CalculatePossibleMoves(std::vector<std::vector<std::shared_ptr<ChessFigure>>>& board_cells) {

    std::vector<std::pair<size_t, size_t>> possible_moves;

    std::cout << "Calculate pawn moves:\n";


    if (figure_color == Color::WHITE) {


        if (y_index > 0 && board_cells[y_index-1][x_index]->GetColor() == Color::EMPTY) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_index-1, x_index));
                
            if (y_index > 1 && board_cells[y_index-2][x_index]->GetColor() == Color::EMPTY) {

                possible_moves.push_back(std::pair<size_t, size_t>(y_index-2, x_index));

            }

        }

        if (y_index > 0 && x_index > 0 && board_cells[y_index-1][x_index-1]->GetColor() != Color::EMPTY && 
            board_cells[y_index-1][x_index-1]->GetColor() != figure_color) {
            
            possible_moves.push_back(std::pair<size_t, size_t>(y_index-1, x_index-1));

        }

        if (y_index > 0 && x_index < 7 && board_cells[y_index-1][x_index+1]->GetColor() != Color::EMPTY &&
            board_cells[y_index-1][x_index+1]->GetColor() != figure_color) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_index-1, x_index+1));

        }


    } else if (figure_color == Color::BLACK) {


        if (y_index < 7 && board_cells[y_index+1][x_index]->GetColor() == Color::EMPTY) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_index+1, x_index));
                
            if (y_index < 6 && board_cells[y_index+2][x_index]->GetColor() == Color::EMPTY) {

                possible_moves.push_back(std::pair<size_t, size_t>(y_index+2, x_index));

            }

        }

        if (y_index < 7 && x_index > 0 && board_cells[y_index+1][x_index-1]->GetColor() != Color::EMPTY && 
            board_cells[y_index+1][x_index-1]->GetColor() != figure_color) {
            
            possible_moves.push_back(std::pair<size_t, size_t>(y_index+1, x_index-1));

        }

        if (y_index < 7 && x_index < 7 && board_cells[y_index+1][x_index+1]->GetColor() != Color::EMPTY &&
            board_cells[y_index+1][x_index+1]->GetColor() != figure_color) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_index+1, x_index+1));

        }



    }

    return possible_moves;

}

