#pragma once

#include <pqxx/pqxx>
#include <string>
#include <iostream>

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
    UpdateRatingResult UpdatePlayerRating(const std::string& nickname, const size_t& new_rating);

private:
    pqxx::connection sql_connection;

};
