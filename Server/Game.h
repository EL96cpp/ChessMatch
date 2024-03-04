#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <vector>

#include "ClientConnection.h"
#include "ChessFigure.h"

class ClientConnection;

enum class TurnColor {

    WHITE,
    BLACK

};


class Game {

public:
    Game(std::shared_ptr<ClientConnection>& first_player, std::shared_ptr<ClientConnection>& second_player);

    bool CheckIfMoveIsCorrect(const char& letter_from, const size_t& index_from, const char& letter_to, const size_t& index_to);
    void MakeMove(const char& letter_from, const size_t& index_from, const char& letter_to, const size_t& index_to);

private:
    std::shared_ptr<ClientConnection> first_player;
    std::shared_ptr<ClientConnection> second_player;
    
    TurnColor current_turn_color;

    std::vector<std::vector<std::shared_ptr<ChessFigure>>> board_cells;
    std::vector<std::shared_ptr<ChessFigure>> taken_by_first_player;
    std::vector<std::shared_ptr<ChessFigure>> taken_by_second_player;

};



