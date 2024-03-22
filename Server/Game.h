#pragma once

#include <string>
#include <iostream>
#include <chrono>
#include <memory>
#include <vector>
#include <map>


class ChessFigure;
class ClientConnection;
enum class PlayerColor;


class Game {

public:
    Game();
    Game(std::shared_ptr<ClientConnection>& white_player, std::shared_ptr<ClientConnection>& black_player);

    bool CheckIfMoveIsCorrect(const char& letter_from, const size_t& index_from, const char& letter_to, const size_t& index_to, const PlayerColor& sender_color);

    PlayerColor DrawOfferedBy();
    bool CheckIfPlayerIsAGameMember(std::shared_ptr<ClientConnection>& player);
    PlayerColor GetCurrentTurnPlayerColor();


private:
    void CreateStartField();
    void SendStartGameMessages();
    void MakeMove(const char& letter_from, const size_t& index_from, const char& letter_to, const size_t& index_to);


private:
    std::shared_ptr<ClientConnection> white_player;
    std::shared_ptr<ClientConnection> black_player;
       
    PlayerColor draw_offered_by;

    PlayerColor current_turn_color;

    std::chrono::time_point<std::chrono::system_clock> start_timepoint;
    std::chrono::time_point<std::chrono::system_clock> end_timepoint;

    std::vector<std::vector<std::shared_ptr<ChessFigure>>> board_cells;
    std::vector<std::shared_ptr<ChessFigure>> taken_by_white_player;
    std::vector<std::shared_ptr<ChessFigure>> taken_by_black_player;

};



