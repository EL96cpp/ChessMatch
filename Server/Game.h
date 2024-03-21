#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <vector>


class ChessFigure;
class ClientConnection;

enum class TurnColor {

    WHITE,
    BLACK

};


class Game {

public:
    Game();
    Game(std::shared_ptr<ClientConnection>& white_player, std::shared_ptr<ClientConnection>& black_player);

    bool CheckIfMoveIsCorrect(const char& letter_from, const size_t& index_from, const char& letter_to, const size_t& index_to, std::shared_ptr<ClientConnection>& sender);


private:
    void CreateStartField();
    void SendStartGameMessages();
    void MakeMove(const char& letter_from, const size_t& index_from, const char& letter_to, const size_t& index_to);


private:
    std::shared_ptr<ClientConnection> white_player;
    std::shared_ptr<ClientConnection> black_player;
    
    
    TurnColor current_turn_color;

    std::vector<std::vector<std::shared_ptr<ChessFigure>>> board_cells;
    std::vector<std::shared_ptr<ChessFigure>> taken_by_white_player;
    std::vector<std::shared_ptr<ChessFigure>> taken_by_black_player;


};



