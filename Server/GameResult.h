#pragma once

#include <string>



struct GameResult {

    GameResult(const std::string& white_nickname, const std::string& black_nickname, const std::string& winner,
               const size_t& number_of_moves, const size_t& total_time) : white_nickname(white_nickname), black_nickname(black_nickname), winner(winner),  
                                                                          number_of_moves(number_of_moves), total_time(total_time) {}


    std::string white_nickname;
    std::string black_nickname;
    std::string winner;
    size_t number_of_moves;
    size_t total_time;   //Duration of game in seconds


};
