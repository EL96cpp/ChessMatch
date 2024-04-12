#pragma once

#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <map>
#include <boost/property_tree/ptree.hpp>


enum class LoginResult {

    SUCCESS,
    INCORRECT_PASSWORD,
    NO_NICKNAME_IN_DATABASE,
    DATABASE_ERROR

};

enum class RegisterResult {

    SUCCESS,
    USER_ALREADY_EXISTS

};

enum class UpdateRatingResult {

    SUCCESS,
    NO_NICKNAME_IN_DATABASE

};


class SqlService { 

public:
    SqlService();
    
    LoginResult Login(const std::string& nickname, const std::string& password);
    RegisterResult Register(const std::string& nickname, const std::string& password);
    bool CheckIfUserExists(const std::string& nickname);
    int GetPlayerRating(const std::string& nickname);
    int GetPlayersGamesPlayed(const std::string& nickname);
    void AddGameResult(const std::string& white_nickname, const std::string& black_nickname, const std::string& winner, const int& number_of_moves, const int& total_time);
    UpdateRatingResult UpdatePlayerRating(const std::string& nickname, const size_t& new_rating, const size_t& games_played);
    boost::property_tree::ptree GetTopHundredPlayersRating();

private:
    pqxx::connection sql_connection;

};
