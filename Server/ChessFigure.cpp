ChessFigure::ChessFigure(const Color& figure_color, const FigureType& figure_type, 
                         const size_t& y_index, const size_t& x_index) : figure_color(figure_color), 
                                                                         figure_type(figure_type), 
                                                                         made_first_step(false),
                                                                         y_index(y_index),
                                                                         x_index(x_index) {}

Color ChessFigure::GetColor() {

    return figure_color;

}

FigureType ChessFigure::GetType() {

    return figure_type;

}

bool ChessFigure::MadeFirstStep() {

    return made_first_step;

}

void ChessFigure::SetMadeFirstStep(const bool& made_first_step) {

    this->made_first_step = made_first_step;

}

void ChessFigure::SetCoordinates(const size_t& y_index_to, const size_t& x_index_to) {

    y_index = y_index_to;
    x_index = x_index_to;

    if (!made_first_step) {

        made_first_step = true;

    }


}


std::vector<std::pair<size_t, size_t>> ChessFigure::CalculatePossibleMoves(std::vector<std::vector<std::shared_ptr<ChessFigure>>>& board_cells) {

    std::vector<std::pair<size_t, size_t>> possible_moves;
    return possible_moves;

}


