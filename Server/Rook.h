#pragma once

#include <map>
#include <memory>
#include <vector>

#include "ChessFigure.h"


class Rook : public ChessFigure {

public:
    Rook(const Color& figure_color, const size_t& y_index, const size_t& x_index);

    std::vector<std::pair<size_t, size_t>> CalculatePossibleMoves(std::vector<std::vector<std::shared_ptr<ChessFigure>>>& board_cells) override;
 

};

