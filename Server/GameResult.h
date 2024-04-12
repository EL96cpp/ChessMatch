#pragma once

#include <string>



struct GameResult {

    GameResult(const std::string& white_nickname, 
               const std::string& black_nickname, 
               const std::string& winner,
               const size_t& number_of_moves, 
               const size_t& total_time, 
               const size_t& new_white_player_rating,
               const size_t& white_player_games_played, 
               const size_t& new_black_player_rating, 
               const size_t& black_player_games_played) : white_nickname(white_nickname), black_nickname(black_nickname), 
                                                          winner(winner), number_of_moves(number_of_moves), 
                                                          total_time(total_time), new_white_player_rating(new_white_player_rating),
                                                          white_player_games_played(white_player_games_played), new_black_player_rating(new_black_player_rating),
                                                          black_player_games_played(black_player_games_played) {}


    std::string white_nickname;
    std::string black_nickname;
    std::string winner;
    
    size_t new_white_player_rating;
    size_t white_player_games_played;
    size_t new_black_player_rating;
    size_t black_player_games_played;
    size_t number_of_moves;
    size_t total_time;   //Duration of game in seconds


};
