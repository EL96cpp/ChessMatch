#include "Bishop.h"

Bishop::Bishop(const Color& figure_color,
               const size_t& y_index,
               const size_t& x_index) : ChessFigure(figure_color, FigureType::BISHOP,
                                                    y_index, x_index) {}


std::vector<std::pair<size_t, size_t>> Bishop::CalculatePossibleMoves(std::vector<std::vector<std::shared_ptr<ChessFigure>>>& board_cells) {

    std::vector<std::pair<size_t, size_t>> possible_moves;

    std::cout << "Calculate bishop moves:\n";


    int y_upper_left = y_index-1, x_upper_left = x_index-1;
    
    while (y_upper_left >= 0 && x_upper_left >= 0) {


        if (board_cells[y_upper_left][x_upper_left]->GetColor() == Color::EMPTY) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_upper_left, x_upper_left));            
            --y_upper_left;
            --x_upper_left;

        } else if (board_cells[y_upper_left][x_upper_left]->GetColor() != figure_color) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_upper_left, x_upper_left));
            break;

        } else {

            break;

        } 


    }


    int y_upper_right = y_index-1, x_upper_right = x_index+1;

    while (y_upper_right >= 0 && x_upper_right < 8) {


        if (board_cells[y_upper_right][x_upper_right]->GetColor() == Color::EMPTY) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_upper_right, x_upper_right));            
            --y_upper_right;
            ++x_upper_right;

        } else if (board_cells[y_upper_right][x_upper_right]->GetColor() != figure_color) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_upper_right, x_upper_right));
            break;

        } else {

            break;

        } 


    }


    int y_lower_right = y_index+1, x_lower_right = x_index+1;
    
    while (y_lower_right < 8 && x_lower_right < 8) {


        if (board_cells[y_lower_right][x_lower_right]->GetColor() == Color::EMPTY) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_lower_right, x_lower_right));            
            ++y_lower_right;
            ++x_lower_right;

        } else if (board_cells[y_lower_right][x_lower_right]->GetColor() != figure_color) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_lower_right, x_lower_right));
            break;

        } else {

            break;

        } 


    }


    int y_lower_left = y_index+1, x_lower_left = x_index-1;

    while (y_lower_left < 8 && x_lower_left >= 0) {


        if (board_cells[y_lower_left][x_lower_left]->GetColor() == Color::EMPTY) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_lower_left, x_lower_left));            
            ++y_lower_left;
            --x_lower_left;

        } else if (board_cells[y_lower_left][x_lower_left]->GetColor() != figure_color) {

            possible_moves.push_back(std::pair<size_t, size_t>(y_lower_left, x_lower_left));
            break;

        } else {

            break;

        } 


    }

    return possible_moves;

}

