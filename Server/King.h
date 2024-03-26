#pragma once

#include <vector>
#include <map>

#include "ChessFigure.h"

class King : public ChessFigure {

public:
    King(const Color& figure_color, const size_t& y_index, const size_t& x_index);

    std::vector<std::pair<size_t, size_t>> CalculatePossibleMoves(std::vector<std::vector<std::shared_ptr<ChessFigure>>>& board_cells);


};


